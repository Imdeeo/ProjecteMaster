#include "Globals.fxh"

Texture3D CubeTexture : register( t0 );
SamplerState CubeSampler : register( s0 );

struct TVertexVS
{
	float3 Pos : POSITION;
};

struct TVertexPS
{
	float4 Pos : SV_POSITION;
	float3 UV : TEXCOORD0;
};

TVertexPS mainVS(TVertexVS IN)
{
	TVertexPS l_Output=(TVertexPS)0;
	l_Output.UV=IN.Pos-float3(0.0, 0.5, 0.0);
	l_Output.Pos=float4(m_CameraPosition.xyz+l_Output.UV*m_CameraProjectionInfo.y,1.0);
	l_Output.Pos=mul(l_Output.Pos, m_View);
	l_Output.Pos=mul(l_Output.Pos, m_Projection);
	l_Output.Pos=l_Output.Pos.xyzw;
	return l_Output;
}

float4 mainPS(TVertexPS IN) : SV_Target
{	
	float3 outColor = tex3D(CubeSampler, IN.UV);
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
