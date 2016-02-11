/*

% Shadowmap Shader.

date: 05022016

*/

#include "Globals.fxh"
#include "Samplers.fxh"

float maxAttenuation = 0.6;
float m_StartLinearFog = 15;
float m_EndLinearFog = 5;
float3 m_FogColor = float3(.2, .2, .2);

struct TVertexVS
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

struct TVertexPS
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
};
float CalcAttenuation(float Depth, float StartFog, float EndFog)
{
	float attenuation;
	if(Depth<EndFog && Depth>StartFog){
		return maxAttenuation*smoothstep(StartFog, EndFog, Depth);
	} else {
		return maxAttenuation;
	}
}
float4 CalcLinearFog(float Depth, float StartFog, float EndFog, float3 FogColor) 
{
	float l_Fog = CalcAttenuation(Depth, StartFog, EndFog);
	return float4(FogColor, 1.0-l_Fog);
} 
float4 CalcExp2Fog(float Depth, float ExpDensityFog, float3 FogColor) 
{ 
	const float LOG2E = 1.442695; // = 1 / log(2) 
	float l_Fog = exp2(-ExpDensityFog * ExpDensityFog * Depth * Depth * LOG2E); 
	return float4(FogColor, 1.0-l_Fog);
} 
float4 CalcExpFog(float Depth, float ExpDensityFog, float3 FogColor) 
{ 
	const float LOG2E = 1.442695; // = 1 / log(2) 
	float l_Fog = exp2(-ExpDensityFog * Depth * LOG2E); 
	return float4(FogColor, 1.0-l_Fog);
} 
float3 GetFogColor(float Depth, float3 CurrentColor) 
{ 
	float4 l_FogColor=CalcLinearFog(Depth, m_StartLinearFog, m_EndLinearFog, m_FogColor); 
	return float3(CurrentColor*(1.0-l_FogColor.a)+l_FogColor.xyz*l_FogColor.a);
} 

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = IN.Pos;
	l_Out.UV = IN.UV;
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : SV_Target
{
	float l_DistanceEyeToWorldPosition=T1Texture.Sample(S1Sampler,IN.UV);	
	float4 l_FinalColor = T0Texture.Sample(S0Sampler,IN.UV);
	return l_FinalColor; // renderiza la textura tal cual, sin modificar
	return float4(GetFogColor(l_DistanceEyeToWorldPosition, l_FinalColor.xyz), l_FinalColor.a);
}