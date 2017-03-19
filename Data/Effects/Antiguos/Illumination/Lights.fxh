#ifndef LIGHTS_FXH
#define LIGHTS_FXH

#include "Globals.fxh"
#include "Samplers.fxh"

float4 shadowMapCalc(float3 l_WorldPosition)
{
	if(m_UseShadowMapArray[0]==1.0)
	{
		float4 l_LightViewPosition=mul(float4(l_WorldPosition, 1), m_LightView[0]);
		l_LightViewPosition=mul(l_LightViewPosition, m_LightProjection[0]);
		float2 l_ProjectedLightCoords=float2(((l_LightViewPosition.x/l_LightViewPosition.w)/2.0f)+0.5f, ((-l_LightViewPosition.y/l_LightViewPosition.w)/2.0f)+0.5f);
		float l_DepthShadowMap=T6Texture.Sample(S6Sampler, l_ProjectedLightCoords).r;
		float l_LightDepth=l_LightViewPosition.z/l_LightViewPosition.w;
		float m_ShadowMapBias = 0.001f;
		l_DepthShadowMap=l_DepthShadowMap+m_ShadowMapBias;
		if((saturate(l_ProjectedLightCoords.x)==l_ProjectedLightCoords.x) && (saturate(l_ProjectedLightCoords.y)==l_ProjectedLightCoords.y))
		{
			if(l_LightDepth>l_DepthShadowMap)
			{
				return float4(0,0,0,1);
			}
		}
	}
	return float4(1,1,1,1);
}


float4 spotLight(float3 l_WorldPosition, float3 Nn, float4 l_albedo, int lightIndex, float _SpecularPower, float _SpecularFactor)
{	
	// Factors in the final multiplication.	
	float l_DiffuseContrib;
	float l_DistanceAttenuation;
	float l_SpotAttenuation;
	
	// Intermediate values
	float3 l_RayDirection = normalize(l_WorldPosition - m_LightPosition[lightIndex]);
	float l_Distance = distance(l_WorldPosition, m_LightPosition[lightIndex]);
	float l_DirectionContrib = dot(l_RayDirection, normalize(m_LightDirection[lightIndex]));
	
	// Diffusion
	l_DiffuseContrib = dot(Nn, -l_RayDirection);
	l_DiffuseContrib = saturate(l_DiffuseContrib);
	
	// Distance attenuation (linear)
	l_DistanceAttenuation = 1 - saturate((l_Distance - m_LightAttenuationStartRangeArray[lightIndex]) / (m_LightAttenuationEndRangeArray[lightIndex] - m_LightAttenuationStartRangeArray[lightIndex]));

	// Angle attenuation (linear)
	l_SpotAttenuation = 1 - saturate((acos(l_DirectionContrib) - m_LightAngleArray[lightIndex]/2) / (m_LightFallOffAngleArray[lightIndex]/2 - m_LightAngleArray[lightIndex]/2));
	
	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - l_WorldPosition);
	float3 H = normalize(cameraToVertex - m_LightDirection[lightIndex]);
	float3 specular = saturate(_SpecularFactor*m_LightColor[lightIndex]*pow(dot(Nn, H), _SpecularPower));
	specular *= l_SpotAttenuation;
	specular *= l_DistanceAttenuation;
	
	float4 outLight = float4((l_albedo*l_DiffuseContrib*l_DistanceAttenuation*l_SpotAttenuation*m_LightColor[lightIndex]*m_LightIntensityArray[lightIndex]+specular*m_LightIntensityArray[lightIndex]).xyz,1);
		
	return saturate(outLight);
}

float4 directionalLight(float3 l_WorldPosition, float3 Nn, float4 l_albedo, int lightIndex, float _SpecularPower, float _SpecularFactor)
{
	float l_DiffuseContrib;	
	l_DiffuseContrib = dot(Nn, (-m_LightDirection[lightIndex]));
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	
	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - l_WorldPosition);
	float3 H = normalize(cameraToVertex -m_LightDirection[lightIndex]);
	float aux = dot(cameraToVertex, - m_LightDirection[lightIndex]);
	float4 specular = saturate(_SpecularFactor* m_LightColor[lightIndex] * pow(dot(Nn, H), _SpecularPower));
	float4 outLight = float4((l_albedo*l_DiffuseContrib*m_LightColor[lightIndex]*m_LightIntensityArray[lightIndex]+specular*m_LightIntensityArray[lightIndex]).xyz,1);
	
	return saturate(outLight);
}

float4 omniLight(float3 l_WorldPosition, float3 Nn, float4 l_albedo, int lightIndex, float _SpecularPower, float _SpecularFactor)
{
	float l_DistanceAttenuation;
	float l_DiffuseContrib;	
	float l_Distance = distance(l_WorldPosition, m_LightPosition[lightIndex]);
	l_DiffuseContrib = dot(Nn, normalize(m_LightPosition[lightIndex]-l_WorldPosition));
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	
	// Distance attenuation (linear)
	l_DistanceAttenuation = 1 - saturate((l_Distance - m_LightAttenuationStartRangeArray[lightIndex]) / (m_LightAttenuationEndRangeArray[lightIndex] - m_LightAttenuationStartRangeArray[lightIndex]));
	//l_DistanceAttenuation = 
	
	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - l_WorldPosition);
	float3 lightToVertex = normalize(m_LightPosition[lightIndex].xyz - l_WorldPosition);
	float3 H = normalize(cameraToVertex + lightToVertex);
	float3 specular = saturate(_SpecularFactor*m_LightColor[lightIndex]*pow(dot(Nn, H), _SpecularPower));
	//specular *= l_SpotAttenuation;
	specular *= l_DistanceAttenuation;
	
	float4 outLight = float4((l_albedo*l_DiffuseContrib*l_DistanceAttenuation*m_LightColor[lightIndex]*m_LightIntensityArray[lightIndex]+specular*m_LightIntensityArray[lightIndex]).xyz,1);
	
	return saturate(outLight);	
}

float4 applyLights(float3 l_WorldPosition, float3 Nn, float4 l_albedo, int lightIndex, float l_SpecularPower, float l_SpecularFactor)
{
	if(m_LightTypeArray[lightIndex] == 0) //OMNI
	{
		return omniLight(l_WorldPosition, Nn, l_albedo, lightIndex, l_SpecularPower, l_SpecularFactor);
	}
	if(m_LightTypeArray[lightIndex] == 1) //DIRECTIONAL
	{
		return directionalLight(l_WorldPosition, Nn, l_albedo, lightIndex, l_SpecularPower, l_SpecularFactor);
	}
	if(m_LightTypeArray[lightIndex] == 2) //SPOT
	{
		return spotLight(l_WorldPosition, Nn, l_albedo, lightIndex, l_SpecularPower, l_SpecularFactor);
	}
	return float4(0,0,0,1);
}

#endif //LIGHTS_FXH