/*

% Normalmap+Spot light shader.

date: 17122015

*/

textureCUBE <float4> T0;

samplerCUBE T0Sampler = sampler_state
{
	Texture = <T0>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};
	
	

float4x4 View : View;
float4x4 World : World;
float4x4 Projection : Projection;
float4x4 InverseView : ViewInverse;
const float PI = 3.14159265f;
const float g_EnvironmentFactor = 1.0;

struct TVertexVS
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float3 WorldPosition : WORLDPOSITION;
};

struct TVertexPS
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float3 Pixelpos : TEXCOORD1;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), World);
	l_Out.Pixelpos = l_Out.Pos.xyz;
	l_Out.Pos = mul(l_Out.Pos, View);
	l_Out.Pos = mul(l_Out.Pos, Projection);
	l_Out.Normal = normalize(mul(IN.Normal, (float3x3)World));
	l_Out.UV = IN.UV;
	
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : COLOR
{	
	float3 Nn=normalize(IN.Normal);
	float3 l_EyeToWorldPosition = normalize(IN.Pixelpos-InverseView[3].xyz);
	float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, Nn));
	float3 outColor = texCUBE(T0Sampler, l_ReflectVector).xyz*g_EnvironmentFactor;	
	
	return float4(outColor,1);
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
