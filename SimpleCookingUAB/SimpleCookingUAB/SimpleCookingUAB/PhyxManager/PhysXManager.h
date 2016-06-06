#ifndef PHYSX_MANAGER_H
#define PHYSX_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include "Base\Math\Vector4.h"
#include "Base\Math\Quatn.h"

class CEmptyPointerClass;
class CStaticMesh;

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
			class PxVisualDebuggerConnectionFlags;
		}
	}

	typedef debugger::comm::PvdConnection PxVisualDebuggerConnection;
	typedef unsigned int PxU32;
}

#define USE_PHYSX_DEBUG 1

#define PHYSX_UPDATE_STEP 0.05

class CPhysXManager
{
protected:
	CPhysXManager(){};
public:
	static CPhysXManager* CreatePhysXManager();
	virtual ~CPhysXManager(){ Destroy(); }

	void CreateConvexMesh(const std::string _name, const CStaticMesh* _Mesh);
	void CreateTriangleMesh(const std::string _name, const CStaticMesh* _Mesh, bool _FlipNormals = false);

protected:
	
	physx::PxFoundation				*m_Foundation;
	physx::PxPhysics				*m_PhysX;

	physx::PxDefaultCpuDispatcher	*m_Dispatcher;
	physx::PxScene					*m_Scene;
	physx::PxCooking				*m_Cooking;
	physx::PxControllerManager		*m_ControllerManager;

	void Destroy()
	{

	}

};

#endif //PHYSX_MANAGER_H