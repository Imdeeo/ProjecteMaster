#if !(defined NDEBUG ^ defined _DEBUG)
#define _DEBUG
#endif

#include "PhysXManager.h"
#include "Math\Vector3.h"
#include "Math\Quatn.h"

#include <assert.h>

#define N_CPUS 2
#define PHYSX_VERSION_VS10 ((3<<24) + (3<<16) + (3<<8) + 0)

#include <PxPhysicsAPI.h>

static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;

#if USE_PHYSX_DEBUG
#define PVD_HOST "127.0.0.1"
#endif

#ifdef CHECKED_RELEASE
#undef CHECKED_RELEASE
#endif
#define CHECKED_RELEASE(x) if(x!=nullptr) {x->release();x=nullptr;}


inline physx::PxVec3 CastVec(const Vect3f& v)
{ return physx::PxVec3(v.x,v.y,v.z);}
inline Vect3f CastVec(const physx::PxVec3& v)
{ return Vect3f(v.x,v.y,v.z);}
inline Vect3f CastVec(const physx::PxExtendedVec3& v)
{ return Vect3f((float)v.x,(float)v.y,(float)v.z);}

inline physx::PxQuat CastQuat(const Quatf& q)
{ return physx::PxQuat(q.x,q.y,q.z,q.w);}
inline Quatf CastQuat(const physx::PxQuat& q)
{ return Quatf(q.x,q.y,q.z,q.w);}

inline void L_PutGroupToShape(physx::PxShape* shape, int _group)
{
	physx::PxFilterData filterData;
	filterData.setToDefault();
	filterData.word0 = _group;
	shape->setQueryFilterData(filterData);
}

class CPhysXManagerImplementation:
	public CPhysXManager,
	public physx::PxSimulationEventCallback, 
	public physx::PxUserControllerHitReport
{
private :
	void Init()
	{
		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		assert(m_Foundation);
		physx::PxProfileZoneManager* profileZoneManager = nullptr;
#if USE_PHYSX_DEBUG
		profileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(m_Foundation);
#endif
		m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation,physx::PxTolerancesScale(),true,profileZoneManager);
		assert(m_PhysX);

		m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(physx::PxTolerancesScale()));
		assert(m_Cooking);

#if USE_PHYSX_DEBUG		
		if(m_PhysX->getPvdConnectionManager())
		{
			m_PhysX->getVisualDebugger()->setVisualizeConstraints(true);
			m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS,true);
			m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS,true);
			m_DebugConnection = physx::PxVisualDebuggerExt::createConnection(m_PhysX->getPvdConnectionManager(),PVD_HOST,5425,10);
		}
		else
		{
			m_DebugConnection = nullptr;
		}
#endif
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(N_CPUS);

		physx::PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f,-10.f,0.0f);
		sceneDesc.cpuDispatcher = m_Dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		sceneDesc.flags = physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
		m_Scene = m_PhysX->createScene(sceneDesc);
		assert(m_Scene);

		m_Scene->setSimulationEventCallback(this);

#if PHYSX_VERSION_VS10 == PX_PHYSICS_VERSION
		m_ControllerManager = PxCreateControllerManager(*m_Scene);
#else
		m_ControllerManager = PxCreateControllerManager(*m_Scene,true);
#endif
		m_ControllerManager->setOverlapRecoveryModule(true);

	}

	void Destroy()
	{
		CleanCharacterControllers();

		CHECKED_RELEASE(m_ControllerManager);
		CHECKED_RELEASE(m_Scene);
		CHECKED_RELEASE(m_Dispatcher);
		physx::PxProfileZoneManager* profileZoneManager = m_PhysX->getProfileZoneManager();

#if USE_PHYSX_DEBUG
		CHECKED_RELEASE(m_DebugConnection);
#endif

		CHECKED_RELEASE(m_Cooking);
		CHECKED_RELEASE(m_PhysX);
		CHECKED_RELEASE(profileZoneManager);
		CHECKED_RELEASE(m_Foundation);

		CPhysXManager::Destroy();
	}

	void CleanCharacterControllers()
	{
		for (auto it = m_CharacterControllers.begin(); it != m_CharacterControllers.end(); ++it)
		{
			CHECKED_RELEASE(it->second);
		}
		m_CharacterControllers.clear();
	}

