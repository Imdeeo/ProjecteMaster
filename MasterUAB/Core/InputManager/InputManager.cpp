#include <InputManager\InputManager.h>
#include "XML\tinyxml2.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

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
	m_KeyBoard = new CKeyboardInput();
	m_Map = new gainput::InputMap(*m_Manager, "Keymap");	
}

CInputManager::~CInputManager(void)
{
	CHECKED_DELETE(m_GamepadId);
	CHECKED_DELETE(m_MouseId);
	CHECKED_DELETE(m_KeyboardId);
	CHECKED_DELETE(m_Map);
	CHECKED_DELETE(m_Manager);
	CHECKED_DELETE(m_KeyBoard);
}

void CInputManager::SetWindow(HWND _hWnd, int _width, int _height)
{
	/*RECT l_Rect;
	GetWindowRect(_hWnd, &l_Rect);
	int l_Width = l_Rect.right - l_Rect.left;
	int l_Heigth = l_Rect.bottom - l_Rect.top;*/
	RAWINPUTDEVICE l_RID[1];
	l_RID[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	l_RID[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	l_RID[0].dwFlags = RIDEV_INPUTSINK;
	l_RID[0].hwndTarget = _hWnd;
	RegisterRawInputDevices(l_RID, 1, sizeof(l_RID[0]));
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

void CInputManager::Load(std::string _file)
{	
	m_Filename = _file;
	std::string l_Name;
	std::string l_Device;
	std::string l_Key;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(_file.c_str());

	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		tinyxml2::XMLElement* l_Element;
		l_Element = doc.FirstChildElement("input");

		l_Element = l_Element->FirstChildElement();
		while (l_Element != NULL)
		{		
			if (l_Element->Name() == std::string("action"))
			{
				l_Name = l_Element->GetPszProperty("name");
				l_Device = l_Element->GetPszProperty("device");
				l_Key = l_Element->GetPszProperty("key");

				if (l_Device == "Keyboard")
					m_Map->MapBool(GetAction(l_Name), *m_KeyboardId, GetInput(l_Key));
				else if (l_Device == "Mouse")
					m_Map->MapBool(GetAction(l_Name), *m_MouseId, GetInput(l_Key));
				else if (l_Device == "Gamepad")
					m_Map->MapBool(GetAction(l_Name), *m_GamepadId, GetInput(l_Key));
				else
					assert("This should not happen!");
			}
			else if (l_Element->Name() == std::string("axis"))
			{
				l_Name = l_Element->GetPszProperty("name");
				l_Device = l_Element->GetPszProperty("device");
				l_Key = l_Element->GetPszProperty("key");

				if (l_Device == "Keyboard")
					m_Map->MapFloat(GetAction(l_Name), *m_KeyboardId, GetInput(l_Key));
				else if (l_Device == "Mouse")
					m_Map->MapFloat(GetAction(l_Name), *m_MouseId, GetInput(l_Key));
				else if (l_Device == "Gamepad")
					m_Map->MapFloat(GetAction(l_Name), *m_GamepadId, GetInput(l_Key));
				else
					assert("This should not happen!");
			}
			l_Element = l_Element->NextSiblingElement();
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
		m_Map->MapBool(CInputManager::Mouse3, *m_MouseId, gainput::MouseButton3);
		m_Map->MapBool(CInputManager::Mouse4 , *m_MouseId, gainput::MouseButton4);
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
		m_Map->MapBool(CInputManager::DebugConsole, *m_KeyboardId, gainput::Key7);
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
		m_Map->MapBool(CInputManager::DebugToggleRenderAStar, *m_KeyboardId, gainput::KeyO);
		m_Map->MapBool(CInputManager::DebugToggleLoadVideo, *m_KeyboardId, gainput::KeyI);
		//m_Map->MapBool(CInputManager::DebugStopAllSounds, *m_KeyboardId, gainput::KeyF9);
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
	else if (_name == "Mouse3")
		return CInputManager::Mouse3;
	else if (_name == "Mouse4")
		return CInputManager::Mouse4;
	else if (_name == "Mouse5")
		return CInputManager::Mouse5;
	else if (_name == "Mouse6")
		return CInputManager::Mouse6;
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
	else if (_name == "DebugToggleRenderAStar")
		return CInputManager::DebugToggleRenderAStar;
	else if (_name == "DebugToggleLoadVideo")
		return CInputManager::DebugToggleLoadVideo;
	else if (_name == "DebugConsole")
		return CInputManager::DebugConsole;
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
	/*else if (_name == "DebugStopAllSounds")
		return CInputManager::DebugStopAllSounds;*/

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
	else if (_name == "MouseButton0")
		return gainput::MouseButton0;
	else if (_name == "MouseButton1")
		return gainput::MouseButton1;
	else if (_name == "MouseButton2")
		return gainput::MouseButton2;
	else if (_name == "MouseButton3")
		return gainput::MouseButton3;
	else if (_name == "MouseButton4")
		return gainput::MouseButton4;
	else if (_name == "MouseButton5")
		return gainput::MouseButton5;
	else if (_name == "MouseButton6")
		return gainput::MouseButton6;
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
	else if (_name == "KeyComma")
		return gainput::KeyComma;
	else if (_name == "KeyPeriod")
		return gainput::KeyPeriod;
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

void CInputManager::UpdateAxis(LONG _x, LONG _y)
{
	m_AxisX = (float)_x;
	m_AxisY = (float)_y;
}

Vect2f CInputManager::GetCursor()
{
	return Vect2f(
		m_Map->GetFloat(CInputManager::AxisX),
		m_Map->GetFloat(CInputManager::AxisY));
}

Vect2f CInputManager::GetCursorMovement()
{
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
