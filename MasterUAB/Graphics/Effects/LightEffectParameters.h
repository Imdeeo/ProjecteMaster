#ifndef LIGHT_EFFECT_SHADER_H
#define LIGHT_EFFECT_SHADER_H

#include "Math\Matrix44.h"

class CLightEffectParameters
{
public:
	Vect4f m_LightAmbient;
	float m_LightEnabled[MAX_LIGHTS_BY_SHADER];
	float m_LightType[MAX_LIGHTS_BY_SHADER];
	Vect4f m_LightPosition[MAX_LIGHTS_BY_SHADER];
	Vect4f m_LightDirection[MAX_LIGHTS_BY_SHADER];
	float m_LightAngle[MAX_LIGHTS_BY_SHADER];
	float m_LightFallOffAngle[MAX_LIGHTS_BY_SHADER];
	float m_LightAttenuationStartRange[MAX_LIGHTS_BY_SHADER];
	float m_LightAttenuationEndRange[MAX_LIGHTS_BY_SHADER];
	float m_LightIntensity[MAX_LIGHTS_BY_SHADER];
	Vect4f m_LightColor[MAX_LIGHTS_BY_SHADER];
	float m_UseShadowMap[MAX_LIGHTS_BY_SHADER];
	float m_UseShadowMask[MAX_LIGHTS_BY_SHADER];
	Mat44f m_LightView[MAX_LIGHTS_BY_SHADER];
	Mat44f m_LightProjection[MAX_LIGHTS_BY_SHADER];
	Vect4f m_MaxAttenuation_StartLinearFog_EndLinearFog;
	Vect4f m_FogColor;
};

#endif //LIGHT_EFFECT_SHADER_H