#include "ParticleSystemInstance.h"
#include "ParticleManager.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

CParticleSystemInstance::CParticleSystemInstance(CXMLTreeNode &TreeNode) : CRenderableObject(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	m_Type = UABEngine.GetInstance()->GetParticleManager()->GetResource(l_Element.GetPszProperty("type"));
	m_NextParticleEmission = l_Element.GetFloatProperty("next_particle_emission", 1.0f);
	m_Awake = l_Element.GetBoolProperty("awake", false);
	m_AwakeTimer = l_Element.GetFloatProperty("awake_timer", 1.0f);
	m_EmissionBoxHalfSize = l_Element.GetVect3fProperty("emission_box_half_size", Vect3f(1.0, 1.0, 1.0)) * 0.5f;
	m_EmissionVolume = m_EmissionBoxHalfSize.x * m_EmissionBoxHalfSize.y * m_EmissionBoxHalfSize.z * 8;
	m_EmissionScaler = m_Type->GetEmitAbsolute() ? 1 : 1.0f / m_EmissionVolume;
}

CParticleSystemInstance::~CParticleSystemInstance(void)
{
	Destroy();
}

void CParticleSystemInstance::Destroy()
{
	//CHECKED_RELEASE(m_Type);
}

float CParticleSystemInstance::GetRandomValue(float min, float max)
{
	float a = m_UnitDistribution(m_RandomEngine);
	float value = mathUtils::Lerp(min, max, a);
	return value;
}

Vect3f CParticleSystemInstance::GetRandomValue(Vect3f min, Vect3f max)
{
	float a1 = m_UnitDistribution(m_RandomEngine);
	float a2 = m_UnitDistribution(m_RandomEngine);
	float a3 = m_UnitDistribution(m_RandomEngine);
	Vect3f value;
	value.x = mathUtils::Lerp(min.x, max.x, a1);
	value.y = mathUtils::Lerp(min.y, max.y, a1);
	value.z = mathUtils::Lerp(min.z, max.z, a1);
	return value;
}

CColor CParticleSystemInstance::GetRandomValue(CColor min, CColor max)
{
	float a = m_UnitDistribution(m_RandomEngine);
	CColor value = min.Lerp(max, a);
	return value;
}

float CParticleSystemInstance::GetRandomValue(Vect2f value)
{
	return GetRandomValue(value.x, value.y);
}