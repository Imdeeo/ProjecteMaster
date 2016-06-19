#include "RenderableObjectsManager.h"
#include "StaticMesh\InstanceMesh.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "Particles\ParticleSystemInstance.h"
#include "Manchas\ManchasSystemInstance.h"

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
}


CRenderableObject * CRenderableObjectsManager::AddMeshInstance(CXMLTreeNode &TreeNode, bool _Update)
{
	CInstanceMesh* instanceMesh = new CInstanceMesh(TreeNode);
	if (_Update ? AddUpdateResource(instanceMesh->GetName(), instanceMesh) : AddResource(instanceMesh->GetName(), instanceMesh))
		return instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position, const Quatf _Rotation, const float _Scale, const bool _Visible, bool _Update)
{
	CInstanceMesh* instanceMesh = new CInstanceMesh(InstanceName, CoreMeshName);
	instanceMesh->SetPosition(Position);
	instanceMesh->SetRotation(_Rotation);
	instanceMesh->SetScale(_Scale);
	instanceMesh->SetVisible(_Visible);
	if (_Update ? AddUpdateResource(InstanceName, instanceMesh) : AddResource(InstanceName, instanceMesh))
		return instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(CXMLTreeNode &TreeNode, bool _Update)
{
	CAnimatedInstanceModel* l_AnimatedInstanceModel = new CAnimatedInstanceModel(TreeNode);
	if (_Update ? AddUpdateResource(l_AnimatedInstanceModel->GetName(), l_AnimatedInstanceModel) : AddResource(l_AnimatedInstanceModel->GetName(), l_AnimatedInstanceModel))
		return l_AnimatedInstanceModel;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position, bool _Update)
{
	assert(!"this method must not be called");
	return nullptr;
}

CRenderableObjectsManager & CRenderableObjectsManager::operator=(CRenderableObjectsManager& _RenderableObjectManager)
{
	m_Filename = _RenderableObjectManager.m_Filename;
	m_Name = _RenderableObjectManager.m_Name;
	*((CTemplatedVectorMapManager<CRenderableObject>*)this) = (CTemplatedVectorMapManager<CRenderableObject>)_RenderableObjectManager;
	return *this;
}

CRenderableObject * CRenderableObjectsManager::AddParticleSystemInstance(CXMLTreeNode &TreeNode)
{
	CParticleSystemInstance* l_ParticleSystemInstance = new CParticleSystemInstance(TreeNode);
	if (AddResource(l_ParticleSystemInstance->GetName(), l_ParticleSystemInstance))
		return l_ParticleSystemInstance;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddManchasSystemInstance(CXMLTreeNode &TreeNode)
{
	CManchasSystemInstance* l_ManchasSystemInstance = new CManchasSystemInstance(TreeNode);
	if (AddResource(l_ManchasSystemInstance->GetName(), l_ManchasSystemInstance))
		return l_ManchasSystemInstance;
	else
		return nullptr;
}