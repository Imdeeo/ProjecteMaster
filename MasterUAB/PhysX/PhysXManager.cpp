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
	}
public:
	CPhysXManagerImplementation(){Init();}
	virtual ~CPhysXManagerImplementation(){Destroy();}

	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count){}
	void onWake(physx::PxActor** actors, physx::PxU32 count){}
	void onSleep(physx::PxActor** actors, physx::PxU32 count){}
	void onContact(const physx::PxContactPairHeader& pairHeader,const physx::PxContactPair* pairs, physx::PxU32 nbPairs){}
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count){}

	void onShapeHit(const physx::PxControllerShapeHit& hit){}
	void onControllerHit(const physx::PxControllersHit& hit){}
	void onObstacleHit(const physx::PxControllerObstacleHit& hit){}
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

void CPhysXManager::AddActor(std::string _actorName, Vect3f _position, Quatf _orientation, physx::PxActor* _actor)
{
	int index = m_Actors.size();

	m_ActorIndexs[_actorName] = index;
	m_ActorNames.push_back(_actorName);
	m_ActorPositions.push_back(_position);
	m_ActorOrientations.push_back(_orientation);
	m_Actors.push_back(_actor);

#ifdef _DEBUG
	AssertCoordArrays();
#endif

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

void CPhysXManager::GetActorPositionAndOrientation(const std::string& _actorName,Vect3f* Pos_, Quatf* Orienation_)
{
	size_t l_index = GetActorIndexFromName(_actorName);
	Pos_ = &(m_ActorPositions[l_index]);
	Orienation_ = &(m_ActorOrientations[l_index]);
}

void CPhysXManager::CreateStaticShape(Vect3f _size,physx::PxMaterial &_Material,Vect3f _position, Quatf _orientation,size_t* index)
{
	physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(_size.x/2, _size.y/2, _size.z/2), _Material);
	physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(_position),CastQuat(_orientation)));
	body->attachShape(*shape);
	body->userData = (void*)index;
	m_Scene->addActor(*body);

	shape->release();
}

void CPhysXManager::CreateStaticPlane(Vect4f _size,physx::PxMaterial &_Material,Vect3f _position, Quatf _orientation,size_t* index)
{
	physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*m_PhysX, physx::PxPlane(_size.x, _size.y, _size.z,_size.w), _Material);
	groundPlane->userData = (void*)index;
	m_Scene->addActor(*groundPlane);

	physx::PxShape* shape;
	size_t numShapes = groundPlane->getShapes(&shape,1);
	assert(numShapes == 1);
}