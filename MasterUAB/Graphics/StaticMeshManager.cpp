#include "StaticMeshManager.h"


CStaticMeshManager::CStaticMeshManager(void)
{
}


CStaticMeshManager::~CStaticMeshManager(void)
{
}

bool CStaticMeshManager::Load(const std::string &FileName)
{
	m_Filename = FileName;
	return true;
}

bool CStaticMeshManager::Reload()
{
	this->Reload()
	return false;
}