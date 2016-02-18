#include "Globals.fxh"
#include "Samplers.fxh"

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


float4 spotLight(PS_INPUT IN)
{	
	// Factors in the final multiplication.
	float4 l_albedo =T0Texture.Sample(S0Sampler, IN.UV);
	float l_DiffuseContrib;
	float l_DistanceAttenuation;
	float l_SpotAttenuation;
	float P = 50;
	float4 SpecularColor = float4(1, 1, 1, 1);
	float l_Depth=T3Texture.Sample(S3Sampler, IN.UV).r;
	float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
	float3 Nn=Texture2Normal(T2Texture.Sample(S2Sampler, IN.UV).xyz);
	
	if(m_UseShadowMapArray[0]==1.0)
	{
		float4 l_LightViewPosition=mul(float4(l_WorldPosition, 1), m_LightView[0]);
		l_LightViewPosition=mul(l_LightViewPosition, m_LightProjection[0]);
		float2 l_ProjectedLightCoords=float2(((l_LightViewPosition.x/l_LightViewPosition.w)/2.0f)+0.5f, ((-l_LightViewPosition.y/l_LightViewPosition.w)/2.0f)+0.5f);
		float l_DepthShadowMap=T6Texture.Sample(S6Sampler, l_ProjectedLightCoords).r;
		float l_LightDepth=l_LightViewPosition.z/l_LightViewPosition.w;
		float m_ShadowMapBias = 0.0001f;
		l_DepthShadowMap=l_DepthShadowMap+m_ShadowMapBias;
		if((saturate(l_ProjectedLightCoords.x)==l_ProjectedLightCoords.x) && (saturate(l_ProjectedLightCoords.y)==l_ProjectedLightCoords.y))
		{
			if(l_LightDepth>l_DepthShadowMap)
			{
				return float4(0,0,0,1);
			}
		}
	}


	
	// Intermediate values
	float3 l_RayDirection = normalize(l_WorldPosition - m_LightPosition[0]);
	float l_Distance = distance(l_WorldPosition, m_LightPosition[0]);
	float l_DirectionContrib = dot(l_RayDirection, normalize(m_LightDirection[0]));
	
	// Diffusion
	l_DiffuseContrib = dot(Nn, -l_RayDirection);
	l_DiffuseContrib = saturate(l_DiffuseContrib);
	
	// Distance attenuation (linear)
	l_DistanceAttenuation = 1 - saturate((l_Distance - m_LightAttenuationStartRangeArray[0]) / (m_LightAttenuationEndRangeArray[0] - m_LightAttenuationStartRangeArray[0]));

	// Angle attenuation (linear)
	l_SpotAttenuation = 1 - saturate((acos(l_DirectionContrib) - m_LightAngleArray[0]/2) / (m_LightFallOffAngleArray[0]/2 - m_LightAngleArray[0]/2));
	
	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - l_WorldPosition);
	float3 H = normalize(cameraToVertex - m_LightDirection[0]);
	float3 specular = saturate(SpecularColor*m_LightColor[0]*pow(dot(Nn, H), P));
	//specular *= l_SpotAttenuation;
	
	float4 outLight = float4((l_albedo*l_DiffuseContrib*l_DistanceAttenuation*l_SpotAttenuation*m_LightColor[0]*m_LightIntensityArray[0]+specular).xyz,0);
		
	return saturate(outLight);
}

float4 directionalLight(PS_INPUT IN)
{
	float4 l_albedo = T0Texture.Sample(S0Sampler, IN.UV);

	float P = 50;
	float4 SpecularColor = float4(1, 1, 1, 1);
	float l_DiffuseContrib;
	float3 Nn=Texture2Normal(T2Texture.Sample(S2Sampler, IN.UV).xyz);
	l_DiffuseContrib = dot(Nn, (-m_LightDirection[0]));
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	
	// Specular
	float l_Depth = T3Texture.Sample(S3Sampler, IN.UV).r;
	float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
	
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - l_WorldPosition);
	float3 H = normalize(cameraToVertex -m_LightDirection[0]);
	float aux = dot(cameraToVertex, - m_LightDirection[0]);
	float4 specular = saturate(SpecularColor * m_LightColor[0] * pow(dot(Nn, H), P));
	float4 outLight = float4((l_albedo*l_DiffuseContrib*m_LightColor[0]*m_LightIntensityArray[0]+specular).xyz,1);
	
	return saturate(outLight);
}

float4 omniLight(PS_INPUT IN)
{
	float4 l_albedo =T0Texture.Sample(S0Sampler, IN.UV);

	float P = 50;
	float4 SpecularColor = float4(1, 1, 1, 1);
	float l_DiffuseContrib;
	
	float l_Depth=T3Texture.Sample(S3Sampler, IN.UV).r;
	float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
	float3 Nn=Texture2Normal(T2Texture.Sample(S2Sampler, IN.UV).xyz);
	l_DiffuseContrib = dot(Nn, normalize(m_LightPosition[0]-l_WorldPosition));
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	
	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - l_WorldPosition);
	float3 lightToVertex = normalize(m_LightPosition[0].xyz - l_WorldPosition);
	float3 H = normalize(cameraToVertex + lightToVertex);
	float3 specular = saturate(SpecularColor*m_LightColor[0]*pow(dot(Nn, H), P));
	
	float4 outLight = float4((l_albedo*l_DiffuseContrib*m_LightColor[0]*m_LightIntensityArray[0]+specular).xyz,1);
	
	return saturate(outLight);	
}

float4 applyLights(PS_INPUT IN)
{
	float4 lightContrib=float4(0,0,0,1);
	if(m_LightEnabledArray[0]==1)
	{
		if(m_LightTypeArray[0] == 0) //OMNI
		{
			lightContrib = omniLight(IN);
		}
		if(m_LightTypeArray[0] == 1) //DIRECTIONAL
		{
			lightContrib = directionalLight(IN);
		}
		if(m_LightTypeArray[0] == 2) //SPOT
		{
			lightContrib = spotLight(IN);
		}
	}
	return max(min(lightContrib,1),0);
}

float4 mainPS(PS_INPUT IN) : SV_Target
{		
	return applyLights(IN);
}