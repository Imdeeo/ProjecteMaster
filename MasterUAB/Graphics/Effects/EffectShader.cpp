#include "Effects\EffectShader.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "EffectManager.h"

#include "SceneEffectParameters.h"
#include "AnimatedModelEffectParameters.h"
#include "LightEffectParameters.h"

#include "RenderableObjects\VertexTypes.h"
#include "Utils.h"

#include "XML\XMLTreeNode.h"

#include <assert.h>

#include <d3dcommon.h>

#ifdef WIN7
#include <D3DX11async.h>
#endif

#include <d3d11.h>

#include <D3Dcompiler.h>

CEffectShader::CEffectShader(const CXMLTreeNode &TreeNode):CNamed(TreeNode){
	m_Filename = TreeNode.GetPszProperty("file");
	m_ShaderModel = TreeNode.GetPszProperty("shader_model");
	m_EntryPoint = TreeNode.GetPszProperty("entry_point");
	m_Preprocessor = TreeNode.GetPszProperty("preprocessor","");
}

CEffectShader::~CEffectShader(void)
{
	CHECKED_DELETE(m_ShaderMacros);
}

void CEffectShader::CreateShaderMacro()
{
	m_PreprocessorMacros.clear();
	if (m_Preprocessor.empty())
	{
		m_ShaderMacros = NULL;
		return;
	}
	std::vector<std::string> l_PreprocessorItems;
	SplitString(m_Preprocessor, ';', l_PreprocessorItems);
	m_ShaderMacros = new D3D10_SHADER_MACRO[l_PreprocessorItems.size() + 1];
	for (size_t i = 0; i<l_PreprocessorItems.size(); ++i)
	{
		std::vector<std::string> l_PreprocessorItem;
		SplitString(l_PreprocessorItems[i], '=', l_PreprocessorItem);
		if (l_PreprocessorItem.size() == 1)
		{
			m_PreprocessorMacros.push_back(l_PreprocessorItems[i]);
			m_PreprocessorMacros.push_back("1");
		}
		else if (l_PreprocessorItem.size() == 2)
		{
			m_PreprocessorMacros.push_back(l_PreprocessorItem[0]);
			m_PreprocessorMacros.push_back(l_PreprocessorItem[1]);
		}
		else
		{
			//Info("Error creating shader macro '%s', with wrong size on parameters", l_PreprocessorItems[i]);
			printf("Error creating shader macro '%s', with wrong size on parameters", l_PreprocessorItems[i]);
			CHECKED_DELETE_ARRAY(m_ShaderMacros);
			return;
		}
	}
	for (size_t i = 0; i<l_PreprocessorItems.size(); ++i)
	{
		m_ShaderMacros[i].Name = m_PreprocessorMacros[i * 2].c_str();
		m_ShaderMacros[i].Definition = m_PreprocessorMacros[(i * 2) + 1].c_str();
	}
	m_ShaderMacros[l_PreprocessorItems.size()].Name = NULL;
	m_ShaderMacros[l_PreprocessorItems.size()].Definition = NULL;
}

bool CEffectShader::LoadShader(const std::string &Filename, const std::string &EntryPoint, const std::string &ShaderModel, ID3DBlob **BlobOut)
{
#ifdef WIN7
	// D3DX
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(Filename.c_str(), m_ShaderMacros, NULL, EntryPoint.c_str(), ShaderModel.c_str(), dwShaderFlags, 0, NULL, BlobOut, &pErrorBlob, NULL);
#else
	// D3D11
	UINT dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3D11Device *l_Device = UABEngine.GetRenderManager()->GetDevice();
	//LPCSTR profile = (l_Device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";
	const D3D_SHADER_MACRO defines[] = { "EXAMPLE_DEFINE", "1", NULL, NULL };
	ID3DBlob* pErrorBlob = nullptr;
	std::wstring wFilename;
	wFilename.assign(Filename.begin(), Filename.end());
	HRESULT hr = D3DCompileFromFile(wFilename.c_str(), m_ShaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint.c_str(), ShaderModel.c_str(), dwShaderFlags, 0, BlobOut, &pErrorBlob);
#endif
	
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob)
			pErrorBlob->Release();
		return false;
	}
	if (pErrorBlob)
	{
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		pErrorBlob->Release();
	}
	return true;
}

