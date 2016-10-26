#include "AnimatedInstanceModel.h"

#include "Math/Matrix44.h"
#include "Math/Quatn.h"
#include "Utils.h"

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

CAnimatedInstanceModel::CAnimatedInstanceModel(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) :CRenderableObject(TreeNode, _LevelId)
{
	Initialize(UABEngine.GetAnimatedModelsManager()->GetResource(TreeNode->GetPszProperty("core_name")));
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

	MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX* l_Vertexs =
		(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX*)malloc
		(m_NumFaces * 3 * sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX));
	CalIndex *l_Faces = (CalIndex *)malloc(m_NumFaces * 3 * sizeof(CalIndex));
	m_CalHardwareModel->setVertexBuffer((char*)l_Vertexs, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX));
	m_CalHardwareModel->setNormalBuffer(((char*)l_Vertexs) + 12, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX));
	m_CalHardwareModel->setWeightBuffer(((char*)l_Vertexs) + 24, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX));
	m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vertexs) + 40, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX));
	m_CalHardwareModel->setTextureCoordNum(1);
	m_CalHardwareModel->setTextureCoordBuffer(0, ((char*)l_Vertexs) + 56, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX));
	m_CalHardwareModel->setTangentSpaceBuffer(1, ((char*)l_Vertexs) + 64, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX));
	m_CalHardwareModel->setIndexBuffer(l_Faces);
	m_CalHardwareModel->load( 0, 0, MAXBONES);
	m_NumFaces=m_CalHardwareModel->getTotalFaceCount();
	m_NumVertices=m_CalHardwareModel->getTotalVertexCount();

	CalcTangentsAndBinormals(l_Vertexs,										//void *VtxsData, 
		(unsigned short*)l_Faces,											//unsigned short *IdxsData, 
		m_NumVertices,															//size_t VtxCount, 
		m_NumFaces * 3,															//size_t IdxCount, 
		sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX),				//size_t VertexStride, 
		offsetof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX, Position),	//size_t GeometryStride, 
		offsetof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX, Normal),	//size_t NormalStride, 
		offsetof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX, Tangent),	//size_t TangentStride,
		offsetof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX, Binormal),	//size_t BiNormalStride,
		offsetof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX, UV));		//size_t TextureCoordsStride

	if(sizeof(CalIndex)==2)
		m_RenderableVertexs = new	CUABTriangleListRenderableIndexed16Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX>(l_Vertexs, m_NumVertices, l_Faces, m_NumFaces * 3);
	else
		m_RenderableVertexs = new	CUABTriangleListRenderableIndexed32Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX>(l_Vertexs, m_NumVertices, l_Faces, m_NumFaces * 3);
	free(l_Vertexs);
	free(l_Faces);
	return true;
}

