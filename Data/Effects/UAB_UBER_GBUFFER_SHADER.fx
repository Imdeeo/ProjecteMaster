#include "globals.fxh"
#include "samplers.fxh"

static float m_Active = m_RawDataArray[0];
static float m_Exposure = m_RawDataArray[1];
static float m_SpecularPower = m_RawDataArray[2];
static float m_SpecularFactor = m_RawDataArray[3];
static float m_ReflectionFactor = m_RawDataArray[4];
static float m_SSRReflection = m_RawDataArray[5];
static float m_BumpFactor = m_RawDataArray[6];
static float m_MetalnessFactor = m_RawDataArray[7];
static float3 m_MetalColor = float3(m_RawDataArray[8], m_RawDataArray[9], m_RawDataArray[10]);
static float m_MetalColorWeight = m_RawDataArray[11];

struct VS_INPUT
{
	float3 Pos : POSITION;

	#ifdef HAS_WEIGHT_INDICES
		float4 Weight:BLENDWEIGHT;
		float4 Indices:BLENDINDICES;
	#endif

	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	#ifdef HAS_UV2
		float2 UV2 : TEXCOORD1;
	#endif
	#ifdef HAS_TANGENT
		float4 Tangent : TEXCOORD3;
	#endif
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	#ifdef HAS_UV2
		float2 UV2 : TEXCOORD1;
	#endif
	#ifdef HAS_TANGENT
		float3 WorldBinormal: TEXCOORD2;
		float3 WorldTangent: TEXCOORD3;
		float3 WorldNormal: TEXCOORD4;
	#endif
	float4 HPos : TEXCOORD5;
	float4 WorldPos : TEXCOORD6;
};

struct PS_OUTPUT
{
	float4 Target0 : SV_Target0;
	float4 Target1 : SV_Target1;
	float4 Target2 : SV_Target2;
	float4 Target3 : SV_Target3;
	float4 Target4 : SV_Target4;
};

float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}
float3 Texture2Normal(float3 Color)
{
	return (Color-0.5)*2;
}

PS_INPUT mainVS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;

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
		#else
			l_Normal = IN.Normal;
		#endif

		l_Output.Pos = mul(float4(l_Position, 1.0), m_World);
	#else
		l_Normal = IN.Normal;
		l_Output.Pos = mul( float4(IN.Pos, 1.0), m_World );
		//l_Output.Normal = normalize(mul(l_Normal, (float3x3)m_World));
	#endif

	l_Output.WorldPos = l_Output.Pos;
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.HPos = l_Output.Pos ;
	l_Output.Normal = normalize(mul(l_Normal, (float3x3)m_World));
	//l_Output.Normal=float3(1,0,1);
	l_Output.UV = IN.UV;

	#ifdef HAS_TANGENT
		l_Output.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
		l_Output.WorldBinormal = mul(cross(IN.Tangent.xyz,l_Normal),(float3x3)m_World);
		l_Output.WorldNormal = mul(l_Normal.xyz,(float3x3)m_World);
	#endif

	#ifdef HAS_UV2
		l_Output.UV2 = IN.UV2;
	#endif

	return l_Output;
}

float3 GetRadiosityNormalMap(float3 Nn, float2 UV, Texture2D LightmapXTexture, SamplerState
	LightmapXSampler, Texture2D LightmapYTexture, SamplerState LightmapYSampler, Texture2D
	LightmapZTexture, SamplerState LightmapZSampler)
{
	float3 l_LightmapX=LightmapXTexture.Sample(LightmapXSampler, UV).xyz;
	float3 l_LightmapY=LightmapYTexture.Sample(LightmapYSampler, UV).xyz;
	float3 l_LightmapZ=LightmapZTexture.Sample(LightmapZSampler, UV).xyz;
	float3 l_BumpBasisX=normalize(float3(0.816496580927726, 0, 0.5773502691896258));
	float3 l_BumpBasisY=normalize(float3(-0.408248290463863,  0.7071067811865475, 0.5773502691896258));
	float3 l_BumpBasisZ=normalize(float3(-0.408248290463863, -0.7071067811865475, 0.5773502691896258));
	float3 l_RNMLighting=saturate(dot(Nn, l_BumpBasisX)) * l_LightmapX+saturate(dot(Nn, l_BumpBasisY)) * l_LightmapY + saturate(dot(Nn, l_BumpBasisZ)) * l_LightmapZ;

	return l_RNMLighting;
}

float3 CalcNormalMap(float3 Normal, float3 Tangent, float3 Binormal, float4 NormalMap)
{
	float g_Bump = 2.4 * m_BumpFactor;
	float3 bump=g_Bump*((NormalMap.xyz) - float3(0.5,0.5,0.5));
	float3 Nn = Normal + bump.x*Tangent + bump.y*Binormal;
	return normalize(Nn);
}

float3 CalcParallaxMap(float3 Vn, float3 WorldNormal, float3 WorldTangent, float3 WorldBinormal, float2 UV, out float2 OutUV, float4 NormalMap)
{
	float2 l_UV = UV;

	// parallax code
	float3x3 tbnXf = float3x3(WorldTangent,WorldBinormal,WorldNormal);
	float height = NormalMap.w * 0.06 - 0.03;
	l_UV += height * mul(tbnXf,Vn).xy;

	// normal map
	float3 tNorm = NormalMap.xyz - float3(0.5,0.5,0.5);

	// transform tNorm to world space
	tNorm = normalize(tNorm.x*WorldTangent - tNorm.y*WorldBinormal + tNorm.z*WorldNormal);
	OutUV=l_UV;
	return tNorm;
}

