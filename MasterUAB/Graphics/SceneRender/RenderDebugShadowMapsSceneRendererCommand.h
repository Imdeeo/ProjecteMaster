#include "SceneRendererCommand.h"
#include "RenderableObjects\RenderableObjectTechnique.h"

class CRenderDebugShadowMapsSceneRendererCommand :
	public CSceneRendererCommand
{
private:
	float m_Width;
	float m_Offset;
	CRenderableObjectTechnique	*m_RenderableObjecTechnique;
public:
	CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CRenderDebugShadowMapsSceneRendererCommand();

	void Execute(CRenderManager &RenderManager);
};

