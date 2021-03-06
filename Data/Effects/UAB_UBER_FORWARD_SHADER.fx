#include "Samplers.fxh"
#include "Lights.fxh"

static float m_Active = m_RawDataArray[0];
static float m_Exposure = m_RawDataArray[1];
static float m_SpecularPower = m_RawDataArray[2];
static float m_SpecularFactor = m_RawDataArray[3];
static float m_ReflectionFactor = m_RawDataArray[4];
static float m_BumpFactor = m_RawDataArray[6];
static float m_MetalnessFactor = m_RawDataArray[7];
static float3 m_MetalColor = float3(m_RawDataArray[8], m_RawDataArray[9], m_RawDataArray[10]);
static float m_MetalColorWeight = m_RawDataArray[11];

#ifndef HAS_UV
	#ifndef HAS_COLOR
		#define HAS_COLOR
	#endif
#endif

struct TVertexVS
{
	#ifdef HAS_POSITION4
		float4 Pos : POSITION;
	#else
		float3 Pos : POSITION;
	#endif

	#ifdef HAS_COLOR
		float4 Color:COLOR0;
	#endif

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

	#ifdef HAS_TANGENT
		float4 Tangent : TEXCOORD3;
	#endif
};

struct TVertexPS
{
	float4 Pos : SV_POSITION;
	#ifdef HAS_COLOR
		float4 Color:COLOR0;
	#endif

	#ifdef HAS_NORMAL
		float3 Normal : NORMAL;
	#endif

	#ifdef HAS_UV
		float2 UV : TEXCOORD0;
	#endif

	#ifdef HAS_LIGHTS
		#if HAS_COLOR
			float3 Pixelpos : TEXCOORD0;
		#else
			float3 Pixelpos : COLOR0;
		#endif
	#else
		#if HAS_REFLECTION
			#if HAS_COLOR
				float3 Pixelpos : TEXCOORD0;
			#else
				float3 Pixelpos : COLOR0;
			#endif
		#endif
	#endif

	#ifdef HAS_UV2
		float2 UV2 : TEXCOORD1;
	#endif

	#ifdef HAS_TANGENT
		float3 WorldTangent: TEXCOORD2;
		float3 WorldBinormal: TEXCOORD3;
	#endif
};


TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	float3 l_Normal= 0;
	#ifdef HAS_WEIGHT_INDICES
		float4 l_TempPos=float4(IN.Pos.xyz, 1.0);
		float3 l_Position= 0;
		float4 l_Indices=IN.Indices;
		l_Position=mul(l_TempPos, m_Bones[l_Indices.x]).xyz * IN.Weight.x;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.y]).xyz * IN.Weight.y;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.z]).xyz * IN.Weight.z;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.w]).xyz * IN.Weight.w;

		#ifdef HAS_NORMAL
			
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
		#ifdef HAS_NORMAL
			l_Normal = IN.Normal;
		#endif
	#endif

	#ifdef HAS_LIGHTS
		l_Out.Pixelpos = l_Out.Pos.xyz;
	#else
		#ifdef HAS_REFLECTION
			l_Out.Pixelpos = l_Out.Pos.xyz;
		#endif
	#endif

	l_Out.Pos = mul(l_Out.Pos, m_View);
	l_Out.Pos = mul(l_Out.Pos, m_Projection);

	#ifdef HAS_NORMAL
		//#ifdef HAS_WEIGHT_INDICES
		//	l_Out.Normal = l_Normal;
		//#else
			l_Out.Normal = normalize(mul(l_Normal, (float3x3)m_World));
		//#endif
	#endif

	#ifdef HAS_UV
		l_Out.UV = IN.UV;
	#endif

	#ifdef HAS_UV2
		l_Out.UV2 = IN.UV2;
	#endif

	#ifdef HAS_COLOR
		l_Out.Color = IN.Color;
	#endif

	#ifdef HAS_TANGENT
		l_Out.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
		l_Out.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
	#endif

	return l_Out;
}

#ifdef HAS_LIGHTS
float4 applyAllLights(TVertexPS IN, float3 Nn, float SpecularFactor, float4 Albedo, float AlbedoFactor, float Metalness, float3 SpecularColor)
{
	float l_specularFactor=SpecularFactor;
	float4 l_Out = Albedo;

	#ifdef HAS_UV2
		float4 lightContrib = T1Texture.Sample(S1Sampler, IN.UV2);
	#else
		float4 lightContrib = m_LightAmbient*AlbedoFactor*l_Out;
	#endif

	float l_SpecularPower = (pow(MAX_SPECULAR_POWER/MIN_SPECULAR_POWER, m_SpecularPower/100) * MIN_SPECULAR_POWER);

	for(int i = 0;i<MAX_LIGHTS_BY_SHADER;i++)
	{
		if (m_LightEnabledArray[i])
		{
			lightContrib += applyLights(IN.Pixelpos,Nn,l_Out,i, l_SpecularPower, l_specularFactor, Metalness, SpecularColor);
		}
	}
	return saturate(float4(lightContrib.xyz, l_Out.w));
}
#endif

