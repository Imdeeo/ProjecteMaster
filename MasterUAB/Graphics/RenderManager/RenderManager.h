#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Camera\Frustum.h"
#include "Camera\Camera.h"

class CEffectTechnique;
class CTexture;
class CColor;

class CMaterialManager;

#ifdef _DEBUG
class CDebugRender;
#else
class CRenderHelper;
#endif

class CContextManager;

class ID3D11BlendState;
class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;

class ID3D11RenderTargetView;
class ID3D11DepthStencilView;

class CEmptyPointerClass;

class CRenderManager
{
public:
	CRenderManager();
	virtual ~CRenderManager();

	void Init();

	void SetCurrentCamera(const CCamera& _CurrentCamera);
	const CCamera *GetCurrentCamera();
	const CFrustum* GetFrustum();
	void SetDebugCamera(const CCamera& _DebugCamera) { m_DebugCamera = _DebugCamera; }

	void SetUseDebugCamera(bool _use) { m_UseDebugCamera = _use; }

	void EnableBlendState(ID3D11BlendState* _blendState);
	void EnableAlphaBlendState();
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

	//funcion para sacar posicion en pantalla de la luz desde la camara
	Vect2f GetScreenPosFrom3D(const Vect3f &Position) const;

	//void Init();
#ifdef _DEBUG
	CDebugRender* GetDebugRender()const;
#else
	CRenderHelper* GetRenderHelper()const;
#endif

	CContextManager* GetContextManager()const;
	void SetContextManager(CContextManager* _ContextManager);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain*	GetSwapChain();

	CEmptyPointerClass *GetFrameRateAddress();


private:

	CCamera							m_CurrentCamera;
	CFrustum						m_CullFrustum;
	CCamera							m_DebugCamera;
	bool							m_UseDebugCamera;

#ifdef _DEBUG
	CDebugRender*					m_DebugRender;
#else
	CRenderHelper*					m_RenderHelper;
#endif

	//ID3D11RenderTargetView* const	m_RenderTargetView;
	//ID3D11DepthStencilView*			m_DepthStencilView;


	size_t							m_CurrentRenderlistLength;
	
	CContextManager*				m_ContextManager;

	float m_FrameRate;
	unsigned int m_FrameCounter;
	float m_LastFPSMeasurementTime;
};

#endif //RENDER_MANAGER_H