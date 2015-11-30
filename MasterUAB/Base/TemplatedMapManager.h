#ifndef TEMPLATED_MAP_MANAGER_H
#define TEMPLATED_MAP_MANAGER_H

#include <stdio.h>
#include <string>
#include <map>

template<class T>
class CTemplatedMapManager
{
protected:
	typedef std::map<std::string, T*> TMapResource;
	TMapResource m_Resources;
public:
	virtual ~CTemplatedMapManager()
	{
		Destroy();
	}
	virtual T * GetResource(const std::string &Name)
	{
		if(m_Resources.find(Name)!=m_Resources.end())
		{
			printf("CMapManager error: El recurs %s ja existeix al MapManager\n",Name.c_str());
			return nullptr;
		}
		return m_Resources[Name];
	}
	virtual bool AddResource(const std::string &Name, T *Resource)
	{
		if(m_Resources.find(Name)!=m_Resources.end())
		{
			printf("CMapManager error: El recurs %s ja existeix al MapManager\n",Name.c_str());
			return false;
		}
		if( Resource != nullptr )
		{
			printf("CMapManager error: Recurs NULL\n");
			return false;
		}
		m_Resources[Name] = Resource;
		return true;
	}
	void Destroy()
	{
		typedef TMapResource::iterator it_type;
		for(it_type iterator = m_Resources.begin();iterator != m_Resources.end();iterator++)
		{
			delete iterator->second;
		}
		m_Resources.clear();
	}
	T* CTemplatedMapManager::operator[](std::string b)
	{
		return m_Resources[b];
	}
};

#endif //TEMPLATED_MAP_MANAGER_H