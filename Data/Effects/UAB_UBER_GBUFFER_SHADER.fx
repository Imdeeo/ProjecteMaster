#include "globals.fxh"
#include "samplers.fxh"

static float m_Active = m_RawDataArray[0];
static float m_Exposure = m_RawDataArray[1];
static float m_SpecularPower = m_RawDataArray[2];
static float m_SpecularFactor = m_RawDataArray[3];

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
	#endif
	float4 HPos : TEXCOORD5;
};

struct PS_OUTPUT
{
	float4 Target0 : SV_Target0;
	float4 Target1 : SV_Target1;
	float4 Target2 : SV_Target2;
	float4 Target3 : SV_Target3;
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
		
		l_Output.Pos = mul(float4(l_Position, 1.0), m_World);
	#else
		l_Output.Pos = mul( float4(IN.Pos, 1.0), m_World );
	#endif
		
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.HPos = l_Output.Pos ;
	
	#ifdef HAS_WEIGHT_INDICES
		l_Output.Normal = l_Normal;
	#else
		l_Output.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
	#endif
	
	l_Output.UV = IN.UV;
	
	#ifdef HAS_TANGENT
		l_Output.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
		l_Output.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
	#endif
	#ifdef HAS_UV2
		l_Output.UV2 = IN.UV2;
	#endif
	
	return l_Output;
}

PS_OUTPUT mainPS(PS_INPUT IN) : SV_Target
{
	PS_OUTPUT l_Out = (PS_OUTPUT)0;
	
	float l_Depth = IN.HPos.z / IN.HPos.w;
	
	
	float4 l_Ambient = m_LightAmbient;
	float4 l_Albedo = T0Texture.Sample(S0Sampler, IN.UV);
	if (l_Albedo.w < 0.1)
	{
		clip(-1);
	}
	float3 Nn = IN.Normal;
	#ifdef HAS_REFLECTION
		float3 l_EyeToWorldPosition = normalize(IN.HPos-m_CameraPosition.xyz);
		float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, IN.Normal));
		float4 l_ReflectColor = T8Texture.Sample(S8Sampler, l_ReflectVector);
		l_Albedo = l_Albedo*0.7+l_ReflectColor*0.3;
	#endif
	#ifdef HAS_TANGENT
		float g_Bump = 2.4;
		
		float3 Tn=normalize(IN.WorldTangent);
		float3 Bn=normalize(IN.WorldBinormal);
		float3 bump=g_Bump*((T2Texture.Sample(S2Sampler,IN.UV).rgb) - float3(0.5,0.5,0.5));		
		Nn = Nn + bump.x*Tn + bump.y*Bn;
		Nn = normalize(Nn);
	#endif
	#ifdef HAS_UV2
		l_Ambient = T1Texture.Sample(S1Sampler,IN.UV2);
	#endif

	
	//l_Out.Target1 = float4(l_Albedo.xyz*l_Ambient.xyz, 1.0f);	
	l_Out.Target0 = float4(l_Albedo.xyz, m_SpecularFactor);
	float l_specularPower = 0.25;
	l_Out.Target1 = float4(l_Albedo.xyz*l_Ambient.xyz, l_specularPower);
	l_Out.Target2 = float4(Normal2Texture(Nn), 1.0);
	l_Out.Target3 = float4(l_Depth,l_Depth,l_Depth, 1.0f);

	
	return l_Out;
}
