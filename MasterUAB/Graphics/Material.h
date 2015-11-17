#pragma once

#include "ContextManager.h"

class CMaterial
{
public:
	CMaterial(CContextManager::ERasterizedState _RasterizerState, CContextManager::EDepthStencilStates _DepthStencilState, CContextManager::EBlendStates _BlendState)
		: m_RasterizerState(_RasterizerState)
		, m_DepthStencilState(_DepthStencilState)
		, m_BlendState(_BlendState)
		, m_DebugSize(1)
		, m_BaseColor(1,1,1,1)
	{}

	void SetDebugSize(float _DebugSize) { m_DebugSize = _DebugSize; }
	void SetBaseColor(const CColor& _BaseColor) { m_BaseColor = _BaseColor; }

	virtual void SetShaderParameters(CContextManager* _Context) const
	{
		_Context->SetDebugSize(m_DebugSize);
		_Context->SetBaseColor(m_BaseColor);
	}

	bool HasBlending() const
	{
		switch (m_BlendState)
		{
		case CContextManager::BLEND_SOLID:
		default:
			return false;
		case CContextManager::BLEND_CLASSIC:
		case CContextManager::BLEND_PREMULT:
			return true;
		}
		// TODO: Copiar el siguiente comentario dentro de CContextManager::EBlendState
		// NOTA: Si se añaden más estados aquí, modificar CMaterial::HasBlending
	}

	CContextManager::ERasterizedState GetRasterizerState() const { return m_RasterizerState; }
	CContextManager::EDepthStencilStates GetDepthStencilState() const { return m_DepthStencilState; }
	CContextManager::EBlendStates GetBlendState() const { return m_BlendState; }

private:
	CContextManager::ERasterizedState m_RasterizerState;
	CContextManager::EDepthStencilStates m_DepthStencilState;
	CContextManager::EBlendStates m_BlendState;

	float m_DebugSize;
	CColor m_BaseColor;
};
