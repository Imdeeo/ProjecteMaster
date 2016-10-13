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
#include "ScriptManager\ScriptManager.h"
#include "Utils\CEmptyPointerClass.h"
#include "StaticMesh\StaticMesh.h"

#include "RenderableObjects\VertexTypes.h"
	
#include "RenderableObjects\RenderableVertexs.h"

#include "XML\tinyxml2.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <assert.h>

#define HEADER 51966
#define FOOTER 65226

#define N_CPUS 2
#define PHYSX_VERSION_VS10 ((3<<24) + (3<<16) + (3<<8) + 0)

#include <PxPhysicsAPI.h>

//#define CONTACT_OFFSET			0.01f
#define CONTACT_OFFSET			0.1f
//#define STEP_OFFSET				0.01f
#define STEP_OFFSET				0.2f
//#define STEP_OFFSET				0.1f
//#define STEP_OFFSET				0.2f

#define SLOPE_LIMIT				0.8f
//#define SLOPE_LIMIT				0.0f
//#define INVISIBLE_WALLS_HEIGHT	6.0f
#define INVISIBLE_WALLS_HEIGHT	0.0f
//#define MAX_JUMP_HEIGHT			4.0f
#define MAX_JUMP_HEIGHT			0.0f

static const float gScaleFactor = 1.5f;
static const float gStandingSize = 1.0f * gScaleFactor;
static const float gCrouchingSize = 0.25f * gScaleFactor;
static const float gControllerRadius = 0.3f * gScaleFactor;

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


physx::PxFilterFlags PxMySimulationFilterShader(
	physx::PxFilterObjectAttributes attributes0,
	physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1,
	physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags,
	const void* constantBlock,
	physx::PxU32 constantBlockSize)
{
	
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		}
		else
		{
			pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		}
	}
	return physx::PxFilterFlag::eDEFAULT;
}

class MyQueryFilterCallback : physx::PxQueryFilterCallback
{
	physx::PxQueryHitType::Enum preFilter(
		const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags)
	{
		physx::PxFilterData l_filterData2 = shape->getSimulationFilterData();
		if ((filterData.word0 & l_filterData2.word1) && (l_filterData2.word0 & filterData.word1))
		{
			return physx::PxQueryHitType::eTOUCH;
		}
		return physx::PxQueryHitType::eNONE;
	}

	physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit)
	{
		return physx::PxQueryHitType::eNONE;
	}
};

static std::map<physx::PxU32, physx::PxU32> st_CollisionGroups;

inline void L_PutGroupToShape(physx::PxShape* shape, int _group)
{
	physx::PxFilterData filterData;
	filterData.setToDefault();
	filterData.word0 = _group;
	filterData.word1 = st_CollisionGroups[_group];
	shape->setQueryFilterData(filterData);
	shape->setSimulationFilterData(filterData);
}


class CPhysXManagerImplementation:
	public CPhysXManager,
	public physx::PxSimulationEventCallback, 
	public physx::PxUserControllerHitReport,
	public physx::PxControllerBehaviorCallback
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
			m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONSTRAINTS, true);
			m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS,true);		
			m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
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
		sceneDesc.filterShader = PxMySimulationFilterShader;
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
		//CHECKED_RELEASE(m_DebugConnection);
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

	bool LoadMaterials(tinyxml2::XMLElement* l_Input){

		std::string l_EffectName;
		float l_StaticFriction;
		float l_DynamicFriction;
		float l_Restitution;

		tinyxml2::XMLElement* l_Element = l_Input->FirstChildElement();

		while (l_Element != NULL)
		{
			if (l_Element->Name() == std::string("material"))
			{
				l_EffectName = l_Element->GetPszProperty("name");
				l_StaticFriction = l_Element->GetFloatProperty("static_friction");
				l_DynamicFriction = l_Element->GetFloatProperty("dynamic_friction");
				l_Restitution = l_Element->GetFloatProperty("restitution");
				RegisterMaterial(l_EffectName, l_StaticFriction, l_DynamicFriction, l_Restitution);
			}
			l_Element = l_Element->NextSiblingElement();
		}
		return true;
	}
	bool LoadCollisionGroups(tinyxml2::XMLElement* l_Input)
	{
		std::string l_GroupName;
		m_Groups.clear();

		tinyxml2::XMLElement* l_Element = l_Input->FirstChildElement();

		while (l_Element != NULL)
		{
			if (l_Element->Name() == std::string("collision_group"))
			{
				l_GroupName = l_Element->GetPszProperty("name");
				m_Groups[l_GroupName] = (1 << m_Groups.size());
			}
			l_Element = l_Element->NextSiblingElement();
		}		
		return true;
	}
	bool LoadGroupsRelations(tinyxml2::XMLElement* l_Input){

		std::string l_GroupName;
		physx::PxU32 l_Group;

		tinyxml2::XMLElement* l_Element = l_Input->FirstChildElement();
		tinyxml2::XMLElement* l_ElementAux;

		while (l_Element != NULL)
		{
			if (l_Element->Name() == std::string("group"))
			{
				l_GroupName = l_Element->GetPszProperty("name");
				l_Group = m_Groups[l_GroupName];
				st_CollisionGroups[l_Group] = 0;
				std::string l_Collision;

				l_ElementAux = l_Element->FirstChildElement();
				while (l_ElementAux != NULL)
				{
					if (l_ElementAux->Name() == std::string("collides"))
					{
						l_Collision = l_ElementAux->GetPszProperty("name");
						st_CollisionGroups[l_Group] |= m_Groups[l_Collision];
					}
					l_ElementAux = l_ElementAux->NextSiblingElement();
				}
			}
			l_Element = l_Element->NextSiblingElement();
		}
		return true;
	}
	
