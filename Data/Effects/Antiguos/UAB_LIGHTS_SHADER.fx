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
<<<<<<< HEAD
	uint  LightIndex;
=======
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
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

<<<<<<< HEAD
float4 spotLight(TVertexPS IN): SV_Target
=======
float4 spotLight(TVertexPS IN, uint LightIndex)
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
{	
	// Factors in the final multiplication.
	float l_DiffuseContrib;
	float l_DistanceAttenuation;
	float l_SpotAttenuation;
	
	// Intermediate values
<<<<<<< HEAD
	float3 l_RayDirection = normalize(IN.Pixelpos - m_LightPosition[IN.LightIndex]);
	float l_Distance = distance(IN.Pixelpos, m_LightPosition[IN.LightIndex]);
	float l_DirectionContrib = dot(l_RayDirection, normalize(m_LightDirection[IN.LightIndex]));
=======
	float3 l_RayDirection = normalize(IN.Pixelpos - m_LightPosition[LightIndex]);
	float l_Distance = distance(IN.Pixelpos, m_LightPosition[LightIndex]);
	float l_DirectionContrib = dot(l_RayDirection, normalize(m_LightDirection[LightIndex]));
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
	
	// Diffusion
	l_DiffuseContrib = dot(IN.Normal, -l_RayDirection);
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	
	// Distance attenuation (linear)
<<<<<<< HEAD
	l_DistanceAttenuation = 1 - saturate((l_Distance - m_LightAttenuationStartRangeArray[IN.LightIndex]) / (m_LightAttenuationEndRangeArray[IN.LightIndex] - m_LightAttenuationStartRangeArray[IN.LightIndex]));

	// Angle attenuation (linear)
	l_SpotAttenuation = 1 - saturate((acos(l_DirectionContrib) - m_LightAngleArray[IN.LightIndex]/2) / (m_LightFallOffAngleArray[IN.LightIndex]/2 - m_LightAngleArray[IN.LightIndex]/2));
	
	return l_DiffuseContrib*l_DistanceAttenuation*l_SpotAttenuation*(float4(m_LightColor[IN.LightIndex].xyz, 1.0));
}

float4 directionalLight(TVertexPS IN): SV_Target
{
	float l_DiffuseContrib;
	l_DiffuseContrib = dot(IN.Normal, (-m_LightDirection[IN.LightIndex]));
	l_DiffuseContrib = max(0, l_DiffuseContrib);

	return l_DiffuseContrib*(float4(m_LightColor[IN.LightIndex].xyz, 1.0));
}

float4 omniLight(TVertexPS IN): SV_Target
{
	float l_DiffuseContrib;
	l_DiffuseContrib = dot(IN.Normal, normalize(IN.Pixelpos - m_LightPosition[IN.LightIndex]));
	l_DiffuseContrib = max(0, l_DiffuseContrib);

	return l_DiffuseContrib*(float4(m_LightColor[IN.LightIndex].xyz, 1.0));
}

float4 applyLights(TVertexPS IN): SV_Target
=======
	l_DistanceAttenuation = 1 - saturate((l_Distance - m_LightAttenuationStartRangeArray[LightIndex]) / (m_LightAttenuationEndRangeArray[LightIndex] - m_LightAttenuationStartRangeArray[LightIndex]));

	// Angle attenuation (linear)
	l_SpotAttenuation = 1 - saturate((acos(l_DirectionContrib) - m_LightAngleArray[LightIndex]/2) / (m_LightFallOffAngleArray[LightIndex]/2 - m_LightAngleArray[LightIndex]/2));
	
	return l_DiffuseContrib*l_DistanceAttenuation*l_SpotAttenuation*(float4(m_LightColor[LightIndex].xyz, 1.0))*m_LightIntensityArray[LightIndex];
}

float4 directionalLight(TVertexPS IN,uint LightIndex)
{
	float l_DiffuseContrib;
	l_DiffuseContrib = dot(IN.Normal, (-m_LightDirection[LightIndex]));
	l_DiffuseContrib = max(0, l_DiffuseContrib);

	return l_DiffuseContrib*(float4(m_LightColor[LightIndex].xyz, 1.0))*m_LightIntensityArray[LightIndex];
}

float4 omniLight(TVertexPS IN, uint LightIndex)
{
	float l_DiffuseContrib;
	l_DiffuseContrib = dot(IN.Normal, normalize(IN.Pixelpos - m_LightPosition[LightIndex]));
	l_DiffuseContrib = max(0, l_DiffuseContrib);

	return l_DiffuseContrib*(float4(m_LightColor[LightIndex].xyz, 1.0))*m_LightIntensityArray[LightIndex];
}
float4 applyLights(TVertexPS IN)
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
{
	float4 lightContrib = m_LightAmbient;
	for(uint i = 0;i<MAX_LIGHTS_BY_SHADER;i++)
	{
<<<<<<< HEAD
		if(m_LightEnabledArray[i])
		{
			IN.LightIndex = i;
			if(m_LightTypeArray==0) //OMNI
			{
				lightContrib = lightContrib * omniLight(IN);
			}
			if(m_LightTypeArray==1) //DIRECTIONAL
			{
				lightContrib = lightContrib * directionalLight(IN);
			}
			if(m_LightTypeArray==2) //SPOT
			{
				lightContrib = lightContrib * spotLight(IN);
			}
		}
	}
	return lightContrib;
=======
		if(m_LightEnabledArray[i]==1)
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
	return max(min(lightContrib,1),0);
>>>>>>> 2415e5237c3b6016faf70d3a66b60ecd2a66b0a7
}

float4 mainPS(TVertexPS IN) : SV_Target
{
	return DiffuseTexture.Sample(LinearSampler, IN.UV)*applyLights(IN);
}
