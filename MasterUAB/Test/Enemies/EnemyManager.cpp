#include "EnemyManager.h"
#include "Enemy.h"
#include "BossEnemy.h"
#include "PatrolEnemy.h"
#include "FixedEnemy.h"
#include <string>
#include <map>
#include "XML/XMLTreeNode.h"

CEnemyManager::CEnemyManager()
{
}

CEnemyManager::~CEnemyManager()
{
	for(size_t i=0; i<m_Enemies.size();++i)
		delete m_Enemies[i];
	m_Enemies.clear();
}

void CEnemyManager::Update(float ElapsedTime)
{
	for(size_t i=0; i<m_Enemies.size();++i)
		m_Enemies[i]->Update(ElapsedTime);
}

void CEnemyManager::Render()
{
	for(size_t i=0; i<m_Enemies.size();++i)
		m_Enemies[i]->Render();
}

/*CEnemy * GetBossEnemy()
{
	return new CBossEnemy;
}*/

/*CEnemy * GetPatrolEnemy()
{
	return new CPatrolEnemy;
}*/

/*template<class T>
CEnemy * GetTemplatedEnemy()
{
	return new T;
}*/

typedef CEnemy *(*TEnemyFactory)(CXMLTreeNode &);

void CEnemyManager::AddEnemy(CXMLTreeNode &XMLTreeNode)
{
	std::string l_Type=XMLTreeNode.GetPszProperty("type");

	std::map<std::string, TEnemyFactory> l_Functions;
	l_Functions["boss"]=CEnemy::GetTemplatedEnemy<CBossEnemy>;
	l_Functions["fixed"]=CEnemy::GetTemplatedEnemy<CFixedEnemy>;
	l_Functions["patrol"]=CEnemy::GetTemplatedEnemy<CPatrolEnemy>;

	CEnemy *l_Enemy=NULL;
	std::map<std::string, TEnemyFactory>::iterator it=l_Functions.find(l_Type);
	if(it!=l_Functions.end())
		l_Enemy=it->second(XMLTreeNode);

	/*if(EnemyType=="boss")
		l_Enemy=new CBossEnemy();
	else if(EnemyType=="fixed")
		l_Enemy=new CFixedEnemy();
	else if(EnemyType=="patrol")
		l_Enemy=new CPatrolEnemy();*/
	
	if(l_Enemy!=NULL)
		m_Enemies.push_back(l_Enemy);
}

void CEnemyManager::Load(const std::string &Filename)
{
	CXMLTreeNode l_File;
	if(l_File.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode  l_Enemies=l_File["enemies"];
		if(l_Enemies.Exists())
		{	
			int l_Count= l_Enemies.GetNumChildren();
			for(int i = 0; i <l_Count; ++i)
				AddEnemy(l_Enemies(i));
		}
	}
}
