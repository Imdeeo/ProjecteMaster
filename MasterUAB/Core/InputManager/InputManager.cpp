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

CInputManager::CInputManager() :
	m_Manager(nullptr),
	m_Map(nullptr),
	m_KeyboardId(nullptr),
	m_MouseId(nullptr)
{
	m_Manager = new gainput::InputManager;
	
	m_KeyboardId = new gainput::DeviceId(m_Manager->CreateDevice<gainput::InputDeviceKeyboard>());
	m_MouseId = new gainput::DeviceId(m_Manager->CreateDevice<gainput::InputDeviceMouse>());

	m_Map = new gainput::InputMap(*m_Manager, "Keymap");
}

void CInputManager::SetWindow(HWND _hWnd, int _width, int _height)
{
	/*RECT l_Rect;
	GetWindowRect(_hWnd, &l_Rect);
	int l_Width = l_Rect.right - l_Rect.left;
	int l_Heigth = l_Rect.bottom - l_Rect.top;*/
	m_Manager->SetDisplaySize(_width, _height);
}

void CInputManager::Update()
{
	m_Manager->Update();
}

void CInputManager::LoadLayout(std::string _file)
{	
	// Mouse Mapping
	m_Map->MapBool(CInputManager::LeftClick, *m_MouseId, gainput::MouseButtonLeft);
	m_Map->MapBool(CInputManager::RightClick, *m_MouseId, gainput::MouseButtonRight);
	m_Map->MapBool(CInputManager::MiddleClick, *m_MouseId, gainput::MouseButtonMiddle);
	m_Map->MapBool(CInputManager::WheelUp, *m_MouseId, gainput::MouseButtonWheelUp);
	m_Map->MapBool(CInputManager::WheelDown, *m_MouseId, gainput::MouseButtonWheelDown);
	m_Map->MapFloat(CInputManager::AxisX, *m_MouseId, gainput::MouseAxisX);
	m_Map->MapFloat(CInputManager::AxisY, *m_MouseId, gainput::MouseAxisY);

	// Keyboard Mapping
	m_Map->MapBool(CInputManager::MoveForward, *m_KeyboardId, gainput::KeyF1);
	m_Map->MapBool(CInputManager::MoveBackward, *m_KeyboardId, gainput::KeyF2);
	m_Map->MapBool(CInputManager::StrafeLeft, *m_KeyboardId, gainput::KeyF3);
	m_Map->MapBool(CInputManager::StrafeRight, *m_KeyboardId, gainput::KeyF4);
	m_Map->MapBool(CInputManager::Jump, *m_KeyboardId, gainput::KeyF5);
	m_Map->MapBool(CInputManager::Crouch, *m_KeyboardId, gainput::KeyF6);
	m_Map->MapBool(CInputManager::Run, *m_KeyboardId, gainput::KeyF7);
	m_Map->MapBool(CInputManager::Interact, *m_KeyboardId, gainput::KeyF8);
	m_Map->MapBool(CInputManager::Pause, *m_KeyboardId, gainput::KeyF9);
#ifdef _DEBUG
	m_Map->MapBool(CInputManager::DebugToggleFrustum, *m_KeyboardId, gainput::Key0);
	m_Map->MapBool(CInputManager::DebugSpeedUp, *m_KeyboardId, gainput::Key1);
	m_Map->MapBool(CInputManager::DebugSpeedDown, *m_KeyboardId, gainput::Key2);
	m_Map->MapBool(CInputManager::DebugSanityUp, *m_KeyboardId, gainput::Key3);
	m_Map->MapBool(CInputManager::DebugSanityDown, *m_KeyboardId, gainput::Key4);
	m_Map->MapBool(CInputManager::DebugReloadLua, *m_KeyboardId, gainput::Key5);
	m_Map->MapBool(CInputManager::DebugToggleRenderLights, *m_KeyboardId, gainput::Key6);
	m_Map->MapBool(CInputManager::DebugChangeCameraControl, *m_KeyboardId, gainput::Key7);
	m_Map->MapBool(CInputManager::DebugChangeCameraVision, *m_KeyboardId, gainput::Key8);
	m_Map->MapBool(CInputManager::DebugChangeCamera, *m_KeyboardId, gainput::Key9);
	m_Map->MapBool(CInputManager::DebugToggleRenderCamera, *m_KeyboardId, gainput::KeyF10);
	m_Map->MapBool(CInputManager::DebugMusicVolumeUp, *m_KeyboardId, gainput::KeyF11);
	m_Map->MapBool(CInputManager::DebugMusicVolumeDown, *m_KeyboardId, gainput::KeyF12);
	m_Map->MapBool(CInputManager::DebugFxVolumeUp, *m_KeyboardId, gainput::KeyA);
	m_Map->MapBool(CInputManager::DebugFxVolumeDown, *m_KeyboardId, gainput::KeyN);
	m_Map->MapBool(CInputManager::DebugMonsterRun, *m_KeyboardId, gainput::KeyB);
	m_Map->MapBool(CInputManager::DebugMonsterIdle, *m_KeyboardId, gainput::KeyC);
	m_Map->MapBool(CInputManager::DebugMonsterHit, *m_KeyboardId, gainput::KeyD);
#endif
}
