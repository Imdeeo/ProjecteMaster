#include <InputManager\InputManager.h>

class CDeviceButtonListener : public gainput::InputListener
{
public:
	CDeviceButtonListener(gainput::InputManager& m_Manager, int index) : m_Manager_(m_Manager), index_(index) { }

	bool OnDeviceButtonBool(gainput::DeviceId deviceId, gainput::DeviceButtonId deviceButton, bool oldValue, bool newValue)
	{
		const gainput::InputDevice* device = m_Manager_.GetDevice(deviceId);
		char buttonName[64] = "";
		device->GetButtonName(deviceButton, buttonName, 64);
		UtilsLog("(" + std::to_string(index_) + ") Device " + std::to_string(deviceId) + " (" + device->GetTypeName() + "" + std::to_string(device->GetIndex()) + ") bool button (" + std::to_string(deviceButton) + "/" + buttonName + ") changed: " + std::to_string(oldValue) + " -> " + std::to_string(newValue) + "\n");
		return false;
	}

	bool OnDeviceButtonFloat(gainput::DeviceId deviceId, gainput::DeviceButtonId deviceButton, float oldValue, float newValue)
	{
		const gainput::InputDevice* device = m_Manager_.GetDevice(deviceId);
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
	gainput::InputManager& m_Manager_;
	int index_;
};

class CUserButtonListener : public gainput::MappedInputListener
{
public:
	CUserButtonListener(int index) : index_(index) { }

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

CInputManager::CInputManager(HWND _hWnd, int _width, int _height) :
	m_Manager(nullptr),
	m_Map(nullptr),
	m_DeviceButtonListener(nullptr),
	m_DeviceButtonListener2(nullptr),
	m_UserButtonListener(nullptr),
	m_UserButtonListener2(nullptr)
{
	gainput::InputManager l_Manager;
	m_Manager = &l_Manager;
	gainput::InputMap l_Map(l_Manager, "Keymap");
	m_Map = &l_Map;

	CDeviceButtonListener l_DeviceButtonListener(l_Manager, 1);
	m_DeviceButtonListener = &l_DeviceButtonListener;
	CDeviceButtonListener l_DeviceButtonListener2(l_Manager, 2);
	m_DeviceButtonListener2 = &l_DeviceButtonListener2;

	CUserButtonListener l_UserButtonListener(2);
	m_UserButtonListener = &l_UserButtonListener;
	CUserButtonListener l_UserButtonListener2(1);
	m_UserButtonListener2 = &l_UserButtonListener2;

	m_Manager->CreateDevice<gainput::InputDeviceKeyboard>(gainput::InputDevice::DV_RAW);
	m_KeyboardId = m_Manager->CreateDevice<gainput::InputDeviceKeyboard>();
	m_Manager->CreateDevice<gainput::InputDeviceMouse>(gainput::InputDevice::DV_RAW);
	m_MouseId = m_Manager->CreateDevice<gainput::InputDeviceMouse>();
	//m_Manager.CreateDevice<gainput::InputDevicePad>();
	//m_Manager.CreateDevice<gainput::InputDevicePad>();
	//m_Manager.CreateDevice<gainput::InputDeviceTouch>();

	m_Manager->SetDisplaySize(_width, _height);

	m_Map->MapBool(CInputManager::ButtonToggleListener, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::ButtonToggleMapListener, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::ButtonConfirm, m_MouseId, gainput::MouseButtonLeft);
	m_Map->MapFloat(CInputManager::ButtonMouseX, m_MouseId, gainput::MouseAxisX);

	m_DeviceButtonListenerId = m_Manager->AddListener(m_DeviceButtonListener);
	m_Manager->AddListener(m_DeviceButtonListener2);

	m_UserButtonListenerId = m_Map->AddListener(m_UserButtonListener);
	m_Map->AddListener(m_UserButtonListener2);
}

void CInputManager::Update()
{
	m_Manager->Update();

	if (m_Map->GetBoolWasDown(CInputManager::ButtonToggleListener))
	{
		if (m_DeviceButtonListenerId != gainput::ListenerId(-1))
		{
			m_Manager->RemoveListener(m_DeviceButtonListenerId);
			m_DeviceButtonListenerId = gainput::ListenerId(-1);
			UtilsLog("Device button listener disabled.\n");
		}
		else
		{
			m_DeviceButtonListenerId = m_Manager->AddListener(m_DeviceButtonListener);
			UtilsLog("Device button listener enabled.\n");
		}
	}

	if (m_Map->GetBoolWasDown(CInputManager::ButtonToggleMapListener))
	{
		if (m_UserButtonListenerId != gainput::ListenerId(-1))
		{
			m_Map->RemoveListener(m_UserButtonListenerId);
			m_UserButtonListenerId = gainput::ListenerId(-1);
			UtilsLog("User button listener disabled.\n");
		}
		else
		{
			m_UserButtonListenerId = m_Map->AddListener(m_UserButtonListener);
			UtilsLog("User button listener enabled.\n");
		}
	}
}


