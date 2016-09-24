#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <string>
#include "Utils\TemplatedMapManager.h"
#include "Materials\Material.h"

class CMaterialManager : public CTemplatedLevelMapManager<CMaterial>
{
private:
	std::string m_LevelMaterialsFilename;
	std::string m_DefaultMaterialsFilename;
	void LoadMaterialsFromFile(const std::string &Filename, bool Update = false, std::map<std::string, std::string> *UpdatedNames = nullptr, const std::string &_LevelId = "");
public:
	CMaterialManager();
	virtual ~CMaterialManager();
	void Load(const std::string &Filename, const std::string &DefaultsFileName="",const std::string &_LevelId="");
	void Reload();
	void Save();
};

#endif //MATERIAL_MANAGER_H