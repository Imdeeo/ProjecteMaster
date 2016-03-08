#include "SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Materials\Material.h"


CSceneRendererCommand::CSceneRendererCommand(CXMLTreeNode &TreeNode) :CNamed(TreeNode), CActive(TreeNode)
{

}


CSceneRendererCommand::~CSceneRendererCommand()
{
}
