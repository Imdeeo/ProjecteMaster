#ifndef SCENE_RENDERER_COMMAND_H
#define SCENE_RENDERER_COMMAND_H

#include "Utils\Named.h"
#include "Utils\Active.h"
#include "XML\tinyxml2.h"

class CMaterial;
class CRenderManager;

class CSceneRendererCommand :public CActive, public CNamed
{
protected:
	CMaterial* m_Material;
public:
	CSceneRendererCommand(tinyxml2::XMLElement* TreeNode);
	virtual ~CSceneRendererCommand();
	virtual void Execute(CRenderManager &RenderManager) = 0;
};

#endif //SCENE_RENDERER_COMMAND_H