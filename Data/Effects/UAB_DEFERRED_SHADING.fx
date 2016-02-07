#include "globals.fxh"

Texture2D DiffuseTexture: register( t0 );
SamplerState LinearSampler: register( s0 );

struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float Depth : TEXCOORD1;
};

struct PS_OUTPUT
{ 
	float4 Target0 : SV_Target0;
	float4 Target1 : SV_Target1;
	float4 Target2 : SV_Target2;
	float4 Target3 : SV_Target3;
};

PS_INPUT mainVS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	
	l_Output.Pos = mul( float4(IN.Pos.xyz, 1.0), m_World );
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.UV = IN.UV;
	l_Output.Depth = 1.0f - (l_Output.Pos.z / l_Output.Pos.w);
	
	return l_Output;
}

PS_OUTPUT mainPS(PS_INPUT IN) : SV_Target
{
	PS_OUTPUT l_Out = (PS_OUTPUT)0;
	
	float m_SpecularPower = 50.0f;
	float m_SpecularFactor = 1.0f;
	
	float4 l_Albedo = DiffuseTexture.Sample(LinearSampler, IN.UV);
	l_Out.Target0 = (l_Albedo.xyz, m_SpecularFactor);
	l_Out.Target1 = (m_LightAmbient.xyz, m_SpecularPower);
	l_Out.Target2 = (IN.Normal, 1.0);
	l_Out.Target3 = float4(IN.Depth, IN.Depth, IN.Depth, 1.0f);
		
	return l_Out;
}