public:

	CPhysXManagerImplementation(){Init();}
	virtual ~CPhysXManagerImplementation(){Destroy();}

	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count){}
	void onWake(physx::PxActor** actors, physx::PxU32 count){}
	void onSleep(physx::PxActor** actors, physx::PxU32 count){}
	void onContact(const physx::PxContactPairHeader& pairHeader,const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		size_t l_firstActorIndex = (size_t)pairs->shapes[0]->getActor()->userData;
		size_t l_secondActorIndex = (size_t)pairs->shapes[0]->getActor()->userData;

		std::string l_firstActorName = m_ActorNames[l_firstActorIndex];
		std::string l_secondActorName = m_ActorNames[l_secondActorIndex];

		printf("Contact \"%s\" with \"%s\"", l_firstActorName.c_str(), l_secondActorName.c_str());

	}
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) 
	{
		for (physx::PxU32 i = 0; i < count; i++)
		{
			if ((pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)))
				continue;

			size_t l_indexTrigger = (size_t)pairs[i].triggerActor->userData;
			if (m_TriggerIsActive[l_indexTrigger])
			{

				size_t l_indexActor = (size_t)pairs[i].otherActor->userData;

				std::string l_triggerName = m_ActorNames[l_indexTrigger];
				std::string l_actorName = m_ActorNames[l_indexActor];
				//CRenderableObject* l_ro = UABEngine.GetLayerManager()->GetResource("Triggers")->GetResource(l_triggerName);
				//l_ro->GetComponentManager()->onTrigger(l_actorName);
				std::vector<std::string> l_ActiveActors = m_ActiveActors[l_indexTrigger];
				for (size_t i = 0; i < l_ActiveActors.size(); i++)
				{
					if (l_ActiveActors[i] == l_actorName)
					{
						if (m_TriggerActivated[l_indexTrigger] ==std::string(""))
						{
							if (m_OnTriggerEnterLuaFunctions[l_indexTrigger] != std::string(""))
							{
								UABEngine.GetInstance()->GetScriptManager()->RunCode(m_OnTriggerEnterLuaFunctions[l_indexTrigger] + "(\"" + l_triggerName + "\",\"" + l_actorName + "\")");
							}
							m_TriggerActivated[l_indexTrigger] = l_actorName;
						}
						else
						{
							if (m_OnTriggerExitLuaFunctions[l_indexTrigger] != std::string(""))
							{
								UABEngine.GetInstance()->GetScriptManager()->RunCode(m_OnTriggerExitLuaFunctions[l_indexTrigger] + "(\"" + l_triggerName + "\",\"" + l_actorName + "\")");
							}
							m_TriggerActivated[l_indexTrigger] = "";
						}
					}
				}
				printf("Trigger \"%s\" fired with \"%s\"", l_triggerName.c_str(), l_actorName.c_str());
				//lo suyo seria llamar a una funcion lua que gestionara la activacion del trigger
			}
		}
	}

	void onShapeHit(const physx::PxControllerShapeHit& hit)
	{
		
	}
	void onControllerHit(const physx::PxControllersHit& hit)
	{
		
	}
	void onObstacleHit(const physx::PxControllerObstacleHit& hit)
	{
		
	}

	physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxShape& shape, const physx::PxActor& actor)
	{
		std::cout << "getBehaviorFlags (shape n' actor)!\n";
		return physx::PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
	}
	physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxController& controller)
	{
		std::cout << "getBehaviorFlags (controller)!\n";
		return physx::PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
	}
	physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxObstacle& obstacle)
	{
		std::cout << "getBehaviorFlags (obstacle)!\n";
		return physx::PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
	}

	void CreateCharacterController(const std::string _name, float _height, float _radius, float _density, Vect3f _position, const std::string _MaterialName, std::string _group)
	{
		//SACADA DE SAMPLES
		//assert(m_CharacterControllers.find(_name) == m_CharacterControllers.end());
		if (m_CharacterControllers.find(_name) != m_CharacterControllers.end())
			RemoveActor(_name);

		physx::PxMaterial* l_Material = m_Materials[_MaterialName];
		size_t l_index = m_CharacterControllers.size();

		physx::PxCapsuleControllerDesc capsuleDesc;
		capsuleDesc.height = _height;
		capsuleDesc.radius = _radius;
		capsuleDesc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;

		physx::PxControllerDesc* cDesc;
		cDesc = &capsuleDesc;
		cDesc->density = _density;
		cDesc->scaleCoeff = 0.8f;
		cDesc->material = l_Material;
		cDesc->position = physx::PxExtendedVec3(_position.x, _position.y, _position.z);
		cDesc->slopeLimit = SLOPE_LIMIT;
		cDesc->contactOffset = CONTACT_OFFSET;
		cDesc->stepOffset = STEP_OFFSET;
		cDesc->invisibleWallHeight = INVISIBLE_WALLS_HEIGHT;
		cDesc->maxJumpHeight = MAX_JUMP_HEIGHT;
		cDesc->nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
		cDesc->reportCallback = this;
		cDesc->behaviorCallback = this;
		cDesc->volumeGrowth = 1.5f;

		m_CharacterControllers[_name] = m_ControllerManager->createController(*cDesc);
		physx::PxRigidDynamic* l_actor = m_CharacterControllers[_name]->getActor()->is<physx::PxRigidDynamic>();
		l_actor->setLinearDamping(0.15f);
		l_actor->setAngularDamping(15.0f);
		l_actor->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
		l_actor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);
		l_actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
		l_actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
		l_actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);

		physx::PxShape* shape;
		l_actor->getShapes(&shape, 1);
		shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		L_PutGroupToShape(shape, m_Groups[_group]);
		//l_actor->attachShape(*shape);

		AddActor(_name, _position, Quatf(0, 0, 0, 1), l_actor);
	}

	void ChangeRigidDynamicActorPhysxGroup(const std::string &_ActorName, const std::string &_group){
		physx::PxShape* shape;
		((physx::PxRigidDynamic*)m_Actors[m_ActorIndexs[_ActorName]])->getShapes(&shape, 1);
		L_PutGroupToShape(shape, m_Groups[_group]);
	}

	bool LoadPhysx(const std::string &Filename)
	{
		m_Filename = Filename;
		
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError l_Error = doc.LoadFile(Filename.c_str());

		tinyxml2::XMLElement* l_Element;

		l_Element = doc.FirstChildElement("physx")->FirstChildElement();

		if (l_Error == tinyxml2::XML_SUCCESS)
		{
			while (l_Element != NULL)
			{
				if (l_Element->Name() == std::string("materials"))
				{
					LoadMaterials(l_Element);
				}
				if (l_Element->Name() == std::string("collision_groups"))
				{
					LoadCollisionGroups(l_Element);
				}
				if (l_Element->Name() == std::string("groups_relations"))
				{
					LoadGroupsRelations(l_Element);
				}
				l_Element = l_Element->NextSiblingElement();
			}
		}
		else
		{
			return false;
		}
		return true;
	}
};

