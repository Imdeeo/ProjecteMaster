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

#define MAX_LINE_LENGHT 200

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
	float						m_Size;
	unsigned int				m_LinesCount;

public:
	CLineRenderer(tinyxml2::XMLElement* TreeNode);
	virtual ~CLineRenderer();

	CRenderableObjectTechnique* GetRenderableObjectTechnique();
	void Save(FILE* _File);
	virtual void Update(float ElapsedTime);
	virtual void Render(CRenderManager *RM);

	//UAB
	/*pos inicial
		pos final
		num puntos
		offset x
		offset y
		tipo
		max offset*/
	UAB_BUILD_GET_SET(Vect3f, PosInicial);
	UAB_BUILD_GET_SET(Vect3f, PosFinal);
	UAB_BUILD_GET_SET(int, NumPuntos);
	UAB_BUILD_GET_SET(float, OffsetX);
	UAB_BUILD_GET_SET(float, OffsetY);
	UAB_BUILD_GET_SET(int, Type);
	UAB_BUILD_GET_SET(float, MaxOffsetY);
};

#endif //LINE_RENDERER_H