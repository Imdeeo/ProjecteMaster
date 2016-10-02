#include "Globals.fxh"
#include "Samplers.fxh"

#ifdef HAS_LIGHTSCATTERING
	static bool m_Active=m_RawDataArray[0]==1.0;
	static float m_Exposure=m_RawDataArray[1];
	static float m_Decay=m_RawDataArray[2];
	static float m_Density=m_RawDataArray[3];
	static float m_Weight=m_RawDataArray[4];
	static int m_NumSamples=(int)m_RawDataArray[5];
	static float2 m_LightScatteringPosition=float2(m_RawDataArray[6], m_RawDataArray[7]);
#endif
#ifdef HAS_COMBINE
	static float m_ShowLightScattering=m_RawDataArray[0];
#endif

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
	PS_INPUT l_Out = (PS_INPUT)0;
	l_Out.Pos=IN.Pos;
	l_Out.Color=IN.Color;
	l_Out.UV=IN.UV;
	return l_Out;
}

#ifdef HAS_LIGHTSCATTERING
	void Calc2DLinearFunction(float2 PosA, float2 PosB, out float m, out float b)
	{
		/*
		y=m*x+b
		Ax=PosB.x-PosA.x;
		Ay=PosB.y-PosA.y;
		m=Ay/Ax;
		b=y-m*x
		*/
		float Ax=PosB.x-PosA.x;
		float Ay=PosB.y-PosA.y;
		m=Ay/Ax;
		b=PosA.y-m*PosA.x;
	}

	float CalcPosY2DLinearFunction(float x, float m, float b)
	{
		return m*x+b;
	}

	float CalcPosX2DLinearFunction(float y, float m, float b)
	{
		return (y-b)/m;
	}

	float2 ClampLightScatteringPosition(float2 UV, float2 LightScatteringPosition)
	{
		float2 l_LightScatteringPosition=LightScatteringPosition;
		float l_mLinearFunction;
		float l_bLinearFunction;
		Calc2DLinearFunction(UV, LightScatteringPosition, l_mLinearFunction, l_bLinearFunction);
		float l_LightScatteringPositionRight=CalcPosY2DLinearFunction(1.0, l_mLinearFunction, l_bLinearFunction);
		float l_LightScatteringPositionLeft=CalcPosY2DLinearFunction(0.0, l_mLinearFunction, l_bLinearFunction);
		float l_LightScatteringPositionTop=CalcPosX2DLinearFunction(0.0, l_mLinearFunction, l_bLinearFunction);
		float l_LightScatteringPositionBottom=CalcPosX2DLinearFunction(1.0, l_mLinearFunction, l_bLinearFunction);
		float4 l_Color=float4(1,1,1,1);

		if(l_LightScatteringPosition.x<0.0 || l_LightScatteringPosition.y<0.0 || l_LightScatteringPosition.x>1.0 || l_LightScatteringPosition.y>1.0)
		{
			if(l_LightScatteringPositionRight>=0 && l_LightScatteringPositionRight<=1.0 && LightScatteringPosition.x>=UV.x)
				l_LightScatteringPosition=float2(1.0, l_LightScatteringPositionRight);
			else if(l_LightScatteringPositionLeft>=0 && l_LightScatteringPositionLeft<=1.0 && LightScatteringPosition.x<UV.x)
				l_LightScatteringPosition=float2(0.0, l_LightScatteringPositionLeft);
			else if(l_LightScatteringPositionTop>=0 && l_LightScatteringPositionTop<=1.0 && LightScatteringPosition.y<UV.y)
				l_LightScatteringPosition=float2(l_LightScatteringPositionTop, 0.0);
			else if(l_LightScatteringPositionBottom>=0 && l_LightScatteringPositionBottom<=1.0 && LightScatteringPosition.y>=UV.y)
				l_LightScatteringPosition=float2(l_LightScatteringPositionBottom, 1.0);
		}
		
		return l_LightScatteringPosition;
	}
	float4 PSLightScattering(PS_INPUT IN) : SV_Target
	{
		if(!m_Active)
			return float4(0,0,0,0);
			
		//m_LightScatteringPosition=float2(0.5, 1.3);
		m_LightScatteringPosition.y=1.0-m_LightScatteringPosition.y;
		
		float2 l_LightScatteringPosition=ClampLightScatteringPosition(IN.UV, m_LightScatteringPosition);
		float2 l_DeltaTextCoord=IN.UV-l_LightScatteringPosition;
		
		/*if(length(IN.UV-m_LightScatteringPosition)<0.1)
			return float4(1,0,0,0.3);
		clip(-1);*/
		float2 l_TextCoo=IN.UV;
		l_DeltaTextCoord *= 1.0/float(m_NumSamples) * m_Density;
		float l_IlluminationDecay=1.0;
		float4 l_Color=float4(0,0,0,0);
		for(int i=0; i < m_NumSamples ; i++)
		{
			l_TextCoo -= l_DeltaTextCoord;
			float4 l_Sample=T0Texture.SampleGrad(S0Sampler, l_TextCoo, 0, 0);
			l_Sample*=l_IlluminationDecay*m_Weight;
			l_Color+=l_Sample;
			l_IlluminationDecay*=m_Decay;
		}
		
		return float4(l_Color.xyz*m_Exposure, 1.0);
	}
#endif

#ifdef HAS_COMBINE
	float4 PSLightScatteringCombine(PS_INPUT IN) : SV_Target
	{
		if(m_ShowLightScattering==0.0)
		{
			float4 l_LightScatteringColor=T1Texture.Sample(S1Sampler, IN.UV);
			// Get our original pixel from ColorMap
			float4 l_OriginalColor=T0Texture.Sample(S0Sampler, IN.UV);
			// Combine the two images.
			return l_OriginalColor+l_LightScatteringColor;
		}
		else
		{
			return T1Texture.Sample(S1Sampler, IN.UV);
		}
	}
#endif
