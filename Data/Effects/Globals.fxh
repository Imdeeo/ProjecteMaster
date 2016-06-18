//Globals.fxh
#ifndef GLOBALS_FXH
#define GLOBALS_FXH

#define MAXBONES 150
#define MAX_LIGHTS_BY_SHADER 4

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
	float4 m_MaxAttenuation_StartFog_EndFog;
	float4 m_FogColor;
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
static float m_MaxAttenuation = m_MaxAttenuation_StartFog_EndFog.x;
static float m_StartFog = m_MaxAttenuation_StartFog_EndFog.y;
static float m_EndFog = m_MaxAttenuation_StartFog_EndFog.z;

static float m_UseShadowMapArray[4] = (float[4])m_UseShadowMap;
static float m_UseShadowMaskArray[4] = (float[4])m_UseShadowMask;


#endif //GLOBALS_FXH