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

#define USE_PHYSX_DEBUG 1

class CPhysXManager
{
protected:
	CPhysXManager();
public:
	static CPhysXManager* CreatePhysXManager();
	virtual ~CPhysXManager();

	void RegisterMaterial(const std::string &name, float staticFriction, float dynamicFriction, float restitution);
	inline void AssertCoordArrays();
	inline size_t GetActorIndexFromName(const std::string& _actorName);

	Vect3f GetActorPosition(const std::string& _actorName);
	Quatf GetActorOrientation(const std::string& _actorName);

	void GetActorPositionAndOrientation(const std::string& _actorName,Vect3f* Pos_, Quatf* Orienation_);

	void CreateStaticShape(Vect3f _size,physx::PxMaterial &_Material,Vect3f _position, Quatf _orientation,size_t* index);
	void CreateStaticPlane(Vect4f _size,physx::PxMaterial &_Material,Vect3f _position, Quatf _orientation,size_t* index);

	void CreateDinamicShape(Vect3f _size,physx::PxMaterial &_Material,Vect3f _position, Quatf _orientation,size_t* index, float _density);

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

	std::map<std::string, size_t>	m_ActorIndexs;
	std::vector<std::string>		m_ActorNames;
	std::vector<Vect3f>				m_ActorPositions;
	std::vector<Quatf>				m_ActorOrientations;
	std::vector<physx::PxActor*>	m_Actors;
	
	void AddActor(std::string _actorName, Vect3f _position, Quatf _orientation, physx::PxActor*);
	void CreateComplexShape();

};