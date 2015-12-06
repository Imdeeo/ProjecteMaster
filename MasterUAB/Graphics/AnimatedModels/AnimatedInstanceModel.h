#ifndef ANIMATED_INSTANCE_MODEL_H
#define ANIMATED_INSTANCE_MODEL_H

#include "RenderableObjects\RenderableObject.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "AnimatedCoreModel.h"

class CAnimatedInstanceModel : public CRenderableObject
{
private:
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
	void ExecuteAction(int Id, float DelayIn, float DelayOut, float	WeightTarget=1.0f, bool AutoLock=true);
	void BlendCycle(int Id, float Weight, float DelayIn);
	void ClearCycle(int Id, float DelayOut);
	bool IsCycleAnimationActive(int Id) const;
	bool IsActionAnimationActive(int Id) const;
};
#endif //ANIMATED_INSTANCE_MODEL_H
