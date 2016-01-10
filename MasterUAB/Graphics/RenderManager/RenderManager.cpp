#include "RenderManager\RenderManager.h"

#include "Materials\MaterialManager.h"
#include "RenderableObjects\RenderableObject.h"

#include "Engine\UABEngine.h"

#define SCREEN_DEPTH 20.f
void CRenderManager::SetCurrentCamera(const CCamera& _CurrentCamera)
{
	m_CurrentCamera = _CurrentCamera;
	Mat44f Proj = m_CurrentCamera.GetProjection();
	// Calculate the minimum Z distance in the frustum.
	float zMinimum = -Proj.m32 / Proj.m22;
	float r = SCREEN_DEPTH / (SCREEN_DEPTH - zMinimum);
	Proj.m22 = r;
	Proj.m32 = -r * zMinimum;
	Mat44f viewProj = m_CurrentCamera.GetView() * Proj;
	m_CullFrustum.Update(viewProj);
}

//bool CRenderManager::AddRenderableObjectToRenderList(const CRenderableObject* _RenderableObject)
//{
//	// Alguien se atrebe a arreglar el frustum?
//
//	//if (m_CullFrustum.SphereVisible(_RenderableObject->GetTransform().Position, _RenderableObject->GetBoundingRadius()))
//	if (m_CullFrustum.BoxVisible(_RenderableObject->GetBoundingMax()+_RenderableObject->GetTransform().Position, _RenderableObject->GetBoundingMin()+_RenderableObject->GetTransform().Position))
//	{
//		if (m_CurrentRenderlistLength == m_RenderableObjects.size())
//		{
//			m_RenderableObjects.push_back(_RenderableObject);
//		}
//		else
//		{
//			m_RenderableObjects[m_CurrentRenderlistLength]=_RenderableObject;
//		}
//		++m_CurrentRenderlistLength;
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
struct BlendedSubmesh
{
	const CRenderableVertexs* vertices;
	Mat44f world;
	const CMaterial* material;
	Vect3f position;
};

void CRenderManager::Render()
{
	m_ContextManager->BeginRender();

	if (m_UseDebugCamera)
	{
		m_ContextManager->SetCamera(m_DebugCamera);
	}
	else
	{
		m_ContextManager->SetCamera(m_CurrentCamera);
	}
	//Mat44f view,proj;
	//view.SetIdentity();
	//proj.SetIdentity();
	//m_ContextManager->SetCamera(view,proj);
	// TODO crear un vector para objetos transparentes
	std::vector<BlendedSubmesh> l_SubmeshesWithBlend;

	UABEngine.GetRenderableObjectsManager()->Render(this);

	//for (size_t i = 0; i < m_CurrentRenderlistLength; ++i)
	//{
	//	
	//	Mat44f world;
	//	world.SetFromPosAndAnglesYXZ(l_Transform.Position, l_Transform.Yaw, l_Transform.Pitch, l_Transform.Roll);
	//	_Context->SetWorldMatrix(world);

	//	for (int j = 0; j < l_RenderableObject->GetNumSubmeshes(); ++j)
	//	{
	//		const CRenderableObject::SSubmesh& l_Submesh = l_RenderableObject->GetSubmesh(j);
	//		const CMaterial* l_Material = _MaterialManager->GetMaterial(l_Submesh.material);
	//		// TODO no pintar el objeto, sino añadirlo a la lista l_SubmeshesWithBlend si tiene blend
	//		if(l_Material->HasBlending())
	//		{
	//			struct BlendedSubmesh _BlendedSubmesh;
	//			_BlendedSubmesh.material = l_Material;
	//			_BlendedSubmesh.vertices = l_Submesh.vertices;
	//			_BlendedSubmesh.world = world;
	//			_BlendedSubmesh.position = l_Transform.Position;

	//			l_SubmeshesWithBlend.push_back(_BlendedSubmesh);
	//		}
	//		else
	//		{
	//			l_Material->SetShaderParameters(_Context);
	//			_Context->Draw(l_Submesh.vertices, l_Material->GetRasterizerState(), l_Material->GetDepthStencilState(), l_Material->GetBlendState());
	//		}
	//	}
	//}

	//// TODO: Ordenar objetos según la distáncia a la cámara
	//// NOTA: El quicksort es más rápido que el buble sort cuando la lista tiene más de ~100 objetos. NO OS MATÉIS SI NO HACE FALTA.
	//const Vect3f& l_CameraPosition = m_CurrentCamera.GetPosition();
	//for (int i = 0; i < l_SubmeshesWithBlend.size(); ++i)
	//{
	//	for (int j = 0; j < l_SubmeshesWithBlend.size()-i-1; ++j)
	//	{
	//		struct BlendedSubmesh _BlendedSubmeshAux;
	//		float l_DistanceSQ = l_SubmeshesWithBlend[j].position.SqDistance(l_CameraPosition);
	//		float l_DistanceSQ2 = l_SubmeshesWithBlend[j+1].position.SqDistance(l_CameraPosition);
	//		if(l_DistanceSQ<l_DistanceSQ2)
	//		{
	//			_BlendedSubmeshAux = l_SubmeshesWithBlend[j+1];
	//			l_SubmeshesWithBlend[j+1] = l_SubmeshesWithBlend[j];
	//			l_SubmeshesWithBlend[j] = _BlendedSubmeshAux;
	//		}
	//	}
	//}

	//// TODO: Pintar objetos translúcidos
	//for (int i = 0; i < l_SubmeshesWithBlend.size(); ++i)
	//{
	//	BlendedSubmesh l_BlendedSubmesh = l_SubmeshesWithBlend[i];
	//	l_BlendedSubmesh.material->SetShaderParameters(_Context);
	//	_Context->SetWorldMatrix(l_BlendedSubmesh.world);
	//	_Context->Draw(l_BlendedSubmesh.vertices, l_BlendedSubmesh.material->GetRasterizerState(), l_BlendedSubmesh.material->GetDepthStencilState(), l_BlendedSubmesh.material->GetBlendState());
	//
	//}

	m_CurrentRenderlistLength = 0;

	m_ContextManager->EndRender();
}
