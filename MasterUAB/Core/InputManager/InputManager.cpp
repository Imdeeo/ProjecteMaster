#include <InputManager\InputManager.h>





class MyDeviceButtonListener : public gainput::InputListener
{
public:
	MyDeviceButtonListener(gainput::InputManager& manager, int index) : manager_(manager), index_(index) { }

	bool OnDeviceButtonBool(gainput::DeviceId deviceId, gainput::DeviceButtonId deviceButton, bool oldValue, bool newValue)
	{
		const gainput::InputDevice* device = manager_.GetDevice(deviceId);
		char buttonName[64] = "";
		device->GetButtonName(deviceButton, buttonName, 64);
		UtilsLog("(" + std::to_string(index_) + ") Device " + std::to_string(deviceId) + " (" + device->GetTypeName() + "" + std::to_string(device->GetIndex()) + ") bool button (" + std::to_string(deviceButton) + "/" + buttonName + ") changed: " + std::to_string(oldValue) + " -> " + std::to_string(newValue) + "\n");
		return false;
	}

	bool OnDeviceButtonFloat(gainput::DeviceId deviceId, gainput::DeviceButtonId deviceButton, float oldValue, float newValue)
	{
		const gainput::InputDevice* device = manager_.GetDevice(deviceId);
		char buttonName[64] = "";
		device->GetButtonName(deviceButton, buttonName, 64);
		UtilsLog("(" + std::to_string(index_) + ") Device " + std::to_string(deviceId) + " (" + device->GetTypeName() + "" + std::to_string(device->GetIndex()) + ") float button (" + std::to_string(deviceButton) + "/" + buttonName + ") changed: " + std::to_string(oldValue) + " -> " + std::to_string(newValue) + "\n");
		return true;
	}

	int GetPriority() const
	{
		return index_;
	}

private:
	gainput::InputManager& manager_;
	int index_;
};

class MyUserButtonListener : public gainput::MappedInputListener
{
public:
	MyUserButtonListener(int index) : index_(index) { }

	bool OnUserButtonBool(gainput::UserButtonId userButton, bool oldValue, bool newValue)
	{
		UtilsLog("(" + std::to_string(index_) + ") User bool button " + std::to_string(userButton) + " changed: " + std::to_string(oldValue) + " -> " + std::to_string(newValue) + "\n");
		return true;
	}

	bool OnUserButtonFloat(gainput::UserButtonId userButton, float oldValue, float newValue)
	{
		UtilsLog("(" + std::to_string(index_) + ") User float button " + std::to_string(userButton) + " changed: " + std::to_string(oldValue) + " -> " + std::to_string(newValue) + "\n");
		return true;
	}

	int GetPriority() const
	{
		return index_;
	}

private:
	int index_;
};


void SampleMain()
{
	SfwOpenWindow("Gainput: Listener sample");

	gainput::InputManager manager;

	manager.CreateDevice<gainput::InputDeviceKeyboard>(gainput::InputDevice::DV_RAW);
	const gainput::DeviceId keyboardId = manager.CreateDevice<gainput::InputDeviceKeyboard>();
	manager.CreateDevice<gainput::InputDeviceMouse>(gainput::InputDevice::DV_RAW);
	const gainput::DeviceId mouseId = manager.CreateDevice<gainput::InputDeviceMouse>();
	manager.CreateDevice<gainput::InputDevicePad>();
	manager.CreateDevice<gainput::InputDevicePad>();
	manager.CreateDevice<gainput::InputDeviceTouch>();

#if defined(GAINPUT_PLATFORM_LINUX) || defined(GAINPUT_PLATFORM_WIN)
	manager.SetDisplaySize(SfwGetWidth(), SfwGetHeight());
#endif

	SfwSetInputManager(&manager);
	

	gainput::InputMap map(manager, "testmap");

	map.MapBool(CInputManager::ButtonToggleListener, keyboardId, gainput::KeyF1);
	map.MapBool(CInputManager::ButtonToggleMapListener, keyboardId, gainput::KeyF2);
	map.MapBool(CInputManager::ButtonConfirm, mouseId, gainput::MouseButtonLeft);
	map.MapFloat(CInputManager::ButtonMouseX, mouseId, gainput::MouseAxisX);

	MyDeviceButtonListener myDeviceButtonListener(manager, 1);
	gainput::ListenerId myDeviceButtonListenerId = manager.AddListener(&myDeviceButtonListener);
	MyDeviceButtonListener myDeviceButtonListener2(manager, 2);
	manager.AddListener(&myDeviceButtonListener2);

	MyUserButtonListener myUserButtonListener(2);
	gainput::ListenerId myUserButtonListenerId = map.AddListener(&myUserButtonListener);
	MyUserButtonListener myUserButtonListener2(1);
	map.AddListener(&myUserButtonListener2);

	bool doExit = false;

	while (!SfwIsDone() && !doExit)
	{
		manager.Update();

		MSG msg;
		while (PeekMessage(&msg, SfwGetHWnd(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			manager.HandleMessage(msg);
		}

		if (map.GetBoolWasDown(CInputManager::ButtonToggleListener))
		{
			if (myDeviceButtonListenerId != gainput::ListenerId(-1))
			{
				manager.RemoveListener(myDeviceButtonListenerId);
				myDeviceButtonListenerId = gainput::ListenerId(-1);
				UtilsLog("Device button listener disabled.\n");
			}
			else
			{
				myDeviceButtonListenerId = manager.AddListener(&myDeviceButtonListener);
				UtilsLog("Device button listener enabled.\n");
			}
		}

		if (map.GetBoolWasDown(CInputManager::ButtonToggleMapListener))
		{
			if (myUserButtonListenerId != gainput::ListenerId(-1))
			{
				map.RemoveListener(myUserButtonListenerId);
				myUserButtonListenerId = gainput::ListenerId(-1);
				UtilsLog("User button listener disabled.\n");
			}
			else
			{
				myUserButtonListenerId = map.AddListener(&myUserButtonListener);
				UtilsLog("User button listener enabled.\n");
			}
		}
	}

	SfwCloseWindow();
}

