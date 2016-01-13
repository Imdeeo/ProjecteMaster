/*

% Spot light shader.

date: 17122015

*/
#include "Globals.fxh"

#ifdef HAS_UV
Texture2D DiffuseTexture : register( t0 );
SamplerState LinearSampler : register( s0 );
#endif

#ifdef HAS_UV2
Texture2D LightMapTexture : register( l0 );
SamplerState LinearSampler2 : register( s1 );
#endif

struct TVertexVS
{
	float3 Pos : POSITION;
#ifdef HAS_WEIGHT_INDICES
	float4 Weight:BLENDWEIGHT;
	float4 Indices:BLENDINDICES;
#endif
#ifdef HAS_NORMAL
	float3 Normal : NORMAL;
#endif
#ifdef HAS_UV
	float2 UV : TEXCOORD0;
#endif
#ifdef HAS_UV2
	float2 UV2 : TEXCOORD1;
#endif
};

struct TVertexPS
{
	float4 Pos : SV_POSITION;
#ifdef HAS_NORMAL
	float3 Normal : NORMAL;
#endif
#ifdef HAS_UV
	float2 UV : TEXCOORD0;
#endif
#ifdef HAS_LIGHTS
	float3 Pixelpos : TEXCOORD1;
#endif
#ifdef HAS_UV2
	float2 UV2 : TEXCOORD1;
#endif
};


TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;

#ifdef HAS_WEIGHT_INDICES
	float4 l_TempPos=float4(IN.Pos.xyz, 1.0);
	float3 l_Position= 0;
	float4 l_Indices=IN.Indices;					
	l_Position=mul(l_TempPos, m_Bones[l_Indices.x]) * IN.Weight.x;
	l_Position+=mul(l_TempPos, m_Bones[l_Indices.y]) * IN.Weight.y;
	l_Position+=mul(l_TempPos, m_Bones[l_Indices.z]) * IN.Weight.z;
	l_Position+=mul(l_TempPos, m_Bones[l_Indices.w]) * IN.Weight.w;
#ifdef HAS_NORMAL
	float3 l_Normal= 0;	
	float3x3 m;					
	m[0].xyz = m_Bones[l_Indices.x][0].xyz;
	m[1].xyz = m_Bones[l_Indices.x][1].xyz;
	m[2].xyz = m_Bones[l_Indices.x][2].xyz;				
	l_Normal+=mul(IN.Normal.xyz, m)* IN.Weight.x;					
	m[0].xyz = m_Bones[l_Indices.y][0].xyz;
	m[1].xyz = m_Bones[l_Indices.y][1].xyz;
	m[2].xyz = m_Bones[l_Indices.y][2].xyz;				
	l_Normal+=mul(IN.Normal.xyz, m)* IN.Weight.y;
#endif	
	l_Out.Pos = mul(float4(l_Position, 1.0), m_World);
#else
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
#endif

#ifdef HAS_LIGHTS
	l_Out.Pixelpos = l_Out.Pos.xyz;
#endif
	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);

#ifdef HAS_NORMAL
#ifdef HAS_WEIGHT_INDICES
		l_Out.Normal = l_Normal;
#else
		l_Out.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
#endif
#endif

#ifdef HAS_UV
	l_Out.UV = IN.UV;
#endif
#ifdef HAS_UV2
	l_Out.UV2 = IN.UV2;
#endif
	return l_Out;
}

#ifdef HAS_LIGHTS
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
{
#ifdef HAS_UV2
	float4 lightContrib = LightMapTexture.Sample(LinearSampler2, IN.UV)
#else
	float4 lightContrib = m_LightAmbient;
#endif
	for(uint i = 0;i<MAX_LIGHTS_BY_SHADER;i++)
	{
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
}
#endif

float4 mainPS(TVertexPS IN) : SV_Target
{
#ifdef HAS_LIGHTS
#ifdef HAS_NORMAL
	return DiffuseTexture.Sample(LinearSampler, IN.UV)*applyLights(IN);
#endif
#endif
	return DiffuseTexture.Sample(LinearSampler, IN.UV);
}
