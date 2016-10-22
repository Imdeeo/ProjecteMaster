#include "globals.fxh"
#include "Samplers.fxh"

static float m_Active = m_RawDataArray[0];
static float m_Type = m_RawDataArray[1];
static float m_StartFog = m_RawDataArray[2];
static float m_EndFog = m_RawDataArray[3];
static float m_MaxAttenuation = m_RawDataArray[4];
static float m_DensityFog = m_RawDataArray[5];
static float m_HeightFog = m_RawDataArray[6];
static float4 m_FogColor = float4(m_RawDataArray[7], m_RawDataArray[8], m_RawDataArray[9], m_RawDataArray[10]);

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

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = IN.Pos;
	l_Out.UV = IN.UV;
	
	return l_Out;
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
	return mul(float4(l_PositionView,1), InverseView).xyz;
}

float CalcAttenuation(float Depth, float StartFog, float EndFog)
{
	if(Depth<EndFog)
		return m_MaxAttenuation*smoothstep(StartFog, EndFog, Depth);
	else
		return m_MaxAttenuation;
}

float CalcLinearFog(float Depth, float StartFog, float EndFog) 
{
	float l_Fog = CalcAttenuation(Depth, StartFog, EndFog);
	return l_Fog;
} 

float CalcExpFog(float Depth, float ExpDensityFog) 
{ 
	const float LOG2E = 1.442695; // = 1 / log(2) 
	float l_Fog = exp2(-ExpDensityFog * Depth * LOG2E); 
	return l_Fog;
} 

float CalcExp2Fog(float Depth, float ExpDensityFog) 
{ 
	const float LOG2E = 1.442695; // = 1 / log(2) 
	float l_Fog = exp2(-ExpDensityFog * ExpDensityFog * Depth * Depth * LOG2E); 
	return l_Fog;
} 

float CalcHeightFog( float Depth, // camera to point distance
               float3  rayDir )  // camera to point vector
{
	const float c = 1.0; // hight
	const float b = 0.3; // density
	
    //float fogAmount = c * exp(-m_CameraPosition.y*b) * (1.0-exp( -Depth*rayDir.y*b ))/rayDir.y;
	float fogAmount = m_HeightFog * (1.0 - exp( -Depth * rayDir.y * m_DensityFog )) / rayDir.y;
	if (fogAmount < 0.4) 
	{
		fogAmount = fogAmount;
	}
	
    return fogAmount;
}

float4 GetFogColor(float Depth, float3 Pos) 
{ 
	float l_FogIntensity = 0.0;
	
	if (m_Type == 1.0)
	{
		l_FogIntensity=CalcLinearFog(Depth, m_StartFog, m_EndFog);
	}
	else if (m_Type == 2.0)
	{
		l_FogIntensity=m_MaxAttenuation*(1-CalcExpFog(Depth, m_DensityFog));
	}
	else if (m_Type == 3.0)
	{
		l_FogIntensity=m_MaxAttenuation*(1-CalcExp2Fog(Depth, m_DensityFog));
	}
	else if (m_Type == 4.0)
	{
		l_FogIntensity=CalcHeightFog(Depth, Pos);
	}
	else if (m_Type == 5.0)
	{
		l_FogIntensity=min(CalcLinearFog(Depth, m_StartFog, m_EndFog), CalcHeightFog(Depth, Pos));
	}

		
	return saturate(float4(m_FogColor.xyz,l_FogIntensity));
}

float4 mainPS(TVertexPS IN) : SV_Target
{
	if(m_Active==0.0)
		return float4(0,0,0,0);
	else
	{
		float3 l_worldPos = GetPositionFromZDepthView((T0Texture.Sample(S0Sampler,IN.UV).r),IN.UV, m_InverseView, m_InverseProjection);	
		float l_DistanceEyeToWorldPosition=length(l_worldPos-m_InverseView[3].xyz);	
		return GetFogColor(l_DistanceEyeToWorldPosition, l_worldPos);
	}
}