#include "globals.fxh"

struct VS_INPUT
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
	
	#ifdef HAS_TANGENT
		float4 Tangent : TEXCOORD3;
	#endif
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	
	#ifdef HAS_NORMAL
		float3 Normal : NORMAL;
	#endif
	
	#ifdef HAS_UV
		float2 UV : TEXCOORD0;
	#endif
	
	#ifdef HAS_TANGENT
		float3 WorldTangent: TEXCOORD1;
		float3 WorldBinormal: TEXCOORD2;
	#endif
	
	float4 HPos : TEXCOORD3;
};

PS_INPUT mainVS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	
	#ifdef HAS_WEIGHT_INDICES
		float4 l_TempPos=float4(IN.Pos.xyz, 1.0);
		float3 l_Position= 0;
		float4 l_Indices=IN.Indices;					
		l_Position=mul(l_TempPos, m_Bones[l_Indices.x]).xyz * IN.Weight.x;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.y]).xyz * IN.Weight.y;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.z]).xyz * IN.Weight.z;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.w]).xyz * IN.Weight.w;
		
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
		l_Output.Pos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	#endif	

	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.HPos = l_Output.Pos ;
	
	#ifdef HAS_NORMAL
		#ifdef HAS_WEIGHT_INDICES
			l_Output.Normal = l_Normal;
		#else
			l_Output.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
		#endif
	#endif
	
	#ifdef HAS_UV
		l_Output.UV = IN.UV;
	#endif
	
	#ifdef HAS_TANGENT
		l_Output.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
		l_Output.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
	#endif
	
	return l_Output;
}

float4 mainPS(PS_INPUT IN) : SV_Target
{	
	float l_Depth = IN.HPos.z / IN.HPos.w;
	return float4(l_Depth,l_Depth,l_Depth, 1.0f); 
}