public:
	CPhysXManagerImplementation(){Init();}
	virtual ~CPhysXManagerImplementation(){Destroy();}

	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count){}
	void onWake(physx::PxActor** actors, physx::PxU32 count){}
	void onSleep(physx::PxActor** actors, physx::PxU32 count){}
	void onContact(const physx::PxContactPairHeader& pairHeader,const physx::PxContactPair* pairs, physx::PxU32 nbPairs){}
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		for(physx::PxU32 i = 0; i < count; i++)
		{
			if((pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER| physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)))
				continue;

			size_t l_indexTrigger = (size_t)pairs[i].triggerActor->userData;
			size_t l_indexActor = (size_t)pairs[i].otherActor->userData;

			std::string l_triggerName = m_ActorNames[l_indexTrigger];
			std::string l_actorName = m_ActorNames[l_indexActor];

			printf("Trigger \"%s\" fired with \"%s\"", l_triggerName.c_str(),l_actorName.c_str());
			//lo suyo seria llamar a una funcion lua que gestionara la activacion del trigger
		}
	}

	void onShapeHit(const physx::PxControllerShapeHit& hit){}
	void onControllerHit(const physx::PxControllersHit& hit){}
	void onObstacleHit(const physx::PxControllerObstacleHit& hit){}

	void CreateCharacterController(std::string _name, float _height, float _radius, float _density, Vect3f _position, std::string _MaterialName, int _group)
	{
		assert(m_CharacterControllers.find(_name)!=m_CharacterControllers.end());

		physx::PxMaterial* l_Material = m_Materials[_MaterialName];

		physx::PxCapsuleControllerDesc desc;
		desc.height = _height;
		desc.radius = _radius;
		desc.climbingMode = physx::PxCapsuleClimbingMode::eEASY;
		desc.slopeLimit = cosf(3.1415f / 6); //30º
		desc.stepOffset = 0.5f;
		desc.density = _density;
		desc.reportCallback = this;
		desc.position = physx::PxExtendedVec3(_position.x,_position.y+_radius + _height*0.5,_position.z);
		desc.material = l_Material;
		//desc.userData = (void*)index;

		m_CharacterControllers[_name] = m_ControllerManager->createController(desc);

		physx::PxRigidDynamic* l_actor = m_CharacterControllers[_name]->getActor();
		physx::PxShape *shape = l_actor->createShape(physx::PxBoxGeometry(_radius, _height + _radius * 2, _radius), *l_Material);

		L_PutGroupToShape(shape, _group);

		AddActor(_name,_position,Quatf(0,0,0,1),l_actor);
	}
};

CPhysXManager* CPhysXManager::CreatePhysXManager()
{
	return new CPhysXManagerImplementation();
}

void CPhysXManager::RegisterMaterial(const std::string &name, float staticFriction, float dynamicFriction, float restitution)
{
	/*
	auto it = m_Materials.find(name);
	if (it != m_Materials.end())
	{
		it->second->release(); // if a material with that name exist, we remove it
	}
	/*/
	assert(m_Materials.find(name)==m_Materials.end());
	//*/
	m_Materials[name] = m_PhysX->createMaterial(staticFriction,dynamicFriction,restitution);
}

inline void CPhysXManager::AssertCoordArrays()
{
	assert(m_Actors.size()==m_ActorNames.size()); // AOS sync fail
	assert(m_Actors.size()==m_ActorPositions.size()); // AOS sync fail
	assert(m_Actors.size()==m_ActorOrientations.size()); // AOS sync fail
	assert(m_Actors.size()==m_ActorIndexs.size()); // AOS sync fail
}

size_t CPhysXManager::AddActor(std::string _actorName, Vect3f _position, Quatf _orientation, physx::PxActor* _actor)
{
	size_t index = m_Actors.size();

	_actor->userData = (void*)index;

	m_ActorIndexs[_actorName] = index;
	m_ActorNames.push_back(_actorName);
	m_ActorPositions.push_back(_position);
	m_ActorOrientations.push_back(_orientation);
	m_Actors.push_back(_actor);

#ifdef _DEBUG
	AssertCoordArrays();
#endif

	return m_ActorIndexs[_actorName];

}

inline size_t CPhysXManager::GetActorIndexFromName(const std::string& _actorName)
{
	auto it = m_ActorIndexs.find(_actorName);
	assert(it != m_ActorIndexs.end());
	return it->second;
}

Vect3f CPhysXManager::GetActorPosition(const std::string& _actorName)
{
	return m_ActorPositions[GetActorIndexFromName(_actorName)];
}

Quatf CPhysXManager::GetActorOrientation(const std::string& _actorName)
{
	return m_ActorOrientations[GetActorIndexFromName(_actorName)];
}

void CPhysXManager::GetActorTransform(const std::string& _actorName, Vect3f* Pos_, Quatf* Orienation_)
{
	size_t l_index = GetActorIndexFromName(_actorName);
	Pos_ = &(m_ActorPositions[l_index]);
	Orienation_ = &(m_ActorOrientations[l_index]);
}

void CPhysXManager::CreateStaticShape(const std::string _name, Vect3f _size, physx::PxMaterial &_Material, Vect3f _position, Quatf _orientation, int _group)
{
	physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(_size.x/2, _size.y/2, _size.z/2), _Material);
	physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(_position),CastQuat(_orientation)));

	L_PutGroupToShape(shape, _group);

	body->attachShape(*shape);

	body->userData = (void*)AddActor(_name,_position,_orientation,body);
	m_Scene->addActor(*body);

	shape->release();
}


void CPhysXManager::CreateTrigger(const std::string _name, Vect3f _size, physx::PxMaterial &_Material, Vect3f _position, Quatf _orientation, int _group)
{
	physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(_size.x/2, _size.y/2, _size.z/2), _Material);
	physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(_position),CastQuat(_orientation)));

	L_PutGroupToShape(shape, _group);

	body->attachShape(*shape);

	body->userData = (void*)AddActor(_name,_position,_orientation,body);
	m_Scene->addActor(*body);

	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE,false);
	shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE,true);

	shape->release();

}

