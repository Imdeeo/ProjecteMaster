#include "globals.fxh"
#include "Samplers.fxh"

#ifdef HAS_BRIGHTPASS
	static float m_Threshold=m_RawDataArray[0];
#endif

#ifdef HAS_DOWNSAMPLING
	static float m_TextureSize=m_RawDataArray[0];
#endif

#ifdef HAS_BLUR
	static float m_TextureSize=m_RawDataArray[0];
	static float m_BlurScale=m_RawDataArray[1];
	static float4 m_Color=float4((float)m_RawDataArray[2], (float)m_RawDataArray[3], (float)m_RawDataArray[4], (float)m_RawDataArray[5]);
#endif

#ifdef HAS_COMBINE
	// COMBINE
	static float m_FlareActive=m_RawDataArray[0];
	static float m_FlareIntensity=m_RawDataArray[1];
	static float m_OriginalIntensity=m_RawDataArray[2];
	static float m_ShowFlare=m_RawDataArray[3];
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

#ifdef HAS_BRIGHTPASS
	float4 PSBrightpass(PS_INPUT IN) : SV_Target
	{
		float4 l_Color=T0Texture.Sample(S0Sampler, IN.UV);
		return saturate((l_Color-m_Threshold)/(1-m_Threshold));
	}
#endif

#ifdef HAS_DOWNSAMPLING
	float4 PSDownsampling(PS_INPUT IN) : SV_Target 
	{ 
		const unsigned int samples = 16; 
		//We are downsampling 16 times. 
		const float texelsize = (1.0f / m_TextureSize); 
		//Get horizontal texelsize (outSize is the size of the target texture). 
		
		const float samplerOffset[samples] = 
		{ 1.0f, 2.0f, 3.0f, 4.0f, 3.0f, 2.0f, 2.0f, 1.0f, -1.0f,-2.0f,-3.0f,-4.0f,-5.0f,-6.0f,-7.0f,-8.0f, }; 
		//Downsampling-offsets in pixel. Note that there is no "0.0", cause this point lies on the middle of four texels. 
		
		float4 result = 0.0f; 
		for (unsigned int i = 0; i < samples; ++i)//For all 16 texels... 
		{ 
			float2 texOffset = float2(samplerOffset[i] * texelsize, 0.0f); 
			//Translate the offset(in pixels) to normalized texture coordinates, 
			//by multiplying offset[i] with the size of one pixel in the target texture. 
			
			result += T0Texture.SampleLevel(S0Sampler, IN.UV + texOffset, 0).rgba; 
			//Sample the pixel and add its color to the result, so that we can compute the average.
		} 
		result *= 1.0f / samples; 
		//Take the average by dividing the result by the number of downsample steps. 
		return result; 
	}
#endif

#ifdef HAS_BLUR
	float4 PSBlur(PS_INPUT IN) : SV_Target 
	{ 
		const unsigned int tabs= 21;//21 Tabs 
		const float texelsize = 1.0f / m_TextureSize; 
		//Get horizontal texelsize (outSize is the size of the target texture) 

		const float offsets[] = { -10.0f,-9.0f,-8.0f,-7.0f, 
    	    	    	    	    	    	-6.0f,-5.0f,-4.0f,-3.0f, 
    	    	    	    	    	    	-2.0f,-1.0f, 0.0f, 1.0f, 
    	    	    	    	    	    	2.0f, 3.0f, 4.0f, 5.0f, 
    	    	    	    	    	    	6.0f, 7.0f, 8.0f, 9.0f, 10.0f }; 
		//Tab-offsets(in texel) 

		const float weights[] = { 0.1f, 0.3f, 0.5f, 0.8f, 
												0.8f, 0.8f, 1.0f, 1.0f, 
												1.0f, 1.0f, 1.0f, 1.0f, 
												1.0f, 1.0f, 1.0f, 0.8f, 
												0.8f, 0.8f, 0.5f, 0.3f, 0.1f }; 
		//Tab-weightings 

		const float weightSum = 1.0f / (weights[0] + weights[1] + weights[2] + weights[3] + 
																		weights[4] + weights[5] + weights[6] + weights[7] + 
																		weights[8] + weights[9] + weights[10] + weights[11] + 
																		weights[12] + weights[13] + weights[14] + weights[15] + 
																		weights[16] + weights[17] + weights[18] + weights[19] + 
																		weights[20]); 
		//Compute average of weightings. 

		float4 result = 0.0f; 
		for (unsigned int i = 0; i < tabs; ++i)//For all 21 tabs/samples 
		{ 
			float2 texOffset = float2(offsets[i] * texelsize * m_BlurScale, 0.0f); 
			//Translate the offset(in pixels) to normalized texture coordinates, by 
			//multiplying offset[i] with the size of one pixel in the target texture. 

			result += T0Texture.SampleLevel(S0Sampler, IN.UV + texOffset, 0)*weights[i]; 
			//sample the pixel using the given offset and add its value to the result, 
			//weighted by weights[i].
		} 
		return result * weightSum * m_Color;
		//Last but not least: Multiply the result by 1.0/weighting-sum, in order to 
		//compute the average of all 21 weightend neighbor pixels on the horizontal axis. */
	}
#endif


#ifdef HAS_COMBINE
	float4 PSCombine(PS_INPUT IN) : SV_Target
	{
		if(m_FlareActive==0.0)
			return T0Texture.Sample(S0Sampler, IN.UV);
		else
		{
			if(m_ShowFlare==0.0)
			{
				float4 l_BloomColor = T1Texture.Sample(S1Sampler, IN.UV);
				// Get our original pixel from ColorMap
				float4 l_OriginalColor = T0Texture.Sample(S0Sampler, IN.UV);
				// Adjust color saturation and intensity based on the input variables to the shader
				l_BloomColor = l_BloomColor * m_FlareIntensity;
				l_OriginalColor = l_OriginalColor * m_OriginalIntensity;
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