bool CEffectShader::CreateConstantBuffer(int IdBuffer, unsigned int BufferSize, bool Dynamic = false)
{
	ID3D11Buffer *l_ConstantBuffer;
	/*CRenderManager &l_RenderManager = UABEngine.GetRenderManager();
	ID3D11Device *l_Device = l_RenderManager.GetDevice();*/
	CRenderManager* l_RenderManager = UABEngine.GetRenderManager();
	ID3D11Device *l_Device = l_RenderManager->GetDevice();
	D3D11_BUFFER_DESC l_BufferDescription;
	ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
	l_BufferDescription.Usage = Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	l_BufferDescription.ByteWidth = BufferSize;
	if ((BufferSize % 16) != 0)
		//Info("Constant Buffer '%d' with wrong size '%d' on shader '%s'.", IdBuffer, BufferSize, m_Filename.c_str());
		printf("Constant Buffer '%d' with wrong size '%d' on shader '%s'.", IdBuffer, BufferSize, m_Filename.c_str());

	l_BufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	l_BufferDescription.CPUAccessFlags = Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	l_BufferDescription.MiscFlags = 0;
	l_BufferDescription.StructureByteStride = 0;
	if (FAILED(l_Device->CreateBuffer(&l_BufferDescription, NULL,
		&l_ConstantBuffer)))
	{
		//Info("Constant buffer '%d' couldn't created on shader '%s'.", IdBuffer, m_Filename.c_str());
		printf("Constant buffer '%d' couldn't created on shader '%s'.", IdBuffer, m_Filename.c_str());
		m_ConstantBuffers.push_back(NULL);
		return false;
	}
	m_ConstantBuffers.push_back(l_ConstantBuffer);
	return true;
}

bool CEffectShader::CreateConstantBuffer()
{
	CreateConstantBuffer(SCENE_CONSTANT_BUFFER_ID, sizeof(CSceneEffectParameters));
	CreateConstantBuffer(LIGHT_CONSTANT_BUFFER_ID, sizeof(CLightEffectParameters));
	CreateConstantBuffer(ANIMATED_CONSTANT_BUFFER_ID, sizeof(CAnimatedModelEffectParameters));
	CreateConstantBuffer(MATERIAL_PARAMETERS_CONSTANT_BUFFER_ID, sizeof(Vect4f)*MAX_RAW_DATA_ELEMENTS);

	return true;
}

void CEffectShader::Destroy()
{
	assert(!"this method must not be called");
}

bool CEffectShader::Reload()
{
	assert(!"this method must not be called");
	return false;
}

ID3D11Buffer * CEffectShader::GetConstantBuffer(unsigned int IdBuffer)
{
	if(IdBuffer>=m_ConstantBuffers.size())
		return NULL;
	return m_ConstantBuffers[IdBuffer];
}

// ----------------------------- VERTEX SHADER ----------------------------
CEffectVertexShader::CEffectVertexShader(const CXMLTreeNode &TreeNode):CEffectShader(TreeNode),
	m_VertexShader(nullptr),
	m_VertexLayout(nullptr)	
{
	m_VertexType = TreeNode.GetPszProperty("vertex_type");
}

void CEffectVertexShader::Destroy()
{
	CHECKED_RELEASE(m_VertexLayout);
	CHECKED_RELEASE(m_VertexShader);
	for(int i = 0; i<m_ConstantBuffers.size(); ++i)
	{
		CHECKED_RELEASE(m_ConstantBuffers[i]);
	}
	m_ConstantBuffers.clear();
}

bool CEffectVertexShader::Load()
{
	CreateShaderMacro();
	ID3DBlob *l_VSBlob = NULL;
	bool l_Loaded = LoadShader(m_Filename, m_EntryPoint, m_ShaderModel, &l_VSBlob);
	if (!l_Loaded)
		return false;
	/*CRenderManager &l_RenderManager = UABEngine.GetRenderManager();
	ID3D11Device *l_Device = l_RenderManager.GetDevice();*/
	CRenderManager* l_RenderManager = UABEngine.GetRenderManager();
	ID3D11Device *l_Device = l_RenderManager->GetDevice();
	HRESULT l_HR = l_Device->CreateVertexShader(l_VSBlob->GetBufferPointer(), l_VSBlob->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(l_HR))
	{
		l_VSBlob->Release();
		return false;
	}
	if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_VERTEX")
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX")
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX")
		l_Loaded = MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION4_COLOR_TEXTURE_VERTEX")
		l_Loaded = MV_POSITION4_COLOR_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX")
		l_Loaded = MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_COLOR_VERTEX")
		l_Loaded = MV_POSITION_COLOR_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_TEXTURE_VERTEX")
		l_Loaded = MV_POSITION_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_COLOR_TEXTURE_VERTEX")
		l_Loaded = MV_POSITION_COLOR_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_NORMAL_VERTEX")
		l_Loaded = MV_POSITION_NORMAL_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_TANGENT_VERTEX")
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_TANGENT_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX")
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_TEXTURE2_TANGENT_VERTEX")
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_TEXTURE2_TANGENT_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX")
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else
		//Info("Vertex type '%s' not recognized on CEffectVertexShader::Load", m_VertexType.c_str());
		printf("Vertex type '%s' not recognized on CEffectVertexShader::Load", m_VertexType.c_str());

	l_VSBlob->Release();
	if (!l_Loaded)
		return false;
	return CreateConstantBuffer();
}

bool CEffectVertexShader::Reload()
{
	Destroy();
	return Load();
}

