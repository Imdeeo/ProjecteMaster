#include "RenderableObjectsManager.h"
#include "StaticMesh\InstanceMesh.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "Particles\ParticleSystemInstance.h"
#include "Bilboards\BilboardSystemInstance.h"
#include "Manchas\ManchasSystemInstance.h"
#include "LineRenderer\LineRenderer.h"

#include "LevelManager\LevelManager.h"

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


CRenderableObject * CRenderableObjectsManager::AddMeshInstance(tinyxml2::XMLElement* TreeNode, CLevel* _Level, bool _Update)
{
	CInstanceMesh* instanceMesh = new CInstanceMesh(TreeNode,_Level);
	if (_Update ? AddUpdateResource(instanceMesh->GetName(), instanceMesh) : AddResource(instanceMesh->GetName(), instanceMesh,_Level->GetName()))
		return instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position, CLevel* _Level, const Quatf _Rotation, const float _Scale, const bool _Visible, bool _Update)
{
	CInstanceMesh* instanceMesh = new CInstanceMesh(InstanceName, CoreMeshName,_Level);
	instanceMesh->SetPosition(Position);
	instanceMesh->SetRotation(_Rotation);
	instanceMesh->SetScale(_Scale);
	instanceMesh->SetVisible(_Visible);
	if (_Update ? AddUpdateResource(InstanceName, instanceMesh) : AddResource(InstanceName, instanceMesh,_Level->GetName()))
		return instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(tinyxml2::XMLElement* TreeNode, CLevel* _Level, bool _Update)
{
	CAnimatedInstanceModel* l_AnimatedInstanceModel = new CAnimatedInstanceModel(TreeNode,_Level);
	if (_Update ? AddUpdateResource(l_AnimatedInstanceModel->GetName(), l_AnimatedInstanceModel) : AddResource(l_AnimatedInstanceModel->GetName(), l_AnimatedInstanceModel, _Level->GetName()))
		return l_AnimatedInstanceModel;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position, CLevel* _Level, bool _Update)
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

CRenderableObject * CRenderableObjectsManager::AddParticleSystemInstance(tinyxml2::XMLElement* TreeNode, CLevel* _Level)
{
	CParticleSystemInstance* l_ParticleSystemInstance = new CParticleSystemInstance(TreeNode, _Level);
	if (AddResource(l_ParticleSystemInstance->GetName(), l_ParticleSystemInstance,_Level->GetName()))
		return l_ParticleSystemInstance;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddBilboardSystemInstance(tinyxml2::XMLElement* TreeNode, CLevel* _Level)
{
	CBilboardSystemInstance * l_BilboardSystemInstance = new CBilboardSystemInstance(TreeNode,_Level);
	if (AddResource(l_BilboardSystemInstance->GetName(), l_BilboardSystemInstance,_Level->GetName()))
		return l_BilboardSystemInstance;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddManchasSystemInstance(tinyxml2::XMLElement* TreeNode, CLevel* _Level)
{
	CManchasSystemInstance* l_ManchasSystemInstance = new CManchasSystemInstance(TreeNode,_Level);
	if (AddResource(l_ManchasSystemInstance->GetName(), l_ManchasSystemInstance,_Level->GetName()))
		return l_ManchasSystemInstance;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddLineRendererSystemInstance(tinyxml2::XMLElement* TreeNode, CLevel* _Level)
{
	CLineRenderer* l_LineRenderer = new CLineRenderer(TreeNode,_Level);
	if (AddResource(l_LineRenderer->GetName(), l_LineRenderer,_Level->GetName()))
		return l_LineRenderer;
	else
		return nullptr;
}