CPhysXManager* CPhysXManager::CreatePhysXManager()
{
	return new CPhysXManagerImplementation();
}

void CPhysXManager::RegisterMaterial(const std::string &name, float staticFriction, float dynamicFriction, float restitution)
{
	assert(m_Materials.find(name)==m_Materials.end());
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

void CPhysXManager::RegisterActor(const std::string _name, physx::PxShape* _shape, physx::PxRigidActor* _body, Vect3f _position, Quatf _orientation, std::string _group)
{
	L_PutGroupToShape(_shape, m_Groups[_group]);

	_body->attachShape(*_shape);

	_body->userData = (void*)AddActor(_name, _position, _orientation, _body);
	m_Scene->addActor(*_body);
}

void CPhysXManager::RegisterActor(const std::string _name, physx::PxShape* _shape, physx::PxRigidBody* _body, Vect3f _position, Quatf _orientation, float _density, std::string _group, bool _isKinematic)
{
	_body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, _isKinematic);

	physx::PxRigidBodyExt::updateMassAndInertia(*_body, _density);
	RegisterActor(_name, _shape, _body, _position, _orientation, _group);
}

physx::PxShape* CPhysXManager::CreateStaticShape(const std::string _name, const physx::PxGeometry &_geometry, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group)
{
	physx::PxMaterial* l_Material = m_Materials[_Material];

	physx::PxShape* shape = m_PhysX->createShape(_geometry, *l_Material);
	physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(_position),CastQuat(_orientation)));

	RegisterActor(_name, shape, body, _position, _orientation, _group);

	return shape;
}

