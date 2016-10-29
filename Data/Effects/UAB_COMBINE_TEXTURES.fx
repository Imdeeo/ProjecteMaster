#include "Samplers.fxh"
#include "Lights.fxh"

static float m_ShowTexture = m_RawDataArray[0];
static float m_Intensity = m_RawDataArray[1];
static float4 m_Color = float4(m_RawDataArray[2], m_RawDataArray[3], m_RawDataArray[4], m_RawDataArray[5]);

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
	PS_INPUT l_Out = (PS_INPUT)0;
	l_Out.Pos=IN.Pos;
	l_Out.Color=IN.Color;
	l_Out.UV=IN.UV;
	return l_Out;
}

float4 mainPS(PS_INPUT IN) : SV_Target
{	
	float4 l_NewColor = (saturate(T1Texture.Sample(S1Sampler, IN.UV) - T2Texture.Sample(S2Sampler, IN.UV)*0.9) * m_Color) * m_Intensity;
	if(m_ShowTexture==0.0)
	{
		// Get our original pixel from ColorMap
		float4 l_OriginalColor = T0Texture.Sample(S0Sampler, IN.UV);
		// Combine the two images.
		return l_OriginalColor + l_NewColor;
	}
	else
		return l_NewColor;
}
