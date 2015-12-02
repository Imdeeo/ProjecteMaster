#ifndef H_RENDERABLEOBJECTMANAGER_H
#define H_RENDERABLEOBJECTMANAGER_H

#include "TemplatedVectorMapManager.h"
#include "RenderableObject.h"

class CXMLTreeNode;

class CRenderableObjectsManager : public CTemplatedVectorMapManager<CRenderableObject>
{
	public:
		CRenderableObjectsManager();
		virtual ~CRenderableObjectsManager();
		void Update(float ElapsedTime);
		void Render(CRenderManager *RM);
		CRenderableObject * AddMeshInstance(CXMLTreeNode &TreeNode);
		CRenderableObject * AddMeshInstance(const std::string &CoreMeshName,
			const std::string &InstanceName, const Vect3f &Position, const float _Yaw = 0.f,
			const float _Pitch = 0.f, const float _Roll = 0.f,const float _Scale = 1.f,
			const bool _Visible = true);
		CRenderableObject * AddAnimatedInstanceModel(const std::string &CoreModelName,
			const std::string &InstanceModelName, const Vect3f &Position);
		//void CleanUp();
		bool Load(const std::string &FileName);
		//CRenderableObject * GetInstance(const std::string &Name) const;
};

#endif