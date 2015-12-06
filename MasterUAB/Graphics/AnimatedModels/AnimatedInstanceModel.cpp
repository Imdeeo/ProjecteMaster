#include "AnimatedInstanceModel.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

CAnimatedInstanceModel::CAnimatedInstanceModel(CXMLTreeNode &TreeNode):CRenderableObject(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	SetName(l_Element.GetPszProperty("name"));
	m_AnimatedCoreModel = UABEngine.GetAnimatedModelsManager()->GetResource(l_Element.GetPszProperty("animated_core_name"));
	m_CalModel = new CalModel(m_AnimatedCoreModel->GetCalCoreModel());
	m_CalHardwareModel = new CalHardwareModel(m_AnimatedCoreModel->GetCalCoreModel());
	m_RenderableVertexs = nullptr;
}
CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
}


bool CAnimatedInstanceModel::LoadVertexBuffer()
{
	return true;
}

void CAnimatedInstanceModel::LoadMaterials()
{
	std::vector <CMaterial *> l_Materials = m_AnimatedCoreModel->GetMaterials();
	for(int i = 0; i<l_Materials.size(); i++)
	{
		m_Materials.push_back(l_Materials[i]);
	}
}

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel)
{
	m_AnimatedCoreModel = AnimatedCoreModel;
	m_CalModel = new CalModel(m_AnimatedCoreModel->GetCalCoreModel());
	m_CalHardwareModel = new CalHardwareModel(m_AnimatedCoreModel->GetCalCoreModel());
}
void CAnimatedInstanceModel::Render(CRenderManager &RenderManager)
{
}
void CAnimatedInstanceModel::Update(float ElapsedTime)
{
}
void CAnimatedInstanceModel::Destroy()
{
}
void CAnimatedInstanceModel::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget=1.0f, bool AutoLock=true)
{
}
void CAnimatedInstanceModel::BlendCycle(int Id, float Weight, float DelayIn)
{
}
void CAnimatedInstanceModel::ClearCycle(int Id, float DelayOut)
{
}
bool CAnimatedInstanceModel::IsCycleAnimationActive(int Id) const
{
	return true;
}
bool CAnimatedInstanceModel::IsActionAnimationActive(int Id) const
{
	return true;
}