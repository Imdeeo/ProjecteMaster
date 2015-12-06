#include "AnimatedInstanceModel.h"

#include "Math/Matrix44.h"

#include "Math/Quatn.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

#include "RenderableObjects\VertexTypes.h"
#include "RenderableObjects\TemplatedRenderableIndexedVertexs.h"

CAnimatedInstanceModel::CAnimatedInstanceModel(CXMLTreeNode &TreeNode):CRenderableObject(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	SetName(l_Element.GetPszProperty("name"));
	Initialize(UABEngine.GetAnimatedModelsManager()->GetResource(l_Element.GetPszProperty("core_model_name")));
	/*m_AnimatedCoreModel = UABEngine.GetAnimatedModelsManager()->GetResource(l_Element.GetPszProperty("core_model_name"));
	m_CalModel = new CalModel(m_AnimatedCoreModel->GetCalCoreModel());
	m_CalHardwareModel = new CalHardwareModel(m_AnimatedCoreModel->GetCalCoreModel());*/
	m_RenderableVertexs = nullptr;
}
CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
	Destroy();
}


bool CAnimatedInstanceModel::LoadVertexBuffer()
{
	m_NumVertices=0;
	m_NumFaces=0;
	CalCoreModel *l_CalCoreModel=m_AnimatedCoreModel->GetCalCoreModel();
	for(int i=0; i<l_CalCoreModel->getCoreMeshCount(); ++i)
	{
		CalCoreMesh *l_CalCoreMesh=l_CalCoreModel->getCoreMesh(i);
		for(int j=0; j<l_CalCoreMesh->getCoreSubmeshCount(); ++j)
		{
			CalCoreSubmesh *l_CalCoreSubmesh=l_CalCoreMesh->getCoreSubmesh(j);
			m_NumVertices+=l_CalCoreSubmesh->getVertexCount();
			m_NumFaces+=l_CalCoreSubmesh->getFaceCount();
		}
	}
	MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX* l_Vertexs=
		(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX*)malloc
		(m_NumFaces*3*sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	CalIndex *l_Faces=(CalIndex *)malloc(m_NumFaces*3*sizeof(CalIndex));
	m_CalHardwareModel->setVertexBuffer((char*)l_Vertexs,sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setWeightBuffer(((char*)l_Vertexs) + 12,sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vertexs) + 28,sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setNormalBuffer(((char*)l_Vertexs)+44,sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setTextureCoordNum(1);
	m_CalHardwareModel->setTextureCoordBuffer(0,((char*)l_Vertexs)+56,sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setIndexBuffer(l_Faces);
	m_CalHardwareModel->load( 0, 0, MAXBONES);
	m_NumFaces=m_CalHardwareModel->getTotalFaceCount();
	m_NumVertices=m_CalHardwareModel->getTotalVertexCount();
	if(sizeof(CalIndex)==2)
		m_RenderableVertexs=new	CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_Vertexs, m_NumVertices, l_Faces, m_NumFaces*3);
	else
		m_RenderableVertexs=new	CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_Vertexs, m_NumVertices, l_Faces, m_NumFaces*3);
	free(l_Vertexs);
	free(l_Faces);
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
void CAnimatedInstanceModel::Render(CRenderManager *RenderManager)
{
	Mat44f l_Transform=GetTransform();
	CEffectManager::m_Parameters.m_World=GetTransform();
	for(int l_HardwareMeshId=0; l_HardwareMeshId<m_CalHardwareModel->getHardwareMeshCount(); ++l_HardwareMeshId)
	{
		m_Materials[l_HardwareMeshId]->Apply();
		m_CalHardwareModel->selectHardwareMesh(l_HardwareMeshId);
		Mat44f l_Transformations[MAXBONES];
		for(int l_BoneId=0; l_BoneId<m_CalHardwareModel->getBoneCount();++l_BoneId)
		{
			Quatf l_Quaternion=(const Quatf &)m_CalHardwareModel->getRotationBoneSpace(l_BoneId, m_CalModel->getSkeleton());
			l_Transformations[l_BoneId].SetIdentity();
			l_Transformations[l_BoneId].SetRotByQuat(l_Quaternion);
			CalVector translationBoneSpace=m_CalHardwareModel->getTranslationBoneSpace(l_BoneId, m_CalModel->getSkeleton());
			l_Transformations[l_BoneId].SetPos((const Vect3f&)(translationBoneSpace.x));
		}
		memcpy(&CEffectManager::m_Parameters.m_Bones, l_Transformations,MAXBONES*sizeof(float)*4*4);
		m_RenderableVertexs->RenderIndexed(RenderManager,m_Materials[l_HardwareMeshId]->GetEffectTechnique(),
			&CEffectManager::m_Parameters,m_CalHardwareModel->getFaceCount()*3, m_CalHardwareModel->getStartIndex(),
			m_CalHardwareModel->getBaseVertexIndex());
	}
}
void CAnimatedInstanceModel::Update(float ElapsedTime)
{
	m_CalModel->update(ElapsedTime);
}
void CAnimatedInstanceModel::Destroy()
{
	delete m_CalModel;
	delete m_CalHardwareModel;
	m_Materials.clear();
	delete m_RenderableVertexs;
}
void CAnimatedInstanceModel::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget, bool AutoLock)
{
	m_CalModel->getMixer()->executeAction(Id,DelayIn,DelayOut,WeightTarget,AutoLock);
}
void CAnimatedInstanceModel::BlendCycle(int Id, float Weight, float DelayIn)
{
	m_CalModel->getMixer()->blendCycle(Id, Weight, DelayIn);
}
void CAnimatedInstanceModel::ClearCycle(int Id, float DelayOut)
{
	m_CalModel->getMixer()->clearCycle(Id, DelayOut);
}
bool CAnimatedInstanceModel::IsCycleAnimationActive(int Id) const
{
	return m_CalModel->getMixer()->getAnimationVector()[Id]->getState() == CalAnimation::STATE_IN;
}
bool CAnimatedInstanceModel::IsActionAnimationActive(int Id) const
{
	return m_CalModel->getMixer()->getAnimationVector()[Id]->getState() == CalAnimation::STATE_IN;
}