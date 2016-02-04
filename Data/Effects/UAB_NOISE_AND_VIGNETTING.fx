#include "globals.fxh"

Texture2D T0Texture: register( t0 );
Texture2D T1Texture: register( t1 );
SamplerState S0Sampler: register( s0 );
SamplerState S1Sampler: register( s1 );

static float m_NoisePct=0.5;
static float m_VignettingPct=1.0;
static float m_NoiseAmount=1.0;

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

PS_INPUT NoiseAndVignettingVS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.Color=IN.Color*m_BaseColor;
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 NoiseAndVignettingPS(PS_INPUT IN) : SV_Target
{
	float l_NoiseX=m_Time.y*sin(IN.UV.x*IN.UV.y+m_Time.y);
	l_NoiseX=fmod(l_NoiseX, 8)*fmod(l_NoiseX, 4);
	float l_DistortX=fmod(l_NoiseX, m_NoiseAmount);
	float l_DistortY=fmod(l_NoiseX, m_NoiseAmount+0.002);
	float2 l_DistortUV=float2(l_DistortX, l_DistortY);
	float4 l_Noise=T0Texture.Sample(S0Sampler, IN.UV+l_DistortUV)*m_NoisePct;
	float4 l_Vignetting=T1Texture.Sample(S1Sampler, IN.UV)*m_VignettingPct;
	
	if (IN.UV.y<0.5f)
	{
		clip(-1);
	}
	
	return l_Vignetting;
}