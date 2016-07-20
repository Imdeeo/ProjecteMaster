#include <InputManager\InputManager.h>

class CDeviceButtonListener : public gainput::InputListener
{
public:
	CDeviceButtonListener(gainput::InputManager& m_Manager, int index) : m_Manager_(m_Manager), index_(index) { }
	bool OnDeviceButtonBool(gainput::DeviceId deviceId, gainput::DeviceButtonId deviceButton, bool oldValue, bool newValue) {
		const gainput::InputDevice* device = m_Manager_.GetDevice(deviceId);
		char buttonName[64] = "";
		device->GetButtonName(deviceButton, buttonName, 64);
		return false;
	}
	bool OnDeviceButtonFloat(gainput::DeviceId deviceId, gainput::DeviceButtonId deviceButton, float oldValue, float newValue) {
		const gainput::InputDevice* device = m_Manager_.GetDevice(deviceId);
		char buttonName[64] = "";
		device->GetButtonName(deviceButton, buttonName, 64);
		return true;
	}
	int GetPriority() const { return index_; }

private:
	gainput::InputManager& m_Manager_;
	int index_;
};

class CUserButtonListener : public gainput::MappedInputListener
{
public:
	CUserButtonListener(int index) : index_(index) { }
	bool OnUserButtonBool(gainput::UserButtonId userButton, bool oldValue, bool newValue) { return true; }
	bool OnUserButtonFloat(gainput::UserButtonId userButton, float oldValue, float newValue) { return true; }
	int GetPriority() const { return index_; }

private:
	int index_;
};

CInputManager::CInputManager(HWND _hWnd, int _width, int _height) :
	m_DeviceButtonListener(nullptr),
	m_DeviceButtonListener2(nullptr),
	m_UserButtonListener(nullptr),
	m_UserButtonListener2(nullptr)
{
	RECT l_Rect;
	GetWindowRect(_hWnd, &l_Rect);
	int l_Width = l_Rect.right - l_Rect.left;
	int l_Heigth = l_Rect.bottom - l_Rect.top;	

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

	m_Manager->SetDisplaySize(_width, _height);

	m_DeviceButtonListenerId = m_Manager->AddListener(m_DeviceButtonListener);
	m_Manager->AddListener(m_DeviceButtonListener2);

	m_UserButtonListenerId = m_Map->AddListener(m_UserButtonListener);
	m_Map->AddListener(m_UserButtonListener2);
}

void CInputManager::Update()
{
	m_Manager->Update();

	if (m_Map->GetBoolWasDown(CInputManager::LeftClick))
	{
		UtilsLog("Left click!.\n");
	}

	if (m_Map->GetBoolWasDown(CInputManager::RightClick))
	{
		UtilsLog("Right click!.\n");
	}
}

void CInputManager::LoadLayout(std::string _file)
{
	// Mouse Mapping
	m_Map->MapBool(CInputManager::LeftClick, m_MouseId, gainput::MouseButtonLeft);
	m_Map->MapBool(CInputManager::RightClick, m_MouseId, gainput::MouseButtonRight);
	m_Map->MapBool(CInputManager::MiddleClick, m_MouseId, gainput::MouseButtonMiddle);
	m_Map->MapBool(CInputManager::WheelUp, m_MouseId, gainput::MouseButtonWheelUp);
	m_Map->MapBool(CInputManager::WheelDown, m_MouseId, gainput::MouseButtonWheelDown);
	m_Map->MapFloat(CInputManager::AxisX, m_MouseId, gainput::MouseAxisX);
	m_Map->MapFloat(CInputManager::AxisY, m_MouseId, gainput::MouseAxisY);

	// Keyboard Mapping
	m_Map->MapBool(CInputManager::MoveForward, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::MoveBackward, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::StrafeLeft, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::StrafeRight, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::Jump, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::Crouch, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::Run, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::Interact, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::Pause, m_KeyboardId, gainput::KeyF1);
#ifdef _DEBUG
	m_Map->MapBool(CInputManager::DebugToggleFrustum, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::DebugSpeedUp, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::DebugSpeedDown, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::DebugSanityUp, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::DebugSanityDown, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::DebugReloadLua, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::DebugToggleRenderLights, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::DebugChangeCameraControl, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::DebugChangeCameraVision, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::DebugChangeCamera, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::DebugToggleRenderCamera, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::DebugMusicVolumeUp, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::DebugMusicVolumeDown, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::DebugFxVolumeUp, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::DebugFxVolumeDown, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::DebugMonsterRun, m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::DebugMonsterIdle, m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::DebugMonsterHit, m_KeyboardId, gainput::KeyF2);
#endif
}
