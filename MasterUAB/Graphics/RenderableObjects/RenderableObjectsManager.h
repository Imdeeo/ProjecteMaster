#ifndef H_RENDERABLEOBJECTMANAGER_H
#define H_RENDERABLEOBJECTMANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "RenderableObject.h"
#include "Utils\Named.h"
#include "XML\tinyxml2.h"

#include <string>

class CLevel;

class CRenderableObjectsManager : public CTemplatedLevelVectorMapManager<CRenderableObject>, public CNamed
{
private:
	std::string m_Filename;
	std::string m_LevelName;
public:
	CRenderableObjectsManager(std::string _Name);
	virtual ~CRenderableObjectsManager();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	void RenderDebug(CRenderManager *RM);
	CRenderableObject * AddMeshInstance(tinyxml2::XMLElement* TreeNode, CLevel *_Level, bool _Update = false);
	CRenderableObject * AddMeshInstance(const std::string &CoreMeshName,
		const std::string &InstanceName, const Vect3f &Position, CLevel* _Level,
		const Quatf _Rotation = Quatf(0.0f, 0.0f, 0.0f, 0.0f),
		const float _Scale = 1.f, const bool _Visible = true, bool _Update = false);
	CRenderableObject * AddAnimatedInstanceModel(tinyxml2::XMLElement* TreeNode, CLevel *_Level, bool _Update = false);
	CRenderableObject * AddAnimatedInstanceModel(const std::string &CoreModelName,
		const std::string &InstanceModelName, const Vect3f &Position, CLevel *_Level, bool _Update = false);

	CRenderableObjectsManager & CRenderableObjectsManager::operator=(CRenderableObjectsManager&);
	CRenderableObject * AddParticleSystemInstance(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	CRenderableObject * AddManchasSystemInstance(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	CRenderableObject * AddBilboardSystemInstance(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	CRenderableObject * AddLineRendererSystemInstance(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
};

#endif