/*

% Normal Shader.

date: 17122015

*/


float4x4 View : View;
float4x4 World : World;
float4x4 Projection : Projection;

struct TVertexVS
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
};

struct TVertexPS
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), World);
	l_Out.Pos = mul(l_Out.Pos, View);
	l_Out.Pos = mul(l_Out.Pos, Projection);
	l_Out.Normal = normalize(mul(IN.Normal, (float3x3)World));
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : COLOR
{
	return float4(IN.Normal, 1.0);
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