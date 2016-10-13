#ifndef LINE_RENDERER_H
#define LINE_RENDERER_H

#include "Utils.h"
#include "Utils\Named.h"
#include "RenderableObjects\VertexTypes.h"
#include "RenderableObjects\RenderableObject.h"

namespace tinyxml2
{
	class XMLElement;
}

#define MAX_LINE_LENGHT 20

class CMaterial;
class CLineRendererParameter;
class CRenderableObjectTechnique;
class CRenderableVertexs;

class CLineRenderer : public CRenderableObject
{
private:
	void Destroy();

	MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX m_LineRenderableData[MAX_LINE_LENGHT];
	CRenderableVertexs *m_RenderableVertex;
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	CMaterial*					m_Material;
	unsigned int				m_LinesCount;
	Vect3f m_PosInicial;
	Vect3f m_PosFinal;
	int m_NumPuntos;
	float m_OffsetX;
	float m_OffsetY;
	float m_Size;
	float m_SizeOffset;
	CColor m_Color;

public:
	CLineRenderer(tinyxml2::XMLElement* TreeNode);
	virtual ~CLineRenderer();

	CRenderableObjectTechnique* GetRenderableObjectTechnique();
	void Save(FILE* _File);
	virtual void Update(float ElapsedTime);
	virtual void Render(CRenderManager *RM);
	std::string GetTipo(){ return "LineRendererInstance"; };

	//UAB
	/*pos inicial
		pos final
		num puntos
		offset x
		offset y
		tipo
		max offset*/


	CEmptyPointerClass* GetLuaPosInciial(int index = 0)
	{
		return (CEmptyPointerClass*)&(m_PosInicial[index]);
	}
	CEmptyPointerClass* GetLuaPosFinal(int index = 0)
	{
		return (CEmptyPointerClass*)&(m_PosFinal[index]);
	}
	CEmptyPointerClass* GetLuaNumPuntos()
	{
		return (CEmptyPointerClass*)&m_NumPuntos;
	}
	CEmptyPointerClass* GetLuaOffsetX()
	{
		return (CEmptyPointerClass*)&m_OffsetX;
	}
	CEmptyPointerClass* GetLuaOffsetY()
	{
		return (CEmptyPointerClass*)&m_OffsetY;
	}
	CEmptyPointerClass* GetLuaSize()
	{
		return (CEmptyPointerClass*)&m_Size;
	}
	CEmptyPointerClass* GetLuaSizeOffset()
	{
		return (CEmptyPointerClass*)&m_SizeOffset;
	}
	CEmptyPointerClass* GetLuaColor()
	{
		return (CEmptyPointerClass*)&m_Color;
	}
};

#endif //LINE_RENDERER_H