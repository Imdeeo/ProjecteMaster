#include "UABComponentManager.h"
#include "UABComponent.h" 
#include "RenderManager\RenderManager.h"

CUABComponentManager::CUABComponentManager(){}

CUABComponentManager::~CUABComponentManager()
{
	for (int i = 0; i < m_Resources.size(); ++i)
	{

	}
}

void CUABComponentManager::Update(float ElapsedTime)
{
	for(size_t i =0; i<m_Resources.size();++i)
	{
		m_Resources[i]->Update(ElapsedTime);
	}
}
