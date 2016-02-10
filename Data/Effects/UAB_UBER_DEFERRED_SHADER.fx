#include "Globals.fxh"
#include "Samplers.fxh"

struct VS_INPUT
{
 float4 Pos : POSITION;
 float3 Normal : NORMAL;
 float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
 float4 Pos : SV_POSITION;
 float3 Pixelpos : COLOR0;
 float3 Normal : NORMAL;
 float2 UV : TEXCOORD0;
};

PS_INPUT mainVS(VS_INPUT IN)
{
 PS_INPUT l_Output = (PS_INPUT)0;
 l_Output.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
 l_Output.Pixelpos = l_Output.Pos.xyz;
 l_Output.Pos = mul(l_Output.Pos, m_View);
 l_Output.Pos = mul(l_Output.Pos, m_Projection);
 l_Output.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
 l_Output.UV = IN.UV;
 
 return l_Output;
}

float4 spotLight(PS_INPUT IN)
{	
	// Factors in the final multiplication.
	float l_DiffuseContrib;
	float l_DistanceAttenuation;
	float l_SpotAttenuation;
	float P = 50;
	float4 SpecularColor = float4(1, 1, 1, 1);
	float3 Nn=normalize(IN.Normal);
	
	// Intermediate values
	float3 l_RayDirection = normalize(IN.Pixelpos - m_LightPosition[0]);
	float l_Distance = distance(IN.Pixelpos, m_LightPosition[0]);
	float l_DirectionContrib = dot(l_RayDirection, normalize(m_LightDirection[0]));
	
	// Diffusion
	l_DiffuseContrib = dot(IN.Normal, -l_RayDirection);
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	
	// Distance attenuation (linear)
	l_DistanceAttenuation = 1 - saturate((l_Distance - m_LightAttenuationStartRangeArray[0]) / (m_LightAttenuationEndRangeArray[0] - m_LightAttenuationStartRangeArray[0]));

	// Angle attenuation (linear)
	l_SpotAttenuation = 1 - saturate((acos(l_DirectionContrib) - m_LightAngleArray[0]/2) / (m_LightFallOffAngleArray[0]/2 - m_LightAngleArray[0]/2));
	
	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - IN.Pixelpos);
	float3 H = normalize(cameraToVertex - m_LightDirection[0]);
	float4 specular = SpecularColor * ((m_LightColor[0].xyz, 1.0) * pow(dot(Nn, H), P) * l_DiffuseContrib*l_DistanceAttenuation*l_SpotAttenuation);
	
	float4 outLight = l_DiffuseContrib*l_DistanceAttenuation*l_SpotAttenuation*(float4(m_LightColor[0].xyz, 1.0))*m_LightIntensityArray[0]+specular;
	
	return outLight;
}

float4 directionalLight(PS_INPUT IN)
{
	float P = 50;
	float4 SpecularColor = float4(1, 1, 1, 1);
	float l_DiffuseContrib;
	float3 Nn=normalize(IN.Normal);
	l_DiffuseContrib = dot(IN.Normal, (-m_LightDirection[0]));
	l_DiffuseContrib = max(0, l_DiffuseContrib);
	
	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - IN.Pixelpos);
	float3 H = normalize(cameraToVertex - m_LightDirection[0]);
	float4 specular = SpecularColor * ((m_LightColor[0].xyz, 1.0) * pow(dot(Nn, H), P) * l_DiffuseContrib);
	
	float4 outLight = l_DiffuseContrib*(float4(m_LightColor[0].xyz, 1.0))*m_LightIntensityArray[0]+specular;
	
	return outLight;
}

float4 omniLight(PS_INPUT IN)
{
	float P = 50;
	float4 SpecularColor = float4(1, 1, 1, 1);
	float l_DiffuseContrib;
	float3 Nn=normalize(IN.Normal);
	l_DiffuseContrib = dot(IN.Normal, normalize(m_LightPosition[0]-IN.Pixelpos));
	l_DiffuseContrib = max(0, l_DiffuseContrib);

	// Specular
	float3 cameraToVertex = normalize(m_CameraPosition.xyz - IN.Pixelpos);
	float3 lightToVertex = normalize(m_LightPosition[0].xyz - IN.Pixelpos);
	float3 H = normalize(cameraToVertex - lightToVertex);
	float4 specular = SpecularColor * ((m_LightColor[0].xyz, 1.0) * pow(dot(Nn, H), P) * l_DiffuseContrib);
	
	float4 outLight = l_DiffuseContrib*(float4(m_LightColor[0].xyz, 1.0))*m_LightIntensityArray[0]+specular;
	
	return outLight;
}

float4 applyLights(PS_INPUT IN)
{
	float4 lightContrib;
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
	float4 Out = float4(1,1,1,1);
	Out = T0Texture.Sample(S0Sampler, IN.UV);
	return Out*applyLights(IN);
}