void CAnimatedInstanceModel::CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t
	VtxCount, size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t
	NormalStride, size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride)
{
	Vect3f* tan1 = new Vect3f[VtxCount * 2];
	Vect3f* tan2 = tan1 + VtxCount;
	ZeroMemory(tan1, VtxCount * sizeof(Vect3f) * 2);
	unsigned char *l_VtxAddress = (unsigned char *)VtxsData;

	for (size_t b = 0; b<IdxCount; b += 3)
	{
		unsigned short i1 = IdxsData[b];
		unsigned short i2 = IdxsData[b + 1];
		unsigned short i3 = IdxsData[b + 2];
		Vect3f *v1 = (Vect3f *)&l_VtxAddress[i1*VertexStride + GeometryStride];
		Vect3f *v2 = (Vect3f *)&l_VtxAddress[i2*VertexStride + GeometryStride];
		Vect3f *v3 = (Vect3f *)&l_VtxAddress[i3*VertexStride + GeometryStride];
		Vect2f *w1 = (Vect2f *)&l_VtxAddress[i1*VertexStride + TextureCoordsStride];
		Vect2f *w2 = (Vect2f *)&l_VtxAddress[i2*VertexStride + TextureCoordsStride];
		Vect2f *w3 = (Vect2f *)&l_VtxAddress[i3*VertexStride + TextureCoordsStride];
		float x1 = v2->x - v1->x;
		float x2 = v3->x - v1->x;
		float y1 = v2->y - v1->y;
		float y2 = v3->y - v1->y;
		float z1 = v2->z - v1->z;
		float z2 = v3->z - v1->z;
		float s1 = w2->x - w1->x;
		float s2 = w3->x - w1->x;
		float t1 = w2->y - w1->y;
		float t2 = w3->y - w1->y;
		float r = 1.0F / (s1 * t2 - s2 * t1);
		Vect3f tdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		Vect3f sdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
		assert(i1<VtxCount);
		assert(i2<VtxCount);
		assert(i3<VtxCount);
		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;
		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (size_t b = 0; b<VtxCount; ++b)
	{
		Vect3f *l_NormalVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + NormalStride];
		Vect3f *l_TangentVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + TangentStride];
		Vect4f *l_TangentVtx4 = (Vect4f *)&l_VtxAddress[b*VertexStride + TangentStride];
		Vect3f *l_BiNormalVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + BiNormalStride];
		const Vect3f& t = tan1[b];
		// Gram-Schmidt orthogonalize
		Vect3f l_VAl = t - (*l_NormalVtx)*((*l_NormalVtx)*t);
		l_VAl.Normalize();
		*l_TangentVtx = l_VAl;
		// Calculate handedness
		Vect3f l_Cross;
		l_Cross = (*l_NormalVtx) ^ (*l_TangentVtx);
		l_TangentVtx4->w = (l_Cross*(tan2[b]))< 0.0f ? -1.0f : 1.0f;
		*l_BiNormalVtx = (*l_NormalVtx) ^ (*l_TangentVtx);
	}
	delete[] tan1;
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
	if (m_Visible)
	{
		CEffectManager::m_SceneParameters.m_World = GetTransform();
		for (int l_HardwareMeshId = 0; l_HardwareMeshId < m_CalHardwareModel->getHardwareMeshCount(); ++l_HardwareMeshId)
		{
			m_Materials[l_HardwareMeshId]->Apply();
			m_CalHardwareModel->selectHardwareMesh(l_HardwareMeshId);
			Mat44f l_Transformations[MAXBONES];
			for (int l_BoneId = 0; l_BoneId < m_CalHardwareModel->getBoneCount(); ++l_BoneId)
			{
				Quatf l_Quaternion = (const Quatf &)m_CalHardwareModel->getRotationBoneSpace(l_BoneId, m_CalModel->getSkeleton());
				l_Transformations[l_BoneId].SetIdentity();
				l_Transformations[l_BoneId].SetRotByQuat(l_Quaternion);
				CalVector translationBoneSpace = m_CalHardwareModel->getTranslationBoneSpace(l_BoneId, m_CalModel->getSkeleton());
				l_Transformations[l_BoneId].SetPos(Vect3f(translationBoneSpace.x, translationBoneSpace.y, translationBoneSpace.z));
			}
			memcpy(&CEffectManager::m_AnimatedModelEffectParameters.m_Bones, l_Transformations, MAXBONES*sizeof(float) * 4 * 4);

			CEffectTechnique* l_EffectTechnique = m_Materials[l_HardwareMeshId]->GetRenderableObjectTechnique()->GetEffectTechnique();
			ID3D11Buffer *l_AnimationConstantBufferVS = l_EffectTechnique->GetVertexShader()->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);
			ID3D11Buffer *l_AnimationConstantBufferPS = l_EffectTechnique->GetPixelShader()->GetConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID);

			_RenderManager->GetDeviceContext()->UpdateSubresource(l_AnimationConstantBufferVS, 0, NULL, &(CEffectManager::m_AnimatedModelEffectParameters), 0, 0);
			_RenderManager->GetDeviceContext()->UpdateSubresource(l_AnimationConstantBufferPS, 0, NULL, &(CEffectManager::m_AnimatedModelEffectParameters), 0, 0);

			CEffectManager::SetSceneConstants(l_EffectTechnique);
			m_RenderableVertexs->RenderIndexed(_RenderManager, l_EffectTechnique, CEffectManager::GetRawData(),
				m_CalHardwareModel->getFaceCount() * 3, m_CalHardwareModel->getStartIndex(),
				m_CalHardwareModel->getBaseVertexIndex());
		}
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
	
	std::vector<CalAnimation*> l_Vector = m_CalModel->getMixer()->getAnimationVector();
	CalAnimation* l_Animation = l_Vector.at(Id);

	if (NULL != l_Animation && l_Animation->getType() == CalAnimation::TYPE_ACTION &&
		l_Animation->getState() == CalAnimation::STATE_IN)
		return true;
	else
		return false;
}

bool CAnimatedInstanceModel::IsActionAnimationEnded(int Id) const
{
	std::vector<CalAnimation*> l_Vector = m_CalModel->getMixer()->getAnimationVector();
	CalAnimation* l_Animation = l_Vector.at(Id);

	if (NULL != l_Animation && l_Animation->getType() == CalAnimation::TYPE_ACTION &&
		m_CalModel->getMixer()->getAnimationTime() == m_CalModel->getMixer()->getAnimationDuration())
		return true;
	else
		return false;
}

