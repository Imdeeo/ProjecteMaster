#ifndef SCENE_RENDERER_COMMAND_H
#define SCENE_RENDERER_COMMAND_H

#include "Utils\Named.h"

class CSceneRendererCommand :/*public CActive,*/ public CNamed
{
public:
	CSceneRendererCommand();
	virtual ~CSceneRendererCommand();
};

#endif //SCENE_RENDERER_COMMAND_H