void CPhysXManager::CreateStaticBox(const std::string _name, Vect3f _size, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group)
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

void CPhysXManager::CreateStaticSphere(const std::string _name, float _radius, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group)
{
	CreateStaticShape(
		_name,
		physx::PxSphereGeometry(_radius),
		_Material,
		_position,
		_orientation,
		_group)->release();
}

bool LoadMeshFile(std::string _FileName,physx::PxU8** Data_,physx::PxU32* Size_)
{
	std::fstream l_File(_FileName, std::ios::binary | std::ios::in);
	if (!l_File.is_open())
	{
		return false;
	}
	else
	{
		unsigned short l_BufferUnsignedShort;
		l_File.read((char*)&l_BufferUnsignedShort,sizeof(unsigned short));
		if (l_BufferUnsignedShort != HEADER)
		{
			return false;
		}


		l_File.read((char*)Size_, sizeof(physx::PxU32));

		*Data_ = (physx::PxU8*)malloc(sizeof(physx::PxU8)*(*Size_));
		l_File.read((char*)*Data_, *Size_);

		l_File.read((char*)&l_BufferUnsignedShort, sizeof(unsigned short));
		if (l_BufferUnsignedShort != FOOTER)
		{
			return false;
		}
	}
	return true;
}
void CPhysXManager::CreateStaticConvexMesh(const std::string _name, const CStaticMesh* _Mesh, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group)
{
	std::vector<CRenderableVertexs*> l_RenderableVertex = _Mesh->GetRenderableVertexs();
	for (size_t i = 0; i < l_RenderableVertex.size(); i++)
	{
		char l_FileName[256] = "";
		sprintf_s(l_FileName, "%s\\%s_%u.cmesh", _Mesh->GetPhysxMeshesDirectory().c_str(), _name.c_str(), i);

		physx::PxU8* l_Data;
		physx::PxU32 l_Size;

		if (LoadMeshFile(l_FileName, &l_Data, &l_Size)){
			physx::PxDefaultMemoryInputData l_DefaultMemoryInputData(l_Data, l_Size);
			physx::PxConvexMesh* l_ConvexMesh = m_PhysX->createConvexMesh(l_DefaultMemoryInputData);

			physx::PxRigidStatic* l_Body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(_position), CastQuat(_orientation)));
			physx::PxShape* l_Shape = l_Body->createShape(physx::PxConvexMeshGeometry(l_ConvexMesh), *m_Materials[_Material]);

			char l_ActorName[256] = "";
			sprintf_s(l_ActorName, "%s_%u", _name.c_str(), i);

			L_PutGroupToShape(l_Shape, m_Groups[_group]);

			l_Body->userData = (void*)AddActor(l_ActorName, _position, _orientation, l_Body);
			m_Scene->addActor(*l_Body);
		}
	}
}

