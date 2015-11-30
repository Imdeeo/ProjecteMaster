#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <d3d11.h>
#include "Camera.h"
#include "Frustum.h"
#include "RenderableObjectManager.h"
#include "Utils.h"
#include "ContextManager.h"

class CMaterialManager;

class CRenderManager
{
public:
	CRenderManager()
		: m_UseDebugCamera(false)
		, m_CurrentRenderlistLength(0)
	{}

	void SetCurrentCamera(const CCamera& _CurrentCamera);
	void SetDebugCamera(const CCamera& _DebugCamera) { m_DebugCamera = _DebugCamera; }

	void SetUseDebugCamera(bool _use) { m_UseDebugCamera = _use; }

	//bool AddRenderableObjectToRenderList(CRenderableObject* _RenderableObject);

	void Render();

private:

	CCamera m_CurrentCamera;
	CFrustum m_CullFrustum;
	CCamera m_DebugCamera;
	bool m_UseDebugCamera;

	size_t m_CurrentRenderlistLength;
	
	CRenderableObjectsManager m_RenderableObjectManager;

	UAB_BUILD_GET_SET(CContextManager*,ContextManager);

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