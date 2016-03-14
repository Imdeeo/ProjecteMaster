#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <string>
#include "Utils\TemplatedMapManager.h"
#include "Materials\Material.h"

class CMaterialManager : public CTemplatedMapManager<CMaterial>
{
private:
	std::string m_LevelMaterialsFilename;
	std::string m_DefaultMaterialsFilename;
	void LoadMaterialsFromFile(const std::string &Filename, bool Update=false, std::map<std::string, std::string> *UpdatedNames=nullptr);
public:
	CMaterialManager();
	virtual ~CMaterialManager();
	void Load(const std::string &Filename, const std::string &DefaultsFileName="");
	void Reload();
};

#endif //MATERIAL_MANAGER_H