#include "RenderableObjectsManager.h"
#include "StaticMesh\InstanceMesh.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "Particles\ParticleSystemInstance.h"

CRenderableObjectsManager::CRenderableObjectsManager(std::string _Name):CNamed(_Name) {}
CRenderableObjectsManager::~CRenderableObjectsManager()
{
	CTemplatedVectorMapManager::Destroy();
}

void CRenderableObjectsManager::Update(float ElapsedTime)
{
	for (size_t i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}
void CRenderableObjectsManager::Render(CRenderManager *RM)
{
	for (size_t i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Render(RM);
	}
}

void CRenderableObjectsManager::RenderDebug(CRenderManager *RM)
{
	for (size_t i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->RenderDebug(RM);
	}
}


CRenderableObject * CRenderableObjectsManager::AddMeshInstance(CXMLTreeNode &TreeNode)
{
	CInstanceMesh* instanceMesh = new CInstanceMesh(TreeNode);
	if (AddResource(instanceMesh->GetName(), instanceMesh))
		return instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position,const float _Yaw, const float _Pitch, const float _Roll,const float _Scale, const bool _Visible)
{
	CInstanceMesh* instanceMesh = new CInstanceMesh(InstanceName,CoreMeshName);
	instanceMesh->SetPosition(Position);
	instanceMesh->SetYaw(_Yaw);
	instanceMesh->SetPitch(_Pitch);
	instanceMesh->SetRoll(_Roll);
	instanceMesh->SetScale(_Scale);
	instanceMesh->SetVisible(_Visible);
	if (AddResource(InstanceName, instanceMesh))
		return instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(CXMLTreeNode &TreeNode)
{
	CAnimatedInstanceModel* l_AnimatedInstanceModel = new CAnimatedInstanceModel(TreeNode);
	if (AddResource(l_AnimatedInstanceModel->GetName(), l_AnimatedInstanceModel))
		return l_AnimatedInstanceModel;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position)
{
	assert(!"this method must not be called");
	return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddParticleSystemInstance(CXMLTreeNode &TreeNode)
{
	CParticleSystemInstance* l_ParticleSystemInstance = new CParticleSystemInstance(TreeNode);
	if (AddResource(l_ParticleSystemInstance->GetName(), l_ParticleSystemInstance))
		return l_ParticleSystemInstance;
	else
		return nullptr;
}