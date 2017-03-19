#ifndef ANIMATED_MODEL_EFFECT_PARAMETER_H
#define ANIMATED_MODEL_EFFECT_PARAMETER_H

#include "Math\Matrix44.h"

class CAnimatedModelEffectParameters
{
public:
	Mat44f m_Bones[MAXBONES];
};

#endif //ANIMATED_MODEL_EFFECT_PARAMETER_H