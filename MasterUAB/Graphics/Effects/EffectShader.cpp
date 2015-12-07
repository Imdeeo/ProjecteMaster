#include "Effects\EffectShader.h"
#include "RenderManager\RenderManager.h"
#include "Effects\EffectParameters.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\VertexTypes.h"

#include <assert.h>

#include <string>

#include <D3Dcompiler.h>

#define USE_D3DX
#ifdef USE_D3DX
#include <D3DX11async.h>

#endif

CEffectShader::CEffectShader(const CXMLTreeNode &TreeNode):CNamed(TreeNode),
	m_ConstantBuffer(nullptr)
{
	m_Filename = TreeNode.GetPszProperty("file");
	m_ShaderModel = TreeNode.GetPszProperty("shader_model");
	m_EntryPoint = TreeNode.GetPszProperty("entry_point");
}


CEffectShader::~CEffectShader(void)
{
}


bool CEffectShader::LoadShader(const std::string &Filename, const std::string
&EntryPoint, const std::string &ShaderModel, ID3DBlob **BlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif
	ID3DBlob* pErrorBlob;
	
#ifdef USE_D3DX
	hr=D3DX11CompileFromFile(Filename.c_str(), NULL, NULL, EntryPoint.c_str(),
	ShaderModel.c_str(), dwShaderFlags, 0, NULL, BlobOut, &pErrorBlob, NULL );
#else
	hr=D3DCompileFromFile(std::wstring(Filename.begin(), Filename.end()).c_str(),NULL,NULL,EntryPoint.c_str(), ShaderModel.c_str(),dwShaderFlags,0,NULL,&pErrorBlob);
#endif
	
	if( FAILED(hr) )
	{
		if( pErrorBlob != NULL )
			OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
		if( pErrorBlob )
			pErrorBlob->Release();
		return false;
	}
	if( pErrorBlob )
		pErrorBlob->Release();
	return true;
}

bool CEffectShader::CreateConstantBuffer()
{
	CRenderManager* l_RenderManager=UABEngine.GetRenderManager();
	ID3D11Device *l_Device=l_RenderManager->GetDevice();
	D3D11_BUFFER_DESC l_BufferDescription;
	ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
	l_BufferDescription.Usage=D3D11_USAGE_DEFAULT;
	l_BufferDescription.ByteWidth=sizeof(CEffectParameters);
	l_BufferDescription.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	l_BufferDescription.CPUAccessFlags=0;
	if( FAILED(l_Device->CreateBuffer(&l_BufferDescription, NULL,&m_ConstantBuffer)))
		return false;
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

CEffectVertexShader::CEffectVertexShader(const CXMLTreeNode &TreeNode):CEffectShader(TreeNode),
	m_VertexShader(nullptr),
	m_VertexLayout(nullptr)
{
	m_VertexType = TreeNode.GetPszProperty("vertex_type");
}

bool CEffectVertexShader::Load()
{
	ID3DBlob *l_VSBlob=NULL;
	bool l_Loaded=LoadShader(m_Filename, m_EntryPoint, m_ShaderModel, &l_VSBlob);
	if(!l_Loaded)
		return false;
	CRenderManager* l_RenderManager=UABEngine.GetRenderManager();
	ID3D11Device *l_Device=l_RenderManager->GetDevice();
	HRESULT l_HR=l_Device->CreateVertexShader(l_VSBlob->GetBufferPointer(),
	l_VSBlob->GetBufferSize(), NULL, &m_VertexShader);
	if( FAILED(l_HR) )
	{
		l_VSBlob->Release();
		return false;
	}
	if(m_VertexType=="MV_POSITION_NORMAL_TEXTURE_VERTEX")
		l_Loaded=MV_POSITION_NORMAL_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else if(m_VertexType=="MV_POSITION_COLOR_VERTEX")
		l_Loaded=MV_POSITION_COLOR_VERTEX::CreateInputLayout(l_RenderManager,l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "KG_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX")
		l_Loaded = MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else
		printf("Vertex type '%s' not recognized on CEffectVertexShader::Load",m_VertexType.c_str());
		//Info("Vertex type '%s' not recognized on CEffectVertexShader::Load",m_VertexType.c_str());
	l_VSBlob->Release();
	if(!l_Loaded)
		return false;
	return CreateConstantBuffer();
}

void CEffectVertexShader::Destroy()
{
	delete m_VertexLayout;
	delete m_VertexShader;
	delete m_ConstantBuffer;
}

CEffectPixelShader::CEffectPixelShader(const CXMLTreeNode &TreeNode):CEffectShader(TreeNode),
	m_PixelShader(nullptr)
{
}

bool CEffectPixelShader::Load()
{
	ID3DBlob *l_PSBlob=NULL;
	bool l_Loaded=LoadShader(m_Filename.c_str(), m_EntryPoint.c_str(),
	m_ShaderModel.c_str(), &l_PSBlob);
	if(!l_Loaded)
		return false;
	CRenderManager* l_RenderManager=UABEngine.GetRenderManager();
	ID3D11Device *l_Device=l_RenderManager->GetDevice();
	HRESULT l_HR=l_Device->CreatePixelShader(l_PSBlob->GetBufferPointer(),
	l_PSBlob->GetBufferSize(), NULL, &m_PixelShader);
	l_PSBlob->Release();
	return CreateConstantBuffer();
}

void CEffectPixelShader::Destroy()
{
	delete m_PixelShader;
	delete m_ConstantBuffer;
}