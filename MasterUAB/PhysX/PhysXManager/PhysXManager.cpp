#if !(defined NDEBUG ^ defined _DEBUG)
#define _DEBUG
#endif

#include "PhysXManager.h"
#include "Math\Vector3.h"
#include "Math\Quatn.h"
#include "RenderManager\RenderManager.h"
#include "Effects\EffectManager.h"
#include "Math\MathTypes.h"
#include "Engine\UABEngine.h"
#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"

#include "Utils\CEmptyPointerClass.h"

#include "RenderableObjects\RenderableVertexs.h"

#include "XML\XMLTreeNode.h"

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
			physx::PxVisualDebuggerConnectionFlags connectionFlags ( physx::PxVisualDebuggerExt::getAllConnectionFlags() );

			m_PhysX->getVisualDebugger()->setVisualizeConstraints(true);
			//m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONSTRAINTS, true);
			//m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS,true);		
			m_DebugConnection = physx::PxVisualDebuggerExt::createConnection(m_PhysX->getPvdConnectionManager(), PVD_HOST, 5425, 100, connectionFlags);
			if (m_DebugConnection)
				m_DebugConnection->release();
		}
		else
		{
			m_DebugConnection = nullptr;
		}
#endif
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(N_CPUS);

		physx::PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(.0f, -9.81f, .0f);
		sceneDesc.cpuDispatcher = m_Dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		sceneDesc.flags = physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
		m_Scene = m_PhysX->createScene(sceneDesc);
		assert(m_Scene);

		m_Scene->setGravity(physx::PxVec3(.0f, -9.81f, .0f));
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

	void CreateCharacterController(const std::string _name, float _height, float _radius, float _density, Vect3f _position, const std::string _MaterialName, int _group)
	{
		assert(m_CharacterControllers.find(_name) == m_CharacterControllers.end());
		
		physx::PxMaterial* l_Material = m_Materials[_MaterialName];

		physx::PxCapsuleControllerDesc desc;
		desc.height = _height;
		desc.radius = _radius;
		desc.climbingMode = physx::PxCapsuleClimbingMode::eEASY;
		desc.slopeLimit = cosf(3.1415f / 6); //30º
		desc.stepOffset = 0.5f;
		desc.density = _density;
		desc.reportCallback = this;
		desc.position = physx::PxExtendedVec3(_position.x,_position.y + _radius + _height*0.5f,_position.z);
		desc.material = l_Material;
		size_t l_index = m_CharacterControllers.size();
		desc.userData = (void*)l_index;

		m_CharacterControllers[_name] = m_ControllerManager->createController(desc);


		physx::PxRigidDynamic* l_actor = m_CharacterControllers[_name]->getActor();
		/*physx::PxShape *shape = l_actor->createShape(physx::PxBoxGeometry(_radius, _height + _radius * 2, _radius), *l_Material);

		L_PutGroupToShape(shape, _group);*/

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

bool CPhysXManager::LoadMaterials(const std::string &Filename)
{
	m_Filename = Filename;
	std::string l_EffectName;
	float l_StaticFriction;
	float l_DynamicFriction;
	float l_Restitution;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(m_Filename.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["physx"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("material"))
				{
					l_EffectName = l_Element.GetPszProperty("name");
					l_StaticFriction = l_Element.GetFloatProperty("static_friction");
					l_DynamicFriction = l_Element.GetFloatProperty("dynamic_friction");
					l_Restitution = l_Element.GetFloatProperty("restitution");
					RegisterMaterial(l_EffectName, l_StaticFriction, l_DynamicFriction, l_Restitution);
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
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

void CPhysXManager::RegisterActor(const std::string _name, physx::PxShape* _shape, physx::PxRigidActor* _body, Vect3f _position, Quatf _orientation, int _group)
{
	L_PutGroupToShape(_shape, _group);

	_body->attachShape(*_shape);

	_body->userData = (void*)AddActor(_name, _position, _orientation, _body);
	m_Scene->addActor(*_body);
}

void CPhysXManager::RegisterActor(const std::string _name, physx::PxShape* _shape, physx::PxRigidBody* _body, Vect3f _position, Quatf _orientation, float _density, int _group, bool _isKinematic)
{
	_body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, _isKinematic);

	physx::PxRigidBodyExt::updateMassAndInertia(*_body, _density);
	RegisterActor(_name, _shape, _body, _position, _orientation, _group);
}

physx::PxShape* CPhysXManager::CreateStaticShape(const std::string _name, const physx::PxGeometry &_geometry, const std::string _Material, Vect3f _position, Quatf _orientation, int _group)
{
	physx::PxMaterial* l_Material = m_Materials[_Material];
	//physx::PxVec3 v = CastVec(Size);
	//physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(v.x / 2, v.y / 2, v.z / 2), *l_Material);

	physx::PxShape* shape = m_PhysX->createShape(_geometry, *l_Material);
	physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(_position),CastQuat(_orientation)));

	RegisterActor(_name, shape, body, _position, _orientation, _group);

	return shape;
}

void CPhysXManager::CreateStaticBox(const std::string _name, Vect3f _size, const std::string _Material, Vect3f _position, Quatf _orientation, int _group)
{
	physx::PxVec3 l_HalfSize = physx::PxVec3(_size.x / 2, _size.y / 2, _size.z / 2);
	CreateStaticShape(
		_name,
		physx::PxBoxGeometry(l_HalfSize),
		_Material,
		_position,
		_orientation,
		_group)->release();
}

void CPhysXManager::CreateStaticSphere(const std::string _name, float _radius, const std::string _Material, Vect3f _position, Quatf _orientation, int _group)
{
	CreateStaticShape(
		_name,
		physx::PxSphereGeometry(_radius),
		_Material,
		_position,
		_orientation,
		_group)->release();

}


void CPhysXManager::CreateBoxTrigger(const std::string _name, Vect3f _size, const std::string _Material, Vect3f _position, Quatf _orientation, int _group)
{
	physx::PxShape* shape = CreateStaticShape(
		_name,
		physx::PxBoxGeometry(_size.x / 2, _size.y / 2, _size.z / 2),
		_Material,
		_position,
		_orientation,
		_group);

	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE,false);
	shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE,true);

	shape->release();

}

