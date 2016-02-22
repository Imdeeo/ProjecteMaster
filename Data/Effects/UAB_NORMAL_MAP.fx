#include "Lights.fxh"

struct TVertexVS
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float4 Tangent : TEXCOORD3;
};

struct TVertexPS
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float3 Pixelpos : TEXCOORD3;
	float3 WorldTangent: TEXCOORD1;
	float3 WorldBinormal: TEXCOORD2;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.Pixelpos = l_Out.Pos.xyz;
	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);
	l_Out.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
	l_Out.UV = IN.UV;
	
	l_Out.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
	l_Out.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : SV_Target
{
	float g_Bump = 2.4;
	float l_DiffuseContrib = 0;
	
	float3 Tn=normalize(IN.WorldTangent);
	float3 Bn=normalize(IN.WorldBinormal);
	float3 bump=g_Bump*((T1Texture.Sample(S1Sampler,IN.UV).rgb) - float3(0.5,0.5,0.5));
	float3 Nn = IN.Normal;
	Nn = Nn + bump.x*Tn + bump.y*Bn;
	Nn = normalize(Nn);
	
	float4 aux  = applyLights(IN.Pixelpos, Nn, T0Texture.Sample(S0Sampler,IN.UV), 0);
	return aux;
		
	return float4(Nn,1);
}