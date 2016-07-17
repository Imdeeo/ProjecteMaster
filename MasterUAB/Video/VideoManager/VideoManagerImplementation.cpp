#include "VideoManagerImplementation.h"
#include "Utils.h"
#include "Engine\UABEngine.h"
#include <theoraplayer/TheoraPlayer.h>
#include <theoraplayer/TheoraDataSource.h>
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "Texture\DynamicTexture.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "D3DX11tex.h"
CVideoManagerImplementation::CVideoManagerImplementation()
{
	mgr = new TheoraVideoManager();
}

CVideoManagerImplementation::~CVideoManagerImplementation()
{
	//CHECKED_DELETE(mgr);
}

bool CVideoManagerImplementation::Init()
{
	clip = mgr->createVideoClip("media/bunny.ogv");
	if (clip == nullptr)
		return false;
	return true;
}
 
void CVideoManagerImplementation::Update(float _ElapsedTime)
{
	mgr->update(_ElapsedTime);
	TheoraVideoFrame *frame = clip->getNextFrame();
	if (frame)
	{
		//ID3D11ShaderResourceView *l_Texture;
		//D3DXCreateTexture(device, size.x, size.y, numMipMaps, usage, textureFormat, D3DPOOL_MANAGED, &l_Texture);
		//CDynamicTexture *l_Texture = new CDynamicTexture("pepe", clip->getWidth(), frame->getHeight(), false, "rgba8");
		//l_Texture->m_Texture = (ID3D11ShaderResourceView*)frame->getBuffer();
		//CRenderableObjectTechnique* l_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_NORMAL_TEXTURE_VERTEX");

		//UABEngine.GetRenderManager()->DrawScreenQuad(l_RenderableObjectTechnique->GetEffectTechnique(), l_Texture, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, clip->getWidth(), frame->getHeight(), textureFormat, GL_UNSIGNED_BYTE, frame->getBuffer());

			// transfer the frame pixels to your display device, texure, graphical context or whatever you use.
		clip->popFrame(); // be sure to pop the frame from the frame queue when you're done
	}

}