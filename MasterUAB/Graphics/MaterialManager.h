#pragma once

#include <unordered_map>
#include <string>

#include "Material.h"

class CMaterialManager
{
public:
	~CMaterialManager();

	void AddMaterials(const std::string& path);

	void ClearMaterials();

	const CMaterial* GetMaterial(const std::string& name) const
	{
		std::unordered_map<std::string, CMaterial*>::const_iterator it = m_Materials.find(name);
		return (it == m_Materials.end()) ? nullptr : it->second;
	};

private:

	std::unordered_map<std::string, CMaterial*> m_Materials;

};

