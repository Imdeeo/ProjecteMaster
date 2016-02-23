/*

% Normal Shader.

date: 17122015

*/
#include "globals.fxh"

struct TVertexVS
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
};

struct TVertexPS
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD1;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);
	l_Out.Normal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : SV_Target
{
	return float4(IN.Normal, 1.0);
}