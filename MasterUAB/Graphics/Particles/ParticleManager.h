#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "ParticleSystemType.h"

class CLevel;

class CParticleManager : public CTemplatedLevelMapManager<CParticleSystemType>
{
private:
	std::string m_Filename;
	std::string m_LevelName;
public:
	CParticleManager();
	virtual ~CParticleManager();
	void Load(const std::string &Filename, CLevel* _Level);
	void Reload();
	void Save();
};

#endif //PARTICLE_MANAGER_H