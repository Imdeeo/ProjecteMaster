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
#include "XML\XMLTreeNode.h"
#include "theoraplayer\TheoraPlayer.h"
#include "Texture\DynamicTexture.h"
#include "Math\Color.h"
#include "D3D11.h"

CDrawVideoQuadRendererCommand::CDrawVideoQuadRendererCommand(CXMLTreeNode &TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{
	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_NORMAL_TEXTURE_VERTEX");
	//m_Texture = new CDynamicTexture("pepe", frame->getWidth(), frame->getHeight(), false, "rgba8");
	m_Texture = new CDynamicTexture("pepe", 720, 400, false, "rgba8");
}

CDrawVideoQuadRendererCommand::~CDrawVideoQuadRendererCommand(void)
{
}

void CDrawVideoQuadRendererCommand::Execute(CRenderManager &_RenderManager)
{
	if (GetActive())
	{
		TheoraVideoClip* l_Clip = UABEngine.GetVideoManager()->GetClip();
		TheoraVideoFrame *frame = l_Clip->getNextFrame();
		if (frame)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			HRESULT result;
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

			// Initialize the render target texture description.
			ZeroMemory(&textureDesc, sizeof(textureDesc));

			// Setup the render target texture description.
			textureDesc.Width = frame->getWidth();
			textureDesc.Height = frame->getHeight();
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA srd;
			srd.pSysMem = frame->getBuffer();
			srd.SysMemPitch = frame->getWidth()*4;
			srd.SysMemSlicePitch = 0;

			ID3D11Texture2D* l_Texture2D = NULL;
			ID3D11Device* l_Device = UABEngine.GetRenderManager()->GetDevice();
			HRESULT l_HR = l_Device->CreateTexture2D(&textureDesc, &srd, &l_Texture2D);
			if (!FAILED(l_HR))
			{
				l_HR = l_Device->CreateShaderResourceView(l_Texture2D, 0, &m_Texture->m_Texture);
				//if (!FAILED(l_HR))
				//{
				//}
			}


		//HRESULT l_HR = UABEngine.GetRenderManager()->GetDevice()->CreateTexture2D(&sTexDesc, &sSubData, &l_Texture);
		//UABEngine.GetRenderManager()->GetDevice()->CreateTexture2D(

		//	CDynamicTexture *l_Texture = new CDynamicTexture("pepe", frame->getWidth(), frame->getHeight(), false, "rgba8");
		//	l_Texture->LoadBuffer(frame->getBuffer(), sizeof((frame->getBuffer())));
			//l_Texture->Load("image.png");
			l_Clip->popFrame(); // be sure to pop the frame from the frame queue when you're done
		}
		_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), m_Texture, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
	}
}
