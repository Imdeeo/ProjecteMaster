#include "SetLightPositionSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "Lights\LightManager.h"
#include "Materials\MaterialManager.h"

#include "LevelManager\LevelManager.h"

#include "Materials\MaterialParameter.h"
#include "Camera\CameraControllerManager.h"
#include "Lights\DirectionalLight.h"

CSetLightPositionSceneRendererCommand::CSetLightPositionSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CSceneRendererCommand(TreeNode,_Level)
{
	m_MaterialName = TreeNode->GetPszProperty("material");
	m_LightName = TreeNode->GetPszProperty("light");
}

CSetLightPositionSceneRendererCommand::~CSetLightPositionSceneRendererCommand(){}

void CSetLightPositionSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	CDirectionalLight* l_Light = (CDirectionalLight*)UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetLightManager()->GetResource(m_LightName);
	std::vector<CMaterialParameter *> l_Parameters = UABEngine.GetLevelManager()->GetResource(m_LevelId)->GetMaterialManager()->GetResource(m_MaterialName)->GetParameters();
	Vect2f l_2DPos = UABEngine.GetInstance()->GetRenderManager()->GetScreenPosFrom3D(UABEngine.GetCameraControllerManager()->GetMainCamera()->GetPosition() - l_Light->GetDirection() * 100);
	for (size_t i = 0; i<l_Parameters.size(); i++)
	{
		if (l_Parameters[i]->GetName() == "light_position")
		{
			memcpy(l_Parameters[i]->GetValueAddress(), &l_2DPos, sizeof(Vect2f));
		}
	}
}