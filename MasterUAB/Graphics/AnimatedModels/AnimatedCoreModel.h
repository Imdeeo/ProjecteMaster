#ifndef ANIMATED_CORE_MODEL_H
#define ANIMATED_CORE_MODEL_H

#include "Math\Vector3.h"
#include "Utils\Named.h"

#include <map>
#include <vector>
#include <string>

class CalCoreModel;
class CMaterial;

class CAnimatedCoreModel: public CNamed
{
private:
	CalCoreModel *m_CalCoreModel;
	std::vector<CMaterial *> m_Materials;
	std::string m_Path;
	Vect3f m_BSPosition;
	std::map<std::string,int> m_AnimationsIds;
	float m_BSRadius;
	bool LoadMesh(const std::string &Filename);
	bool LoadSkeleton(const std::string &Filename);
	bool LoadAnimation(const std::string &Name, const std::string &Filename);

	void Destroy();
public:
	CAnimatedCoreModel();
	virtual ~CAnimatedCoreModel();

	CalCoreModel* GetCalCoreModel();
	const std::vector<CMaterial *> & GetMaterials() const {return m_Materials;}
	bool Load(const std::string &Path);
};

#endif //ANIMATED_CORE_MODEL_H
