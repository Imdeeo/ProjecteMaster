#include "Globals.fxh"
#include "Samplers.fxh"

static float m_TextureSize=m_RawDataArray[0];
static float m_BlurScale=m_RawDataArray[1];

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

PS_INPUT mainVS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.Color=IN.Color*m_BaseColor;
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 GetGaussianBlurFromSampler(Texture2D _Texture2D, SamplerState _SamplerState, float2 UV, float2 TextureSize, float TextureScale)
{
	float2 l_PixelKernel[7] =
	{
		{ -1, 1 },
		{ -1, -1 },
		{ -1, 0 },
		{ 0, 0 },
		{ 1, 0 },
		{ 1, -1 },
		{ 1, 1 },
	};
	
	float l_BlurWeights[7] =
	{
		0.120985,
		0.120985,
		0.176033,
		0.199471,
		0.176033,
		0.120985,
		0.120985,
	};

	float4 l_GaussianColor = 0;
	float2 l_TextureSize=TextureSize*TextureScale;
	float l_TotalWeights=0.0;

	for(int i = 0; i<7; ++i)
	{
		float4 l_Color=_Texture2D.Sample(_SamplerState, UV+(l_PixelKernel[i] *l_TextureSize))*l_BlurWeights[i];
		l_TotalWeights+=l_BlurWeights[i];
		l_GaussianColor += l_Color;
	}

	return l_GaussianColor/l_TotalWeights;
}

float4 PSBlur(PS_INPUT IN) : SV_Target
{
	return GetGaussianBlurFromSampler(T0Texture, S0Sampler, IN.UV, 1/m_TextureSize, m_BlurScale);
}