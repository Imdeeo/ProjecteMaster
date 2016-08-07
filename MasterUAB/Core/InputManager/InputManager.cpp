#include <InputManager\InputManager.h>
#include "XML\XMLTreeNode.h"

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
	m_MouseId(nullptr),
	m_GamepadId(nullptr),
	m_Speed(1.f),
	m_AxisX(.0f),
	m_AxisY(.0f),
	m_AxisZ(.0f)
{
	m_Manager = new gainput::InputManager;
	
	m_KeyboardId = new gainput::DeviceId(m_Manager->CreateDevice<gainput::InputDeviceKeyboard>());
	m_MouseId = new gainput::DeviceId(m_Manager->CreateDevice<gainput::InputDeviceMouse>());
	m_GamepadId = new gainput::DeviceId(m_Manager->CreateDevice<gainput::InputDevicePad>());

	gainput::InputDevice::DeviceVariant l_variant = m_Manager->GetDevice(*m_MouseId)->GetVariant();

	m_Map = new gainput::InputMap(*m_Manager, "Keymap");
}

CInputManager::~CInputManager(void)
{
	CHECKED_DELETE(m_GamepadId);
	CHECKED_DELETE(m_MouseId);
	CHECKED_DELETE(m_KeyboardId);
	CHECKED_DELETE(m_Map);
	CHECKED_DELETE(m_Manager);
}

void CInputManager::SetWindow(HWND _hWnd, int _width, int _height)
{
	/*RECT l_Rect;
	GetWindowRect(_hWnd, &l_Rect);
	int l_Width = l_Rect.right - l_Rect.left;
	int l_Heigth = l_Rect.bottom - l_Rect.top;*/
	m_Manager->SetDisplaySize(_width, _height);
}

void CInputManager::Reload()
{
	m_Map->Clear();
	Load(m_Filename);
}

void CInputManager::Update()
{
	m_Manager->Update();
}

void CInputManager::UpdateAxis(LPARAM _param)
{
	DIMOUSESTATE2 l_DIMouseState;

	if (m_Mouse != NULL)
	{
		ZeroMemory(&l_DIMouseState, sizeof(l_DIMouseState));
		HRESULT l_HR = m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &l_DIMouseState);
		if (FAILED(l_HR))
		{
			l_HR = m_Mouse->Acquire();
			while (l_HR == DIERR_INPUTLOST)
				l_HR = m_Mouse->Acquire();
		}
		else
		{
			m_AxisX += l_DIMouseState.lX * m_Speed;// * axis scale
			m_AxisY += l_DIMouseState.lY * m_Speed;// * axis scale
			m_AxisZ += l_DIMouseState.lZ * m_Speed;// * axis scale
		}
	}
}

void CInputManager::Load(std::string _file)
{	
	m_Filename = _file;
	std::string l_Name;
	std::string l_Device;
	std::string l_Key;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(_file.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["input"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("action"))
				{
					l_Name = l_Element.GetPszProperty("name");
					l_Device = l_Element.GetPszProperty("device");
					l_Key = l_Element.GetPszProperty("key");

					if (l_Device == "Keyboard")
						m_Map->MapBool(GetAction(l_Name), *m_KeyboardId, GetInput(l_Key));
					else if (l_Device == "Mouse")
						m_Map->MapBool(GetAction(l_Name), *m_MouseId, GetInput(l_Key));
					else if (l_Device == "Gamepad")
						m_Map->MapBool(GetAction(l_Name), *m_GamepadId, GetInput(l_Key));
					else
						assert("This should not happen!");
				}
				else if (l_Element.GetName() == std::string("axis"))
				{
					l_Name = l_Element.GetPszProperty("name");
					l_Device = l_Element.GetPszProperty("device");
					l_Key = l_Element.GetPszProperty("key");

					if (l_Device == "Keyboard")
						m_Map->MapFloat(GetAction(l_Name), *m_KeyboardId, GetInput(l_Key));
					else if (l_Device == "Mouse")
						m_Map->MapFloat(GetAction(l_Name), *m_MouseId, GetInput(l_Key));
					else if (l_Device == "Gamepad")
						m_Map->MapFloat(GetAction(l_Name), *m_GamepadId, GetInput(l_Key));
					else
						assert("This should not happen!");
				}
			}
		}
	}
	else
	{
		// Mouse Mapping
		m_Map->MapBool(CInputManager::LeftClick, *m_MouseId, gainput::MouseButtonLeft);
		m_Map->MapBool(CInputManager::RightClick, *m_MouseId, gainput::MouseButtonRight);
		m_Map->MapBool(CInputManager::MiddleClick, *m_MouseId, gainput::MouseButtonMiddle);
		m_Map->MapBool(CInputManager::WheelUp, *m_MouseId, gainput::MouseButtonWheelUp);
		m_Map->MapBool(CInputManager::WheelDown, *m_MouseId, gainput::MouseButtonWheelDown);
		m_Map->MapFloat(CInputManager::AxisX, *m_MouseId, gainput::MouseAxisX);
		m_Map->MapFloat(CInputManager::AxisY, *m_MouseId, gainput::MouseAxisY);
		
		// Gamepad Mapping
		m_Map->MapFloat(CInputManager::AxisX, *m_GamepadId, gainput::PadButtonRightStickX);
		m_Map->MapFloat(CInputManager::AxisY, *m_GamepadId, gainput::PadButtonRightStickY);

		// Keyboard Mapping
		m_Map->MapBool(CInputManager::MoveForward, *m_KeyboardId, gainput::KeyF1);
		m_Map->MapBool(CInputManager::MoveBackward, *m_KeyboardId, gainput::KeyF2);
		m_Map->MapBool(CInputManager::StrafeLeft, *m_KeyboardId, gainput::KeyF3);
		m_Map->MapBool(CInputManager::StrafeRight, *m_KeyboardId, gainput::KeyF4);
		m_Map->MapBool(CInputManager::Jump, *m_KeyboardId, gainput::KeyF5);
		m_Map->MapBool(CInputManager::Crouch, *m_KeyboardId, gainput::KeyF6);
		m_Map->MapBool(CInputManager::Run, *m_KeyboardId, gainput::KeyF7);
		m_Map->MapBool(CInputManager::Interact, *m_KeyboardId, gainput::KeyF8);
		m_Map->MapBool(CInputManager::Sing, *m_KeyboardId, gainput::KeyF9);
		m_Map->MapBool(CInputManager::Pause, *m_KeyboardId, gainput::KeyF10);
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
}

