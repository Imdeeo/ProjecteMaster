#include "globals.fxh"

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float4 HPos : TEXCOORD1;
};

PS_INPUT mainVS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	
	l_Output.Pos = mul( float4(IN.Pos, 1.0), m_World );
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.HPos = l_Output.Pos ;
	l_Output.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
	l_Output.UV = IN.UV;
	
	return l_Output;
}

float4 mainPS(PS_INPUT IN) : SV_Target
{	
	float l_Depth = IN.HPos.z / IN.HPos.w;
	return float4(l_Depth,l_Depth,l_Depth, 1.0f); 
}
