#include "RenderableObjectManager.h"

CRenderableObjectsManager::CRenderableObjectsManager() {}

CRenderableObjectsManager::~CRenderableObjectsManager()
{
	
}

void CRenderableObjectsManager::Update(float ElapsedTime)
{
	for (int i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}
void CRenderableObjectsManager::Render(CRenderManager *RM)
{
	for (int i = 0; i<m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Render(RM);
	}
}

CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position)
{
	
}

CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position){return nullptr;};
void CRenderableObjectsManager::AddResource(const std::string &Name, CRenderableObject *RenderableObject);
//void CRenderableObjectsManager::CleanUp(); ---no hacer
void CRenderableObjectsManager::Load(const std::string &FileName);
//CRenderableObject * CRenderableObjectsManager::GetInstance(const std::string &Name) const; ---no hacer