void CPhysXManager::CreateDynamicConvexMesh(const std::string _name, const CStaticMesh* _Mesh, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group)
{
	std::vector<CRenderableVertexs*> l_RenderableVertex = _Mesh->GetRenderableVertexs();
	for (size_t i = 0; i < l_RenderableVertex.size(); i++)
	{
		char l_FileName[256] = "";
		sprintf_s(l_FileName, "%s\\%s_%u.cmesh", _Mesh->GetPhysxMeshesDirectory().c_str(), _name.c_str(), i);

		physx::PxU8* l_Data;
		physx::PxU32 l_Size;

		if (LoadMeshFile(l_FileName, &l_Data, &l_Size)){
			physx::PxDefaultMemoryInputData l_DefaultMemoryInputData(l_Data, l_Size);
			physx::PxConvexMesh* l_ConvexMesh = m_PhysX->createConvexMesh(l_DefaultMemoryInputData);

			physx::PxRigidDynamic* l_Body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(_position), CastQuat(_orientation)));
			physx::PxShape* l_Shape = l_Body->createShape(physx::PxConvexMeshGeometry(l_ConvexMesh), *m_Materials[_Material]);

			char l_ActorName[256] = "";
			sprintf_s(l_ActorName, "%s_%u", _name.c_str(), i);

			L_PutGroupToShape(l_Shape, m_Groups[_group]);

			l_Body->userData = (void*)AddActor(l_ActorName, _position, _orientation, l_Body);
			m_Scene->addActor(*l_Body);
		}
	}
}

void CPhysXManager::CreateStaticTriangleMesh(const std::string _name, const CStaticMesh* _Mesh, const std::string _Directory, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group, bool _FlipNormals)
{
	std::vector<CRenderableVertexs*> l_RenderableVertex = _Mesh->GetRenderableVertexs();
	for (size_t i = 0; i < l_RenderableVertex.size(); i++)
	{

		physx::PxU8* l_Data;
		physx::PxU32 l_Size;

		char l_FileName[256] = "";
		sprintf_s(l_FileName, "%s\\%s_%u.tmesh", _Directory.c_str(), _name.c_str(), i);

		if (LoadMeshFile(l_FileName,&l_Data,&l_Size)){

			physx::PxDefaultMemoryInputData l_InputStream(l_Data, l_Size);
			physx::PxTriangleMesh* l_TriangleMesh = m_PhysX->createTriangleMesh(l_InputStream);

			physx::PxRigidStatic* l_Body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(_position), CastQuat(_orientation)));
			physx::PxShape* l_Shape = l_Body->createShape(physx::PxTriangleMeshGeometry(l_TriangleMesh), *m_Materials[_Material]);

			char l_ActorName[256] = "";
			sprintf_s(l_ActorName, "%s_%u", _name.c_str(), i);

			L_PutGroupToShape(l_Shape, m_Groups[_group]);

			l_Body->userData = (void*)AddActor(l_ActorName, _position, _orientation, l_Body);
			m_Scene->addActor(*l_Body);
		}
	}
}

//physx::PxConvexMesh* L_CreateConvexMesh(std::vector<Vect3f> _vertices, physx::PxCooking* _Cooking, physx::PxPhysics* _PhysX)
//{
//	physx::PxConvexMeshDesc convexDesc;
//	convexDesc.points.count = _vertices.size();
//	convexDesc.points.stride = sizeof(Vect3f);
//	convexDesc.points.data = &_vertices[0];
//	convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
//
//	physx::PxDefaultMemoryOutputStream buff;
//	physx::PxConvexMeshCookingResult::Enum result;
//	bool succes = _Cooking->cookConvexMesh(convexDesc, buff, &result);
//	assert(succes);
//	physx::PxDefaultMemoryInputData input(buff.getData(), buff.getSize());
//	physx::PxConvexMesh* convexMesh = _PhysX->createConvexMesh(input);
//
//	return convexMesh;
//}
//
//void CPhysXManager::CreateComplexDinamicShape(const std::string _name, std::vector<Vect3f> _vertices, const std::string _Material, Vect3f _position, Quatf _orientation,
//	float _density, std::string _group, bool _isKinematic)
//{
//	physx::PxConvexMesh* convexMesh = L_CreateConvexMesh(_vertices, m_Cooking, m_PhysX);
//	CreateDinamicShapeFromBody(_name, physx::PxConvexMeshGeometry(convexMesh), _Material, _position, _orientation, _density, _group, _isKinematic);
//}
//
//void CPhysXManager::CreateComplexStaticShape(const std::string _name, std::vector<Vect3f> _vertices, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group)
//{
//	physx::PxConvexMesh* convexMesh = L_CreateConvexMesh(_vertices, m_Cooking, m_PhysX);
//	CreateStaticShapeFromBody(_name, physx::PxConvexMeshGeometry(convexMesh), _Material, _position, _orientation, _group)->release();
//}

