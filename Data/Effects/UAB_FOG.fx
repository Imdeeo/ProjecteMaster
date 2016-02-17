/*

% Shadowmap Shader.

date: 05022016

*/

#include "Globals.fxh"
#include "Samplers.fxh"

static float maxAttenuation = m_MaxAttenuation_StartLinearFog_EndLinearFog.x;
static float m_StartLinearFog = m_MaxAttenuation_StartLinearFog_EndLinearFog.y;
static float m_EndLinearFog = m_MaxAttenuation_StartLinearFog_EndLinearFog.z;

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
	if(Depth<EndFog){
		return maxAttenuation*smoothstep(StartFog, EndFog, Depth);
	} else {
		return maxAttenuation;
	}
}
float CalcLinearFog(float Depth, float StartFog, float EndFog) 
{
	float l_Fog = CalcAttenuation(Depth, StartFog, EndFog);
	return l_Fog;
} 
float CalcExp2Fog(float Depth, float ExpDensityFog) 
{ 
	const float LOG2E = 1.442695; // = 1 / log(2) 
	float l_Fog = exp2(-ExpDensityFog * ExpDensityFog * Depth * Depth * LOG2E); 
	return l_Fog;
} 
float CalcExpFog(float Depth, float ExpDensityFog) 
{ 
	const float LOG2E = 1.442695; // = 1 / log(2) 
	float l_Fog = exp2(-ExpDensityFog * Depth * LOG2E); 
	return l_Fog;
} 
float4 GetFogColor(float Depth) 
{ 

	float l_FogIntensity=CalcLinearFog(Depth, m_StartLinearFog, m_EndLinearFog);
	//return(l_FogColor.xyz*l_FogColor.a);
	return saturate(float4(m_FogColor.xyz,l_FogIntensity));
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
	float3 l_worldPos = GetPositionFromZDepthView((T0Texture.Sample(S0Sampler,IN.UV).r),IN.UV, m_InverseView, m_InverseProjection);	
	float l_DistanceEyeToWorldPosition=length(l_worldPos-m_InverseView[3].xyz);
	//return float4(l_DistanceEyeToWorldPosition/10,l_DistanceEyeToWorldPosition/10,l_DistanceEyeToWorldPosition/10,1);
	//return l_FinalColor; // renderiza la textura tal cual, sin modificar
	return GetFogColor(l_DistanceEyeToWorldPosition);
}