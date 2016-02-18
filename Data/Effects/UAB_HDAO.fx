#include "globals.fxh"
#include "Samplers.fxh"

#define RING_4 (4)
#define NUM_RING_4_GATHERS (20)

static float m_HDAOActive=m_RawDataArray[0];
static bool m_HDAOShowNormals=m_RawDataArray[1]==1.0;
static bool m_HDAOShowAO=m_RawDataArray[2]==1.0;
static bool m_HDAOUseNormals=m_RawDataArray[3]==1.0;
//static float m_RTSize1=m_RawDataArray[4];
//static float m_RTSize2=m_RawDataArray[5];
static float2 m_RTSize=float2((float)m_RawDataArray[4], (float)m_RawDataArray[5]);
static float m_AcceptAngle=m_RawDataArray[6];
static float m_HDAOIntensity=m_RawDataArray[7];
//static float g_fQTimesZNearKK=m_RawDataArray[];
//static float g_fQKK=m_RawDataArray[9];
static float m_NormalScale=m_RawDataArray[8];
static float m_HDAORejectRadius=m_RawDataArray[9];
static float m_HDAOAcceptRadius=m_RawDataArray[10];

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


static const float m_RingWeightsTotal[RING_4] =
{
	5.30864,
	11.39610,
	17.92677,
	24.93639,
};

#define NUM_NORMAL_LOADS (4)

static const int2 m_NormalLoadPattern[NUM_NORMAL_LOADS] =
{
	{ 1, 8 },
	{ 8, -1 },
	{ 5, 4 },
	{ 4, -4 },
};

// Ring sample pattern
static const float2 m_HDAORingPattern[NUM_RING_4_GATHERS] =
{
	// Ring 1
	{ 1, -1 },
	{ 0, 1 },
	// Ring 2
	{ 0, 3 },
	{ 2, 1 },
	{ 3, -1 },
	{ 1, -3 },
	// Ring 3
	{ 1, -5 },
	{ 3, -3 },
	{ 5, -1 },
	{ 4, 1 },
	{ 2, 3 },
	{ 0, 5 },
	// Ring 4
	{ 0, 7 },
	{ 2, 5 },
	{ 4, 3 },
	{ 6, 1 },
	{ 7, -1 },
	{ 5, -3 },
	{ 3, -5 },
	{ 1, -7 },
};

// Ring weights
static const float4 m_HDAORingWeight[NUM_RING_4_GATHERS] =
{
	// Ring 1 (Sum = 5.30864)
	{ 1.00000, 0.50000, 0.44721, 0.70711 },
	{ 0.50000, 0.44721, 0.70711, 1.00000 },
	// Ring 2 (Sum = 6.08746)
	{ 0.30000, 0.29104, 0.37947, 0.40000 },
	{ 0.42426, 0.33282, 0.37947, 0.53666 },
	{ 0.40000, 0.30000, 0.29104, 0.37947 },
	{ 0.53666, 0.42426, 0.33282, 0.37947 },
	// Ring 3 (Sum = 6.53067)
	{ 0.31530, 0.29069, 0.24140, 0.25495 },
	{ 0.36056, 0.29069, 0.26000, 0.30641 },
	{ 0.26000, 0.21667, 0.21372, 0.25495 },
	{ 0.29069, 0.24140, 0.25495, 0.31530 },
	{ 0.29069, 0.26000, 0.30641, 0.36056 },
	{ 0.21667, 0.21372, 0.25495, 0.26000 },
	// Ring 4 (Sum = 7.00962)
	{ 0.17500, 0.17365, 0.19799, 0.20000 },
	{ 0.22136, 0.20870, 0.24010, 0.25997 },
	{ 0.24749, 0.21864, 0.24010, 0.28000 },
	{ 0.22136, 0.19230, 0.19799, 0.23016 },
	{ 0.20000, 0.17500, 0.17365, 0.19799 },
	{ 0.25997, 0.22136, 0.20870, 0.24010 },
	{ 0.28000, 0.24749, 0.21864, 0.24010 },
	{ 0.23016, 0.22136, 0.19230, 0.19799 },
};

