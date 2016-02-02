#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "DebugRender.h"
#include "Camera\Frustum.h"
#include "RenderableObjects\RenderableObjectManager.h"
#include "Utils.h"
#include "ContextManager\ContextManager.h"

class CMaterialManager;

class CRenderManager
{
public:
	CRenderManager();
	virtual ~CRenderManager();

	void SetCurrentCamera(const CCamera& _CurrentCamera);
	void SetDebugCamera(const CCamera& _DebugCamera) { m_DebugCamera = _DebugCamera; }

	void SetUseDebugCamera(bool _use) { m_UseDebugCamera = _use; }

	//bool AddRenderableObjectToRenderList(CRenderableObject* _RenderableObject);

	void Render();

	void Init();

private:

	CCamera m_CurrentCamera;
	CFrustum m_CullFrustum;
	CCamera m_DebugCamera;
	bool m_UseDebugCamera;

	size_t m_CurrentRenderlistLength;
	
	CRenderableObjectsManager m_RenderableObjectManager;

	CDebugRender * m_DebugRender;
	
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

};

#endif //RENDER_MANAGER_H