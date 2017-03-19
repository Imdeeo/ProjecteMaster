#ifndef LIGHTS_FXH
#define LIGHTS_FXH

#include "Globals.fxh"
#include "Samplers.fxh"

float4 shadowMapCalc(float3 l_WorldPosition)
{
	float4 auxResult = float4(1,1,1,1);
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
				auxResult = float4(0,0,0,1);
			}
		}
	}
	return auxResult;
}


float4 spotLight(float3 l_WorldPosition, float3 Nn, float4 l_albedo, int lightIndex, float _SpecularPower, float _SpecularFactor, float _Metalness, float3 _SpecularColor)
{
	// Factors in the final multiplication.
	float l_DiffuseContrib;
	float l_DistanceAttenuation;
	float l_SpotAttenuation;
	
	// Intermediate values
	float3 l_RayDirection = normalize(l_WorldPosition - m_LightPosition[lightIndex].xyz);
	float l_Distance = distance(l_WorldPosition, m_LightPosition[lightIndex].xyz);
	float l_DirectionContrib = dot(l_RayDirection, normalize(m_LightDirection[lightIndex].xyz));

	// In case of not using the extra gbuffer channel for specular color, calculate it from albedo according to metalness
	//float4 l_SpecularColor = float4((l_albedo.xyz + (1.0f-_Metalness)*(float3(1.0f, 1.0f, 1.0f)-l_albedo.xyz)), 1);
	float l_AlbedoFactor = (1 - _Metalness) * (1 - _SpecularFactor);
	/*
	 * PBR - energy conservation
	 * "a renderer should display rough surfaces as having larger reflection
	 * highlights which appear dimmer than the smaller, sharper highlights of a
	 * smooth surface"
	 */
	float l_BaseIntensity = 0.2;
	float l_SpecularIntensity =  l_BaseIntensity + (1-l_BaseIntensity) * pow(_SpecularPower/MAX_SPECULAR_POWER, SPECULAR_DIMMING_POWER);

	// Diffusion
	l_DiffuseContrib = dot(Nn, -l_RayDirection);
	l_DiffuseContrib = saturate(l_DiffuseContrib);
	
	// Distance attenuation (linear)
	l_DistanceAttenuation = 1 - saturate((l_Distance - m_LightAttenuationStartRangeArray[lightIndex]) / (m_LightAttenuationEndRangeArray[lightIndex] - m_LightAttenuationStartRangeArray[lightIndex]));

	// Angle attenuation (linear)
	l_SpotAttenuation = 1 - saturate((acos(l_DirectionContrib) - m_LightAngleArray[lightIndex]/2) / (m_LightFallOffAngleArray[lightIndex]/2 - m_LightAngleArray[lightIndex]/2));
	
	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - l_WorldPosition);
	float3 H = normalize(cameraToVertex - m_LightDirection[lightIndex].xyz);
	float3 specular = saturate(_SpecularFactor*l_SpecularIntensity*m_LightColor[lightIndex].xyz*pow(saturate(dot(Nn, H)), _SpecularPower)* _SpecularColor);
	specular *= l_SpotAttenuation;
	specular *= l_DistanceAttenuation;
	
	float4 outLight = float4((l_albedo.xyz*l_AlbedoFactor*l_DiffuseContrib*l_DistanceAttenuation*l_SpotAttenuation*m_LightColor[lightIndex].xyz*m_LightIntensityArray[lightIndex]+specular*m_LightIntensityArray[lightIndex]).xyz,1);
		
	return saturate(outLight);
}