void CPhysXManager::CreateStaticPlane(const std::string _name, Vect3f _PlaneNormal, float _PlaneDistance, const std::string _Material,
	Vect3f _position, Quatf _orientation, int _group)
{
	physx::PxMaterial* l_Material = m_Materials[_Material];
	physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*m_PhysX, physx::PxPlane(_PlaneNormal.x, _PlaneNormal.y, _PlaneNormal.z,_PlaneDistance),*l_Material);
	groundPlane->userData = (void*)AddActor(_name,_position,_orientation,groundPlane);
	m_Scene->addActor(*groundPlane);

	physx::PxShape* shape;
	size_t numShapes = groundPlane->getShapes(&shape,1);
	assert(numShapes == 1);

	L_PutGroupToShape(shape, _group);

	shape->userData = groundPlane->userData;

}

void CPhysXManager::CreateDinamicShape(const std::string _name, const physx::PxGeometry &_geometry, const std::string _Material, Vect3f _position, Quatf _orientation,
	float _density, int _group, bool _isKinematic)
{
	physx::PxMaterial* l_Material = m_Materials[_Material];
	physx::PxShape* shape = m_PhysX->createShape(_geometry, *l_Material);
	physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(_position),CastQuat(_orientation)));

	RegisterActor(_name, shape, body, _position, _orientation, _density, _group, _isKinematic);

	shape->release();
}

void CPhysXManager::CreateDinamicBox(const std::string _name, Vect3f _size, const std::string _Material, Vect3f _position, Quatf _orientation,
	float _density, int _group, bool isKinematic)
{
	CreateDinamicShape(_name,
		physx::PxBoxGeometry(_size.x / 2, _size.y / 2, _size.z / 2),
		_Material, _position, _orientation, _density, _group, isKinematic);
}

void CPhysXManager::CreateDinamicSphere(const std::string _name, float _radius, const std::string _Material, Vect3f _position, Quatf _orientation,
	float _density, int _group, bool isKinematic)
{
	CreateDinamicShape(_name,
		physx::PxSphereGeometry(_radius),
		_Material, _position, _orientation, _density, _group, isKinematic);
}

void CPhysXManager::CreateRigidStatic(const std::string &Name, const Vect3f Size, const Vect3f &Position, const Quatf &Orientation, const std::string &MaterialName)
{
	if (m_ActorIndexs.find(Name) != m_ActorIndexs.end())
	{
		RemoveActor(Name);
	}
	AssertCoordArrays();
	const physx::PxMaterial* l_Material = m_Materials[MaterialName];
	physx::PxVec3 v = CastVec(Size);
	physx::PxShape* l_Shape = m_PhysX->createShape(physx::PxBoxGeometry(v.x/2,v.y/2,v.z/2),*l_Material);
	physx::PxRigidStatic* l_Body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(Position),CastQuat(Orientation)));
	
	l_Body->attachShape(*l_Shape);
	size_t index=m_Actors.size();
	l_Body->userData = (void*)index;
	m_Scene->addActor(*l_Body);
	l_Shape->release();
	AddActor(Name, Position, Orientation, l_Body);

}

