#ifndef ANIMATED_INSTANCE_MODEL_H
#define ANIMATED_INSTANCE_MODEL_H

#include "RenderableObjects\RenderableObject.h"
#include <map>

class CalModel;
class CalHardwareModel;
class CRenderManager;
class CAnimatedCoreModel;
class CMaterial;
class CRenderableVertexs;
class CXMLTreeNode;

class CAnimatedInstanceModel : public CRenderableObject
{
private:
	int m_ActualCycleAnimation;
	int m_ActualActionAnimation;

	CalModel *m_CalModel;
	CAnimatedCoreModel *m_AnimatedCoreModel;
	CalHardwareModel *m_CalHardwareModel;
	std::vector<CMaterial *> m_Materials;
	CRenderableVertexs *m_RenderableVertexs;
	int m_NumVertices;
	int m_NumFaces;
	bool LoadVertexBuffer();
	void LoadMaterials();

public:
	CAnimatedInstanceModel(CXMLTreeNode &TreeNode);
	virtual ~CAnimatedInstanceModel();
	void Initialize(CAnimatedCoreModel *AnimatedCoreModel);
	void Render(CRenderManager *RenderManager);
	void Update(float ElapsedTime);
	void Destroy();
	void ExecuteAction(int Id, float DelayIn, float DelayOut, float	WeightTarget=1.0f, bool AutoLock=false);
	void RemoveAction(int Id);
	void BlendCycle(int Id, float Weight, float DelayIn);
	void ClearCycle(int Id, float DelayOut);
	bool IsCycleAnimationActive(int Id) const;
	bool IsActionAnimationActive(int Id) const;
	int	 GetActualCycleAnimation()const{return m_ActualCycleAnimation;}
	int	 GetActualActionAnimation()const{return m_ActualActionAnimation;}
	void RenderDebug(CRenderManager *RenderManager);
};
#endif //ANIMATED_INSTANCE_MODEL_H
