#ifndef H_RENDERABLEOBJECTMANAGER_H
#define H_RENDERABLEOBJECTMANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "RenderableObject.h"
#include "Utils\Named.h"

#include <string>

class CXMLTreeNode;

class CRenderableObjectsManager : public CTemplatedVectorMapManager<CRenderableObject>, public CNamed
{
private:
	std::string m_Filename;
public:
	CRenderableObjectsManager(std::string _Name);
	virtual ~CRenderableObjectsManager();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	void RenderDebug(CRenderManager *RM);
	CRenderableObject * AddMeshInstance(CXMLTreeNode &TreeNode, bool _Update = false);
	CRenderableObject * AddMeshInstance(const std::string &CoreMeshName,
		const std::string &InstanceName, const Vect3f &Position,
		const Quatf _Rotation = Quatf(0.0f, 0.0f, 0.0f, 0.0f),
		const float _Scale = 1.f, const bool _Visible = true, bool _Update = false);
	CRenderableObject * AddAnimatedInstanceModel(CXMLTreeNode &TreeNode, bool _Update = false);
	CRenderableObject * AddAnimatedInstanceModel(const std::string &CoreModelName,
		const std::string &InstanceModelName, const Vect3f &Position, bool _Update = false);

	CRenderableObjectsManager & CRenderableObjectsManager::operator=(CRenderableObjectsManager&);
	CRenderableObject * AddParticleSystemInstance(CXMLTreeNode &TreeNode);
	CRenderableObject * AddManchasSystemInstance(CXMLTreeNode &TreeNode);
};

#endif