#include "Globals.fxh"
#include "Samplers.fxh"

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float3 HPos : TEXCOORD1;
};

PS_INPUT mainVS(VS_INPUT IN)
{
	PS_INPUT l_Out = (PS_INPUT)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.HPos = l_Out.Pos.xyz;
	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);
	l_Out.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
	
	return l_Out;
}

float4 mainPS(PS_INPUT IN) : SV_Target
{	
	float3 Nn=normalize(IN.Normal);
	float3 l_EyeToWorldPosition = normalize(IN.HPos-m_CameraPosition.xyz);
	float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, Nn));
	float3 l_ReflectColor = T1Texture.Sample(S1Sampler, l_ReflectVector).xyz;
	return float4(l_ReflectColor,1);
}