void CEffectVertexShader::SetConstantBuffer(unsigned int IdBuffer, void
	*ConstantBuffer)
{
	ID3D11DeviceContext
		*l_DeviceContext = UABEngine.GetRenderManager()->GetDeviceContext();
	ID3D11Buffer *l_ConstantBuffer = GetConstantBuffer(IdBuffer);
	if (l_ConstantBuffer != NULL)
	{
		l_DeviceContext->UpdateSubresource(l_ConstantBuffer, 0, NULL,
			ConstantBuffer, 0, 0);
		l_DeviceContext->VSSetConstantBuffers(IdBuffer, 1, &l_ConstantBuffer);
	}
}
ID3D11VertexShader* CEffectVertexShader::GetVertexShader()
{
	return m_VertexShader;
}
ID3D11InputLayout*  CEffectVertexShader::GetVertexLayout()
{
	return m_VertexLayout;
}

// ----------------------------- PIXEL SHADER ----------------------------
CEffectPixelShader::CEffectPixelShader(const CXMLTreeNode &TreeNode):CEffectShader(TreeNode),
	m_PixelShader(nullptr)
{
}

void CEffectPixelShader::Destroy()
{
	CHECKED_RELEASE(m_PixelShader);
	for(int i = 0; i<m_ConstantBuffers.size(); ++i)
	{
		CHECKED_RELEASE(m_ConstantBuffers[i]);
	}
	m_ConstantBuffers.clear();
}

bool CEffectPixelShader::Load()
{
	CreateShaderMacro();
	ID3DBlob *l_PSBlob = NULL;
	bool l_Loaded = LoadShader(m_Filename.c_str(), m_EntryPoint.c_str(),
		m_ShaderModel.c_str(), &l_PSBlob);
	if (!l_Loaded)
		return false;
	/*CRenderManager &l_RenderManager = UABEngine.GetRenderManager();
	ID3D11Device *l_Device = l_RenderManager.GetDevice();*/
	CRenderManager* l_RenderManager = UABEngine.GetRenderManager();
	ID3D11Device *l_Device = l_RenderManager->GetDevice();
	HRESULT l_HR = l_Device->CreatePixelShader(l_PSBlob->GetBufferPointer(),
		l_PSBlob->GetBufferSize(), NULL, &m_PixelShader);
	l_PSBlob->Release();
	return CreateConstantBuffer();
}

bool CEffectPixelShader::Reload()
{
	Destroy();
	return Load();
}

void CEffectPixelShader::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{
	ID3D11DeviceContext *l_DeviceContext = UABEngine.GetRenderManager()->GetDeviceContext();
	ID3D11Buffer *l_ConstantBuffer = GetConstantBuffer(IdBuffer);
	l_DeviceContext->UpdateSubresource(l_ConstantBuffer, 0, NULL,
		ConstantBuffer, 0, 0);
	l_DeviceContext->PSSetConstantBuffers(IdBuffer, 1, &l_ConstantBuffer);
}

ID3D11PixelShader* CEffectPixelShader::GetPixelShader()
{
	return m_PixelShader;
}

// ----------------------------- GEOMETRY SHADER ----------------------------
CEffectGeometryShader::CEffectGeometryShader(const CXMLTreeNode &TreeNode) :CEffectShader(TreeNode),
m_GeometryShader(nullptr)
{
}

void CEffectGeometryShader::Destroy()
{
	CHECKED_RELEASE(m_GeometryShader);
	for (int i = 0; i<m_ConstantBuffers.size(); ++i)
	{
		CHECKED_RELEASE(m_ConstantBuffers[i]);
	}
	m_ConstantBuffers.clear();
}

bool CEffectGeometryShader::Load()
{
	CreateShaderMacro();
	ID3DBlob *l_PSBlob = NULL;
	bool l_Loaded = LoadShader(m_Filename.c_str(), m_EntryPoint.c_str(),
		m_ShaderModel.c_str(), &l_PSBlob);
	
	if (!l_Loaded)
		return false;

	CRenderManager* l_RenderManager = UABEngine.GetRenderManager();
	ID3D11Device *l_Device = l_RenderManager->GetDevice();
	HRESULT l_HR = l_Device->CreateGeometryShader(l_PSBlob->GetBufferPointer(),
		l_PSBlob->GetBufferSize(), NULL, &m_GeometryShader);
	l_PSBlob->Release();
	
	return CreateConstantBuffer();
}

bool CEffectGeometryShader::Reload()
{
	Destroy();
	return Load();
}

void CEffectGeometryShader::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{
	ID3D11DeviceContext *l_DeviceContext = UABEngine.GetRenderManager()->GetDeviceContext();
	ID3D11Buffer *l_ConstantBuffer = GetConstantBuffer(IdBuffer);
	l_DeviceContext->UpdateSubresource(l_ConstantBuffer, 0, NULL,
		ConstantBuffer, 0, 0);
	l_DeviceContext->GSSetConstantBuffers(IdBuffer, 1, &l_ConstantBuffer);
}

ID3D11GeometryShader* CEffectGeometryShader::GetGeometryShader()
{
	return m_GeometryShader;
}