/*

% Shadowmap Shader.

date: 05022016

*/

float4x4 View : View;
float4x4 World : World;
float4x4 Projection : Projection;
float4x4 m_InverseView : ViewInverse;
float maxAttenuation = 0.6;
float m_StartLinearFog = 15;
float m_EndLinearFog = 5;
float3 m_FogColor = float3(.2, .2, .2);
struct TVertexVS
{
	float3 Pos : POSITION;
	float3 WorldPosition : WORLDPOSITION;
};

struct TVertexPS
{
	float4 Pos : POSITION;
	float4 Depth : TEXCOORD0;
	float3 WorldPosition : WORLDPOSITION;
};

float CalcAttenuation(float Depth, float StartFog, float EndFog)
{
	float attenuation;
	if(Depth<EndFog && Depth>StartFog){
		return maxAttenuation*smoothstep(StartFog, EndFog, Depth);
	} else {
		return maxAttenuation;
	}
}
float4 CalcLinearFog(float Depth, float StartFog, float EndFog, float3 FogColor) 
{
	float l_Fog = CalcAttenuation(Depth, StartFog, EndFog);
	return float4(FogColor, 1.0-l_Fog); 
} 
float4 CalcExp2Fog(float Depth, float ExpDensityFog, float3 FogColor) 
{ 
	const float LOG2E = 1.442695; // = 1 / log(2) 
	float l_Fog = exp2(-ExpDensityFog * ExpDensityFog * Depth * Depth * LOG2E); 
	return float4(FogColor, 1.0-l_Fog); 
} 
float4 CalcExpFog(float Depth, float ExpDensityFog, float3 FogColor) 
{ 
	const float LOG2E = 1.442695; // = 1 / log(2) 
	float l_Fog = exp2(-ExpDensityFog * Depth * LOG2E); 
	return float4(FogColor, 1.0-l_Fog); 
} 
float3 GetFogColor(float Depth, float3 CurrentColor) 
{ 
	float4 l_FogColor=CalcLinearFog(Depth, m_StartLinearFog, m_EndLinearFog, m_FogColor); 
	return float3(CurrentColor*(1.0-l_FogColor.a)+l_FogColor.xyz*l_FogColor.a); 
} 

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), World);
	l_Out.Pos = mul(l_Out.Pos, View);
	l_Out.Pos = mul(l_Out.Pos, Projection);
	l_Out.WorldPosition = IN.WorldPosition;
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : SV_Target
{
	float l_DistanceEyeToWorldPosition=length(IN.WorldPosition-m_InverseView[3].xyz);
	
	float4 l_FinalColor = (1, 1, 1, 1);
	
	return float4(GetFogColor(l_DistanceEyeToWorldPosition, l_FinalColor.xyz), l_FinalColor.a);  
}

technique technique0
{
	pass p0
	{
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
