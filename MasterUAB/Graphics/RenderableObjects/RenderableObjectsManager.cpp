#include "RenderableObjectsManager.h"
#include "StaticMesh\InstanceMesh.h"
#include "AnimatedModels\AnimatedInstanceModel.h"

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


CRenderableObject * CRenderableObjectsManager::AddMeshInstance(CXMLTreeNode &TreeNode, bool _Update)
{
	CInstanceMesh* instanceMesh = new CInstanceMesh(TreeNode);
	if (_Update ? AddUpdateResource(instanceMesh->GetName(), instanceMesh) : AddResource(instanceMesh->GetName(), instanceMesh))
		return instanceMesh;
	else
		return nullptr;
}

CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position, const float _Yaw, const float _Pitch, const float _Roll, const float _Scale, const bool _Visible, bool _Update)
{
	CInstanceMesh* instanceMesh = new CInstanceMesh(InstanceName,CoreMeshName);
	instanceMesh->SetPosition(Position);
	instanceMesh->SetYaw(_Yaw);
	instanceMesh->SetPitch(_Pitch);
	instanceMesh->SetRoll(_Roll);
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
	assert(!"this methon must not be called");
	return nullptr;
}

CRenderableObjectsManager & CRenderableObjectsManager::operator=(CRenderableObjectsManager& _RenderableObjectManager)
{
	m_Filename = _RenderableObjectManager.m_Filename;
	m_Name= _RenderableObjectManager.m_Name;
	*((CTemplatedVectorMapManager<CRenderableObject>*)this) = (CTemplatedVectorMapManager<CRenderableObject>)_RenderableObjectManager;
	return *this;
}