int CInputManager::GetAction(std::string _name)
{
	if (_name == "AxisX")
		return CInputManager::AxisX;
	else if (_name == "AxisY")
		return CInputManager::AxisY;
	else if (_name == "LeftClick")
		return CInputManager::LeftClick;
	else if (_name == "RightClick")
		return CInputManager::RightClick;
	else if (_name == "MiddleClick")
		return CInputManager::MiddleClick;
	else if (_name == "WheelUp")
		return CInputManager::WheelUp;
	else if (_name == "WheelDown")
		return CInputManager::WheelDown;
	else if (_name == "MoveForward")
		return CInputManager::MoveForward;
	else if (_name == "MoveBackward")
		return CInputManager::MoveBackward;
	else if (_name == "StrafeLeft")
		return CInputManager::StrafeLeft;
	else if (_name == "StrafeRight")
		return CInputManager::StrafeRight;
	else if (_name == "Jump")
		return CInputManager::Jump;
	else if (_name == "Crouch")
		return CInputManager::Crouch;
	else if (_name == "Run")
		return CInputManager::Run;
	else if (_name == "Interact")
		return CInputManager::Interact;
	else if (_name == "Sing")
		return CInputManager::Sing;
	else if (_name == "Pause")
		return CInputManager::Pause;
#ifdef _DEBUG
	else if (_name == "DebugToggleFrustum")
		return CInputManager::DebugToggleFrustum;
	else if (_name == "DebugSpeedUp")
		return CInputManager::DebugSpeedUp;
	else if (_name == "DebugSpeedDown")
		return CInputManager::DebugSpeedDown;
	else if (_name == "DebugSanityUp")
		return CInputManager::DebugSanityUp;
	else if (_name == "DebugSanityDown")
		return CInputManager::DebugSanityDown;
	else if (_name == "DebugReloadLua")
		return CInputManager::DebugReloadLua;
	else if (_name == "DebugToggleRenderLights")
		return CInputManager::DebugToggleRenderLights;
	else if (_name == "DebugChangeCameraControl")
		return CInputManager::DebugChangeCameraControl;
	else if (_name == "DebugChangeCameraVision")
		return CInputManager::DebugChangeCameraVision;
	else if (_name == "DebugChangeCamera")
		return CInputManager::DebugChangeCamera;
	else if (_name == "DebugToggleRenderCamera")
		return CInputManager::DebugToggleRenderCamera;
	else if (_name == "DebugMusicVolumeUp")
		return CInputManager::DebugMusicVolumeUp;
	else if (_name == "DebugMusicVolumeDown")
		return CInputManager::DebugMusicVolumeDown;
	else if (_name == "DebugFxVolumeUp")
		return CInputManager::DebugFxVolumeUp;
	else if (_name == "DebugFxVolumeDown")
		return CInputManager::DebugFxVolumeDown;
	else if (_name == "DebugMonsterRun")
		return CInputManager::DebugMonsterRun;
	else if (_name == "DebugMonsterIdle")
		return CInputManager::DebugMonsterIdle;
	else if (_name == "DebugMonsterHit")
		return CInputManager::DebugMonsterHit;
#endif
	else{
		assert("This should not happen!");
		return 0;
	}
}

