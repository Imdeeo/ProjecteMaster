#ifndef EFFECT_SHADER_H
#define EFFECT_SHADER_H
#include "Named.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include "Utils.h"

class CEffectShader : public CNamed
{
protected:
	ID3D11Buffer *m_ConstantBuffer;
	std::string m_Filename;
	std::string m_Preprocessor;
	std::string m_ShaderModel;
	std::string m_EntryPoint;
	virtual void Destroy();
	bool LoadShader(const std::string &Filename, const std::string &EntryPoint,	const std::string &ShaderModel, ID3DBlob **BlobOut);
	bool CreateConstantBuffer();
public:
	CEffectShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectShader();
	virtual bool Load() = 0;
	virtual bool Reload();
};

class CEffectVertexShader : public CEffectShader
{
protected:
	ID3D11VertexShader *m_VertexShader;
	ID3D11InputLayout *m_VertexLayout;
	std::string m_VertexType;
	void Destroy();
public:
	CEffectVertexShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectVertexShader()
	{
		Destroy();
	}
	bool Load();
	UAB_GET_PROPERTY(ID3D11VertexShader*, VertexShader);
	UAB_GET_PROPERTY(ID3D11InputLayout*, VertexLayout);
	UAB_GET_PROPERTY(ID3D11Buffer*, ConstantBuffer);
};

class CEffectPixelShader : public CEffectShader
{
protected:
	ID3D11PixelShader *m_PixelShader;
	void Destroy();
public:
	CEffectPixelShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectPixelShader()
	{
		Destroy();
	}
	bool Load();
	UAB_GET_PROPERTY(ID3D11PixelShader*, PixelShader);
	UAB_GET_PROPERTY(ID3D11Buffer*, ConstantBuffer);
};

#endif //EFFECT_SHADER_H