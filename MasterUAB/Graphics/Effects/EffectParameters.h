#ifndef EFFECT_PARAMETERS_H
#define EFFECT_PARAMETERS_H

#include "Math\Matrix44.h"
#include "Utils.h";

class CEffectParameters
{
public:
	Mat44f m_World;
	Mat44f m_View;
	Mat44f m_Projection;
	Vect4f m_BaseColor;
	Mat44f m_Bones[MAXBONES];
	Vect4f m_CameraRightVector;
	Vect4f m_CameraUpVector;
	Vect4f m_BillboardPosition;
	Vect4f m_BillboardSize;
	Vect4f m_DebugRenderScale;
};

#endif //EFFECT_PARAMETERS_H