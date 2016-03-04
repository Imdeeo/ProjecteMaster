#include "Character.h"

#include "XML\XMLTreeNode.h"

#include "RenderableObjects\RenderableObjectsManager.h"
#include "Camera\CameraController.h"
#include "Components\UABComponentManager.h"
#include "Layers\LayerManager.h"

#include "Engine\UABEngine.h"
#include "XML\XMLTreeNode.h"

CCharacter::CCharacter() : CNamed("")
{
}

CCharacter::CCharacter(const CXMLTreeNode & _TreeNode) : CNamed(_TreeNode)
{
	m_LuaCommand = _TreeNode.GetPszProperty("lua_command");
	m_LayerName = _TreeNode.GetPszProperty("layer");
	m_RenderableObjectName = _TreeNode.GetPszProperty("renderable_object");

	m_RenderableObject = UABEngine.GetLayerManager()->GetResource(m_LayerName)->GetResource(m_RenderableObjectName);

}

CCharacter::~CCharacter()
{
}


bool CCharacter::Refresh()
{
	m_RenderableObject = UABEngine.GetLayerManager()->GetResource(m_LayerName)->GetResource(m_RenderableObjectName);
	UABEngine.GetScriptManager()->RunCode(m_LuaCommand);

	return true;
}

CRenderableObject* CCharacter::GetRenderableObject()
{
	return m_RenderableObject;
}

void CCharacter::ExecuteLuaCommand()
{
	UABEngine.GetScriptManager()->RunCode(m_LuaCommand);
}
