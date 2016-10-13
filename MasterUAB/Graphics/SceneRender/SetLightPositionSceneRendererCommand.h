#ifndef SET_LIGHT_POSITION_COMMAND_H
#define SET_LIGHT_POSITION_COMMAND_H

#include "SceneRendererCommand.h"

class CXMLTreeNode;
class CRenderManager;

class CSetLightPositionSceneRendererCommand : public CSceneRendererCommand
{
private:
	std::string m_MaterialName;
	std::string m_LightName;
public:
	CSetLightPositionSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CSetLightPositionSceneRendererCommand();
	virtual void Execute(CRenderManager &RenderManager);
};

#endif //CLEAR_SCENE_RENDERER_COMMAND_H