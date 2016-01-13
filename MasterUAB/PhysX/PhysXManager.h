namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxCooking;
	class PxMaterial;
	class Pxactor;
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

#define USE_PHYSX_DEBUG 0

class CPhysXManager
{
protected:
	CPhysXManager();
public:
	static CPhysXManager* CreatePhysXManager();
	virtual ~CPhysXManager();

protected:

	physx::PxFoundation				*m_Foundation;
	physx::PxPhysics				*m_Physics;

#if USE_PHYSX_DEBUG
	physx::PxVisualDebuggerConnection *m_DebugConnection;
#endif

	physx::PxDefaultCpuDispatcher	*m_Dispatcher;
	physx::PxScene					*m_Scene;
	physx::PxCooking				*m_Cooking;
	physx::PxControllerManager		*m_ControllerManager;

};