float NormalRejectionTest(int2 i2ScreenCoord, float2 RTSize)
{
	float3 f3N1;
	float3 f3N2;
	float fDot;
	float fSummedDot = 0.0f;
	int2 i2MirrorPattern;
	int2 i2OffsetScreenCoord;
	int2 i2MirrorOffsetScreenCoord;
	
	for( int iNormal=0; iNormal<NUM_NORMAL_LOADS; iNormal++ )
	{
		i2MirrorPattern = ( m_NormalLoadPattern[iNormal] + int2( 1, 1 ) ) * int2( -1, -1 );
		i2OffsetScreenCoord = i2ScreenCoord + m_NormalLoadPattern[iNormal];
		i2MirrorOffsetScreenCoord = i2ScreenCoord + i2MirrorPattern;
		
		// Clamp our test to screen coordinates
		i2OffsetScreenCoord = ( i2OffsetScreenCoord > ( RTSize - float2( 1.0f, 1.0f ) ) ) ? ( RTSize - float2( 1.0f, 1.0f ) ) : ( i2OffsetScreenCoord );
		i2MirrorOffsetScreenCoord = ( i2MirrorOffsetScreenCoord > ( RTSize - float2( 1.0f, 1.0f ) ) ) ?	( RTSize - float2( 1.0f, 1.0f ) ) : ( i2MirrorOffsetScreenCoord );
		i2OffsetScreenCoord = ( i2OffsetScreenCoord < 0 ) ? ( 0 ) : ( i2OffsetScreenCoord );
		i2MirrorOffsetScreenCoord = ( i2MirrorOffsetScreenCoord < 0 ) ? ( 0 ) : ( i2MirrorOffsetScreenCoord );
		f3N1.zxy = T1Texture.Load( int3( i2OffsetScreenCoord, 0) ).xyz;
		f3N2.zxy = T1Texture.Load( int3( i2MirrorOffsetScreenCoord, 0) ).xyz;
		fDot = dot( f3N1, f3N2 );
		fSummedDot += ( fDot > m_AcceptAngle ) ? ( 0.0f ) : ( 1.0f - ( abs( fDot ) * 0.25f ) );
	}
	
	return ( 0.5f + fSummedDot * 0.25f );
}

float4 GatherSamples( Texture2D Tex, SamplerState _SamplerState, float2 f2TexCoord)
{
	float4 f4Ret;
	f4Ret = Tex.Gather( _SamplerState, f2TexCoord );
	return f4Ret;
}

float4 GatherZSamples( Texture2D Tex, SamplerState _SamplerState, float2 f2TexCoord, float Q, float QTimesZNear)
{
	float4 f4Ret;
	float4 f4Gather;
	f4Gather = GatherSamples( Tex, _SamplerState, f2TexCoord);
	f4Ret = -QTimesZNear.xxxx / ( f4Gather - Q.xxxx );
	return f4Ret;
}

