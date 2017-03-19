#include "OmniLight.h"
#include "Camera\Frustum.h"
#include "Engine\UABEngine.h"
#include "Layers\LayerManager.h"
#include <assert.h>
#include "RenderManager\RenderManager.h"
#include "DebugRender.h"
#include "RenderHelper\RenderHelper.h"
#include "Effects\EffectManager.h"
#include "ContextManager\ContextManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\RenderableVertexs.h"

COmniLight::COmniLight(CLevel* _Level) :CLight(_Level){}

COmniLight::COmniLight(std::string _name, CLevel* _Level) : CLight(_name,_Level)
{
	m_Type = GetLightTypeByName("omni");
}

COmniLight::COmniLight(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CLight(TreeNode,_Level){}

void COmniLight::SetShadowMap(CRenderManager &RenderManager)
{
	assert(!"this method must not be called");
}

bool const COmniLight::GetInsideFrustum()
{
	return m_Frustum->SphereVisible(m_Position, m_EndRangeAttenuation);
}

void COmniLight::Save(FILE* _File)
{
	fprintf_s(_File, "\t<light name=\"%s\" enabled=\"%s\" type=\"omni\" pos=\"%f %f %f\" color=\"%f %f %f %f\" att_start_range=\"%f\" att_end_range=\"%f\" intensity=\"%f\"/>\n",
		m_Name.c_str(), m_Enabled ? "true" : "false", m_Position.x, m_Position.y, m_Position.z, m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha(), 
		m_StartRangeAttenuation, m_EndRangeAttenuation, m_Intensity);
}

void COmniLight::Render(CRenderManager *_RenderManager)
{
	if (m_Enabled)
	{
		CEffectManager::m_SceneParameters.m_BaseColor = CColor(1,1,1,1);
		_RenderManager->GetContextManager()->SetWorldMatrix(GetTransform());
		CEffectTechnique* l_EffectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_lights")->GetEffectTechnique();
		CEffectManager::SetSceneConstants(l_EffectTechnique);
		GetShape(_RenderManager)->RenderIndexed(_RenderManager, l_EffectTechnique, CEffectManager::GetRawData());
	}
}

CRenderableVertexs* COmniLight::GetShape(CRenderManager *_RenderManager)
{
	#ifdef _DEBUG
		return _RenderManager->GetDebugRender()->GetSPhere10();
	#else
		return _RenderManager->GetRenderHelper()->GetSPhere10();
	#endif
}

const Mat44f & COmniLight::GetTransform()
{
	m_ScaleMatrix.SetIdentity();
	m_ScaleMatrix.Scale(m_EndRangeAttenuation*1.1, m_EndRangeAttenuation*1.1, m_EndRangeAttenuation*1.1);

	m_RotationMatrix.SetIdentity();
	m_RotationMatrix = m_Rotation.rotationMatrix();

	m_TranslationMatrix.SetIdentity();
	m_TranslationMatrix.SetPos(m_Position.x, m_Position.y, m_Position.z);

	m_TransformMatrix = m_ScaleMatrix*m_RotationMatrix*m_TranslationMatrix;

	return m_TransformMatrix;
}