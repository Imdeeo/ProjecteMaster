#include "AnimatedInstanceModel.h"

#include "Math/Matrix44.h"
#include "Math/Quatn.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "Effects\EffectManager.h"

#include "AnimatedModelsManager.h"
#include "AnimatedCoreModel.h"

#include "RenderableObjects\RenderableVertexs.h"
#include "RenderableObjects\VertexTypes.h"
#include "RenderableObjects\TemplatedRenderableIndexedVertexs.h"
#include "RenderableObjects\RenderableObjectTechnique.h"

#include "Materials\Material.h"

#include <cal3d\cal3d.h>

CAnimatedInstanceModel::CAnimatedInstanceModel(CXMLTreeNode &TreeNode):CRenderableObject(TreeNode)
{
	Initialize(UABEngine.GetAnimatedModelsManager()->GetResource(TreeNode.GetPszProperty("core_name")));
	/*m_AnimatedCoreModel = UABEngine.GetAnimatedModelsManager()->GetResource(l_Element.GetPszProperty("core_model_name"));
	m_CalModel = new CalModel(m_AnimatedCoreModel->GetCalCoreModel());
	m_CalHardwareModel = new CalHardwareModel(m_AnimatedCoreModel->GetCalCoreModel());*/
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
	m_CalHardwareModel->setNormalBuffer(((char*)l_Vertexs) + 12, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setWeightBuffer(((char*)l_Vertexs) + 24,sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vertexs) + 40,sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));	
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
	for(size_t i = 0; i<l_Materials.size(); i++)
	{
		m_Materials.push_back(l_Materials[i]);
	}
}

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel)
{
	m_AnimatedCoreModel = AnimatedCoreModel;
	m_CalModel = new CalModel(m_AnimatedCoreModel->GetCalCoreModel());
	m_CalHardwareModel = new CalHardwareModel(m_AnimatedCoreModel->GetCalCoreModel());
	
	// attach all meshes to the model
	//int meshId;
	//for (meshId = 0; meshId < AnimatedCoreModel->GetCalCoreModel()->getCoreMeshCount(); meshId++)
	//{
	//	m_CalModel->attachMesh(meshId);
	//}

	LoadVertexBuffer();
	LoadMaterials();

	//BlendCycle(1, 1.0f, 0.0f);
	Update(0.0f);
}

void CAnimatedInstanceModel::Render(CRenderManager *_RenderManager)
{
	CEffectManager::m_SceneParameters.m_World=GetTransform();
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
			l_Transformations[l_BoneId].SetPos(Vect3f(translationBoneSpace.x,translationBoneSpace.y,translationBoneSpace.z));
		}
		memcpy(&CEffectManager::m_AnimatedModelEffectParameters.m_Bones, l_Transformations,MAXBONES*sizeof(float)*4*4);

		CEffectTechnique* l_EffectTechnique = m_Materials[l_HardwareMeshId]->GetRenderableObjectTechnique()->GetEffectTechnique();
		ID3D11Buffer *l_AnimationConstantBufferVS = l_EffectTechnique->GetVertexShader()->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);
		ID3D11Buffer *l_AnimationConstantBufferPS = l_EffectTechnique->GetPixelShader()->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);

		_RenderManager->GetDeviceContext()->UpdateSubresource(l_AnimationConstantBufferVS, 0, NULL, &(CEffectManager::m_AnimatedModelEffectParameters), 0, 0);
		_RenderManager->GetDeviceContext()->UpdateSubresource(l_AnimationConstantBufferPS, 0, NULL, &(CEffectManager::m_AnimatedModelEffectParameters), 0, 0);

		CEffectManager::SetSceneConstants(l_EffectTechnique);
		m_RenderableVertexs->RenderIndexed(_RenderManager, l_EffectTechnique, CEffectManager::GetRawData(), 
			m_CalHardwareModel->getFaceCount()*3, m_CalHardwareModel->getStartIndex(),
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
	m_ActualActionAnimation = Id;
	m_CalModel->getMixer()->executeAction(Id,DelayIn,DelayOut,WeightTarget,AutoLock);
}
void CAnimatedInstanceModel::BlendCycle(int Id, float Weight, float DelayIn)
{
	m_ActualCycleAnimation = Id;
	m_CalModel->getMixer()->blendCycle(Id, Weight, DelayIn);
}
void CAnimatedInstanceModel::ClearCycle(int Id, float DelayOut)
{
	m_ActualCycleAnimation = -1;
	m_CalModel->getMixer()->clearCycle(Id, DelayOut);
}
void CAnimatedInstanceModel::RemoveAction(int Id)
{
	m_ActualActionAnimation = -1;
	m_CalModel->getMixer()->removeAction(Id);
}
bool CAnimatedInstanceModel::IsCycleAnimationActive(int Id) const
{
	return m_ActualCycleAnimation==Id;
}
bool CAnimatedInstanceModel::IsActionAnimationActive(int Id) const
{
	return m_ActualActionAnimation==Id;
}

void CAnimatedInstanceModel::RenderDebug(CRenderManager *RenderManager)
{
}

CAnimatedInstanceModel & CAnimatedInstanceModel::operator=(CAnimatedInstanceModel& _AnimatedInstanceModel)
{
	*((CRenderableObject*)this) = (CRenderableObject)_AnimatedInstanceModel;
	m_ActualActionAnimation = _AnimatedInstanceModel.m_ActualActionAnimation;
	m_ActualCycleAnimation = _AnimatedInstanceModel.m_ActualCycleAnimation;
	m_ActualActionAnimation = _AnimatedInstanceModel.m_ActualActionAnimation;
	m_CalHardwareModel = _AnimatedInstanceModel.m_CalHardwareModel;
	m_CalModel = _AnimatedInstanceModel.m_CalModel;
	m_Materials = _AnimatedInstanceModel.m_Materials;
	m_Name = _AnimatedInstanceModel.m_Name;
	m_NumFaces = _AnimatedInstanceModel.m_NumFaces;
	m_NumVertices = _AnimatedInstanceModel.m_NumVertices;
	m_RenderableVertexs = _AnimatedInstanceModel.m_RenderableVertexs;
	return *(this);
}

void CAnimatedInstanceModel::Save(FILE* _File, std::string _layer)
{
	fprintf_s(_File, "\t<animated_instance_mesh name=\"%s\" layer=\"%s\" core_name=\"%s\" position=\"%f %f %f\" "
		"rotation=\"%f %f %f %f\" create_physics=\"false\" visible=\"%s\"/>\n",
		m_Name.c_str(), _layer.c_str(), m_AnimatedCoreModel->GetName().c_str(), m_Position.x, m_Position.y, m_Position.z, 
		m_Rotation.x, m_Rotation.y, m_Rotation.z, m_Rotation.w, m_Visible ? "true" : "false");
}
