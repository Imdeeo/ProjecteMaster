#include "globals.fxh"
#include "Samplers.fxh"

static float m_ZBlurActive=m_RawDataArray[0];
static float m_ZBlurShowDepths=m_RawDataArray[1];
static float m_ZBlurConstantBlur=m_RawDataArray[2];
static float m_ZBlurFocalStart=m_RawDataArray[3];
static float m_ZBlurFocalEnd=m_RawDataArray[4];
static float m_ZBlurEnd=m_RawDataArray[5];

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
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 GetZBlurColor(float Distance, float4 SourceColor, float4 BlurColor)
{
	float l_Blur=1.0;
	if(Distance<m_ZBlurFocalStart)
		l_Blur=max(Distance/m_ZBlurFocalStart, m_ZBlurConstantBlur);
	else if(Distance>m_ZBlurFocalEnd)
		l_Blur=max(1.0-(Distance-m_ZBlurFocalEnd)/m_ZBlurEnd, m_ZBlurConstantBlur);
		
	if(l_Blur<1.0)
		return BlurColor*(1-l_Blur)+l_Blur*SourceColor;
	else
		return SourceColor;
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

float4 PSZBlur(PS_INPUT IN) : SV_Target
{
	float4 l_SourceColor=T0Texture.Sample(S0Sampler, IN.UV);
	float l_Depth=T2Texture.Sample(S2Sampler, IN.UV).r;
	float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
	float3 l_CameraPosition=m_InverseView[3].xyz;
	float l_Distance=length(l_WorldPosition-l_CameraPosition);
	return GetZBlurColor(l_Distance, T0Texture.Sample(S0Sampler, IN.UV), T1Texture.Sample(S1Sampler, IN.UV));	
}