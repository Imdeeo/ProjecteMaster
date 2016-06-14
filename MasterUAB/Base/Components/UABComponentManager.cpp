#include "UABComponentManager.h"
#include "UABComponent.h" 
#include "RenderManager\RenderManager.h"

CUABComponentManager::CUABComponentManager(){}

CUABComponentManager::~CUABComponentManager()
{
	
}

void CUABComponentManager::Update(float ElapsedTime)
{
	for(size_t i =0; i<m_Resources.size();++i)
	{
		m_Resources[i]->Update(ElapsedTime);
	}
}

void CUABComponentManager::Destroy()
{
	for (size_t i = 0; i < m_Resources.size(); ++i)
	{
		CHECKED_DELETE(m_Resources[i]);
	}
}