float4 PSHDAO(PS_INPUT IN) : SV_Target
{
	if(m_HDAOActive==0.0)
		clip(-1);
		
	// Locals
	int2 i2ScreenCoord;
	float2 f2ScreenCoord_10_1;
	float2 f2ScreenCoord;
	float2 f2MirrorScreenCoord;
	float2 f2TexCoord_10_1;
	float2 f2MirrorTexCoord_10_1;
	float2 f2TexCoord;
	float2 f2MirrorTexCoord;
	float2 f2InvRTSize;
	float fCenterZ;
	float fOffsetCenterZ;
	float fCenterNormalZ;
	float4 f4SampledZ[2];
	float4 f4OffsetSampledZ[2];
	float4 f4SampledNormalZ[2];
	float4 f4Diff;
	float4 f4Compare[2];
	float4 f4Occlusion = 0.0f;
	float fOcclusion;
	int iGather;
	float fDot = 1.0f;
	float m_RTSize1 = m_RTSize.x / 1024.0f;
	float m_RTSize2 = m_RTSize.y / 768.0f;
	float2 f2KernelScale = float2( m_RTSize1, m_RTSize2 );
	float3 f3CameraPos;
	int iNumRings=RING_4;
	int iNumRingGathers=NUM_RING_4_GATHERS;
	float l_Q=m_CameraProjectionInfo.y/(m_CameraProjectionInfo.y-m_CameraProjectionInfo.x);
	float l_QTimesZNear=l_Q*m_CameraProjectionInfo.x;
	
	// Compute integer screen coord, and store off the inverse of the RT Size
	f2InvRTSize = 1.0f / m_RTSize;
	f2ScreenCoord = IN.UV * m_RTSize;
	i2ScreenCoord = int2( f2ScreenCoord );
	
	// Test the normals to see if we should apply occlusion
	fDot = NormalRejectionTest( i2ScreenCoord, m_RTSize);
	
	if( fDot > 0.5f )
	{
		// Sample the center pixel for camera Z
		// For Gather we need to snap the screen coords
		f2ScreenCoord_10_1 = float2( i2ScreenCoord );
		f2TexCoord = float2( f2ScreenCoord_10_1 * f2InvRTSize );
		float fDepth = T2Texture.SampleLevel( S2Sampler, f2TexCoord, 0 ).x;
		fCenterZ = -l_QTimesZNear / ( fDepth - l_Q );
		
		if( m_HDAOUseNormals )
		{
			fCenterNormalZ = T1Texture.SampleLevel( S1Sampler, f2TexCoord, 0 ).x;
			fOffsetCenterZ = fCenterZ + fCenterNormalZ * m_NormalScale;
		}
		
		// Loop through each gather location, and compare with its mirrored location
		for( iGather=0; iGather<iNumRingGathers; iGather++ )
		{
			f2MirrorScreenCoord = ( ( f2KernelScale * m_HDAORingPattern[iGather] ) + float2( 1.0f, 1.0f ) ) * float2( -1.0f, -1.0f );
			f2TexCoord = float2( ( f2ScreenCoord + ( f2KernelScale * m_HDAORingPattern[iGather] ) )	* f2InvRTSize );
			f2MirrorTexCoord = float2( ( f2ScreenCoord + ( f2MirrorScreenCoord ) ) * f2InvRTSize );
			
			// Sample
			f2TexCoord_10_1 = float2( ( f2ScreenCoord_10_1 + ( ( f2KernelScale * m_HDAORingPattern[iGather] ) + float2( 1.0f, 1.0f ) ) ) * f2InvRTSize );
			f2MirrorTexCoord_10_1 = float2( ( f2ScreenCoord_10_1 + ( f2MirrorScreenCoord + float2( 1.0f, 1.0f ) ) ) * f2InvRTSize );
			f4SampledZ[0] = GatherZSamples( T2Texture, S2Sampler, f2TexCoord_10_1, l_Q, l_QTimesZNear);
			f4SampledZ[1] = GatherZSamples( T2Texture, S2Sampler, f2MirrorTexCoord_10_1, l_Q, l_QTimesZNear);
			
			// Detect valleys
			f4Diff = fCenterZ.xxxx - f4SampledZ[0];
			f4Compare[0] = ( f4Diff < m_HDAORejectRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
			f4Compare[0] *= ( f4Diff > m_HDAOAcceptRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
			f4Diff = fCenterZ.xxxx - f4SampledZ[1];
			f4Compare[1] = ( f4Diff < m_HDAORejectRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
			f4Compare[1] *= ( f4Diff > m_HDAOAcceptRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
			f4Occlusion.xyzw += ( m_HDAORingWeight[iGather].xyzw * ( f4Compare[0].xyzw * f4Compare[1].zwxy ) * fDot );
			
			if( m_HDAOUseNormals )
			{
				// Sample normals
				f4SampledNormalZ[0] = GatherSamples( T1Texture, S1Sampler,
				f2TexCoord_10_1);
				f4SampledNormalZ[1] = GatherSamples( T1Texture, S1Sampler,
				f2MirrorTexCoord_10_1);
				
				// Scale normals
				f4OffsetSampledZ[0] = f4SampledZ[0] + ( f4SampledNormalZ[0] * m_NormalScale );
				f4OffsetSampledZ[1] = f4SampledZ[1] + ( f4SampledNormalZ[1] * m_NormalScale );
				
				// Detect valleys
				f4Diff = fOffsetCenterZ.xxxx - f4OffsetSampledZ[0];
				f4Compare[0] = ( f4Diff < m_HDAORejectRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
				f4Compare[0] *= ( f4Diff > m_HDAOAcceptRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
				f4Diff = fOffsetCenterZ.xxxx - f4OffsetSampledZ[1];
				f4Compare[1] = ( f4Diff < m_HDAORejectRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
				f4Compare[1] *= ( f4Diff > m_HDAOAcceptRadius.xxxx ) ? ( 1.0f ) : ( 0.0f );
				f4Occlusion.xyzw += ( m_HDAORingWeight[iGather].xyzw * ( f4Compare[0].xyzw * f4Compare[1].zwxy ) * fDot );
			}			
		}		
	}

	// Finally calculate the HDAO occlusion value
	m_HDAOUseNormals=true;
	if( m_HDAOUseNormals )
	{
		fOcclusion = ( ( f4Occlusion.x + f4Occlusion.y + f4Occlusion.z + f4Occlusion.w ) / ( 3.0f *	m_RingWeightsTotal[iNumRings - 1] ) );
	}	
	else
	{
		fOcclusion = ( ( f4Occlusion.x + f4Occlusion.y + f4Occlusion.z + f4Occlusion.w ) / ( 2.0f *	m_RingWeightsTotal[iNumRings - 1] ) );
	}
	
	fOcclusion *= ( m_HDAOIntensity );
	fOcclusion = 1.0f - saturate( fOcclusion );	

	if(m_HDAOShowAO)
		return float4(fOcclusion, fOcclusion, fOcclusion, 1.0);

	if(m_HDAOShowNormals)
		return float4(T1Texture.Sample(S1Sampler, IN.UV).xyz, 1.0);

	return float4(T0Texture.Sample(S0Sampler, IN.UV).xyz*fOcclusion, 1.0);
}