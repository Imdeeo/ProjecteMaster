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
	float4 l_Out = float4(1,1,1,1);
	#ifdef HAS_UV
		l_Out = T0Texture.Sample(S0Sampler, IN.UV);
		l_Out.w = 0.5;
	#endif
	
	#ifdef HAS_UV2
		float4 lightContrib = T1Texture.Sample(S1Sampler, IN.UV2);
	#else		
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
		if (m_LightEnabledArray[i])
		{
			lightContrib += applyLights(IN.Pixelpos,Nn,l_Out,i, m_SpecularPower, l_specularFactor);
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

	#ifdef HAS_LIGHTS
		#ifdef HAS_NORMAL
			Out = Out*applyAllLights(IN);
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
		float3 l_EyeToWorldPosition = normalize(IN.Pixelpos-m_InverseView[3].xyz);
		float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, IN.Normal));
		float4 l_ReflectColor = T8Texture.Sample(S8Sampler, l_ReflectVector);
		Out = Out * m_Exposure + l_ReflectColor * (1 - m_Exposure);
	#endif
	#ifdef HAS_TRIGGER
		return float4(0,1,0,0.5);
	#endif
	return Out;
}