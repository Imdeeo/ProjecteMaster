#include "globals.fxh"
#include "Samplers.fxh"

static float m_Enabled=m_RawDataArray[0];
static float m_SSRQuality=m_RawDataArray[1];
static float m_SSROpacity=m_RawDataArray[2];
static float m_SSRIncrementMultiplier=m_RawDataArray[3];
static float m_OffsetScreen=m_RawDataArray[4];
static float2 m_ScreenResolution=float2(m_RawDataArray[5],m_RawDataArray[6]);

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

float4 CalcSSRColor(float2 UV, float4x4 ViewProjection, float4 SourceColor, float3 WorldPosition, float3 Nn, float ReflectionFactor)
{
	float4 l_Color=float4(0, 0, 0, 0);
	float3 l_CameraToWorldPosition=WorldPosition-m_InverseView[3].xyz;
	float3 l_CameraToWorldNormalized=normalize(l_CameraToWorldPosition);
	float l_CameraToWorldDistance=distance(WorldPosition,m_InverseView[3].xyz);
	float3 l_ReflectedVector=normalize(reflect(l_CameraToWorldNormalized, Nn));
	
	if(dot(l_ReflectedVector, l_CameraToWorldNormalized)<0)
		return SourceColor;
	
	float4 l_ScreenPosition;
	float i=0;
	float3 l_RayTrace=WorldPosition;
	float l_CurrentWorldDistance, l_RayDistance;
	float incr = 1 / m_SSRQuality;
	float total_distance;
	
	l_ReflectedVector = l_ReflectedVector * incr;
	
	do
	{
		i+=0.05;
		
		l_RayTrace+=l_ReflectedVector;
		l_ScreenPosition=mul(float4(l_RayTrace, 1), ViewProjection);
		
		l_ScreenPosition.xyz/=l_ScreenPosition.w;
		l_ScreenPosition.x=(l_ScreenPosition.x+1)*0.5;
		l_ScreenPosition.y=(1-l_ScreenPosition.y)*0.5;
		float2 l_ScreenCoords=l_ScreenPosition.xy;
		
		float l_Depth=T2Texture.Sample(S2Sampler, l_ScreenCoords).x;
		float3 l_CurrentWorldPosition=GetPositionFromZDepthView(l_Depth, l_ScreenCoords, m_InverseView, m_InverseProjection); // Aqui multiplicaba por la resolucion pero a partir de un valor muy bajo, como 3 o 4 ya no cambiaba el resultado. Al quitarselo parece que empieza a pintar desde la base pero da la impresion de que entonces se ven varios reflejos
		l_CurrentWorldDistance=distance(l_CurrentWorldPosition.xyz,m_InverseView[3].xyz);
		l_RayDistance=distance(l_RayTrace.xyz,m_InverseView[3].xyz);
		
		if(l_ScreenPosition.x>1 || l_ScreenPosition.x<-1 || l_ScreenPosition.y>1 ||	l_ScreenPosition.y<-1 || i>=0.5 || l_CameraToWorldDistance>l_CurrentWorldDistance)
			break;
	} while(l_RayDistance < l_CurrentWorldDistance);
	
	l_Color=T0Texture.Sample(S0Sampler, l_ScreenPosition);
	float l_SSRContrib=0.0;
	
	if(l_ScreenPosition.x>1 || l_ScreenPosition.x<0 || l_ScreenPosition.y>1 || l_ScreenPosition.y<0)
		l_SSRContrib=1.0;
	else if(l_CameraToWorldDistance>l_CurrentWorldDistance)
		l_SSRContrib=1.0;
	else
	{
		float l_SSRContribX=l_ScreenPosition.x<0.5 ? (1.0-min(1.0, l_ScreenPosition.x/m_OffsetScreen)) : max(0.0, (l_ScreenPosition.y-(1.0f-m_OffsetScreen))/m_OffsetScreen);
		float l_SSRContribY=l_ScreenPosition.y<0.5 ? (1.0-min(1.0, l_ScreenPosition.y/m_OffsetScreen)) : max(0.0, (l_ScreenPosition.y-(1.0f-m_OffsetScreen))/m_OffsetScreen);
		l_SSRContrib=max(l_SSRContribX, l_SSRContrib);
		l_SSRContrib=max(l_SSRContribY, l_SSRContrib);
	}
	
	l_Color = SourceColor * l_SSRContrib + l_Color * (1-l_SSRContrib) * ReflectionFactor;
	return l_Color;
}

float4 SSRPS(PS_INPUT IN) : SV_Target
{
	float4 l_Color=T0Texture.Sample(S0Sampler, IN.UV);
	float l_ReflectionFactor = T1Texture.Sample(S1Sampler, IN.UV).w;
	
	if(m_Enabled && l_ReflectionFactor > 0.0f)
	{
		float3 Nn=normalize(T1Texture.Sample(S1Sampler, IN.UV.xy).xyz * 2 - 1);
		float l_Depth=T2Texture.Sample(S2Sampler, IN.UV).x;
		float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
		float4x4 l_ViewProjection=mul(m_View, m_Projection);
		return float4(CalcSSRColor(IN.UV.xy, l_ViewProjection, l_Color, l_WorldPosition, Nn, l_ReflectionFactor).xyz, m_SSROpacity);
	}

	clip(-1);
	return l_Color;
}