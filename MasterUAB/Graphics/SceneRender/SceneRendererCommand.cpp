#include "SceneRendererCommand.h"
#include "Materials\Material.h"


CSceneRendererCommand::CSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) :CNamed(TreeNode), CActive(TreeNode), m_LevelId(_LevelId)
{
}


CSceneRendererCommand::~CSceneRendererCommand()
{
}
