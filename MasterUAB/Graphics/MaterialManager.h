#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <string>
#include "TemplatedMapManager.h"
#include "Material.h"

class CMaterialManager : public CTemplatedMapManager<CMaterial>
{
private:
	std::string m_Filename;
public:
	CMaterialManager();
	virtual ~CMaterialManager();
	void Load(const std::string &Filename);
	void Reload();
};

#endif //MATERIAL_MANAGER_H