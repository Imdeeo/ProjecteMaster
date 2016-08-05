#if !(defined NDEBUG ^ defined _DEBUG)
#define _DEBUG
#endif

#include "PhysXManager.h"
#include "Base\Math\Vector3.h"
#include "Base\Math\Quatn.h"
#include "Base\Math\MathTypes.h"
#include "Base\Utils\CEmptyPointerClass.h"
#include "StaticMesh\StaticMesh.h"

#include "StaticMesh\VertexTypes.h"
	
#include "StaticMesh\RenderableVertexs.h"

#include "Base\XML\XMLTreeNode.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <assert.h>
#include <direct.h>

#define HEADER 51966
#define FOOTER 65226

#define N_CPUS 2
#define PHYSX_VERSION_VS10 ((3<<24) + (3<<16) + (3<<8) + 0)

#include <PxPhysicsAPI.h>

#define CONTACT_OFFSET			0.01f
//	#define CONTACT_OFFSET			0.1f
//	#define STEP_OFFSET				0.01f
#define STEP_OFFSET				0.05f
//	#define STEP_OFFSET				0.1f
//	#define STEP_OFFSET				0.2f

//	#define SLOPE_LIMIT				0.8f
#define SLOPE_LIMIT				0.0f
//	#define INVISIBLE_WALLS_HEIGHT	6.0f
#define INVISIBLE_WALLS_HEIGHT	0.0f
//	#define MAX_JUMP_HEIGHT			4.0f
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

		m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation,physx::PxTolerancesScale(),true,profileZoneManager);
		assert(m_PhysX);

		m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(physx::PxTolerancesScale()));
		assert(m_Cooking);

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

		CHECKED_RELEASE(m_ControllerManager);
		CHECKED_RELEASE(m_Scene);
		CHECKED_RELEASE(m_Dispatcher);
		physx::PxProfileZoneManager* profileZoneManager = m_PhysX->getProfileZoneManager();


		CHECKED_RELEASE(m_Cooking);
		CHECKED_RELEASE(m_PhysX);
		CHECKED_RELEASE(profileZoneManager);
		CHECKED_RELEASE(m_Foundation);

		CPhysXManager::Destroy();
	}
public:

	CPhysXManagerImplementation(){Init();}
	virtual ~CPhysXManagerImplementation(){Destroy();}

	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count){}
	void onWake(physx::PxActor** actors, physx::PxU32 count){}
	void onSleep(physx::PxActor** actors, physx::PxU32 count){}
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs){}
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {}

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

};

CPhysXManager* CPhysXManager::CreatePhysXManager()
{
	return new CPhysXManagerImplementation();
}


void WriteMeshFile(std::string _Filename, physx::PxU8* _Data, physx::PxU32 _Size)
{
	printf_s("Writing file: %s\n", _Filename.c_str());
	std::fstream l_File(_Filename, std::ios::binary | std::ios::out );
	if (l_File.is_open())
	{
		unsigned short l_BufferUnsignedShort = HEADER;
		l_File.write((char*)&l_BufferUnsignedShort, sizeof(unsigned short));

		l_File.write((char*)&_Size, sizeof(physx::PxU32));
		l_File.write((char*)_Data, _Size);

		l_BufferUnsignedShort = FOOTER;
		l_File.write((char*)&l_BufferUnsignedShort, sizeof(unsigned short));
		l_File.close();
	}
	else
	{
		printf_s("Cannot open file: %s\n", _Filename.c_str());
	}
}

