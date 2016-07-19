#include "globals.fxh"
#include "Samplers.fxh"

static float enable = m_RawDataArray[0];
static float m_Angle = m_RawDataArray[1];
static float4 m_CenterRadius = float4(m_RawDataArray[2],m_RawDataArray[3],m_RawDataArray[4],m_RawDataArray[5]);


struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float2 UV_ORIG : TEXCOORD1;
};

PS_INPUT mainVS(VS_INPUT IN)
{	
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	float2 uv_aux = IN.UV - m_CenterRadius.xy;
	l_Output.UV = IN.UV;
	l_Output.UV_ORIG = uv_aux;
	return l_Output;
}

float4 mainPS(PS_INPUT IN) : SV_Target
{
	if(enable)
	{
		float2 offset = IN.UV_ORIG;
		float angle = 1.0 - length(offset / m_CenterRadius.zw);
		angle = max (0, angle);
		angle = angle * angle * m_Angle;
		float cosLength, sinLength;
		sincos (angle, sinLength, cosLength);
		
		float2 uv_aux;
		uv_aux.x = cosLength * offset[0] - sinLength * offset[1];
		uv_aux.y = sinLength * offset[0] + cosLength * offset[1];
		uv_aux += m_CenterRadius.xy;
		return T0Texture.Sample(S0Sampler, uv_aux);
	}
	else
	{
		return T0Texture.Sample(S0Sampler, IN.UV);
	}
}