#ifndef ANIMATED_CORE_MODEL_H
#define ANIMATED_CORE_MODEL_H

#include "Math\Vector3.h"

#include <cal3d/cal3d.h>

#include <string>
#include <vector>

class CAnimatedCoreModel
{
private:
	CalCoreModel *m_CalCoreModel;
	std::vector<CMaterial *> m_Materials;
	std::string m_Path;
	Vect3f m_BSPosition;
	Vect3f m_BSRadius;
	bool LoadMesh(const std::string &Filename);
	bool LoadSkeleton(const std::string &Filename);
	bool LoadAnimation(const std::string &Name, const std::string &Filename);
public:
	CAnimatedCoreModel();
	virtual ~CAnimatedCoreModel();
	CalCoreModel *GetCoreModel();
	const std::vector<CMaterial *> & GetMaterials() const {return m_Materials;}
	void Load(const std::string &Path);
};

#endif //ANIMATED_CORE_MODEL_H
