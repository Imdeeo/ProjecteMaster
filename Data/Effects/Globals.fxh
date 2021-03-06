//Globals.fxh
#ifndef GLOBALS_FXH
#define GLOBALS_FXH

#define MAXBONES 150
#define MAX_LIGHTS_BY_SHADER 4

#define MAX_SPECULAR_POWER 300
#define MIN_SPECULAR_POWER 3
#define SPECULAR_DIMMING_POWER 0.7
#define METAL_SPECFACTOR 0.85
#define DEFAULT_SPEC_COLOR float3(0.7f, 0.7f, 0.7f)

// difference in glossiness (specular power) that corresponds to one mip level
// with 12, max mip level (glossiness 0) is 8.33, suitable for a cube map of size 512x512
#define GLOSS_UNITS_PER_MIP_LEVEL 12.0
// number of mip levels (including level 0) for a cube map of size 512x512
#define NUM_LEVELS_CUBE512 10.0

#define FRESNEL_POWER 5

cbuffer SceneConstantBuffer : register( b0 )
{
	float4x4 m_World;
	float4x4 m_View;
	float4x4 m_InverseView;	
	float4x4 m_Projection;
	float4x4 m_InverseProjection;
	float4 m_CameraPosition;
	float4 m_CameraRightVector;
	float4 m_CameraUpVector;
	float4 m_BaseColor;
	float4 m_Time;
	float4 m_CameraProjectionInfo;
}

cbuffer LightsConstantBuffer : register (b1)
{
	float4 m_LightAmbient;
	float4 m_LightEnabled;
	float4 m_LightType; //0 : OMNI, 1 : DIRECTIONAL, 2 : SPOT
	float4 m_LightPosition[MAX_LIGHTS_BY_SHADER];
	float4 m_LightDirection[MAX_LIGHTS_BY_SHADER];
	float4 m_LightAngle;
	float4 m_LightFallOffAngle;
	float4 m_LightAttenuationStartRange;
	float4 m_LightAttenuationEndRange;
	float4 m_LightIntensity;
	float4 m_LightColor[MAX_LIGHTS_BY_SHADER];
	float4 m_UseShadowMap;
	float4 m_UseShadowMask;
	float4x4 m_LightView[MAX_LIGHTS_BY_SHADER];
	float4x4 m_LightProjection[MAX_LIGHTS_BY_SHADER];
}

cbuffer AnimatedModelConstantBuffer : register (b2)
{
	float4x4 m_Bones[MAXBONES];
}

cbuffer MaterialConstantBuffer : register(b3)
{
	float4 m_RawData[16];
}

static float m_RawDataArray[64]=((float[64])m_RawData);

static float m_LightEnabledArray[4]=(float[4])m_LightEnabled;
static float m_LightTypeArray[4]=(float[4])m_LightType;
static float m_LightAngleArray[4]=(float[4])m_LightAngle;
static float m_LightFallOffAngleArray[4]=(float[4])m_LightFallOffAngle;
static float m_LightAttenuationStartRangeArray[4]=(float[4])m_LightAttenuationStartRange;
static float m_LightAttenuationEndRangeArray[4]=(float[4])m_LightAttenuationEndRange;
static float m_LightIntensityArray[4]=(float[4])m_LightIntensity;
static float m_UseShadowMapArray[4] = (float[4])m_UseShadowMap;
static float m_UseShadowMaskArray[4] = (float[4])m_UseShadowMask;

#endif //GLOBALS_FXH