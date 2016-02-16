#include "globals.fxh"
#include "samplers.fxh"

struct VS_INPUT
{
	float3 Pos : POSITION;
};

struct PS_INPUT
{
	float4 Pos : POSITION;
	float Depth : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);
	l_Out.Depth = 1.0f - (l_Out.Pos.z / l_Out.Pos.w);
	
	return l_Out;
}

float4 PS(PS_INPUT IN) : SV_Target
{	
	return float4(IN.Depth, IN.Depth, IN.Depth, 1.0f); 
}
