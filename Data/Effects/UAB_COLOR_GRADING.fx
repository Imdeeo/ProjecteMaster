#include "Globals.fxh"
#include "Samplers.fxh"

static float m_BrownSepiaColorGrading=m_RawDataArray[0];
static float m_GreenSepiaColorGrading=m_RawDataArray[1];
static float m_BlueSepiaColorGrading=m_RawDataArray[2];
static float m_MonochromeColorGrading=m_RawDataArray[3];
static float m_ColorColorGrading=m_RawDataArray[4];
static float m_ContrastColorGrading=m_RawDataArray[5];
static float m_BrightnessColorGrading=m_RawDataArray[6];

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

PS_INPUT mainVS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.Color=IN.Color;
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 ColorGrading(float4 _Color)
{
	//-------Color Matrices for Color Correction--------------
	float4x4 gray = {0.299,0.587,0.184,0,
					0.299,0.587,0.184,0,
					0.299,0.587,0.184,0,
					0,0,0,1};
		
	float4x4 sepia = {0.299,0.587,0.184,0.1,
					0.299,0.587,0.184,0.018,
					0.299,0.587,0.184,-0.090,
					0,0,0,1};
		
	float4x4 sepia2 = {0.299,0.587,0.184,-0.090,
					0.299,0.587,0.184,0.018,
					0.299,0.587,0.184,0.1,
					0,0,0,1};
		
	float4x4 sepia3 = {0.299,0.587,0.184,-0.090,
					0.299,0.587,0.184,0.1,
					0.299,0.587,0.184,0.1,
					0,0,0,1};
		
	float3 monochrome = (_Color.r * 0.3f + _Color.g * 0.59f + _Color.b * 0.11f);
	float4 monochrome4 = float4(monochrome,1);
	float4 result2 = float4(_Color.xyz, 1.0);
	float4 brownsepia = mul(sepia,result2);
	float4 greensepia = mul(sepia3,result2);
	float4 bluesepia = mul(sepia2,result2);
	float4 combine = (brownsepia *m_BrownSepiaColorGrading ) + (greensepia *m_GreenSepiaColorGrading )+ (bluesepia * m_BlueSepiaColorGrading )+ (monochrome4 * m_MonochromeColorGrading)+(m_ColorColorGrading * result2);
	return (combine * m_ContrastColorGrading) + m_BrightnessColorGrading;
}

float4 PSGUIColorGrading(PS_INPUT IN) : SV_Target
{
	float4 l_Color=T0Texture.Sample(S0Sampler, IN.UV);
	return ColorGrading(l_Color);
}