void CPhysXManager::CreateStaticPlane(const std::string _name, Vect3f _PlaneNormal, float _PlaneDistance,physx::PxMaterial &_Material,
	Vect3f _position, Quatf _orientation, int _group)
{
	physx::PxRigidStatic* groundPlane = 
		physx::PxCreatePlane(*m_PhysX, physx::PxPlane(_PlaneNormal.x, _PlaneNormal.y, _PlaneNormal.z,_PlaneDistance),_Material);
	groundPlane->userData = (void*)AddActor(_name,_position,_orientation,groundPlane);
	m_Scene->addActor(*groundPlane);

	physx::PxShape* shape;
	size_t numShapes = groundPlane->getShapes(&shape,1);
	assert(numShapes == 1);

	L_PutGroupToShape(shape, _group);

}

void CPhysXManager::CreateDinamicShape(const std::string _name,Vect3f _size,physx::PxMaterial &_Material,Vect3f _position, Quatf _orientation,
	float _density, int _group)
{
	physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(_size.x/2, _size.y/2, _size.z/2), _Material);
	physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(_position),CastQuat(_orientation)));

	L_PutGroupToShape(shape, _group);

	body->attachShape(*shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*body,_density);
	body->userData = (void*)AddActor(_name,_position,_orientation,body);
	m_Scene->addActor(*body);

	shape->release();
}

void CPhysXManager::CreateComplexShape(const std::string _name, physx::PxMaterial &_Material, Vect3f _position, Quatf _orientation,
	float _density, int _group)
{
	std::vector<Vect3f> l_vertices;
	physx::PxConvexMeshDesc convexDesc;
	convexDesc.points.count = l_vertices.size();
	convexDesc.points.stride = sizeof(Vect3f);
	convexDesc.points.data = &l_vertices[0];
	convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

	physx::PxDefaultMemoryOutputStream buff;
	physx::PxConvexMeshCookingResult::Enum result;
	bool succes = m_Cooking->cookConvexMesh(convexDesc,buff,&result);
	assert (succes);
	physx::PxDefaultMemoryInputData input(buff.getData(),buff.getSize());
	physx::PxConvexMesh* convexMesh = m_PhysX->createConvexMesh(input);

	physx::PxRigidDynamic* body = m_PhysX ->createRigidDynamic(physx::PxTransform(CastVec(_position),CastQuat(_orientation)));
	physx::PxShape* shape = body->createShape(physx::PxConvexMeshGeometry(convexMesh), _Material);

	L_PutGroupToShape(shape, _group);

	body->attachShape(*shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*body, _density);
	
	body->userData = (void*)AddActor(_name,_position,_orientation,body);
	m_Scene->addActor(*body);
}

void CPhysXManager::Update(float _dt)
{
	m_LeftoverSeconds = m_LeftoverSeconds + _dt;
	if(m_LeftoverSeconds >= PHYSX_UPDATE_STEP )
	{
		m_Scene->simulate(PHYSX_UPDATE_STEP);
		m_Scene->fetchResults(true);

		physx::PxU32 numActiveTransform;
		const physx::PxActiveTransform* activeTransforms = m_Scene->getActiveTransforms(numActiveTransform);

		for(physx::PxU32 i = 0;i<numActiveTransform; i++)
		{
			uintptr_t index = (uintptr_t)(activeTransforms[i].userData);
			m_ActorPositions[index] = CastVec(activeTransforms[i].actor2World.p);
			m_ActorOrientations[index] = CastQuat(activeTransforms[i].actor2World.q);
		}

		m_LeftoverSeconds = fmod(m_LeftoverSeconds, PHYSX_UPDATE_STEP);
	}
}

void CPhysXManager::CharacterControllerMove(std::string _name, Vect3f _movement, float _elapsedTime)
{
	physx::PxController* cct = m_CharacterControllers[_name];
	const physx::PxControllerFilters filters(nullptr, nullptr, nullptr);

	size_t index = (size_t)cct->getUserData();

	cct->move(CastVec(_movement), _movement.Length()*0.01, _elapsedTime, filters);

	physx::PxRigidDynamic* actor = cct->getActor();

	physx::PxExtendedVec3 p = cct->getFootPosition();
	physx::PxVec3 v = actor->getLinearVelocity();
}

bool CPhysXManager::RayHit(Vect3f _origin, Vect3f _dir, float _len, int GROUPS)
{
	physx::PxFilterData l_filterData;
	l_filterData.setToDefault();
	l_filterData.word0 = GROUPS;  // GROUP1 | GROUP2;
	physx::PxRaycastBuffer L_hit;
	return m_Scene->raycast(
		CastVec(_origin),
		CastVec(_dir),
		_len,
		L_hit,
		physx::PxHitFlags(physx::PxHitFlag::eDEFAULT),
		physx::PxQueryFilterData(
			l_filterData,
			physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC)
		);
}