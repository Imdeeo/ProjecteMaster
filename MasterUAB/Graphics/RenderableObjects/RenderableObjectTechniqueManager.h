#ifndef H_RENDERABLEOBJECTTECHNIQUEMANAGER_H
#define H_RENDERABLEOBJECTTECHNIQUEMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "RenderableObjectTechnique.h"

#include <string>

class CPoolRenderableObjectTechnique;

class CRenderableObjectTechniqueManager : public CTemplatedMapManager<CRenderableObjectTechnique>
{
private:
	CTemplatedMapManager<CPoolRenderableObjectTechnique> m_PoolRenderableObjectTechniques;
	std::string m_Filename;
public:
	CRenderableObjectTechniqueManager();
	virtual ~CRenderableObjectTechniqueManager();
	bool InsertRenderableObjectTechnique(CPoolRenderableObjectTechnique *PoolRenderableObjectTechniques, const std::string &RenderableObjectTechniqueName, const std::string &TechniqueName);
	void Destroy();
	bool Load(const std::string &Filename);
	bool Reload();
	CTemplatedMapManager<CPoolRenderableObjectTechnique> & GetPoolRenderableObjectTechniques(){ return m_PoolRenderableObjectTechniques;}
};

#endif // H_RENDERABLEOBJECTTECHNIQUEMANAGER_H