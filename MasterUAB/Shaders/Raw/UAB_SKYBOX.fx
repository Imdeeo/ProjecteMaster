#include "Globals.fxh"
#include "Samplers.fxh"

struct VS_INPUT
{
	float3 Pos : POSITION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 UV : TEXCOORD0;
};

PS_INPUT mainVS(VS_INPUT IN)
{
	PS_INPUT l_Output=(PS_INPUT)0;
	l_Output.UV=IN.Pos;
	l_Output.Pos=float4(m_CameraPosition.xyz+l_Output.UV*m_CameraProjectionInfo.y,1.0);
	l_Output.Pos=mul(l_Output.Pos, m_View);
	l_Output.Pos=mul(l_Output.Pos, m_Projection);
	l_Output.Pos=l_Output.Pos.xyzw;
	return l_Output;
}

float4 mainPS(PS_INPUT IN) : SV_Target
{	
	float3 outColor = T8Texture.Sample(S8Sampler, IN.UV);
	//float3 outColor = tex3D(S8Sampler, IN.UV);
	return float4(outColor,1);
}