physx::PxShape* CPhysXManager::CreateStaticShapeFromBody(const std::string _name, const physx::PxGeometry &_geometry, const std::string _Material, Vect3f _position, Quatf _orientation, int _group)
{
	physx::PxMaterial* l_Material = m_Materials[_Material];
	physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(_position), CastQuat(_orientation)));
	physx::PxShape* shape = body->createShape(_geometry, *l_Material);

	RegisterActor(_name, shape, body, _position, _orientation, _group);

	return shape;
}

void CPhysXManager::CreateDinamicShapeFromBody(const std::string _name, const physx::PxGeometry &_geometry, const std::string _Material, Vect3f _position, Quatf _orientation,
	float _density, int _group, bool _isKinematic)
{
	physx::PxMaterial* l_Material = m_Materials[_Material];
	physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(_position), CastQuat(_orientation)));
	physx::PxShape* shape = body->createShape(_geometry, *l_Material);

	RegisterActor(_name, shape, body, _position, _orientation, _density, _group, _isKinematic);
}

physx::PxConvexMesh* L_CreateConvexMesh(std::vector<Vect3f> _vertices, physx::PxCooking* _Cooking, physx::PxPhysics* _PhysX)
{
	physx::PxConvexMeshDesc convexDesc;
	convexDesc.points.count = _vertices.size();
	convexDesc.points.stride = sizeof(Vect3f);
	convexDesc.points.data = &_vertices[0];
	convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

	physx::PxDefaultMemoryOutputStream buff;
	physx::PxConvexMeshCookingResult::Enum result;
	bool succes = _Cooking->cookConvexMesh(convexDesc, buff, &result);
	assert(succes);
	physx::PxDefaultMemoryInputData input(buff.getData(), buff.getSize());
	physx::PxConvexMesh* convexMesh = _PhysX->createConvexMesh(input);

	return convexMesh;
}

void CPhysXManager::CreateComplexDinamicShape(const std::string _name, std::vector<Vect3f> _vertices, const std::string _Material, Vect3f _position, Quatf _orientation,
	float _density, int _group, bool _isKinematic)
{
	physx::PxConvexMesh* convexMesh = L_CreateConvexMesh(_vertices, m_Cooking, m_PhysX);
	CreateDinamicShapeFromBody(_name, physx::PxConvexMeshGeometry(convexMesh), _Material, _position, _orientation, _density, _group, _isKinematic);
}

void CPhysXManager::CreateComplexStaticShape(const std::string _name, std::vector<Vect3f> _vertices, const std::string _Material, Vect3f _position, Quatf _orientation, int _group)
{
	physx::PxConvexMesh* convexMesh = L_CreateConvexMesh(_vertices, m_Cooking, m_PhysX);
	CreateStaticShapeFromBody(_name, physx::PxConvexMeshGeometry(convexMesh), _Material, _position, _orientation, _group)->release();
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

	Vect3f movemenAux = _movement;
	Vect3f move1 = CastVec(cct->getPosition());
	movemenAux = _elapsedTime*movemenAux;
	cct->move(CastVec(movemenAux), movemenAux.Length()*0.01, _elapsedTime, filters);

	Vect3f move2 = CastVec(cct->getPosition());
	physx::PxRigidDynamic* actor = cct->getActor();
	

	physx::PxExtendedVec3 p = cct->getFootPosition();
	physx::PxVec3 v = actor->getLinearVelocity();
}

bool CPhysXManager::Raycast(const Vect3f _origin, const Vect3f _end, int GROUPS,RaycastData* result_)
{
	physx::PxFilterData l_filterData;
	l_filterData.setToDefault();
	l_filterData.word0 = GROUPS;  // GROUP1 | GROUP2;
	physx::PxRaycastBuffer l_hit;
	bool didHit = m_Scene->raycast(
		CastVec(_origin),
		CastVec((_end - _origin).GetNormalized()),
		_origin.Distance(_end),
		l_hit,
		physx::PxHitFlags(physx::PxHitFlag::eDEFAULT),
		physx::PxQueryFilterData(
			l_filterData,
			physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC)
		);
	if (didHit)
	{
		if (l_hit.hasBlock)
		{
			result_->position = CastVec(l_hit.block.position);
			result_->normal = CastVec(l_hit.block.normal);
			result_->distance = l_hit.block.distance;
			result_->actorname = m_ActorNames[(size_t)l_hit.block.actor->userData];
		}
		else
			didHit = false;
	}
	return didHit;
}