void CPhysXManager::CreateTrigger(const std::string _name, physx::PxShape* shape, Vect3f _position, Quatf _orientation, std::string _group, std::string _OnTriggerEnterLuaFunction, std::string _OnTriggerStayLuaFunction, std::string _OnTriggerExitLuaFunction, std::vector<std::string> _ActiveActors, bool isActive)
{
	L_PutGroupToShape(shape, m_Groups[_group]);

	physx::PxRigidStatic* l_Body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(_position), CastQuat(_orientation)));
	l_Body->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);
	l_Body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	l_Body->attachShape(*shape);

	AddActor(_name, _position, _orientation, l_Body);

	size_t index = m_ActorIndexs[_name];
	m_TriggerIsActive[index] = isActive;
	m_TriggerActivated[index] = "";
	m_OnTriggerEnterLuaFunctions[index] = _OnTriggerEnterLuaFunction;
	m_OnTriggerStayLuaFunctions[index] = _OnTriggerStayLuaFunction;
	m_OnTriggerExitLuaFunctions[index] = _OnTriggerExitLuaFunction;
	m_ActiveActors[index] = _ActiveActors;

	m_Scene->addActor(*l_Body);

	shape->release();
}

void CPhysXManager::EnableTrigger(const std::string _name)
{
	ChangeRigidDynamicActorPhysxGroup(_name, "2");
}

void CPhysXManager::DisableTrigger(const std::string _name)
{
	ChangeRigidDynamicActorPhysxGroup(_name, "9");
}

void CPhysXManager::CreateBoxTrigger(const std::string _name, Vect3f _size, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group, std::string _OnTriggerEnterLuaFunction, std::string _OnTriggerStayLuaFunction, std::string _OnTriggerExitLuaFunction, std::vector<std::string> _ActiveActors,bool isActive)
{	
	physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(_size.x / 2, _size.y / 2, _size.z / 2), *m_Materials[_Material], true);
	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

	CreateTrigger(_name, shape, _position, _orientation, _group, _OnTriggerEnterLuaFunction, _OnTriggerStayLuaFunction, _OnTriggerExitLuaFunction, _ActiveActors, isActive);
}

void CPhysXManager::CreateSphereTrigger(const std::string _name, float _radius, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group, std::string _OnTriggerEnterLuaFunction, std::string _OnTriggerStayLuaFunction, std::string _OnTriggerExitLuaFunction, std::vector<std::string> _ActiveActors, bool isActive)
{
	physx::PxShape* shape = m_PhysX->createShape(physx::PxSphereGeometry(_radius), *m_Materials[_Material], true);
	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

	CreateTrigger(_name, shape, _position, _orientation, _group, _OnTriggerEnterLuaFunction, _OnTriggerStayLuaFunction, _OnTriggerExitLuaFunction, _ActiveActors, isActive);
}


void CPhysXManager::CreateStaticPlane(const std::string _name, Vect3f _PlaneNormal, float _PlaneOffset, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group)
{
	physx::PxMaterial* l_Material = m_Materials[_Material];
	physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*m_PhysX, physx::PxPlane(_PlaneNormal.x, _PlaneNormal.y, _PlaneNormal.z, _PlaneOffset), *l_Material);
	physx::PxShape* shape;

	size_t numShapes = groundPlane->getShapes(&shape,1);
	assert(numShapes == 1);

	L_PutGroupToShape(shape, m_Groups[_group]);
	groundPlane->userData = (void*)AddActor(_name, _position, _orientation, groundPlane);
	shape->userData = groundPlane->userData;

	m_Scene->addActor(*groundPlane);
}

