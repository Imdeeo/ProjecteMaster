#include "UABComponentManager.h"
#include "UABComponent.h" 
#include "RenderManager\RenderManager.h"

CUABComponentManager::CUABComponentManager(){}

CUABComponentManager::~CUABComponentManager()
{
	Destroy();
}

void CUABComponentManager::Update(float ElapsedTime)
{
	for(size_t i =0; i<m_ResourcesVector.size();++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}

void CUABComponentManager::Render(CRenderManager &RenderManager)
{
	for(size_t i =0; i<m_ResourcesVector.size();++i)
	{
		m_ResourcesVector[i]->Render(RenderManager);
	}
}

void CUABComponentManager::RenderDebug(CRenderManager &RenderManager)
{
	for(size_t i =0; i<m_ResourcesVector.size();++i)
	{
		m_ResourcesVector[i]->RenderDebug(RenderManager);
	}
}

void CUABComponentManager::AddComponent(const std::string &Name, CUABComponent *Component)
{

}