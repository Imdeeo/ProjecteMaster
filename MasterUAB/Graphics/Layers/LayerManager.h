#ifndef H_LAYER_MANAGER_H
#define H_LAYER_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "XML\tinyxml2.h"
#include <string>

class CRenderManager;

class CLayerManager : public CTemplatedVectorMapManager<CRenderableObjectsManager>
{
private:
	std::string m_Filename;
	CRenderableObjectsManager *m_DefaultLayer;
	
	
	CRenderableObjectsManager * AddLayer(tinyxml2::XMLElement* TreeNode, bool _Update = false);
public:
	CLayerManager();
	virtual ~CLayerManager();
	void Destroy();
	void Load(const std::string &Filename);
	void Reload();
	void Update(float ElapsedTime);
	void Save();
	void Render(CRenderManager *RenderManager);
	void Render(CRenderManager *RenderManager, const std::string &LayerName);
#ifdef _DEBUG
	void RenderDebug(CRenderManager *RenderManager, const std::string &LayerName);
#endif
	CRenderableObjectsManager * GetLayer();
	CRenderableObjectsManager * GetLayer(std::string _layer);
	CRenderableObjectsManager * GetLayer(tinyxml2::XMLElement* TreeNode);
};

#endif //H_LAYER_MANAGER_H

