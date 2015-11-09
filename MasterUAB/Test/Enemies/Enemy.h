#ifndef _ENEMY
#define _ENEMY

#pragma once

#include "XML\XMLTreeNode.h"

class CEnemy
{
private:
	float m_X, m_Y, m_Z;
	float m_Yaw;
	float m_Pitch;
	float m_Roll;
	int m_Lifes;
public:
	CEnemy(CXMLTreeNode &XMLTreeNode)
	{
		const char *l_Pos=XMLTreeNode.GetPszProperty("pos", "0.0 0.0 0.0");
		sscanf_s(l_Pos, "%f %f %f", &m_X, &m_Y, &m_Z);
		m_Yaw=XMLTreeNode.GetFloatProperty("yaw", 0.0f);
		m_Pitch=XMLTreeNode.GetFloatProperty("pitch", 0.0f);
		m_Roll=XMLTreeNode.GetFloatProperty("roll", 0.0f);
		m_Lifes=XMLTreeNode.GetIntProperty("life", 0);
	}
	virtual ~CEnemy() {}
	virtual void Update(float ElapsedTime) = 0;
	virtual void Render() = 0;
	
	void SetX(float X) {m_X=X;}
	void SetY(float Y) {m_Y=Y;}
	void SetZ(float Z) {m_Z=Z;}

	
	template<class T>
	static CEnemy * GetTemplatedEnemy(CXMLTreeNode &XMLTreeNode)
	{
		return new T(XMLTreeNode);
	}
};

#endif