void CPhysXManager::CreateDinamicShape(const std::string _name, const physx::PxGeometry &_geometry, const std::string _Material, Vect3f _position, Quatf _orientation,
	float _density, std::string _group, bool _isKinematic)
{
	physx::PxMaterial* l_Material = m_Materials[_Material];
	physx::PxShape* shape = m_PhysX->createShape(_geometry, *l_Material);
	physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(_position),CastQuat(_orientation)));

	RegisterActor(_name, shape, body, _position, _orientation, _density, _group, _isKinematic);

	shape->release();
}

void CPhysXManager::CreateDynamicBox(const std::string _name, Vect3f _size, const std::string _Material, Vect3f _position, Quatf _orientation,
	float _density, std::string _group, bool isKinematic)
{
	CreateDinamicShape(_name,
		physx::PxBoxGeometry(_size.x / 2, _size.y / 2, _size.z / 2),
		_Material, _position, _orientation, _density, _group, isKinematic);
}

void CPhysXManager::CreateDynamicSphere(const std::string _name, float _radius, const std::string _Material, Vect3f _position, Quatf _orientation,
	float _density, std::string _group, bool isKinematic)
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

physx::PxShape* CPhysXManager::CreateStaticShapeFromBody(const std::string _name, const physx::PxGeometry &_geometry, const std::string _Material, Vect3f _position, Quatf _orientation, std::string _group)
{
	physx::PxMaterial* l_Material = m_Materials[_Material];
	physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(_position), CastQuat(_orientation)));
	physx::PxShape* shape = body->createShape(_geometry, *l_Material);

	RegisterActor(_name, shape, body, _position, _orientation, _group);

	return shape;
}

void CPhysXManager::CreateDinamicShapeFromBody(const std::string _name, const physx::PxGeometry &_geometry, const std::string _Material, Vect3f _position, Quatf _orientation,
	float _density, std::string _group, bool _isKinematic)
{
	physx::PxMaterial* l_Material = m_Materials[_Material];
	physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(_position), CastQuat(_orientation)));
	physx::PxShape* shape = body->createShape(_geometry, *l_Material);

	RegisterActor(_name, shape, body, _position, _orientation, _density, _group, _isKinematic);
}

void CPhysXManager::Update(float _dt)
{
	m_LeftoverSeconds = m_LeftoverSeconds + _dt;
	if(m_LeftoverSeconds >= PHYSX_UPDATE_STEP)
	{
		m_Scene->simulate((physx::PxReal)PHYSX_UPDATE_STEP);
		m_Scene->fetchResults(true);

		physx::PxU32 numActiveTransform;
		const physx::PxActiveTransform* activeTransforms = m_Scene->getActiveTransforms(numActiveTransform);

		for(physx::PxU32 i = 0;i<numActiveTransform; i++)
		{
			uintptr_t index = (uintptr_t)(activeTransforms[i].userData);
			m_ActorPositions[index] = CastVec(activeTransforms[i].actor2World.p);
			m_ActorOrientations[index] = CastQuat(activeTransforms[i].actor2World.q);
		}

		m_LeftoverSeconds = (float)fmod(m_LeftoverSeconds, PHYSX_UPDATE_STEP);
		for (std::map<size_t, bool>::iterator l_iterator = m_TriggerIsActive.begin(); l_iterator != m_TriggerIsActive.end(); l_iterator++)
		{
			int l_index = l_iterator->first;
			if (l_iterator->second)
			{
				if (m_TriggerActivated[l_index]!=std::string(""))
				{
					if (m_OnTriggerStayLuaFunctions[l_index] != std::string(""))
					{
						UABEngine.GetInstance()->GetScriptManager()->RunCode(m_OnTriggerStayLuaFunctions[l_index] + "(\"" + m_ActorNames[l_index] + "\",\"" + m_TriggerActivated[l_index] + "\")");
					}
				}
			}
		}
	}
}

#define MAX_SHAPES_PER_ACTOR 16

