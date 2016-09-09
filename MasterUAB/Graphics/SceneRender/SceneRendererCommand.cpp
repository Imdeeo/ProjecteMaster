#include "SceneRendererCommand.h"
#include "Materials\Material.h"


CSceneRendererCommand::CSceneRendererCommand(tinyxml2::XMLElement* TreeNode) :CNamed(TreeNode), CActive(TreeNode)
{

}


CSceneRendererCommand::~CSceneRendererCommand()
{
}
