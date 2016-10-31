#include "DrawVideoQuadRenderCommand.h"

#include "RenderManager/RenderManager.h"
#include "RenderableObjects/RenderableObjectTechnique.h"
#include "Effects/EffectTechnique.h"
#include "Materials\Material.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Materials\MaterialManager.h"
#include "Utils.h"
#include "VideoManager\VideoManager.h"
#include "theoraplayer\TheoraPlayer.h"
#include "Texture\DynamicTexture.h"
#include "Math\Color.h"
#include "D3D11.h"

#include "MutexManager\MutexManager.h"

#include "LevelManager\Level.h"

CDrawVideoQuadRendererCommand::CDrawVideoQuadRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) :CStagedTexturedSceneRendererCommand(TreeNode,_Level)
{
	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_NORMAL_TEXTURE_VERTEX");
	m_Texture = new CDynamicTexture("pepe", 360, 400, false, "r8u");
}

CDrawVideoQuadRendererCommand::~CDrawVideoQuadRendererCommand(void)
{
}

void CDrawVideoQuadRendererCommand::Execute(CRenderManager &_RenderManager)
{
	if (GetActive())
	{
		IVideoManager* l_VideoManager = UABEngine.GetVideoManager();
		TheoraVideoClip* l_Clip = l_VideoManager->GetActualClip();
		if (l_Clip != nullptr)
		{
			TheoraVideoFrame *frame = l_Clip->getNextFrame();
			if (frame)
			{
				D3D11_TEXTURE2D_DESC textureDesc;

				// Initialize the render target texture description.
				ZeroMemory(&textureDesc, sizeof(textureDesc));

				// Setup the render target texture description.
				textureDesc.Width = frame->getWidth();
				textureDesc.Height = frame->getHeight();
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				textureDesc.CPUAccessFlags = 0;
				textureDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA srd;
				srd.pSysMem = frame->getBuffer();
				srd.SysMemPitch = frame->getWidth() * 4;
				srd.SysMemSlicePitch = 0;

				ID3D11Texture2D* l_Texture2D = NULL;
				ID3D11Device* l_Device = UABEngine.GetRenderManager()->GetDevice();

				std::mutex* l_DeviceMutex = &(UABEngine.GetMutexManager()->g_DeviceMutex);
				l_DeviceMutex->lock();
				HRESULT l_HR = l_Device->CreateTexture2D(&textureDesc, &srd, &l_Texture2D);
				l_DeviceMutex->unlock();
				if (!FAILED(l_HR))
				{
					l_DeviceMutex->lock();
					l_HR = l_Device->CreateShaderResourceView(l_Texture2D, 0, &m_Texture->m_Texture);
					l_DeviceMutex->unlock();
				}

				l_Clip->popFrame(); // be sure to pop the frame from the frame queue when you're done
			}
			_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), m_Texture, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
		}
	}
}
