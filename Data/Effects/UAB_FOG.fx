/*

% Shadowmap Shader.

date: 05022016

*/

#include "Globals.fxh"
#include "Samplers.fxh"

float maxAttenuation = 1;
float m_StartLinearFog = 1;
float m_EndLinearFog = 5;
float3 m_FogColor = float3(1.0, 1.0, 1.0);

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

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV, float4x4 InverseProjection)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, InverseProjection);
	// Divide by w to get the view-space position
	return l_PositionVS.xyz / l_PositionVS.w;
}

float3 GetPositionFromZDepthView(float ZDepthView, float2 UV, float4x4 InverseView, float4x4 InverseProjection)
{
	float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV, InverseProjection);
	return mul(float4(l_PositionView,1), InverseView).xyz;
}

float CalcAttenuation(float Depth, float StartFog, float EndFog)
{
	
float maxAttenuation = m_RawData[0].x;
	if(Depth<EndFog){
		return maxAttenuation*smoothstep(StartFog, EndFog, Depth);
	} else {
		return maxAttenuation;
	}
}
float4 CalcLinearFog(float Depth, float StartFog, float EndFog, float3 FogColor) 
{
	float l_Fog = CalcAttenuation(Depth, StartFog, EndFog);
	return float4(FogColor, l_Fog);
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

float m_StartLinearFog = m_RawData[0].y;
float m_EndLinearFog = m_RawData[0].z;
float3 m_FogColor = ((float3)m_RawData[1].xyz);
	float4 l_FogColor=CalcLinearFog(Depth, m_StartLinearFog, m_EndLinearFog, m_FogColor);
	//return(l_FogColor.xyz*l_FogColor.a);
	return saturate(CurrentColor+l_FogColor.xyz*l_FogColor.a);
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
	float3 l_worldPos = GetPositionFromZDepthView((T1Texture.Sample(S1Sampler,IN.UV).r),IN.UV, m_InverseView, m_InverseProjection);	
	float l_DistanceEyeToWorldPosition=length(l_worldPos-m_InverseView[3].xyz);
	//return float4(l_DistanceEyeToWorldPosition/10,l_DistanceEyeToWorldPosition/10,l_DistanceEyeToWorldPosition/10,1);
	float4 l_FinalColor = T0Texture.Sample(S0Sampler,IN.UV);
	//return l_FinalColor; // renderiza la textura tal cual, sin modificar
	return float4(GetFogColor(l_DistanceEyeToWorldPosition, l_FinalColor.xyz),1);
}