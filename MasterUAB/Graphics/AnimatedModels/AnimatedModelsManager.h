#ifndef ANIMATED_MODELS_MANAGER_H
#define ANIMATED_MODELS_MANAGER_H


#include "TemplatedMapManager.h"
#include "AnimatedCoreModel.h"

#include <string.h>

class CMaterial;

class CAnimatedModelsManager: public CTemplatedMapManager<CAnimatedCoreModel>
{
private:
	std::string m_Filename;
public:
	CAnimatedModelManager();
	virtual ~CAnimatedModelManager();
	bool Load(const std::string &Filename);
	bool Reload();
};

#endif //ANIMATED_MODELS_MANAGER_H