void CPhysXManager::RemoveActor(const std::string _ActorName)
{
	size_t l_index = m_ActorIndexs[_ActorName];
	auto it_controller = m_CharacterControllers.find(_ActorName);
	if (it_controller != m_CharacterControllers.end())
	{
		CHECKED_RELEASE(it_controller->second);
		m_CharacterControllers.erase(it_controller);
	}
	else
	{
		m_Actors[l_index]->release();
	}
	if (m_Actors.size() > 1)
	{
		m_Actors[l_index] = m_Actors[m_Actors.size() - 1];
		m_Actors.resize(m_Actors.size() - 1);

		m_ActorNames[l_index] = m_ActorNames[m_Actors.size()];
		m_ActorNames.resize(m_Actors.size());

		m_ActorPositions[l_index] = m_ActorPositions[m_Actors.size()];
		m_ActorPositions.resize(m_Actors.size());

		m_ActorOrientations[l_index] = m_ActorOrientations[m_Actors.size()];
		m_ActorOrientations.resize(m_Actors.size());

		m_ActorIndexs[m_ActorNames[l_index]] = l_index;
		m_ActorIndexs.erase(m_ActorIndexs.find(_ActorName));
		m_Actors[l_index]->userData = (void *)l_index;
	}
	else
	{
		m_Actors.resize(0);
		m_ActorNames.resize(0);
		m_ActorPositions.resize(0);
		m_ActorOrientations.resize(0);
		m_ActorIndexs.clear();
	}
	
}

void CPhysXManager::Render(const std::string _name, CRenderManager *RenderManager)
{
	if (_name == "player")
	{
		CEffectManager::m_SceneParameters.m_BaseColor = CColor(1, 0, 0, 1);
	}
	else
	{
		CEffectManager::m_SceneParameters.m_BaseColor = CColor(0, 0, 1, 1);
	}
	
	CEffectManager::m_SceneParameters.m_BaseColor.SetAlpha(1.f);
	Mat44f l_ScaleMatrix, l_RotationMatrix, l_TranslationMatrix;

	l_ScaleMatrix.SetIdentity();
	l_ScaleMatrix.Scale(1.0f, 1.0f, 1.0f);

	l_RotationMatrix.SetIdentity();
	l_RotationMatrix.SetPitchRollYaw(Vect3f(0, 0, 0));
	
	l_TranslationMatrix.SetIdentity();
	
	l_TranslationMatrix.SetPos(m_CharacterControllers[_name]->getPosition().x, m_CharacterControllers[_name]->getPosition().y, m_CharacterControllers[_name]->getPosition().z);

	l_TranslationMatrix = l_ScaleMatrix*l_RotationMatrix*l_TranslationMatrix;

	RenderManager->GetContextManager()->SetWorldMatrix(l_TranslationMatrix);
	CEffectTechnique* l_EffectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_lights")->GetEffectTechnique();
	CEffectManager::SetSceneConstants(l_EffectTechnique);
	RenderManager->GetDebugRender()->GetCone()->RenderIndexed(RenderManager, l_EffectTechnique, CEffectManager::GetRawData());
}


Vect3f CPhysXManager::GetCharacterControllersPosition(const std::string _name)
{
	physx::PxController* aux = m_CharacterControllers[_name];
	return Vect3f(aux->getPosition().x, aux->getPosition().y, aux->getPosition().z);
}

CEmptyPointerClass* CPhysXManager::GetCharacterControllersPositionX(const std::string _name)
{
	physx::PxController* aux = m_CharacterControllers[_name];
	return (CEmptyPointerClass*)(&aux->getPosition().x);
}
CEmptyPointerClass* CPhysXManager::GetCharacterControllersPositionY(const std::string _name)
{
	physx::PxController* aux = m_CharacterControllers[_name];
	return (CEmptyPointerClass*)(&aux->getPosition().y);
}
CEmptyPointerClass* CPhysXManager::GetCharacterControllersPositionZ(const std::string _name)
{
	physx::PxController* aux = m_CharacterControllers[_name];
	return (CEmptyPointerClass*)(&aux->getPosition().z);
}
