#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <string>
#include "Utils\TemplatedMapManager.h"
#include "Materials\Material.h"

class CLevel;

class CMaterialManager : public CTemplatedLevelMapManager<CMaterial>
{
private:
	std::string m_LevelMaterialsFilename;
	std::string m_DefaultMaterialsFilename;
	std::string m_LevelId;
	void LoadMaterialsFromFile(const std::string &Filename, CLevel *_Level, bool Update = false, std::map<std::string, std::string> *UpdatedNames = nullptr);
public:
	CMaterialManager();
	virtual ~CMaterialManager();
	void Load(const std::string &Filename, CLevel *_Level, const std::string &DefaultsFileName = "");
	void Reload();
	void Save();
};

#endif //MATERIAL_MANAGER_H