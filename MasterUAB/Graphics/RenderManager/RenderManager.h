#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "DebugRender.h"
#include "Camera\Frustum.h"
#include "Utils.h"
#include "ContextManager\ContextManager.h"

class CEffectTechnique;
class CTexture;

class CMaterialManager;

class CDebugRender;

class CRenderManager
{
public:
	CRenderManager();
	virtual ~CRenderManager();

	void Init();

	void SetCurrentCamera(const CCamera& _CurrentCamera);
	void SetDebugCamera(const CCamera& _DebugCamera) { m_DebugCamera = _DebugCamera; }

	void SetUseDebugCamera(bool _use) { m_UseDebugCamera = _use; }

	void EngableAlphaBlendState();
	void DisableAlphaBlendState();

	//bool AddRenderableObjectToRenderList(CRenderableObject* _RenderableObject);
	//void UnsetRenderTargets();
	void SetRenderTargets(int _NumViews, ID3D11RenderTargetView **_RenderTargetViews,ID3D11DepthStencilView *_DepthStencilView);

	void Render();
	void Clear(bool renderTarget, bool depthStencil);
	void Present();
	void SetAntTweakBar();
	void CRenderManager::SetMatrixViewProjection();

	//funciones de Draw
	void DrawScreenQuad(CEffectTechnique *_EffectTechnique, CTexture *_Texture, float x, float y, float _Width, float _Height, const CColor &Color);

	//void Init();
private:

	CCamera							m_CurrentCamera;
	CFrustum						m_CullFrustum;
	CCamera							m_DebugCamera;
	bool							m_UseDebugCamera;

	CDebugRender*					m_DebugRender;

	//ID3D11RenderTargetView* const	m_RenderTargetView;
	//ID3D11DepthStencilView*			m_DepthStencilView;


	size_t							m_CurrentRenderlistLength;
	
	UAB_GET_PROPERTY(CDebugRender*, DebugRender)
	UAB_BUILD_GET_SET(CContextManager*,ContextManager)



	ID3D11Device* GetDevice ()
	{
		return m_ContextManager->GetDevice();
	}

	ID3D11DeviceContext* GetDeviceContext ()
	{
		return m_ContextManager->GetDeviceContext();
	}

	IDXGISwapChain*	GetSwapChain()
	{
		return m_ContextManager->GetSwapChain();
	}

};

#endif //RENDER_MANAGER_H