float4 directionalLight(float3 l_WorldPosition, float3 Nn, float4 l_albedo, int lightIndex, float _SpecularPower, float _SpecularFactor, float _Metalness, float3 _SpecularColor)
{
	float l_DiffuseContrib;	
	l_DiffuseContrib = dot(Nn, (-m_LightDirection[lightIndex].xyz));
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	// In case of not using the extra gbuffer channel for specular color, calculate it from albedo according to metalness
	//float4 l_SpecularColor = float4((l_albedo.xyz + (1.0f-_Metalness)*(float3(1.0f, 1.0f, 1.0f)-l_albedo.xyz)), 1);
	float l_AlbedoFactor = (1 - _Metalness) * (1 - _SpecularFactor);
	// PBR - energy conservation
	float l_BaseIntensity = 0.2;
	float l_SpecularIntensity =  l_BaseIntensity + (1-l_BaseIntensity) * pow(_SpecularPower/MAX_SPECULAR_POWER, SPECULAR_DIMMING_POWER);

	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - l_WorldPosition);
	float3 H = normalize(cameraToVertex -m_LightDirection[lightIndex].xyz);
	float aux = dot(cameraToVertex, - m_LightDirection[lightIndex].xyz);
	float3 specular = saturate(_SpecularFactor*l_SpecularIntensity*m_LightColor[lightIndex].xyz * pow(saturate(dot(Nn, H)), _SpecularPower)*_SpecularColor);
	float4 outLight = float4((l_albedo.xyz*l_AlbedoFactor*l_DiffuseContrib*m_LightColor[lightIndex].xyz*m_LightIntensityArray[lightIndex]+specular*m_LightIntensityArray[lightIndex]).xyz,1);
	
	return saturate(outLight);
}

float4 omniLight(float3 l_WorldPosition, float3 Nn, float4 l_albedo, int lightIndex, float _SpecularPower, float _SpecularFactor, float _Metalness, float3 _SpecularColor)
{
	float l_DistanceAttenuation;
	float l_DiffuseContrib;	
	float l_Distance = distance(l_WorldPosition, m_LightPosition[lightIndex].xyz);
	l_DiffuseContrib = dot(Nn, normalize(m_LightPosition[lightIndex].xyz-l_WorldPosition));
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	// In case of not using the extra gbuffer channel for specular color, calculate it from albedo according to metalness
	//float4 l_SpecularColor = float4((l_albedo.xyz + (1.0f-_Metalness)*(float3(1.0f, 1.0f, 1.0f)-l_albedo.xyz)), 1);
	float l_AlbedoFactor = (1 - _Metalness) * (1 - _SpecularFactor);
	
	// PBR - energy conservation
	float l_BaseIntensity = 0.2;
	float l_SpecularIntensity =  l_BaseIntensity + (1-l_BaseIntensity) * pow(_SpecularPower/MAX_SPECULAR_POWER, SPECULAR_DIMMING_POWER);

	// Distance attenuation (linear)
	l_DistanceAttenuation = 1 - saturate((l_Distance - m_LightAttenuationStartRangeArray[lightIndex]) / (m_LightAttenuationEndRangeArray[lightIndex] - m_LightAttenuationStartRangeArray[lightIndex]));
	
	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - l_WorldPosition);
	float3 lightToVertex = normalize(m_LightPosition[lightIndex].xyz - l_WorldPosition);
	float3 H = normalize(cameraToVertex + lightToVertex);
	float3 specular = saturate(_SpecularFactor*l_SpecularIntensity*m_LightColor[lightIndex].xyz*pow(saturate(dot(Nn, H)), _SpecularPower)*_SpecularColor);
	//specular *= l_SpotAttenuation;
	specular *= l_DistanceAttenuation;
	
	float4 outLight = float4((l_albedo.xyz*l_AlbedoFactor*l_DiffuseContrib*l_DistanceAttenuation*m_LightColor[lightIndex].xyz*m_LightIntensityArray[lightIndex]+specular*m_LightIntensityArray[lightIndex]).xyz,1);
	
	return saturate(outLight);	
}

float4 applyLights(float3 l_WorldPosition, float3 Nn, float4 l_albedo, int lightIndex, float l_SpecularPower, float l_SpecularFactor, float l_Metalness, float3 l_SpecularColor)
{
	float4 result = float4(0,0,0,1);
	if(m_LightTypeArray[lightIndex] == 0) //OMNI
	{
		result = omniLight(l_WorldPosition, Nn, l_albedo, lightIndex, l_SpecularPower, l_SpecularFactor, l_Metalness, l_SpecularColor);
	}
	if(m_LightTypeArray[lightIndex] == 1) //DIRECTIONAL
	{
		result = directionalLight(l_WorldPosition, Nn, l_albedo, lightIndex, l_SpecularPower, l_SpecularFactor, l_Metalness, l_SpecularColor);
	}
	if(m_LightTypeArray[lightIndex] == 2) //SPOT
	{
		result = spotLight(l_WorldPosition, Nn, l_albedo, lightIndex, l_SpecularPower, l_SpecularFactor, l_Metalness, l_SpecularColor);
	}
	return result;
}

#endif //LIGHTS_FXH