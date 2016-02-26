#ifndef ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H
#define ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class ID3D11BlendState;

class CBlend : public CNamed
{
private:
	int m_Blend;

	int CheckBlendType(const std::string _type);
	void Destroy();
public:
	CBlend(const CXMLTreeNode &TreeNode, const std::string _name);
	virtual ~CBlend() { Destroy(); }
	UAB_GET_PROPERTY(int, Blend);
};

class CBlendOp : public CNamed
{
private:
	int m_BlendOp;

	int CheckBlendOpType(const std::string _type);
	void Destroy();
public:
	CBlendOp(const CXMLTreeNode &TreeNode, const std::string _name);
	virtual ~CBlendOp() { Destroy(); }
	UAB_GET_PROPERTY(int, BlendOp);
};

class CEnableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{
private:
	CBlend m_SrcBlend;
	CBlend m_DestBlend;
	CBlend m_SrcAlphaBlend;
	CBlend m_DestAlphaBlend;
	CBlendOp m_OpBlend;
	CBlendOp m_OpAlphaBlend;
	ID3D11BlendState *m_BlendState;
public:
	CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CEnableAlphaBlendSceneRendererCommand();

	virtual void Execute(CRenderManager &RenderManager);
};

#endif //ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H