float4 mainPS(TVertexPS IN) : SV_Target
{
	float4 Out = float4(1,1,1,1);

	#ifdef HAS_COLOR
		Out = IN.Color;
	#endif
	
	#ifdef HAS_NORMAL
		float3 Nn = IN.Normal;
	#endif

	#if defined(HAS_LIGHTS) || defined(HAS_REFLECTION)
		float4 l_Albedo = float4(1,1,1,1);
		#ifdef HAS_UV
			l_Albedo = T0Texture.Sample(S0Sampler, IN.UV);
		#endif
		float l_SpecularFactor = 1.0;
		#ifdef HAS_TANGENT
			float g_Bump = 2.4 * m_BumpFactor;

			float3 Tn=normalize(IN.WorldTangent);
			float3 Bn=normalize(IN.WorldBinormal);
			float4 auxNormal = T2Texture.Sample(S2Sampler,IN.UV);
			float3 bump=g_Bump*((auxNormal.xyz) - float3(0.5,0.5,0.5));
			Nn = Nn + bump.x*Tn + bump.y*Bn;
			Nn = normalize(Nn);
			l_SpecularFactor *= auxNormal.w;
		#endif
		float3 l_EyeToWorldPosition = normalize(IN.Pixelpos-m_InverseView[3].xyz);
		
		#ifdef HAS_SPECULAR_MAP
			float4 l_SpecularColor = T10Texture.Sample(S10Sampler, IN.UV);
			l_SpecularFactor *= max(l_SpecularColor.r, max(l_SpecularColor.g, l_SpecularColor.b));
		#endif
		float l_Fresnel = pow(1 - dot(-l_EyeToWorldPosition, Nn), FRESNEL_POWER);
		l_SpecularFactor *= m_SpecularFactor + l_Fresnel * (1-m_SpecularFactor);
		
		#if defined(HAS_SPECULAR_MAP)
			float l_AlbedoFactor = 1 - l_SpecularFactor;
			float l_Metalness = 0.0f;
		#else
			#ifdef HAS_METALNESS_MAP
				float l_Metalness = T10Texture.Sample(S10Sampler, IN.UV).x * m_MetalnessFactor;
			#else
				float l_Metalness = m_MetalnessFactor;
			#endif
			l_SpecularFactor = l_SpecularFactor + l_Metalness * max(0, METAL_SPECFACTOR-l_SpecularFactor);
			float l_AlbedoFactor = 1 - METAL_SPECFACTOR*l_Metalness;
			l_AlbedoFactor *= 1 - l_SpecularFactor;
			float l_MetalColorRatio = l_Metalness * m_MetalColorWeight;
			float3 l_ReflectionColor = l_MetalColorRatio * m_MetalColor + (1-l_MetalColorRatio) * l_Albedo.rgb;
			float4 l_SpecularColor = float4(l_ReflectionColor + (1.0f-l_Metalness)*(float3(1.0f, 1.0f, 1.0f)-l_ReflectionColor), 1);
		#endif
		
		#ifdef HAS_GLOSSINESS_MAP
			float l_Glossiness = T5Texture.Sample(S5Sampler, IN.UV).x;
			m_SpecularPower *= l_Glossiness;
		#endif
	#endif

	#ifdef HAS_LIGHTS
		#ifdef HAS_METALNESS_MAP
			l_SpecularColor = float4(l_Albedo.rgb + (1.0f-l_Metalness)*(float3(1.0f, 1.0f, 1.0f)-l_Albedo.rgb), 1);
		#endif
		#ifdef HAS_NORMAL
			Out = Out*applyAllLights(IN, Nn, l_SpecularFactor, l_Albedo, l_AlbedoFactor, l_Metalness, l_SpecularColor.rgb);
			if (Out.w < 0.1)
			{
				clip(-1);
			}
		#endif
	#else
		#ifdef HAS_UV
			Out = T0Texture.Sample(S0Sampler, IN.UV).xyzw;
		#endif
	#endif

	#ifdef HAS_REFLECTION
		float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, Nn));
		float4 l_ReflectColor = T8Texture.SampleBias(S8Sampler, l_ReflectVector, (100 - m_SpecularPower) / 12);
		Out += float4(l_ReflectColor.rgb * l_SpecularColor.rgb * l_SpecularFactor * m_ReflectionFactor, 0);
	#endif
	#ifdef HAS_TRIGGER
		return float4(0,1,0,0.5);
	#endif
	return Out;
}
