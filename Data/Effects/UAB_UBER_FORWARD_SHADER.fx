#include "Samplers.fxh"
#include "Lights.fxh"

static float m_Active = m_RawDataArray[0];
static float m_Exposure = m_RawDataArray[1];
static float m_SpecularPower = m_RawDataArray[2];
static float m_SpecularFactor = m_RawDataArray[3];

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
	#endif
	
	#ifdef HAS_UV2
		float2 UV2 : TEXCOORD1;
	#endif
	
	#ifdef HAS_TANGENT
		float3 WorldTangent: TEXCOORD1;
		float3 WorldBinormal: TEXCOORD2;
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
		//#ifdef HAS_WEIGHT_INDICES
		//	l_Out.Normal = l_Normal;
		//#else
			l_Out.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
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
float4 applyAllLights(TVertexPS IN)
{
	float3 Nn = IN.Normal;
	float l_specularFactor=m_SpecularFactor;
	#ifdef HAS_UV2
		float4 lightContrib = T1Texture.Sample(S1Sampler, IN.UV2);
	#else
		float4 l_Out = float4(1,1,1,1);
		#ifdef HAS_UV
			l_Out = T0Texture.Sample(S0Sampler, IN.UV);
		#endif
		float4 lightContrib = m_LightAmbient*l_Out;
	#endif
	
	#ifdef HAS_TANGENT
		float g_Bump = 2.4;
		
		float3 Tn=normalize(IN.WorldTangent);
		float3 Bn=normalize(IN.WorldBinormal);
		float4 auxNormal = T2Texture.Sample(S2Sampler,IN.UV);
		float3 bump=g_Bump*((auxNormal.xyz) - float3(0.5,0.5,0.5));		
		Nn = Nn + bump.x*Tn + bump.y*Bn;
		Nn = normalize(Nn);
		l_specularFactor *= auxNormal;
	#endif
	
	for(int i = 0;i<MAX_LIGHTS_BY_SHADER;i++)
	{
		lightContrib += applyLights(IN.Pixelpos,Nn,l_Out,i, m_SpecularPower, l_specularFactor);
	}
	return saturate(float4(lightContrib.xyz, l_Out.w));
}

float CalcAttenuation(float Depth, float StartFog, float EndFog)
{
	if(Depth<EndFog)
	{
		return m_MaxAttenuation*smoothstep(StartFog, EndFog, Depth);
	} 
	else 
	{
		return m_MaxAttenuation;
	}
}

float CalcLinearFog(float Depth, float StartFog, float EndFog) 
{
	float l_Fog = CalcAttenuation(Depth, StartFog, EndFog);
	return l_Fog;
}

float CalcExp2Fog(float Depth, float ExpDensityFog) 
{ 
	const float LOG2E = 1.442695; // = 1 / log(2) 
	float l_Fog = exp2(-ExpDensityFog * ExpDensityFog * Depth * Depth * LOG2E); 
	return l_Fog;
} 

float CalcExpFog(float Depth, float ExpDensityFog) 
{ 
	const float LOG2E = 1.442695; // = 1 / log(2) 
	float l_Fog = exp2(-ExpDensityFog * Depth * LOG2E); 
	return l_Fog;
}

float4 GetFogColor(float Depth) 
{ 
	float l_FogIntensity=CalcLinearFog(Depth, m_StartFog, m_EndFog);
	return saturate(float4(m_FogColor.xyz,l_FogIntensity));
}
#endif

float4 mainPS(TVertexPS IN) : SV_Target
{
	float4 Out = float4(1,1,1,1);

	#ifdef HAS_COLOR
		Out = IN.Color;
	#endif

	#ifdef HAS_LIGHTS
		#ifdef HAS_NORMAL
			Out = Out*applyAllLights(IN);
			if (Out.w < 0.1)
			{
				clip(-1);
			}
		#endif
		
		float l_DistanceEyeToWorldPosition=length(IN.Pixelpos-m_InverseView[3].xyz);
		float4 l_FogColor = GetFogColor(l_DistanceEyeToWorldPosition);
		Out = float4(Out.xyz+l_FogColor.xyz*l_FogColor.a,Out.a);		
	#endif
	
	#ifdef HAS_REFLECTION
		float3 l_EyeToWorldPosition = normalize(IN.Pixelpos-m_InverseView[3].xyz);
		float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, IN.Normal));
		float4 l_ReflectColor = T8Texture.Sample(S8Sampler, l_ReflectVector);
		Out = Out*0.7 + l_ReflectColor*0.3;
	#endif
	#ifdef HAS_TRIGGER
		return float4(0,1,0,1);
	#endif
	return Out;
}