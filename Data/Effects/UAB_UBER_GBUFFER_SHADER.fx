#include "globals.fxh"
#include "samplers.fxh"

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
		
	#ifdef HAS_TANGENT
		float4 Tangent : TEXCOORD3;
	#endif
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float4 HPos : TEXCOORD3;
	#ifdef HAS_TANGENT
		float3 WorldTangent: TEXCOORD1;
		float3 WorldBinormal: TEXCOORD2;
	#endif
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
	
	l_Output.Pos = mul( float4(IN.Pos, 1.0), m_World );
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.HPos = l_Output.Pos ;
	l_Output.Normal = normalize(mul(IN.Normal, (float3x3)m_World));
	l_Output.UV = IN.UV;
	
	#ifdef HAS_TANGENT
		l_Output.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
		l_Output.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
	#endif
	
	return l_Output;
}

PS_OUTPUT mainPS(PS_INPUT IN) : SV_Target
{
	PS_OUTPUT l_Out = (PS_OUTPUT)0;
	
	float l_Depth = IN.HPos.z / IN.HPos.w;
	
	float m_SpecularPower = 1.0f;
	float m_SpecularFactor = 1.0f;
	
	float4 l_Albedo = T0Texture.Sample(S0Sampler, IN.UV);
	float3 Nn = IN.Normal;
	#ifdef HAS_REFLECTION
		float3 l_EyeToWorldPosition = normalize(IN.HPos-m_CameraPosition.xyz);
		float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, IN.Normal));
		float4 l_ReflectColor = T8Texture.Sample(S8Sampler, l_ReflectVector)*0.75;
		l_Albedo += l_ReflectColor*0.6;
	#endif
	#ifdef HAS_TANGENT
		float g_Bump = 2.4;
		
		float3 Tn=normalize(IN.WorldTangent);
		float3 Bn=normalize(IN.WorldBinormal);
		float3 bump=g_Bump*((T1Texture.Sample(S1Sampler,IN.UV).rgb) - float3(0.5,0.5,0.5));
		
		Nn = Nn + bump.x*Tn + bump.y*Bn;
		Nn = normalize(Nn);
	#endif

	l_Out.Target0 = float4(l_Albedo.xyz, m_SpecularFactor);
	l_Out.Target1 = float4(l_Albedo.xyz*m_LightAmbient.xyz, m_SpecularPower);
	l_Out.Target2 = float4(Normal2Texture(Nn), 1.0);
	l_Out.Target3 = float4(l_Depth,l_Depth,l_Depth, 1.0f);
		
	return l_Out;
}
