#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "ParticleSystemType.h"

class CParticleManager : public CTemplatedMapManager<CParticleSystemType>
{
private:
	std::string m_Filename;
public:
	CParticleManager(void);
	virtual ~CParticleManager(void);
	void Load(const std::string &Filename);
	void Reload();
};

#endif //PARTICLE_MANAGER_H