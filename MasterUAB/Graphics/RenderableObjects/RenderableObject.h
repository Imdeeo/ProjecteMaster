#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include <vector>

#include "Utils\LevelInfo.h"

#include "Utils\Named.h"
#include "3DElement\3DElement.h"
#include "XML\tinyxml2.h"

class CUABComponentManager;
class CRenderManager;
class CLevel;

class CRenderableObject : public C3DElement, public CNamed, public CLevelInfo
{
public:
	enum TRenderableObjectType
	{
		STATIC_MODEL = 0,
		ANIMATED_MODEL,
		PARTICLE_EMITER
	};
private:
	bool m_DebugRender;
public:
	CRenderableObject(CLevel*);
	CRenderableObject(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	virtual ~CRenderableObject();
	virtual void Save(FILE* _File, std::string _layer){}
	virtual void Update(float ElapsedTime){};
	virtual void Render(CRenderManager *RM){};
	void SetDebugRender(bool _DebugRender){ m_DebugRender = _DebugRender;}
	bool GetDebugRender(){ return m_DebugRender; }
	virtual std::string GetTipo() {
		return "RenderableObject";
	};
	virtual CRenderableObject& CRenderableObject::operator=(CRenderableObject&);
};

#endif //RENDERABLE_OBJECT_H

	// COSAS DE ISAAC!!!!
	/*struct SSubmesh
	{
		const CRenderableVertexs* vertices;
		std::string material;
	};

	void AddSubmesh(const CRenderableVertexs* _Vertices, const std::string& _Material, float _BoundingRadius, const Vect3f& _BoundingBoxMin, const Vect3f& _BoundingBoxMax);

	void SetTransform(const SPositionOrientation& _transform) { m_Transform = _transform; }
	void SetPosition(const Vect3f& _position) { m_Transform.Position = _position; }
	void SetYaw(float _yaw) { m_Transform.Yaw = _yaw; }
	void SetPitch(float _pitch) { m_Transform.Pitch = _pitch; }
	void SetRoll(float _roll) { m_Transform.Roll = _roll; }

	int GetNumSubmeshes() const { return m_Submeshes.size(); }
	const SSubmesh& GetSubmesh(size_t i) const { return m_Submeshes[i]; }
	const SPositionOrientation& GetTransform() const { return m_Transform; }
	SPositionOrientation* GetPtrTransform() { return &m_Transform; }

	float GetBoundingRadius() const { return m_BoundingRadius; }
	Vect3f GetBoundingMin() const { return m_BoundingBoxMin; }
	Vect3f GetBoundingMax() const { return m_BoundingBoxMax; }

private:
	std::vector<SSubmesh> m_Submeshes;
	SPositionOrientation m_Transform;
	float m_BoundingRadius;
	Vect3f m_BoundingBoxMin, m_BoundingBoxMax;*/
