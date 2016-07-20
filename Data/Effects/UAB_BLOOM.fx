#include "globals.fxh"
#include "Samplers.fxh"

#ifdef HAS_BLOOM
	// BLOOM
	static float m_Threshold=m_RawDataArray[0];
#endif	
#ifdef HAS_BLUR
	// BLUR
	static float m_TextureSize=m_RawDataArray[0];
	static float m_BlurScale=m_RawDataArray[1];
#endif
#ifdef HAS_COMBINE
	// COMBINE
	static float m_BloomActive=m_RawDataArray[0];
	static float m_BloomIntensity=m_RawDataArray[1];
	static float m_OriginalIntensity=m_RawDataArray[2];
	static float m_BloomSaturation=m_RawDataArray[3];
	static float m_OriginalSaturation=m_RawDataArray[4];
	static float m_ShowBloom=m_RawDataArray[5];
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
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.Color=IN.Color;
	l_Output.UV=IN.UV;
	return l_Output;
}

#ifdef HAS_BLOOM
	// BLOOM
	float4 PSBloom(PS_INPUT IN) : SV_Target
	{
		float4 l_Color=T0Texture.Sample(S0Sampler, IN.UV);
		return saturate((l_Color-m_Threshold)/(1-m_Threshold));
	}
#endif
	
#ifdef HAS_BLUR
	float4 GetGaussianBlurFromSampler(Texture2D _Texture2D, SamplerState _SamplerState, float2 UV, float2 TextureSize, float TextureScale)
	{		
		float2 l_PixelKernel[9] =
		{
			{ -1, 1 },
			{ -1, 0 },
			{ -1, -1 },
			{ 0, 1 },
			{ 0, 0 },
			{ 0, -1 },
			{ 1, 1 },
			{ 1, 0 },
			{ 1, -1 },
		};
		
		float l_BlurWeights[9] =
		{
			0.11, 0.15, 0.11,
			0.15, 0.19, 0.15,
			0.11, 0.15, 0.11, 
		};
		
		float4 l_GaussianColor = 0;
		float2 l_TextureSize=TextureSize*TextureScale;
		float l_TotalWeights=0.0;
		for(int i = 0; i < 9; ++i)
		{
			float4 l_Color=_Texture2D.Sample(_SamplerState, UV+(l_PixelKernel[i]*l_TextureSize))*l_BlurWeights[i];
			l_TotalWeights+=l_BlurWeights[i];
			l_GaussianColor += l_Color;
		}
		return l_GaussianColor/l_TotalWeights;
	}

	// BLUR
	float4 PSBlur(PS_INPUT IN) : SV_Target
	{
		return GetGaussianBlurFromSampler(T0Texture, S0Sampler, IN.UV, 1/m_TextureSize,	m_BlurScale);
	}
#endif

#ifdef HAS_COMBINE
	// COMBINE
	float4 AdjustSaturation(float4 Color, float Saturation)
	{
		// We define gray as the same color we used in the grayscale shader
		float l_Grey=dot(Color, float3(0.3, 0.59, 0.11));
		return lerp(l_Grey, Color, Saturation);
	}

	float4 PSBloomCombine(PS_INPUT IN) : SV_Target
	{
		if(m_BloomActive==0.0)
			return T0Texture.Sample(S0Sampler, IN.UV);
		else
		{
			if(m_ShowBloom==0.0)
			{
				float4 l_BloomColor=T1Texture.Sample(S1Sampler, IN.UV);
				// Get our original pixel from ColorMap
				float4 l_OriginalColor=T0Texture.Sample(S0Sampler, IN.UV);
				// Adjust color saturation and intensity based on the input variables to the shader
				l_BloomColor=AdjustSaturation(l_BloomColor, m_BloomSaturation)*m_BloomIntensity;
				l_OriginalColor=AdjustSaturation(l_OriginalColor, m_OriginalSaturation)*m_OriginalIntensity;
				// make the originalColor darker in very bright areas, avoiding these areas look burned-out
				l_OriginalColor*=(1-saturate(l_BloomColor));
				// Combine the two images.
				return l_OriginalColor+l_BloomColor;
			}
			else
			{
				return T1Texture.Sample(S1Sampler, IN.UV);
			}
		}
	}
#endif