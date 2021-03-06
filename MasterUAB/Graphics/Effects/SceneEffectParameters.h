#ifndef SCENE_EFFECT_H
#define SCENE_EFFECT_H

#include "Math\Matrix44.h"
#include "Math\Color.h"

class CSceneEffectParameters
{
public:
	Mat44f m_World;
	Mat44f m_View;
	Mat44f m_InverseView;
	Mat44f m_Projection;
	Mat44f m_InverseProjection;
	Vect4f m_CameraPosition;
	Vect4f m_CameraRightVector;
	Vect4f m_CameraUpVector;	
	CColor m_BaseColor;
	Vect4f m_Time;
	Vect4f m_CameraProjectionInfo;
};

#endif //SCENE_EFFECT_H