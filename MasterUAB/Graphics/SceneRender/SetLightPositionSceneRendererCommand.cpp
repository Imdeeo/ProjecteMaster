#include "SetLightPositionSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

CSetLightPositionSceneRendererCommand::CSetLightPositionSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	m_MaterialName = TreeNode.GetPszProperty("material");
	m_LightName = TreeNode.GetPszProperty("light");
}

CSetLightPositionSceneRendererCommand::~CSetLightPositionSceneRendererCommand(){}

void CSetLightPositionSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	CLight* l_Light = UABEngine.GetInstance()->GetLightManager()->GetResource(m_LightName);
	std::vector<CMaterialParameter *> l_Parameters = UABEngine.GetInstance()->GetMaterialManager()->GetResource(m_MaterialName)->GetParameters();
	Vect2f l_2DPos = UABEngine.GetInstance()->GetRenderManager()->GetScreenPosFrom3D(l_Light->GetPosition());
	for (int i = 0; i<l_Parameters.size(); i++)
	{
		if (l_Parameters[i]->GetName() == "light_position")
		{
			memcpy(l_Parameters[i]->GetValueLuaAddress(), &l_2DPos, sizeof(Vect2f));
		}
	}
}