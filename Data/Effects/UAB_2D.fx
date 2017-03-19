#include "globals.fxh"
#include "Samplers.fxh"

static float m_Angle = m_RawDataArray[0];

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
	l_Output.Color=IN.Color;
	#ifdef HAS_LOAD
		l_Output.UV=IN.UV-float2(0.5,0.5);
		l_Output.UV=mul(l_Output.UV, float2x2(cos(m_Angle), sin(m_Angle), -sin(m_Angle), cos(m_Angle))); 
		l_Output.UV=l_Output.UV+float2(0.5,0.5);
	#else
		l_Output.UV=IN.UV;
	#endif
	return l_Output;
}

float4 mainPS(PS_INPUT IN) : SV_Target
{
	//return float4(IN.UV, 0,1);
	return T0Texture.Sample(S0Sampler, IN.UV)*IN.Color;
}

/*float4 mainPS(PS_INPUT IN) : SV_Target
{
	return float4(T0Texture.Sample(S0Sampler, IN.UV).xyz, 1.0);
}*/