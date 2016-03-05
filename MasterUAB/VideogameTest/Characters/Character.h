#ifndef CHARACTER_H
#define CHARACTER_H

#include "Utils\Named.h"
#include <string>

class CXMLTreeNode;
class CRenderManager;
class CRenderableObject;

class CCharacter : public CNamed
{
public:
	CCharacter();
	CCharacter(const CXMLTreeNode&);
	virtual ~CCharacter();

	virtual void Update(float _ElapsedTime) = 0;
	//virtual void Render(CRenderManager&) = 0;

	virtual bool Refresh();

	CRenderableObject* GetRenderableObject();

	void ExecuteLuaCommand();

protected:
	std::string m_LuaCommand;
	std::string m_RenderableObjectName;
	std::string m_LayerName;

	CRenderableObject* m_RenderableObject;
};

#endif

