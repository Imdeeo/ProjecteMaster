//UAB_POSITION_NORMAL_TEXTURE_VERTEXShader.fx
#include "Globals.fxh"
Texture2D DiffuseTexture : register( t0 );
SamplerState LinearSampler : register( s0 );
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos = mul( IN.Pos, m_World );
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.UV = IN.UV;
	l_Output.WorldNormal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	return l_Output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT IN) : SV_Target
{
	float3 g_LightDirection=normalize(float3(0.5, -0.3, 0.5));
	float4 g_AmbientLight=float4(0.5, 0.6, 0.55, 1.0)*0.3;
	float4 l_Albedo=DiffuseTexture.Sample(LinearSampler, IN.UV);
	float3 Nn=normalize(IN.WorldNormal);
	float l_DiffuseContrib=max(dot(Nn, -g_LightDirection), 0);
	return l_Albedo;
	return float4(l_Albedo.xyz*g_AmbientLight+l_DiffuseContrib*l_Albedo.xyz, l_Albedo.a);
}