float CAnimatedInstanceModel::GetAnimationTime() const
{
	return m_CalModel->getMixer()->getAnimationTime();
}

float CAnimatedInstanceModel::GetAnimationDuration() const
{
	return m_CalModel->getMixer()->getAnimationDuration();
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

Vect3f CAnimatedInstanceModel::GetRightObjectPosition()
{
	CalVector l_BonePos = m_CalModel->getSkeleton()->getBone(RIGHT_OBJECT_BONE_ID)->getTranslationAbsolute();
	return Vect3f(l_BonePos.x, l_BonePos.y, l_BonePos.z);
}

Vect3f CAnimatedInstanceModel::GetLeftObjectPosition()
{
	CalVector l_BonePos = m_CalModel->getSkeleton()->getBone(LEFT_OBJECT_BONE_ID)->getTranslationAbsolute();
	return Vect3f(l_BonePos.x, l_BonePos.y, l_BonePos.z);
}

Quatf CAnimatedInstanceModel::GetRightObjectRotation()
{
	CalQuaternion l_BonePos = m_CalModel->getSkeleton()->getBone(RIGHT_OBJECT_BONE_ID)->getRotationAbsolute();
	return Quatf(l_BonePos.x, l_BonePos.y, l_BonePos.z, l_BonePos.w);
}

Quatf CAnimatedInstanceModel::GetLeftObjectRotation()
{
	CalQuaternion l_BonePos = m_CalModel->getSkeleton()->getBone(LEFT_OBJECT_BONE_ID)->getRotationAbsolute();
	return Quatf(l_BonePos.x, l_BonePos.y, l_BonePos.z, l_BonePos.w);
}

Mat33f CAnimatedInstanceModel::GetRightObjectTransform()
{
	CalMatrix l_BonePos = m_CalModel->getSkeleton()->getBone(RIGHT_OBJECT_BONE_ID)->getTransformMatrix();
	return Mat33f(l_BonePos.dxdx, l_BonePos.dydx, l_BonePos.dzdx, l_BonePos.dxdy, l_BonePos.dydy, l_BonePos.dzdy, l_BonePos.dxdz, l_BonePos.dydz, l_BonePos.dzdz);
}

Mat33f CAnimatedInstanceModel::GetLeftObjectTransform()
{
	CalMatrix l_BonePos = m_CalModel->getSkeleton()->getBone(LEFT_OBJECT_BONE_ID)->getTransformMatrix();
	return Mat33f(l_BonePos.dxdx, l_BonePos.dydx, l_BonePos.dzdx, l_BonePos.dxdy, l_BonePos.dydy, l_BonePos.dzdy, l_BonePos.dxdz, l_BonePos.dydz, l_BonePos.dzdz);
}

Vect3f CAnimatedInstanceModel::GetBonePosition(int _bone)
{
	CalVector aux = m_CalModel->getSkeleton()->getBone(_bone)->getTranslationAbsolute();
	return Vect3f(aux.x, aux.y, aux.z);
}

Quatf CAnimatedInstanceModel::GetBoneRotation(int _bone)
{
	CalQuaternion aux = m_CalModel->getSkeleton()->getBone(_bone)->getRotation();
	return Quatf(aux.x, aux.y, aux.z, aux.w);
}

void CAnimatedInstanceModel::SetBoneRotation(Quatf _rotation, int _bone)
{
	CalQuaternion aux;
	aux.x = _rotation.x;
	aux.y = _rotation.y;
	aux.z = _rotation.z;
	aux.w = _rotation.w;
	m_CalModel->getSkeleton()->getBone(_bone)->setRotation(aux);
	m_CalModel->getSkeleton()->getBone(_bone)->calculateState();
}

void CAnimatedInstanceModel::PrintBoneList()
{
	std::vector<CalBone *> l_BoneList = m_CalModel->getSkeleton()->getVectorBone();
	for (size_t i = 0; i < l_BoneList.size(); i++) {
		UtilsLog("Bone #" + std::to_string(i) + ": " + l_BoneList[i]->getCoreBone()->getName());
	}
}

void CAnimatedInstanceModel::RemoveAnimations()
{
	if (m_ActualActionAnimation != -1)
	{
		RemoveAction(m_ActualActionAnimation);
	}
	if (m_ActualCycleAnimation != -1)
	{
		ClearCycle(m_ActualCycleAnimation, 0.0);
	}
}
