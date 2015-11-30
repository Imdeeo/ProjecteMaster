#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <d3d11.h>
#include "Camera.h"
#include "Frustum.h"
#include "RenderableObject.h"
#include "Utils.h"

class CContextManager;
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

	bool AddRenderableObjectToRenderList(const CRenderableObject* _RenderableObject);

	void Render(CContextManager* _Context, CMaterialManager* _MaterialManager);

private:

	CCamera m_CurrentCamera;
	CFrustum m_CullFrustum;
	CCamera m_DebugCamera;
	bool m_UseDebugCamera;

	size_t m_CurrentRenderlistLength;
	std::vector<const CRenderableObject*> m_RenderableObjects;

	UAB_BUILD_GET_SET(CContextManager*,ContextManager);

	ID3D11Device GetDevice const()
	{
		return m_ContextManager->GetDevice();
	}

};

#endif //RENDER_MANAGER_H