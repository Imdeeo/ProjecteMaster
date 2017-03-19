/*

% Spot light shader.

date: 17122015

*/
#include "Globals.fxh"


struct TVertexVS
{
	float3 Pos : POSITION;
	float4 Color : COLOR0;
};

struct TVertexPS
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};


TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : SV_Target
{
	return m_BaseColor;
}
