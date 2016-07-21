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


CDrawVideoQuadRendererCommand::CDrawVideoQuadRendererCommand(CXMLTreeNode &TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{
	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_NORMAL_TEXTURE_VERTEX");
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
		//if (frame)
		//{
			CDynamicTexture *l_Texture = new CDynamicTexture("pepe", frame->getWidth(), frame->getHeight(), false, "rgba8");
			l_Texture->LoadBuffer(frame->getBuffer(), sizeof((frame->getBuffer())));
			//l_Texture->Load("image.png");
			//l_Clip->popFrame(); // be sure to pop the frame from the frame queue when you're done
			_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), l_Texture, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
		//}
	}
}
