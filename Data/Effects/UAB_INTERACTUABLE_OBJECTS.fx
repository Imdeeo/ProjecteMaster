#include "Samplers.fxh"
#include "Lights.fxh"

struct TVertexVS
{
	float3 Pos : POSITION;
};

struct TVertexPS
{
	float4 Pos : SV_POSITION;
};


TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	float3 l_Normal= 0;
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);

	return l_Out;
}

float4 mainPS(TVertexPS IN) : SV_Target
{
	return float4(1,1,1,1);
}
