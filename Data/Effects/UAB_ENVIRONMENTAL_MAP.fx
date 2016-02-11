/*

% Normalmap+Spot light shader.

date: 17122015

*/
#include "Globals.fxh"
#include "Samplers.fxh"

	
const float g_EnvironmentFactor = 1.0;

struct TVertexVS
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
};

struct TVertexPS
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float3 Pixelpos : TEXCOORD1;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.Pixelpos = l_Out.Pos.xyz;
	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);
	l_Out.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
	
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : SV_Target
{	
	float3 Nn=normalize(IN.Normal);
	float3 l_EyeToWorldPosition = normalize(IN.Pixelpos-m_CameraPosition.xyz);
	float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, Nn));
	float3 outColor = tex3D(CubeSampler, l_ReflectVector).xyz*g_EnvironmentFactor;
	return float4(outColor,1);
}