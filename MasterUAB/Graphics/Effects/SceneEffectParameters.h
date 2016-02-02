#ifndef SCENE_EFFECT_H
#define SCENE_EFFECT_H

#include "Math\Matrix44.h"
#include "Math\Color.h"

class CSceneEffectParameters
{
public:
	Mat44f m_World;
	Mat44f m_View;
	Mat44f m_Projection;
	Vect4f m_CameraPosition;
	Vect4f m_CameraRightVector;
	Vect4f m_CameraUpVector;
	Vect4f m_BaseColor;
	CColor m_ColorBase;
};

#endif //SCENE_EFFECT_H