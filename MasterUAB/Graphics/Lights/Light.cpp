#include "Light.h"

#include "XML\XMLTreeNode.h"
#include "RenderManager\RenderManager.h"

CLight::CLight() : CNamed(""){}

CLight::CLight(CXMLTreeNode &TreeNode) : CNamed("")
{
	SetName(TreeNode.GetPszProperty("name"));
	//SetType(TreeNode.GetPszProperty("type"));
	SetPosition(TreeNode.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f), true));
	SetColor(CColor(TreeNode.GetVect4fProperty("color", Vect4f(255.0f, 255.0f, 255.0f, 0.0f), true)));
	SetStartRangeAttenuation(TreeNode.GetFloatProperty("att_start_range"));
	SetEndRangeAttenuation(TreeNode.GetFloatProperty("att_end_range"));
	SetIntensity(TreeNode.GetFloatProperty("intensity"));
}

CLight::~CLight(){}

void CLight::Render(CRenderManager *RenderManager)
{
	RenderManager->Render();
}

/*TLightType CLight::GetLightTypeByName(const std::string &StrLightType)
{
	return m_Type[StrLightType];
}*/