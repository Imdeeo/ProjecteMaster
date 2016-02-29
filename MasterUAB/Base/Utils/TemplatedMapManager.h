#ifndef TEMPLATED_MAP_MANAGER_H
#define TEMPLATED_MAP_MANAGER_H

#include <stdio.h>
#include <string>
#include <map>
#include "Utils.h"

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

	void RemoveResource(const std::string &Name)
	{
		TMapResource::iterator it;	
		it = m_Resources.find(Name);
		if(it!=m_Resources.end())
		{
			delete it->second;
			m_Resources.erase(it);          
		}
	}

	virtual T * GetResource(const std::string &Name)
	{
		if(m_Resources.find(Name)==m_Resources.end())
		{
			printf("CMapManager error: El recurs %s no existeix al MapManager\n",Name.c_str());
			return nullptr;
		}
		return m_Resources[Name];
	}

	virtual bool AddResource(const std::string &Name, T *Resource)
	{
		if (Resource == nullptr)
		{
			printf("CMapManager error: Recurs NULL\n");
			return false;
		}
		if(m_Resources.find(Name)!=m_Resources.end())
		{
			CHECKED_DELETE(Resource);
			printf("CMapManager error: El recurs %s ja existeix al MapManager\n",Name.c_str());
			return false;
		}
		m_Resources[Name] = Resource;
		return true;
	}

	virtual bool AddUpdateResource(const std::string &Name, T *Resource)
	{
		if (Resource == nullptr)
		{
			printf("CMapManager error: Recurs NULL\n");
			return false;
		}
		if (m_Resources.find(Name) != m_Resources.end())
		{
			*(m_Resources[Name]) = *(Resource);
			CHECKED_DELETE(Resource);
			return true;
		}
		m_Resources[Name] = Resource;
		return true;
	}

	void Destroy()
	{
		typedef TMapResource::iterator it_type;
		for(it_type iterator = m_Resources.begin();iterator != m_Resources.end();iterator++)
		{
			CHECKED_DELETE(iterator->second);
		}
		m_Resources.clear();
	}

	T* CTemplatedMapManager::operator[](std::string b)
	{
		return m_Resources[b];
	}

	size_t Size()
	{
		return m_Resources.size();
	}

	TMapResource & GetResourcesMap()
	{
		return m_Resources;
	}

	bool RemoveAllBut(std::map<std::string, std::string> _Names)
	{
		std::vector<std::string> l_NamesToRemove;
		typedef TMapResource::iterator it_type;
		for (it_type iterator = m_Resources.begin(); iterator != m_Resources.end(); iterator++)
		{
			if (_Names.find(iterator->first) == _Names.end())
			{
				l_NamesToRemove.push_back(iterator->first);
			}
		}
		for (int i = 0; i < l_NamesToRemove.size(); i++)
		{
			RemoveResource(l_NamesToRemove[i]);
		}
		return true;
	}

	/*T* GetElementsArray()
	{
		std::vector<T> l_ElementsVector;
		typedef TMapResource::iterator it_type;
		for (it_type iterator = m_Resources.begin(); iterator != m_Resources.end(); iterator++)
		{
			l_ElementsVector.push_back(*(iterator->second));
		}
		return &l_ElementsVector[0];
	}
	CEmptyPointerClass* GetEmptyPointerArray()
	{
		return (CEmptyPointerClass*)GetElementsArray();
	}*/
};

#endif //TEMPLATED_MAP_MANAGER_H