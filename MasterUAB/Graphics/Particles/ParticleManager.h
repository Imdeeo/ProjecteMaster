#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "ParticleSystemType.h"

class CParticleManager : public CTemplatedLevelMapManager<CParticleSystemType>
{
private:
	std::string m_Filename;
public:
	CParticleManager(void);
	virtual ~CParticleManager(void);
	void Load(const std::string &Filename, const std::string &_LevelId);
	void Reload();
	void Save();
};

#endif //PARTICLE_MANAGER_H