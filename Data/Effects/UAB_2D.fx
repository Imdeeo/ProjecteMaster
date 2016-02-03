#include "globals.fxh"

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

PS_INPUT VS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.Color=IN.Color*m_BaseColor;
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	if (IN.UV.x<0.5f)
		Clip(-1);
		
	return T0Texture.Sample(S0Sampler, IN.UV)*IN.Color;
}