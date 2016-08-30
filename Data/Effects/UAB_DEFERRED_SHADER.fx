#include "Lights.fxh"

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

PS_INPUT mainVS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.Color=IN.Color;
	l_Output.UV=IN.UV;
	return l_Output; 
}

float3 Texture2Normal(float3 Color)
{
	return (Color-0.5)*2;
}

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
	return mul(float4(l_PositionView,1.0), InverseView).xyz;
}

float4 mainPS(PS_INPUT IN) : SV_Target
{		
	float l_Depth=T3Texture.Sample(S3Sampler, IN.UV).r;
	float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
	float4 l_NormalMap = T2Texture.Sample(S2Sampler, IN.UV);
	float3 Nn=Texture2Normal(l_NormalMap.xyz);
	float4 l_albedo = T0Texture.Sample(S0Sampler, IN.UV);
	float l_SpecularFactor = l_albedo.w;
	float4 lightmap = T1Texture.Sample(S1Sampler, IN.UV);
	float4 l_SpecularMap = T4Texture.Sample(S4Sampler, IN.UV);
	float l_SpecularPower = lightmap.w*100;
	float4 l_Out = applyLights(l_WorldPosition, Nn, l_albedo, 0, l_SpecularPower, l_SpecularFactor, l_SpecularMap.a, l_SpecularMap.rgb);
	return l_Out * shadowMapCalc(l_WorldPosition);
}