#ifndef H_LAYER_MANAGER_H
#define H_LAYER_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "RenderManager\RenderManager.h"

#include <string>

class CRenderableObjectsManager;
class CXMLTreeNode;

class CLayerManager : public CTemplatedVectorMapManager<CRenderableObjectsManager>
{
private:
	std::string m_Filename;
	CRenderableObjectsManager *m_DefaultLayer;
	
	
	CRenderableObjectsManager * AddLayer(CXMLTreeNode &TreeNode);
public:
	CLayerManager();
	virtual ~CLayerManager();
	void Destroy();
	void Load(const std::string &Filename);
	void Reload();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RenderManager);
	void Render(CRenderManager *RenderManager, const std::string &LayerName);
	void RenderDebug(CRenderManager *RenderManager, const std::string &LayerName);
	CRenderableObjectsManager * GetLayer();
	CRenderableObjectsManager * GetLayer(CXMLTreeNode &Node);
};

#endif //H_LAYER_MANAGER_H

