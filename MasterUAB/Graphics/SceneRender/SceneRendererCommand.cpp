#include "SceneRendererCommand.h"


CSceneRendererCommand::CSceneRendererCommand(CXMLTreeNode &TreeNode) :CNamed(TreeNode), CActive(TreeNode)
{

}


CSceneRendererCommand::~CSceneRendererCommand()
{
}
