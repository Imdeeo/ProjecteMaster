#ifndef ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H
#define ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
class CEnableAlphaBlendSceneRendererCommand :
	public CSceneRendererCommand
{
private:
	CBlend m_SrcBlend;
	CBlend m_DestBlend;
	CBlend m_SrcAlphaBlend;					//C2146 falta ';' delante del identificador  --> C4430 falta especificador de tipo; se presupone int. Nota: C++ no admite default-int
	CBlend m_DestAlphaBlend;
	CBlendOp m_OpBlend;
	CBlendOp m_OpAlphaBlend;
public:
	CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CEnableAlphaBlendSceneRendererCommand();

	virtual void Execute(CRenderManager &RenderManager);
};

class CBlend : public CNamed
{
private:
	int m_Blend;

	int CheckBlendType(const std::string _type);
	void Destroy();
public:
	CBlend():CNamed(""){} // Si no hago esto, da error de que no tiene constructor predefinido.
	CBlend(const CXMLTreeNode &TreeNode, const std::string _name);
	virtual ~CBlend() { Destroy(); }
};

class CBlendOp : public CNamed
{
private:
	int m_BlendOp;

	int CheckBlendOpType(const std::string _type);
	void Destroy();
public:
	CBlendOp() :CNamed(""){}
	CBlendOp(const CXMLTreeNode &TreeNode, const std::string _name);
	virtual ~CBlendOp() { Destroy(); }
};

#endif //ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H