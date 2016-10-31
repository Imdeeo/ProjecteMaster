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
};

#endif //MUTEX_MANAGER_H