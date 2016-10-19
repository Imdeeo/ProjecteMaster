
#include "GamePlayManager.h"
#include "Components\LUAComponent.h"
#include "Utils.h"

CGamePlayManager::CGamePlayManager()
{
}


CGamePlayManager::~CGamePlayManager()
{
	Destroy();
}


void CGamePlayManager::Update(float ElapsedTime)
{
	for (size_t i = 0; i<m_Resources.size(); ++i)
	{
		m_Resources[i]->Update(ElapsedTime);
	}
}

void CGamePlayManager::Destroy()
{
	UtilsLog("Destroying GamePlayManager");
	for (size_t i = 0; i < m_Resources.size(); ++i)
	{
//		CHECKED_DELETE(m_Resources[i]);
	}
	Clear();
}

void CGamePlayManager::Clear()
{
	m_Resources.clear();
}

void CGamePlayManager::AddComponent(CLUAComponent* _LUAComponent)
{
	m_Resources.push_back(_LUAComponent);
}

CLUAComponent* CGamePlayManager::GetComponent(size_t _id)
{
	return m_Resources[_id];
}