void CPhysXManager::CharacterControllerMove(std::string _name, Vect3f _movement, float _elapsedTime)
{	
	static MyQueryFilterCallback s_MyQueryFilterCallback;

	physx::PxController* cct = m_CharacterControllers[_name];
	physx::PxRigidDynamic* actor = cct->getActor();

	physx::PxFilterData filterData;
	filterData.setToDefault();
	filterData.word0 = 0;
	filterData.word1 = 0;

	physx::PxShape* l_CCTShapes[MAX_SHAPES_PER_ACTOR];
	physx::PxU32 l_NShapes = actor->getShapes(l_CCTShapes, MAX_SHAPES_PER_ACTOR);

	for (size_t i = 0; i < l_NShapes; i++)
	{
		filterData.word0 |= l_CCTShapes[i]->getSimulationFilterData().word0;
		filterData.word1 |= l_CCTShapes[i]->getSimulationFilterData().word0 | l_CCTShapes[i]->getSimulationFilterData().word1;
	}

	const physx::PxControllerFilters filters(&filterData, (physx::PxQueryFilterCallback*)&s_MyQueryFilterCallback, nullptr);

	size_t index = (size_t)cct->getUserData();

	Vect3f movemenAux = _movement;
	Vect3f move1 = CastVec(cct->getPosition());
	movemenAux = _elapsedTime*movemenAux;
	cct->move(CastVec(movemenAux), movemenAux.Length()*0.01f, (physx::PxF32)_elapsedTime, filters);

	Vect3f move2 = CastVec(cct->getPosition());

	physx::PxExtendedVec3 p = cct->getFootPosition();
	physx::PxVec3 v = actor->getLinearVelocity();
}

void CPhysXManager::CharacterControllerWarp(std::string _name, Vect3f _movement)
{
	physx::PxController* cct = m_CharacterControllers[_name];

	physx::PxExtendedVec3 p = cct->getFootPosition();
	p.x += _movement.x;
	p.y += _movement.y;
	p.z += _movement.z;

	cct->setFootPosition(p);
	size_t l_index = GetActorIndexFromName(_name);
	m_ActorPositions[l_index] = Vect3f(p.x,p.y,p.z);
}

void CPhysXManager::CharacterControllerTeleport(std::string _name, Vect3f _newPos)
{
	physx::PxController* cct = m_CharacterControllers[_name];

	physx::PxExtendedVec3 p = cct->getFootPosition();
	p.x = _newPos.x;
	p.y = _newPos.y;
	p.z = _newPos.z;

	cct->setFootPosition(p);
	size_t l_index = GetActorIndexFromName(_name);
	m_ActorPositions[l_index] = _newPos;
}


bool CPhysXManager::Raycast(const Vect3f _origin, const Vect3f _end, int _GROUPS,RaycastData* result_)
{
	physx::PxFilterData l_filterData;
	l_filterData.setToDefault();
	l_filterData.word0 = _GROUPS;  // GROUP1 | GROUP2;
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
		if (l_index < m_Actors.size() - 1)
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
			m_Actors[l_index]->userData = (void *)l_index;
		}
		else
		{
			m_Actors.resize(m_Actors.size() - 1);

			m_ActorNames.resize(m_Actors.size());

			m_ActorPositions.resize(m_Actors.size());

			m_ActorOrientations.resize(m_Actors.size());
		}

		m_ActorIndexs.erase(m_ActorIndexs.find(_ActorName));
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
	l_RotationMatrix.SetRotByQuat(Quatf(0, 0, 0, 0));
	
	l_TranslationMatrix.SetIdentity();
	
	l_TranslationMatrix.SetPos((float)m_CharacterControllers[_name]->getPosition().x, (float)m_CharacterControllers[_name]->getPosition().y, (float)m_CharacterControllers[_name]->getPosition().z);

	l_TranslationMatrix = l_ScaleMatrix*l_RotationMatrix*l_TranslationMatrix;

#ifdef _DEBUG
	RenderManager->GetContextManager()->SetWorldMatrix(l_TranslationMatrix);
	CEffectTechnique* l_EffectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_lights")->GetEffectTechnique();
	CEffectManager::SetSceneConstants(l_EffectTechnique);
	RenderManager->GetDebugRender()->GetCone()->RenderIndexed(RenderManager, l_EffectTechnique, CEffectManager::GetRawData());
#endif
}

Vect3f CPhysXManager::GetCharacterControllersPosition(const std::string _name)
{
	physx::PxController* aux = m_CharacterControllers[_name];
	return Vect3f((float)aux->getPosition().x, (float)aux->getPosition().y, (float)aux->getPosition().z);
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

void CPhysXManager::SetCharacterControllersHeight(const std::string _name, float _value)
{
	physx::PxController* aux = m_CharacterControllers[_name];
	aux->resize((physx::PxReal)_value);
}