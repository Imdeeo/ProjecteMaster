#ifndef H_RENDERABLEOBJECTMANAGER_H
#define H_RENDERABLEOBJECTMANAGER_H

#include "TemplatedVectorMapManager.h"
#include "RenderableObject.h"

class CRenderableObjectsManager : public CTemplatedVectorMapManager<CRenderableObject>
{
	public:
		CRenderableObjectsManager();
		virtual ~CRenderableObjectsManager();
		void Update(float ElapsedTime);
		void Render(CRenderManager *RM);
		CRenderableObject * AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position);
		CRenderableObject * AddAnimatedInstanceModel(const std::string &CoreModelName,const std::string &InstanceModelName, const Vect3f &Position);
		void AddResource(const std::string &Name, CRenderableObject *RenderableObject);
		//void CleanUp();
		void Load(const std::string &FileName);
		//CRenderableObject * GetInstance(const std::string &Name) const;
};

#endif