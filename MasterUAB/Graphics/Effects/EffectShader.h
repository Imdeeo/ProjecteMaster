#ifndef EFFECT_SHADER_H
#define EFFECT_SHADER_H
#include "Utils\Named.h"
#include "Utils.h"

#include <d3dcommon.h>
#include <d3d11.h>

#define SCENE_CONSTANT_BUFFER_ID	0
#define LIGHT_CONSTANT_BUFFER_ID	1
#define ANIMATED_CONSTANT_BUFFER_ID	2

class CEffectShader : public CNamed
{
protected:
	std::vector<ID3D11Buffer *> m_ConstantBuffers;
	std::string m_Preprocessor;
	std::string m_ShaderModel;
	std::string m_EntryPoint;
	std::string m_Filename;
	std::vector<std::string> m_PreprocessorMacros;
	D3D10_SHADER_MACRO *m_ShaderMacros;

	virtual void Destroy();
	void CreateShaderMacro();
	bool LoadShader(const std::string &Filename, const std::string &EntryPoint,	const std::string &ShaderModel, ID3DBlob **BlobOut);
	bool CreateConstantBuffer(int IdBuffer, unsigned int BufferSize);
	bool CreateConstantBuffer();
public:
	CEffectShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectShader();
	virtual bool Load() = 0;
	virtual bool Reload();
	virtual void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer) = 0;
	ID3D11Buffer * GetConstantBuffer(unsigned int IdBuffer);
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
	virtual ~CEffectVertexShader() { Destroy(); }

	bool Load();
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);

	UAB_GET_PROPERTY(ID3D11VertexShader*, VertexShader);
	UAB_GET_PROPERTY(ID3D11InputLayout*, VertexLayout);
	//UAB_GET_PROPERTY(ID3D11Buffer*, ConstantBuffer);
};

class CEffectPixelShader : public CEffectShader
{
protected:
	ID3D11PixelShader *m_PixelShader;

	void Destroy();
public:
	CEffectPixelShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectPixelShader() { Destroy(); }

	bool Load();
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);

	UAB_GET_PROPERTY(ID3D11PixelShader*, PixelShader);
	//UAB_GET_PROPERTY(ID3D11Buffer*, ConstantBuffer);
};

#endif //EFFECT_SHADER_H