int CInputManager::GetInput(std::string _name)
{
	if (_name == "MouseButtonLeft")
		return gainput::MouseButtonLeft;
	else if (_name == "MouseButtonRight")
		return gainput::MouseButtonRight;
	else if (_name == "MouseButtonMiddle")
		return gainput::MouseButtonMiddle;
	else if (_name == "MouseButtonWheelUp")
		return gainput::MouseButtonWheelUp;
	else if (_name == "MouseButtonWheelDown")
		return gainput::MouseButtonWheelDown;
	else if (_name == "MouseAxisX")
		return gainput::MouseAxisX;
	else if (_name == "MouseAxisY")
		return gainput::MouseAxisY;
	else if (_name == "KeySpace")
		return gainput::KeySpace;
	else if (_name == "KeyA")
		return gainput::KeyA;
	else if (_name == "KeyB")
		return gainput::KeyB;
	else if (_name == "KeyC")
		return gainput::KeyC;
	else if (_name == "KeyD")
		return gainput::KeyD;
	else if (_name == "KeyE")
		return gainput::KeyE;
	else if (_name == "KeyF")
		return gainput::KeyF;
	else if (_name == "KeyG")
		return gainput::KeyG;
	else if (_name == "KeyH")
		return gainput::KeyH;
	else if (_name == "KeyI")
		return gainput::KeyI;
	else if (_name == "KeyJ")
		return gainput::KeyJ;
	else if (_name == "KeyK")
		return gainput::KeyK;
	else if (_name == "KeyL")
		return gainput::KeyL;
	else if (_name == "KeyM")
		return gainput::KeyM;
	else if (_name == "KeyN")
		return gainput::KeyN;
	else if (_name == "KeyO")
		return gainput::KeyO;
	else if (_name == "KeyP")
		return gainput::KeyP;
	else if (_name == "KeyQ")
		return gainput::KeyQ;
	else if (_name == "KeyR")
		return gainput::KeyR;
	else if (_name == "KeyS")
		return gainput::KeyS;
	else if (_name == "KeyT")
		return gainput::KeyT;
	else if (_name == "KeyU")
		return gainput::KeyU;
	else if (_name == "KeyV")
		return gainput::KeyV;
	else if (_name == "KeyW")
		return gainput::KeyW;
	else if (_name == "KeyX")
		return gainput::KeyX;
	else if (_name == "KeyY")
		return gainput::KeyY;
	else if (_name == "KeyZ")
		return gainput::KeyZ;
	else if (_name == "Key0")
		return gainput::Key0;
	else if (_name == "Key1")
		return gainput::Key1;
	else if (_name == "Key2")
		return gainput::Key2;
	else if (_name == "Key3")
		return gainput::Key3;
	else if (_name == "Key4")
		return gainput::Key4;
	else if (_name == "Key5")
		return gainput::Key5;
	else if (_name == "Key6")
		return gainput::Key6;
	else if (_name == "Key7")
		return gainput::Key7;
	else if (_name == "Key8")
		return gainput::Key8;
	else if (_name == "Key9")
		return gainput::Key9;
	else if (_name == "PadButtonRightStickX")
		return gainput::PadButtonRightStickX;
	else if (_name == "PadButtonRightStickY")
		return gainput::PadButtonRightStickY;
	else{
		assert("This should not happen!");
		return 0;
	}
}

void CInputManager::SetFocus(bool _focus)
{
	m_Focus = _focus;
}

bool CInputManager::GetFocus() const
{
	return m_Focus;
}

Vect2f CInputManager::GetCursor()
{
	return Vect2f(
		m_Map->GetFloat(CInputManager::AxisX),
		m_Map->GetFloat(CInputManager::AxisY));
}

Vect2f CInputManager::GetCursorMovement()
{
	m_Map->GetBoolIsNew(CInputManager::DebugMonsterIdle);
	return Vect2f(
		m_Map->GetFloatDelta(CInputManager::AxisX),
		m_Map->GetFloatDelta(CInputManager::AxisY));
}

float CInputManager::GetAxisX()
{
	return m_AxisX;
}

float CInputManager::GetAxisY()
{
	return m_AxisY;
}

bool CInputManager::IsActionActive(std::string _name)
{
	return m_Map->GetBool(GetAction(_name));
}

bool CInputManager::IsActionNew(std::string _name)
{
	return m_Map->GetBoolIsNew(GetAction(_name));
}

bool CInputManager::IsActionReleased(std::string _name)
{
	return m_Map->GetBoolWasDown(GetAction(_name));
}

bool CInputManager::WasActionActive(std::string _name)
{
	return m_Map->GetBoolPrevious(GetAction(_name));
}
