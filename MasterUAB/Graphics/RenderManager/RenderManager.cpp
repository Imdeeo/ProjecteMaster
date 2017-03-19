#include "RenderManager\RenderManager.h"

#include "Materials\MaterialManager.h"
#include "RenderableObjects\RenderableObject.h"
#include "RenderableObjects\RenderableVertexs.h"

#include "Engine\UABEngine.h"
#include "ContextManager\ContextManager.h"
#include "DebugHelper\DebugHelper.h"

#include "SceneRender\SceneRendererCommandManager.h"
#include "Effects\EffectManager.h"

#include "LevelManager\LevelManager.h"

#include "Effects\EffectTechnique.h"
#include "Texture\Texture.h"

#include "MutexManager\MutexManager.h"

#ifdef _DEBUG
#include "DebugRender.h"
#else
#include "RenderHelper\RenderHelper.h"
#endif

#define SCREEN_DEPTH 20.f

CRenderManager::CRenderManager()
	: m_UseDebugCamera(false)
	, m_CurrentRenderlistLength(0)
	, m_FrameCounter(0)
	, m_FrameRate(0.0)
#ifdef _DEBUG
	,m_DebugRender(nullptr)
#else
	,m_RenderHelper(nullptr)
#endif
	/*m_RenderTargetView(nullptr),
	m_DepthStencilView(nullptr)	*/
{
	m_LastFPSMeasurementTime = (float)timeGetTime();

}

CRenderManager::~CRenderManager()
{
#ifdef _DEBUG
	CHECKED_DELETE(m_DebugRender);
#else
	CHECKED_DELETE(m_RenderHelper);
#endif
}

void CRenderManager::Init()
{
#ifdef _DEBUG
	m_DebugRender = new CDebugRender(m_ContextManager->GetDevice());
#else
	m_RenderHelper = new CRenderHelper(m_ContextManager->GetDevice());
#endif
}

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

const CCamera *CRenderManager::GetCurrentCamera()
{
	if (m_UseDebugCamera)
		return &m_DebugCamera;
	else
		return &m_CurrentCamera;
}

CFrustum* CRenderManager::GetFrustum()
{
	return &m_CullFrustum;
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
	m_FrameCounter++;
	float l_TimeNow = (float)timeGetTime();
	float l_TimeSinceLastTime = l_TimeNow - m_LastFPSMeasurementTime;
	if (l_TimeSinceLastTime >= 1000.0) {
		m_FrameRate = m_FrameCounter * 1000 / l_TimeSinceLastTime;
		m_FrameCounter = 0;
		m_LastFPSMeasurementTime = l_TimeNow;
	}

	m_ContextManager->BeginRender();

	UABEngine.GetLevelManager()->ExecuteSceneCommands(this);

	m_ContextManager->EndRender();
}

void CRenderManager::EnableBlendState(ID3D11BlendState* _blendState)
{
	UABEngine.GetMutexManager()->g_DeviceContextMutex.lock();
	m_ContextManager->GetDeviceContext()->OMSetBlendState(_blendState, NULL, 0xffffffff);
	UABEngine.GetMutexManager()->g_DeviceContextMutex.unlock();
}
void CRenderManager::EnableAlphaBlendState()
{
	ID3D11BlendState* l_AlphaBlendState = m_ContextManager->GetBlendState(CContextManager::BLEND_ALPHA);
	UABEngine.GetMutexManager()->g_DeviceContextMutex.lock();
	m_ContextManager->GetDeviceContext()->OMSetBlendState(l_AlphaBlendState,NULL,0xffffffff);
	UABEngine.GetMutexManager()->g_DeviceContextMutex.unlock();
}
void CRenderManager::DisableAlphaBlendState()
{
	UABEngine.GetMutexManager()->g_DeviceContextMutex.lock();
	m_ContextManager->GetDeviceContext()->OMSetBlendState(NULL,NULL,0xffffffff);
	UABEngine.GetMutexManager()->g_DeviceContextMutex.unlock();
}

void CRenderManager::Clear(bool renderTarget, bool depthStencil)
{
	m_ContextManager->Clear(renderTarget, depthStencil);
}

void CRenderManager::Present()
{
	m_ContextManager->Present();
}

