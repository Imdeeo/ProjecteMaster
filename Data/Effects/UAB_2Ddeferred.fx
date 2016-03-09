#include "globals.fxh"
#include "Samplers.fxh"

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
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 mainPS(PS_INPUT IN) : SV_Target
{
	//return float4(0.2,0,0.2,1);
	float4 aux = T0Texture.Sample(S0Sampler,IN.UV);
	return float4(aux.rgb,4);
}