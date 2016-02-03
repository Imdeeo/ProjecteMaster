#include "globals.fxh"

Texture2D T0Texture: register( t0 );
SamplerState S0Sampler: register( s0 );

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

float4 mainPS(PS_INPUT IN) : SV_Target
{
	if (IN.UV.y<0.5f)
	{
		clip(-1);
	}
		
	return T0Texture.Sample(S0Sampler, IN.UV)*IN.Color;
}