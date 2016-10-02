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


CRenderableObject * CRenderableObjectsManager::AddMeshInstance(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId, bool _Update)
{
	CInstanceMesh* instanceMesh = new CInstanceMesh(TreeNode,_LevelId);
	if (_Update ? AddUpdateResource(instanceMesh->GetName(), instanceMesh) : AddResource(instanceMesh->GetName(), instanceMesh,_LevelId))
		return instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position, const std::string &_LevelId, const Quatf _Rotation, const float _Scale, const bool _Visible, bool _Update)
{
	CInstanceMesh* instanceMesh = new CInstanceMesh(InstanceName, CoreMeshName);
	instanceMesh->SetPosition(Position);
	instanceMesh->SetRotation(_Rotation);
	instanceMesh->SetScale(_Scale);
	instanceMesh->SetVisible(_Visible);
	if (_Update ? AddUpdateResource(InstanceName, instanceMesh) : AddResource(InstanceName, instanceMesh,_LevelId))
		return instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId, bool _Update)
{
	CAnimatedInstanceModel* l_AnimatedInstanceModel = new CAnimatedInstanceModel(TreeNode);
	if (_Update ? AddUpdateResource(l_AnimatedInstanceModel->GetName(), l_AnimatedInstanceModel) : AddResource(l_AnimatedInstanceModel->GetName(), l_AnimatedInstanceModel, _LevelId))
		return l_AnimatedInstanceModel;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position, const std::string &_LevelId, bool _Update)
{
	assert(!"this method must not be called");
	return nullptr;
}

CRenderableObjectsManager & CRenderableObjectsManager::operator=(CRenderableObjectsManager& _RenderableObjectManager)
{
	m_Filename = _RenderableObjectManager.m_Filename;
	m_Name = _RenderableObjectManager.m_Name;
	*((CTemplatedLevelVectorMapManager<CRenderableObject>*)this) = (CTemplatedLevelVectorMapManager<CRenderableObject>)_RenderableObjectManager;
	return *this;
}

CRenderableObject * CRenderableObjectsManager::AddParticleSystemInstance(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId)
{
	CParticleSystemInstance* l_ParticleSystemInstance = new CParticleSystemInstance(TreeNode, _LevelId);
	if (AddResource(l_ParticleSystemInstance->GetName(), l_ParticleSystemInstance,_LevelId))
		return l_ParticleSystemInstance;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddManchasSystemInstance(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId)
{
	CManchasSystemInstance* l_ManchasSystemInstance = new CManchasSystemInstance(TreeNode,_LevelId);
	if (AddResource(l_ManchasSystemInstance->GetName(), l_ManchasSystemInstance,_LevelId))
		return l_ManchasSystemInstance;
	else
		return nullptr;
}