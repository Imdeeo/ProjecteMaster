#ifndef H_TEMPLATEDVECTORMAPMANAGER_H
#define H_TEMPLATEDVECTORMAPMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <assert.h>

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
	typedef std::map<std::string, CMapResourceValue*> TMapResources;

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
		CMapResouceValue l_ResourceValue = m_ResourcesMap[Name];
		size_t index = l_ResourceValue.m_Id;
		delete m_ResourcesVector[index];
		m_ResourcesMap.erase(Name);
		m_ResourcesVector.erase(m_ResourcesVector.begin()+index);
		for(TMapResources::iterator l_iterator = m_ResourcesMap.begin();l_iterator != m_ResourcesMap.end();l_iterator++)
		{
			if(l_iterator->second->m_Id>index)
			{
				l_iterator->second->m_Id--;
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
		return m_ResourcesMap[Name]->m_Value;
	}

	virtual bool AddResource(const std::string &Name, T *Resource)
	{
		if(m_ResourcesMap.find(Name)!=m_ResourcesMap.end())
		{
			return false;
		}
		m_ResourcesMap[Name] = new CMapResourceValue (Resource,m_ResourcesVector.size());
		m_ResourcesVector.push_back(Resource);
		return true;
	}

	virtual void Destroy()
	{
		for(size_t i = 0; i<m_ResourcesVector.size();i++)
		{
			delete m_ResourcesVector[i];
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
};

#endif