void CRenderManager::SetMatrixViewProjection()
{
	if (m_UseDebugCamera)
	{
		m_ContextManager->SetCamera(m_DebugCamera);		
		UABEngine.GetEffectManager()->m_SceneParameters.m_CameraPosition = m_DebugCamera.GetPosition();
		UABEngine.GetEffectManager()->m_SceneParameters.m_CameraUpVector = m_DebugCamera.GetUp();
		UABEngine.GetEffectManager()->m_SceneParameters.m_CameraRightVector = m_CurrentCamera.GetRightVector();
	}
	else
	{
		m_ContextManager->SetCamera(m_CurrentCamera);
		UABEngine.GetEffectManager()->m_SceneParameters.m_CameraPosition = m_CurrentCamera.GetPosition();
		UABEngine.GetEffectManager()->m_SceneParameters.m_CameraUpVector = m_CurrentCamera.GetUp();
		UABEngine.GetEffectManager()->m_SceneParameters.m_CameraRightVector = m_CurrentCamera.GetRightVector();
	}
	UABEngine.GetEffectManager()->m_SceneParameters.m_InverseView = UABEngine.GetEffectManager()->m_SceneParameters.m_View;
	UABEngine.GetEffectManager()->m_SceneParameters.m_InverseView.Invert();
	UABEngine.GetEffectManager()->m_SceneParameters.m_InverseProjection = UABEngine.GetEffectManager()->m_SceneParameters.m_Projection;
	UABEngine.GetEffectManager()->m_SceneParameters.m_InverseProjection.Invert();
}

void CRenderManager::SetAntTweakBar()
{
	CDebugHelper::GetDebugHelper()->Render();
}

//Funcones Draw
void CRenderManager::DrawScreenQuad(CEffectTechnique *_EffectTechnique, CTexture *_Texture, float x, float y, float _Width, float _Height, const CColor &Color)
{
	CEffectManager::m_SceneParameters.m_BaseColor=Color;
	if(_Texture!=NULL)
		_Texture->Activate(0);

	D3D11_VIEWPORT *l_CurrentViewport=m_ContextManager->getViewPort();
	D3D11_VIEWPORT l_Viewport;

	l_Viewport.Width = _Width*l_CurrentViewport->Width;
	l_Viewport.Height = _Height*(l_CurrentViewport->Height);
	l_Viewport.MinDepth = 0.0f;
	l_Viewport.MaxDepth = 1.0f;
	l_Viewport.TopLeftX = x*l_CurrentViewport->Width;
	l_Viewport.TopLeftY = y*l_CurrentViewport->Height;
	std::mutex * l_DeviceContextMutex = &(UABEngine.GetMutexManager()->g_DeviceContextMutex);
	l_DeviceContextMutex->lock();
	m_ContextManager->GetDeviceContext()->RSSetViewports(1, &l_Viewport);
	l_DeviceContextMutex->unlock();

	CEffectManager::SetSceneConstants(_EffectTechnique);

#ifdef _DEBUG
	m_DebugRender->GetQuadRV()->Render(this, _EffectTechnique,	CEffectManager::GetRawData());
#else
	m_RenderHelper->GetQuadRV()->Render(this, _EffectTechnique, CEffectManager::GetRawData());
#endif
	l_DeviceContextMutex->lock();
	m_ContextManager->GetDeviceContext()->RSSetViewports(1, l_CurrentViewport);
	l_DeviceContextMutex->unlock();
}


void CRenderManager::SetRenderTargets(int _NumViews, ID3D11RenderTargetView **_RenderTargetViews,
	ID3D11DepthStencilView *_DepthStencilView)
{
	m_ContextManager->SetRenderTargets(_NumViews, _RenderTargetViews, _DepthStencilView);
}

/*void CRenderManager::UnsetRenderTargets()
{
	GetContextManager()->Unset();
	m_ContextManager->GetDeviceContext()->RSSetViewports(1, m_ContextManager->getViewPort());
}*/

Vect2f CRenderManager::GetScreenPosFrom3D(const Vect3f &Position) const
{
	return m_CurrentCamera.GetPositionInScreenCoordinates(Position);
}

CContextManager* CRenderManager::GetContextManager()const
{
	return m_ContextManager;
}
void CRenderManager::SetContextManager(CContextManager* _ContextManager)
{
	m_ContextManager = _ContextManager;
	m_ContextManager->GetDeviceContext()->RSSetState(m_ContextManager->GetRasterizerState(CContextManager::RS_SOLID_BACK_CULL));
	//m_ContextManager->GetDeviceContext()->OMSetBlendState(NULL, NULL, 0xffffffff);
}

ID3D11Device* CRenderManager::GetDevice()
{
	return m_ContextManager->GetDevice();
}

ID3D11DeviceContext* CRenderManager::GetDeviceContext()
{
	return m_ContextManager->GetDeviceContext();
}

IDXGISwapChain*	CRenderManager::GetSwapChain()
{
	return m_ContextManager->GetSwapChain();
}

float CRenderManager::GetFrameRate() {
	return m_FrameRate;
}

CEmptyPointerClass *CRenderManager::GetFrameRateAddress() {
	return (CEmptyPointerClass *) &m_FrameRate;
}

#ifdef _DEBUG
CDebugRender* CRenderManager::GetDebugRender()const
{
	return m_DebugRender;
}
#else
CRenderHelper* CRenderManager::GetRenderHelper()const
{
	return m_RenderHelper;
}
#endif