PS_OUTPUT mainPS(PS_INPUT IN)
{
	PS_OUTPUT l_Out = (PS_OUTPUT)0;
	float3 l_EyeToWorldPosition = normalize(IN.WorldPos.xyz-m_CameraPosition.xyz);
	float l_specularFactor=m_SpecularFactor;
	float l_Depth = IN.HPos.z / IN.HPos.w;

	float4 l_Ambient = m_LightAmbient;
	float4 l_Albedo = T0Texture.Sample(S0Sampler, IN.UV);
	if (l_Albedo.w < 0.1)
	{
		clip(-1);
	}
	float3 Nn = IN.Normal;
	
	#ifdef HAS_TANGENT
		Nn=normalize(IN.WorldNormal);
		float3 Tn=normalize(IN.WorldTangent);
		float3 Bn=normalize(IN.WorldBinormal);
		float4 l_NormalMap = T2Texture.Sample(S2Sampler,IN.UV);

		Nn=CalcNormalMap(Nn, Tn, Bn, l_NormalMap);
		#ifdef HAS_PARALLAX
			Nn=CalcParallaxMap(l_EyeToWorldPosition, Nn, Tn, Bn, IN.UV, IN.UV, l_NormalMap);
		#endif

		l_specularFactor *= l_NormalMap.w;
	#endif
	
	#ifdef HAS_GLOSSINESS_MAP
		float l_Glossiness = T5Texture.Sample(S5Sampler, IN.UV).x;
		m_SpecularPower *= l_Glossiness;
	#endif

	#ifdef HAS_SPECULAR_MAP
		float4 l_SpecularColor = T10Texture.Sample(S10Sampler, IN.UV);
		l_specularFactor *= max(l_SpecularColor.r, max(l_SpecularColor.g, l_SpecularColor.b));
	#endif
	// PBR modifications according to http://www.marmoset.co/toolbag/learn/pbr-theory
	// PBR: fresnel (the formula is arbitrary, not based on any source, but the curve would look somewhat similar to the examples)
	
	float fresnel = pow(1 - dot(-l_EyeToWorldPosition, Nn), FRESNEL_POWER);
	l_specularFactor += fresnel * (1-l_specularFactor);
	// PBR: energy conservation: "reflection and diffusion are mutually exclusive"
	// "This is easy to enforce in a shading system: one simply subtracts reflected light before allowing the diffuse shading to occur."
	#ifdef HAS_SPECULAR_MAP
		float l_AlbedoFactor = 1 - l_specularFactor;
		float l_Metalness = 0.0f;
	#else
		#ifdef HAS_METALNESS_MAP
			float l_Metalness = T10Texture.Sample(S10Sampler, IN.UV).x * m_MetalnessFactor;
		#else
			float l_Metalness = m_MetalnessFactor;
		#endif
		l_specularFactor = l_specularFactor + l_Metalness * max(0, METAL_SPECFACTOR-l_specularFactor);
		float l_AlbedoFactor = 1 - METAL_SPECFACTOR*l_Metalness;
		l_AlbedoFactor *= 1 - l_specularFactor;
		float l_MetalColorRatio = l_Metalness * m_MetalColorWeight;
		float3 l_ReflectionColor = l_MetalColorRatio * m_MetalColor + (1-l_MetalColorRatio) * l_Albedo.rgb;
		float4 l_SpecularColor = float4(l_ReflectionColor + (1.0f-l_Metalness)*(float3(1.0f, 1.0f, 1.0f)-l_ReflectionColor), 1);
		// If gbuffer doesn't have a channel for specular color, we can get almost the same effect by setting albedo to reflection color.
		// l_Albedo = float4(l_ReflectionColor, 1);
	#endif

	#ifdef HAS_UV2
		#if defined(HAS_TANGENT) && defined(HAS_RNM)
			l_Ambient = float4(GetRadiosityNormalMap(l_NormalMap.xyz, IN.UV2, T1Texture, S1Sampler, T3Texture, S3Sampler, T4Texture, S4Sampler), 1.0);
		#elif defined(HAS_RNM)
			l_Ambient = float4(GetRadiosityNormalMap(float3(0,0,1), IN.UV2, T1Texture, S1Sampler, T3Texture, S3Sampler, T4Texture, S4Sampler), 1.0);
		#else
			l_Ambient = T1Texture.Sample(S1Sampler,IN.UV2);
		#endif
	#endif

	float4 l_AmbientIllumination = l_Albedo * l_AlbedoFactor * l_Ambient;
	#ifdef HAS_REFLECTION
		float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, Nn));
		float4 l_ReflectColor = T8Texture.SampleLevel(S8Sampler, l_ReflectVector, (100 - m_SpecularPower) / 12);
		#ifdef HAS_SPECULAR_MAP
			l_AmbientIllumination += float4(l_ReflectColor.xyz * l_specularFactor * m_ReflectionFactor * l_SpecularColor.xyz, 1);
		#else
			l_AmbientIllumination += l_ReflectColor * l_specularFactor * m_ReflectionFactor * l_SpecularColor;
		#endif
	#endif


	float l_SpecularPower = m_SpecularPower / 100;
	l_Out.Target0 = float4(l_Albedo.xyz, l_specularFactor);
	#ifdef HAS_GLOW
		l_Out.Target1 = float4(T0Texture.Sample(S0Sampler, IN.UV).xyz, l_SpecularPower);
	#else
		l_Out.Target1 = float4(l_AmbientIllumination.xyz, l_SpecularPower);
	#endif
	l_Out.Target2 = float4(Normal2Texture(Nn), m_SSRReflection);
	l_Out.Target3 = float4(l_Depth,l_Depth,l_Depth, 1.0f);
	l_Out.Target4 = float4(l_SpecularColor.rgb, l_Metalness);


	return l_Out;
}
