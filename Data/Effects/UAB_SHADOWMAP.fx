#include "globals.fxh"

struct VS_INPUT
{
	float3 Pos : POSITION;
};

struct PS_INPUT
{
	float4 Pos : POSITION;
	float Depth : TEXCOORD0;
};

PS_INPUT mainVS(VS_INPUT IN)
{
	PS_INPUT l_Out = (PS_INPUT)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);
	l_Out.Depth = 1.0f - (l_Out.Pos.z / l_Out.Pos.w);
	
	return l_Out;
}

float4 mainPS(PS_INPUT IN) : SV_Target
{	
	return float4(IN.Depth, IN.Depth, IN.Depth, 1.0f); 
}
