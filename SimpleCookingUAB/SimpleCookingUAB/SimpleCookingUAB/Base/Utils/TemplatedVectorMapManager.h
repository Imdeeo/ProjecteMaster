#ifndef H_TEMPLATEDVECTORMAPMANAGER_H
#define H_TEMPLATEDVECTORMAPMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <assert.h>
#include "Utils.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

template<class T>
class CTemplatedVectorMapManager
{
public:
	class CMapResourceValue
	{
	public:
		T *m_Value;
		size_t m_Id;
		CMapResourceValue(){assert(!"Method must not be called");}
		CMapResourceValue(T *Value, size_t Id) : m_Value(Value), m_Id(Id){}
	};

	typedef std::vector<T *> TVectorResources;
	typedef std::map<std::string, CMapResourceValue> TMapResources;

protected:
	TVectorResources m_ResourcesVector;
	TMapResources m_ResourcesMap;

public:
	CTemplatedVectorMapManager(){}
	virtual ~CTemplatedVectorMapManager()
	{
		Destroy();
	}

	bool RemoveResource(const std::string &Name)
	{
		CMapResourceValue l_ResourceValue = m_ResourcesMap[Name];
		size_t index = l_ResourceValue.m_Id;
		delete m_ResourcesVector[index];
		//delete m_ResourcesMap[Name];
		m_ResourcesMap.erase(Name);
		m_ResourcesVector.erase(m_ResourcesVector.begin()+index);
		for (TMapResources::iterator l_iterator = m_ResourcesMap.begin(); l_iterator != m_ResourcesMap.end(); l_iterator++)
		{
			if(l_iterator->second.m_Id>index)
			{
				l_iterator->second.m_Id--;
			}
		}
		return true;
	}

	virtual T * GetResourceById(size_t Id)
	{
		return m_ResourcesVector[Id];
	}

	virtual T * GetResource(const std::string &Name)
	{
		if (m_ResourcesMap.find(Name) == m_ResourcesMap.end())
		{
			return nullptr;
		}
		return m_ResourcesMap[Name].m_Value;
	}

	virtual bool AddResource(const std::string &Name, T *_Resource)
	{
		if (m_ResourcesMap.find(Name) != m_ResourcesMap.end())
		{
			CHECKED_DELETE(_Resource);
			return false;
		}

		m_ResourcesMap.insert(std::pair<std::string, CMapResourceValue>(Name, CMapResourceValue(_Resource, m_ResourcesVector.size())));
		m_ResourcesVector.push_back(_Resource);
		return true;
	}

	virtual bool AddUpdateResource(const std::string &Name, T *_Resource)
	{
		if (_Resource == nullptr)
		{
			printf("CMapManager error: Recurs NULL\n");
			return false;
		}
		if (m_ResourcesMap.find(Name) != m_ResourcesMap.end())
		{
			*(m_ResourcesMap[Name].m_Value) = *(_Resource);
			CHECKED_DELETE(_Resource);
			return true;
		}
		m_ResourcesMap.insert(std::pair<std::string, CMapResourceValue>(Name, CMapResourceValue(_Resource, m_ResourcesVector.size())));
		m_ResourcesVector.push_back(_Resource);
		return true;
	}

	virtual void Destroy()
	{
		for(size_t i = 0; i<m_ResourcesVector.size();i++)
		{
			CHECKED_DELETE(m_ResourcesVector[i]);
		}

		m_ResourcesMap.clear();
		m_ResourcesVector.clear();
	}

	TMapResources & GetResourcesMap()
	{
		return m_ResourcesMap;
	}

	TVectorResources & GetResourcesVector()
	{
		return m_ResourcesVector;
	}

	size_t Size()
	{
		return m_ResourcesVector.size();
	}

	luabind::object GetElementsArray(lua_State *L)
	{
		luabind::object l_ElementsVector = luabind::newtable(L);
		for (size_t i = 0; i < m_ResourcesVector.size();i++)
		{
			l_ElementsVector[i] = m_ResourcesVector[i];
		}
		return l_ElementsVector;
	}

	virtual CTemplatedVectorMapManager<T>& CTemplatedVectorMapManager<T>::operator=(CTemplatedVectorMapManager<T>& _TemplatedVectorMapManager)
	{
		TMapResources::iterator l_iterator = m_ResourcesMap.begin();
		for (; l_iterator != m_ResourcesMap.end(); l_iterator++)
		{
			if (_TemplatedVectorMapManager.m_ResourcesMap.find(l_iterator->first) != _TemplatedVectorMapManager.m_ResourcesMap.end())
			{
				*(l_iterator->second.m_Value) = *(_TemplatedVectorMapManager.m_ResourcesMap[l_iterator->first].m_Value);
			}
		}
		return *this;
	}

};

#endif