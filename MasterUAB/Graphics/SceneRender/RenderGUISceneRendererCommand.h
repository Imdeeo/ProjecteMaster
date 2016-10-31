// TODO
#ifndef RENDER_GUI_SCENE_RENDERER_COMMAND_H
#define RENDER_GUI_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CXMLTreeNode;
class CGUIManager;
class CLevel;

class CRenderGUISceneRendererCommand : public CSceneRendererCommand
{
private:
	CGUIManager *m_GUI;
public:
	CRenderGUISceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	virtual ~CRenderGUISceneRendererCommand();
	virtual void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_GUI_SCENE_RENDERER_COMMAND_H