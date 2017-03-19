#include "SceneRendererCommand.h"
#include "Materials\Material.h"
#include "LevelManager\Level.h"

CSceneRendererCommand::CSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) :CNamed(TreeNode), CActive(TreeNode)
{
	m_LevelId =_Level->GetName();
}


CSceneRendererCommand::~CSceneRendererCommand()
{
}
