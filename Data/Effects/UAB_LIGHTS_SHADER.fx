/*

% Spot light shader.

date: 17122015

*/
#include "Globals.fxh"

Texture2D DiffuseTexture : register( t0 );
SamplerState LinearSampler : register( s0 );

struct TVertexVS
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct TVertexPS
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float3 Pixelpos : TEXCOORD1;
};


TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.Pixelpos = l_Out.Pos.xyz;
	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);
	l_Out.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
	l_Out.UV = IN.UV;
	
	return l_Out;
}

float4 spotLight(TVertexPS IN, uint LightIndex)
{	
	// Factors in the final multiplication.
	float l_DiffuseContrib;
	float l_DistanceAttenuation;
	float l_SpotAttenuation;
	
	// Intermediate values
	float3 l_RayDirection = normalize(IN.Pixelpos - m_LightPosition[LightIndex]);
	float l_Distance = distance(IN.Pixelpos, m_LightPosition[LightIndex]);
	float l_DirectionContrib = dot(l_RayDirection, normalize(m_LightDirection[LightIndex]));
	
	// Diffusion
	l_DiffuseContrib = dot(IN.Normal, -l_RayDirection);
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	
	// Distance attenuation (linear)
	l_DistanceAttenuation = 1 - saturate((l_Distance - m_LightAttenuationStartRangeArray[LightIndex]) / (m_LightAttenuationEndRangeArray[LightIndex] - m_LightAttenuationStartRangeArray[LightIndex]));

	// Angle attenuation (linear)
	l_SpotAttenuation = 1 - saturate((acos(l_DirectionContrib) - m_LightAngleArray[LightIndex]/2) / (m_LightFallOffAngleArray[LightIndex]/2 - m_LightAngleArray[LightIndex]/2));
	
	return l_DiffuseContrib*l_DistanceAttenuation*l_SpotAttenuation*(float4(m_LightColor[LightIndex].xyz, 1.0));
}

float4 directionalLight(TVertexPS IN,uint LightIndex)
{
	float l_DiffuseContrib;
	l_DiffuseContrib = dot(IN.Normal, (-m_LightDirection[LightIndex]));
	l_DiffuseContrib = max(0, l_DiffuseContrib);

	return l_DiffuseContrib*(float4(m_LightColor[LightIndex].xyz, 1.0));
}

float4 omniLight(TVertexPS IN, uint LightIndex)
{
	float l_DiffuseContrib;
	l_DiffuseContrib = dot(IN.Normal, normalize(IN.Pixelpos - m_LightPosition[LightIndex]));
	l_DiffuseContrib = max(0, l_DiffuseContrib);

	return l_DiffuseContrib*(float4(m_LightColor[LightIndex].xyz, 1.0));
}
float4 applyLights(TVertexPS IN)
{
	float4 lightContrib = m_LightAmbient;
	for(uint i = 0;i<MAX_LIGHTS_BY_SHADER;i++)
	{
		if(m_LightEnabledArray[i])
		{
			if(m_LightTypeArray[i] == 0) //OMNI
			{
				lightContrib = lightContrib + omniLight(IN,i);
			}
			if(m_LightTypeArray[i] == 1) //DIRECTIONAL
			{
				lightContrib = lightContrib + directionalLight(IN,i);
			}
			if(m_LightTypeArray[i] == 2) //SPOT
			{
				lightContrib = lightContrib + spotLight(IN,i);
			}
		}
	}
	return lightContrib;
}

float4 mainPS(TVertexPS IN) : SV_Target
{
	return DiffuseTexture.Sample(LinearSampler, IN.UV)*applyLights(IN);
}
