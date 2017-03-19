#include "globals.fxh"
#include "Samplers.fxh"

static bool m_Enabled=m_RawDataArray[0]==1;
static float m_SSROpacity=m_RawDataArray[1];
static float m_OffsetScreen=m_RawDataArray[2];
 
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

float3 CalcIntersection(float3 CameraPosition, float3 VectorDirectorNewPixel, float3 OriginalPixelPosition, float3 VectorDirectorReflected)
{
	//line vs plane method
	float3 l_Normal=normalize(cross(VectorDirectorNewPixel, VectorDirectorReflected));
	l_Normal=normalize(cross(l_Normal, VectorDirectorReflected));
	//Plane=Normal+OriginalPixelPosition
	//Line=VectorDirectorNewPixel+CameraPosition
	//P0=OriginalPixelPosition
	//L0=CameraPosition
	//d=((p0-l0)·n)/(l·n);
	
	float d=dot((OriginalPixelPosition-CameraPosition), l_Normal)/(dot(VectorDirectorNewPixel, l_Normal));
	return CameraPosition+d*VectorDirectorNewPixel;
}

float3 GetReflectedColor(float3 WorldPosition, float3 Nn, float2 UV)
{
	float3 l_CameraToWorldPosition=WorldPosition-m_InverseView[3].xyz; // vector director desde la camara al pixel
	float3 l_CameraToWorldNormalized=normalize(l_CameraToWorldPosition);
	
	float3 l_ReflectedVector=normalize(reflect(l_CameraToWorldNormalized, Nn)); // vector reflejado a partir del vector directior anterior
	
	float3 l_WorldPositionWithReflection=WorldPosition+l_ReflectedVector;
	float4 l_WorldPositionWithReflectionUV=mul(float4(l_WorldPositionWithReflection, 1.0), m_View);
	l_WorldPositionWithReflectionUV=mul(float4(l_WorldPositionWithReflectionUV.xyz, 1.0), m_Projection);

	float2 l_ReflectedDirectionUV = 0.5 * float2(l_WorldPositionWithReflectionUV.x/l_WorldPositionWithReflectionUV.w,-l_WorldPositionWithReflectionUV.y/l_WorldPositionWithReflectionUV.w) + 0.5; // vector que apunta a un pixel "rebotado" en coordenada de pantalla
	l_ReflectedDirectionUV=normalize(l_ReflectedDirectionUV-UV); // vector director en coordenada de pantalla
	float3 l_Color;
	
	float2 l_UV=UV;
	float i=0;
	float l_Increment=1.0/50.0;
	do
	{
		i=i+l_Increment;
		l_UV=UV+l_ReflectedDirectionUV*i;
		
		float l_Depth=T2Texture.Sample(S2Sampler, l_UV).x;
		float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, l_UV, m_InverseView, m_InverseProjection);
		//Buscar punto de corte entre el Vector reflejado y el vector que une el ojo con el punto de de pantalla en coordenadas de mundo
		float3 l_EyeToWorldPosition=l_WorldPosition-m_InverseView[3].xyz;
		float l_DistanceToWorldPosition=length(l_EyeToWorldPosition);
		float3 l_EyeToWorldPositionVector=l_EyeToWorldPosition/l_DistanceToWorldPosition;
		float3 l_IntersectionPoint=CalcIntersection(m_InverseView[3].xyz, l_EyeToWorldPositionVector, WorldPosition, l_ReflectedVector);
		float l_DistanceToIntersectionPoint=length(l_IntersectionPoint-m_InverseView[3].xyz);
		
		if(l_DistanceToIntersectionPoint>=l_DistanceToWorldPosition)
			break;
			
	} while(i<1.0);
	
	if(l_UV.x>=(1.0-m_OffsetScreen) || l_UV.x<m_OffsetScreen || l_UV.y>=(1.0-m_OffsetScreen) || l_UV.y<m_OffsetScreen)
		l_UV=UV;
	else
	{
		bool l_CorrectJitter=true;
		if(l_CorrectJitter)
		{
			float l_TotalElements=30.0;
			l_Increment=l_Increment/l_TotalElements;
			i=0;
			float2 l_UV2=l_UV;
			do
			{
				i=i+1;
				l_UV=l_UV2-l_ReflectedDirectionUV*i*l_Increment;
				
				float l_Depth=T2Texture.Sample(S2Sampler, l_UV).x;
				float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, l_UV, m_InverseView, m_InverseProjection);
				//Buscar punto de corte entre el Vector reflejado y el vector que une el ojo con el punto de de pantalla en coordenadas de mundo
				float3 l_EyeToWorldPosition=l_WorldPosition-m_InverseView[3].xyz;
				float l_DistanceToWorldPosition=length(l_EyeToWorldPosition);
				float3 l_EyeToWorldPositionVector=l_EyeToWorldPosition/l_DistanceToWorldPosition;
				float3 l_IntersectionPoint=CalcIntersection(m_InverseView[3].xyz, l_EyeToWorldPositionVector, WorldPosition, l_ReflectedVector);
				float l_DistanceToIntersectionPoint=length(l_IntersectionPoint-m_InverseView[3].xyz);
				
				if(l_DistanceToIntersectionPoint<l_DistanceToWorldPosition)
				{
					l_UV=l_UV+l_ReflectedDirectionUV*l_Increment;
					break;
				}
			} while(i<l_TotalElements);
		}
	}
	
	l_Color=T0Texture.Sample(S0Sampler, l_UV).xyz;
	return l_Color;
}

float4 SSRPS(PS_INPUT IN) : SV_Target
{
	float l_Pct=T1Texture.Sample(S1Sampler, IN.UV).w;
	if(l_Pct> 0.1 && m_Enabled)
	{
		float l_Depth=T2Texture.Sample(S2Sampler, IN.UV.xy).x;
		float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
		
		float3 Nn=normalize(T1Texture.Sample(S1Sampler, IN.UV.xy).xyz * 2.0 - 1.0);
		return float4(GetReflectedColor(l_WorldPosition, Nn, IN.UV).xyz, m_SSROpacity*l_Pct);
	}
	clip(-1);
	return float4(1,1,1,1);
}
