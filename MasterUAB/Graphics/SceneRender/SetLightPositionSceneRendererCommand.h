#ifndef SET_LIGHT_POSITION_COMMAND_H
#define SET_LIGHT_POSITION_COMMAND_H

#include "SceneRendererCommand.h"

class CXMLTreeNode;
class CRenderManager;
class CLevel;

class CSetLightPositionSceneRendererCommand : public CSceneRendererCommand
{
private:
	std::string m_MaterialName;
	std::string m_LightName;
public:
	CSetLightPositionSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	virtual ~CSetLightPositionSceneRendererCommand();
	virtual void Execute(CRenderManager &RenderManager);
};

#endif //CLEAR_SCENE_RENDERER_COMMAND_H