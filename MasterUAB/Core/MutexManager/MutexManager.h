#ifndef MUTEX_MANAGER_H
#define MUTEX_MANAGER_H

#include <mutex>

class CMutexManager
{
public:
	CMutexManager();
	virtual ~CMutexManager();

	std::mutex g_DeviceMutex;
	std::mutex g_DeviceContextMutex;
	std::mutex g_PhysXMutex;
	std::mutex g_PhysXActorMutex;
	std::mutex g_PhysXSceneMutex;

	std::mutex g_LuaMutex;
};

#endif //MUTEX_MANAGER_H