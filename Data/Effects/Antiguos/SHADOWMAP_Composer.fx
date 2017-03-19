#include "globals.fxh"
#include "samplers.fxh"

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
	float Depth : TEXCOORD0;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Out = (TVertexPS)0;
	
	l_Out.Pos = mul(float4(IN.Pos.xyz, 1.0), World);
	l_Out.Pos = mul(l_Out.Pos, View);
	l_Out.Pos = mul(l_Out.Pos, Projection);
	l_Out.Depth = 1.0f - (l_Out.Pos.z / l_Out.Pos.w);
	
	return l_Out;
}

float4 mainPS(TVertexPS IN) : SV_Target
{	
	return float4(IN.Depth, IN.Depth, IN.Depth, 1.0f); 
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
