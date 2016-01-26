#ifndef PHYSX_MANAGER_H
#define PHYSX_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include "Math\Vector4.h"
#include "Math\Quatn.h"

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxCooking;
	class PxMaterial;
	class PxActor;
	class PxControllerManager;
	class PxController;
	class PxShape;
	class PxGeometry;
	class PxShape;
	class PxRigidBody;
	class PxRigidActor;

	namespace debugger
	{
		namespace comm
		{
			class PvdConnection;
		}
	}

	typedef debugger::comm::PvdConnection PxVisualDebuggerConnection;
}

typedef struct SRaycastData
{
	Vect3f position;
	Vect3f normal;
	float distance;
	std::string actorname;
} RaycastData;

#define USE_PHYSX_DEBUG 1

#define PHYSX_UPDATE_STEP 1

class CPhysXManager
{
protected:
	CPhysXManager(){};
public:
	static CPhysXManager* CreatePhysXManager();
	virtual ~CPhysXManager(){ Destroy(); }

	void RegisterMaterial(const std::string &name, float staticFriction, float dynamicFriction, float restitution);
	inline void AssertCoordArrays();
	inline size_t GetActorIndexFromName(const std::string& _actorName);

	Vect3f GetActorPosition(const std::string& _actorName);
	Quatf GetActorOrientation(const std::string& _actorName);

	void GetActorTransform(const std::string& _actorName,Vect3f* Pos_, Quatf* Orienation_);

	void CreateStaticBox(const std::string _name, Vect3f _size, const std::string _Material, Vect3f _position, Quatf _orientation, int _group);
	void CreateStaticSphere(const std::string _name, float _radius, const std::string _Material, Vect3f _position, Quatf _orientation, int _group);
	void CreateStaticPlane(const std::string _name, Vect3f _PlaneNormal, float _PlaneDistance, const std::string _Material, Vect3f _position, Quatf _orientation, int _group);

	void CreateDinamicBox(const std::string _name, Vect3f _size, const std::string _Material, Vect3f _position, Quatf _orientation, float _density, int _group, bool _isKinematic = false);
	void CreateDinamicSphere(const std::string _name, float _radius, const std::string _Material, Vect3f _position, Quatf _orientation, float _density, int _group, bool _isKinematic = false);

	void CreateComplexDinamicShape(const std::string _name, std::vector<Vect3f> _vertices, const std::string _Material, Vect3f _position, Quatf _orientation, float _density, int _group, bool _isKinematic = false);
	void CreateComplexStaticShape(const std::string _name, std::vector<Vect3f> _vertices, const std::string _Material, Vect3f _position, Quatf _orientation, int _group);

	void CreateBoxTrigger(const std::string _name, Vect3f _size, const std::string _Material, Vect3f _position, Quatf _orientation, int _group);

	virtual void CreateCharacterController(const std::string _name, float _height, float _radius, float _density, Vect3f _position, const std::string _MaterialName, int _group) = 0;

	void CharacterControllerMove(std::string _name, Vect3f _movement, float _elapsedTime);

	bool Raycast(const Vect3f _origin, const Vect3f _end, int _GROUPS, RaycastData* result_);

	void RemoveActor(const std::string _ActorName);

	void Update(float _dt);
protected:

	physx::PxFoundation				*m_Foundation;
	physx::PxPhysics				*m_PhysX;

#if USE_PHYSX_DEBUG
	physx::PxVisualDebuggerConnection *m_DebugConnection;
#endif

	physx::PxDefaultCpuDispatcher	*m_Dispatcher;
	physx::PxScene					*m_Scene;
	physx::PxCooking				*m_Cooking;
	physx::PxControllerManager		*m_ControllerManager;

	std::map<std::string,physx::PxMaterial*> m_Materials;
	std::map<std::string,physx::PxController*> m_CharacterControllers;

	std::map<std::string, size_t>	m_ActorIndexs;
	std::vector<std::string>		m_ActorNames;
	std::vector<Vect3f>				m_ActorPositions;
	std::vector<Quatf>				m_ActorOrientations;
	std::vector<physx::PxActor*>	m_Actors;

	float							m_LeftoverSeconds;
	
	size_t AddActor(std::string _actorName, Vect3f _position, Quatf _orientation, physx::PxActor*);

	physx::PxShape* CreateStaticShape(const std::string _name, physx::PxGeometry _geometry, const std::string _Material, Vect3f _position, Quatf _orientation, int _group);
	physx::PxShape* CreateStaticShapeFromBody(const std::string _name, physx::PxGeometry _geometry, const std::string _Material, Vect3f _position, Quatf _orientation, int _group);
	void CreateDinamicShape(const std::string _name, physx::PxGeometry _geometry, const std::string _Material, Vect3f _position, Quatf _orientation, float _density, int _group, bool _isKinematic = false);
	void CreateDinamicShapeFromBody(const std::string _name, physx::PxGeometry _geometry, const std::string _Material, Vect3f _position, Quatf _orientation, float _density, int _group, bool _isKinematic = false);

	void RegisterActor(const std::string _name, physx::PxShape* _shape, physx::PxRigidActor* _body, Vect3f _position, Quatf _orientation, int _group);
	void RegisterActor(const std::string _name, physx::PxShape* _shape, physx::PxRigidBody* _body, Vect3f _position, Quatf _orientation, float _density, int _group, bool _isKinematic = false);

	void Destroy()
	{

	}

};

#endif //PHYSX_MANAGER_H