void CPhysXManager::CreateConvexMesh(const std::string &_name, const CStaticMesh* _Mesh)
{
	printf_s("Creating Convex Mesh: %s\n", _name.c_str());
	mkdir(_Mesh->GetPhysxMeshesDirectory().c_str());
	std::vector<CRenderableVertexs*> l_RenderableVertex = _Mesh->GetRenderableVertexs();
	for (size_t i = 0; i < l_RenderableVertex.size(); i++)
	{
		std::vector<Vect3f> l_vertexos(l_RenderableVertex[i]->GetNVertexs());

		physx::PxConvexMeshDesc l_ConvexMeshDesc;
		l_ConvexMeshDesc.setToDefault();
		l_ConvexMeshDesc.points.count = l_RenderableVertex[i]->GetNVertexs();
		if (l_ConvexMeshDesc.points.count < 0)
		{
			l_ConvexMeshDesc.points.stride = sizeof(Vect3f);
			l_ConvexMeshDesc.points.data = (const void*)(&l_RenderableVertex[i]->GetVertexs()[0]);

			l_ConvexMeshDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX | physx::PxConvexFlag::eINFLATE_CONVEX;
			physx::PxDefaultMemoryOutputStream l_DefaultMemoryOutputStream;
			physx::PxConvexMeshCookingResult::Enum l_Result;
			bool success = m_Cooking->cookConvexMesh(l_ConvexMeshDesc, l_DefaultMemoryOutputStream, &l_Result);

			if (success)
			{
				char l_FileName[256] = "";
				sprintf_s(l_FileName, (_Mesh->GetPhysxMeshesDirectory() + std::string("\\%s_%u.cmesh")).c_str(), _name.c_str(), i);

				WriteMeshFile(l_FileName, l_DefaultMemoryOutputStream.getData(), l_DefaultMemoryOutputStream.getSize());
			}
			else
			{
				printf_s("Cannot Create Convex Mesh: %s\n", _name.c_str());
			}
		}
		else
		{
			printf_s("WARNING: The Mesh has no vertexs: %s\n", _name.c_str());
		}
	}
}

void CPhysXManager::CreateTriangleMesh(const std::string &_name, const CStaticMesh* _Mesh, bool _FlipNormals)
{
	printf_s("Creating Triangle Mesh: %s\n",_name.c_str());
	mkdir(_Mesh->GetPhysxMeshesDirectory().c_str());
	std::vector<CRenderableVertexs*> l_RenderableVertex = _Mesh->GetRenderableVertexs();
	for (size_t i = 0; i < l_RenderableVertex.size(); i++)
	{
		physx::PxTriangleMeshDesc l_TriangleMeshDesc;
		l_TriangleMeshDesc.setToDefault();
		l_TriangleMeshDesc.points.count = l_RenderableVertex[i]->GetNVertexs();
		if (l_TriangleMeshDesc.points.count < 0)
		{
			l_TriangleMeshDesc.points.stride = sizeof(Vect3f);
			l_TriangleMeshDesc.points.data = (const void*)(&l_RenderableVertex[i]->GetVertexs()[0]);

			l_TriangleMeshDesc.triangles.count = l_RenderableVertex[i]->GetNIndexs() / 3;
			l_TriangleMeshDesc.triangles.stride = 3 * l_RenderableVertex[i]->GetSizeOfIndexs();
			l_TriangleMeshDesc.triangles.data = l_RenderableVertex[i]->GetIndexs();

			if (l_TriangleMeshDesc.triangles.stride == sizeof(unsigned short)* 3)
				l_TriangleMeshDesc.flags |= physx::PxMeshFlag::e16_BIT_INDICES;
			if (_FlipNormals)
				l_TriangleMeshDesc.flags |= physx::PxMeshFlag::eFLIPNORMALS;

			physx::PxDefaultMemoryOutputStream l_DefaultMemoryOutputStream;
			bool success = m_Cooking->cookTriangleMesh(l_TriangleMeshDesc, l_DefaultMemoryOutputStream);

			if (success)
			{
				char l_FileName[256] = "";
				sprintf_s(l_FileName, (_Mesh->GetPhysxMeshesDirectory() + std::string("\\%s_%u.tmesh")).c_str(), _name.c_str(), i);

				WriteMeshFile(l_FileName, l_DefaultMemoryOutputStream.getData(), l_DefaultMemoryOutputStream.getSize());
			}
			else
			{
				printf_s("Cannot Create Triangle Mesh: %s\n", _name.c_str());
			}
		}
		else
		{
			printf_s("WARNING: The Mesh has no vertexs: %s\n", _name.c_str());
		}

	}
}
