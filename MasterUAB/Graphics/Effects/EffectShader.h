#ifndef EFFECT_SHADER_H
#define EFFECT_SHADER_H

#include "Utils\Named.h"
#include "XML\tinyxml2.h"
#include <vector>

#define SCENE_CONSTANT_BUFFER_ID	0
#define LIGHT_CONSTANT_BUFFER_ID	1
#define ANIMATED_CONSTANT_BUFFER_ID	2
#define MATERIAL_PARAMETERS_CONSTANT_BUFFER_ID	3

class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11GeometryShader;
class ID3D11InputLayout;
class ID3D11Buffer;

typedef struct _D3D_SHADER_MACRO D3D_SHADER_MACRO;
typedef D3D_SHADER_MACRO D3D10_SHADER_MACRO;

class ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

class CEffectShader : public CNamed
{
protected:
	std::vector<ID3D11Buffer *> m_ConstantBuffers;
	std::string m_Preprocessor;
	std::string m_ShaderModel;
	std::string m_EntryPoint;
	std::string m_Filename;
	std::string m_Path = "Data/Effects/";
	std::string m_CompiledPath = "Data/Effects/Compiled/";
	std::vector<std::string> m_PreprocessorMacros;
	D3D10_SHADER_MACRO *m_ShaderMacros;

	virtual void Destroy();
	void CreateShaderMacro();
	bool LoadShader(const std::string &Filename, const std::string &EntryPoint,	const std::string &ShaderModel, ID3DBlob **BlobOut);
	bool CreateConstantBuffer(int IdBuffer, unsigned int BufferSize, bool Dynamic);
	bool CreateConstantBuffer();
public:
	CEffectShader(const CXMLTreeNode &TreeNode);
	CEffectShader(tinyxml2::XMLElement* TreeNode);
	virtual ~CEffectShader();
	virtual bool Load() = 0;
	virtual bool Reload() = 0;
	virtual void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer) = 0;
	ID3D11Buffer * GetConstantBuffer(unsigned int IdBuffer);
};

class CEffectVertexShader : public CEffectShader
{
protected:
	ID3D11VertexShader *m_VertexShader;
	ID3D11InputLayout  *m_VertexLayout;
	std::string m_VertexType;

	void Destroy();
public:
	CEffectVertexShader(const CXMLTreeNode &TreeNode);
	CEffectVertexShader(tinyxml2::XMLElement* TreeNode);
	virtual ~CEffectVertexShader() { Destroy(); }

	bool Load();
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);
	bool Reload();
	ID3D11VertexShader* GetVertexShader();
	ID3D11InputLayout*  GetVertexLayout();
};

class CEffectPixelShader : public CEffectShader
{
protected:
	ID3D11PixelShader *m_PixelShader;

	void Destroy();
public:
	CEffectPixelShader(const CXMLTreeNode &TreeNode);
	CEffectPixelShader(tinyxml2::XMLElement* TreeNode);
	virtual ~CEffectPixelShader() { Destroy(); }
	bool Reload();
	bool Load();
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);

	ID3D11PixelShader* GetPixelShader();
};

class CEffectGeometryShader : public CEffectShader
{
protected:
	ID3D11GeometryShader *m_GeometryShader;

	void Destroy();
public:
	CEffectGeometryShader(const CXMLTreeNode &TreeNode);
	CEffectGeometryShader(tinyxml2::XMLElement* TreeNode);
	virtual ~CEffectGeometryShader(){ Destroy(); }
	bool Reload();
	bool Load();
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);

	ID3D11GeometryShader* GetGeometryShader();
};

#endif //EFFECT_SHADER_H