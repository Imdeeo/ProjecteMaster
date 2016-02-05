/*

% Shadowmap Shader.

date: 05022016

*/

float4x4 View : View;
float4x4 World : World;
float4x4 Projection : Projection;

struct TVertexVS
{
	float3 Pos : POSITION;
};

struct TVertexPS
{
	float4 Pos : POSITION;
	float4 Depth : TEXCOORD0;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), World);
	l_Out.Pos = mul(l_Out.Pos, View);
	l_Out.Pos = mul(l_Out.Pos, Projection);
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : SV_Target
{
	float l_Depth=IN.Depth.z/IN.Depth.w;
	
	return float4(l_Depth,l_Depth,l_Depth,1); 
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
