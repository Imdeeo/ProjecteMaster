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

	void CreateStaticShape(const std::string _name, Vect3f _size, physx::PxMaterial &_Material, Vect3f _position, Quatf _orientation, int _group);
	void CreateStaticPlane(const std::string _name, Vect3f _PlaneNormal, float _PlaneDistance, physx::PxMaterial &_Material, Vect3f _position, Quatf _orientation, int _group);

	void CreateDinamicShape(const std::string _name, Vect3f _size, physx::PxMaterial &_Material, Vect3f _position, Quatf _orientation, float _density, int _group);
	void CreateComplexShape(const std::string _name, physx::PxMaterial &_Material, Vect3f _position, Quatf _orientation, float _density, int _group);

	void CreateTrigger(const std::string _name, Vect3f _size, physx::PxMaterial &_Material, Vect3f _position, Quatf _orientation, int _group);

	virtual void CreateCharacterController(std::string _name, float _height, float _radius, float _density, Vect3f _position, std::string _MaterialName, int _group) = 0;

	void CharacterControllerMove(std::string _name, Vect3f _movement, float _elapsedTime);

	bool Raycast(const Vect3f _origin, const Vect3f _end, int _GROUPS, RaycastData* result_);

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

	void Destroy()
	{

	}

};

#endif //PHYSX_MANAGER_H