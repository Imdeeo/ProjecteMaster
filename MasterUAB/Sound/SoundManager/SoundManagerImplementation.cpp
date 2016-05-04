#include "SoundManagerImplementation.h"
#include "Math\Quatn.h"
#include "Camera\Camera.h"
#include "XML\XMLTreeNode.h"
#include "3DElement\3DElement.h"


// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST	be defined by the game developer.
namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}
	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for mare details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
		)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
		)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

CSoundManagerImplementation::CSoundManagerImplementation()
:m_LastGameObjectID(0)
{

}

CSoundManagerImplementation::~CSoundManagerImplementation()
{
	Terminate();
	m_FreeObjectsIDs.clear();
	m_NamedSpeakers.clear();
	m_GameObjectSpeakers.clear();
}

bool CSoundManagerImplementation::Init() {
	// Initialize audio engine
	// Memory.
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	// Streaming.
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	AkInitSettings l_InitSettings;
	AkPlatformInitSettings l_platInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(l_InitSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(l_platInitSettings);

	// Setting pool sizes for this game.
	// These sizes are tuned for this game, every game should determine its own optimal values.
	l_InitSettings.uDefaultPoolSize = 64 * 1024 * 1024;
	l_InitSettings.uMaxNumPaths = 16;
	l_InitSettings.uMaxNumTransitions = 128;

	l_platInitSettings.uLEngineDefaultPoolSize = 64 * 1024 * 1024;

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	//AKRESULT eResult = AK::SoundEngine::Init(&l_InitSettings, &l_platInitSettings);

	AKRESULT eResult = AK::SOUNDENGINE_DLL::Init(
		&memSettings,
		&stmSettings,
		&deviceSettings,
		&l_InitSettings,
		&l_platInitSettings,
		&musicInit);
	if (eResult != AK_Success)
	{
		// Then, we will run the game without sound
		AK::SOUNDENGINE_DLL::Term();
		//LOGGER->AddNewLog(ELL_ERROR, "CSoundManager::Init->No se ha podido inicializar el motor de audio.");
		//throw CException(__FILE__, __LINE__, "CSoundManager::Init->No se ha podido inicializar el motor de audio.");
		return false;
	}

	m_DefaultSpeakerId = GenerateObjectID();
	AK::SoundEngine::RegisterGameObj(m_DefaultSpeakerId);

	return true;
}

AkGameObjectID CSoundManagerImplementation::GenerateObjectID()
{
	AkGameObjectID result;
	if (m_FreeObjectsIDs.size() > 0)
	{
		result = m_FreeObjectsIDs.back();
		m_FreeObjectsIDs.pop_back();
	}
	else
	{
		result = ++m_LastGameObjectID;
	}
	return result;
}

void CSoundManagerImplementation::RegisterSpeaker(const C3DElement* _speaker)
{
	assert(m_GameObjectSpeakers.find(_speaker) == m_GameObjectSpeakers.end());

	AkGameObjectID id = GenerateObjectID();
	m_GameObjectSpeakers[_speaker] = id;

	Vect3f l_Position = _speaker->GetPosition();
	Quatf l_Rotation = _speaker->GetRotation();
	Vect3f l_Orientation = l_Rotation.GetForwardVector();

	AkSoundPosition l_SoundPosition = {};

	l_SoundPosition.Position.X = l_Position.x;
	l_SoundPosition.Position.Y = l_Position.y;
	l_SoundPosition.Position.Z = l_Position.z;

	l_SoundPosition.Orientation.X = l_Orientation.x;
	l_SoundPosition.Orientation.Y = l_Orientation.y;
	l_SoundPosition.Orientation.Z = l_Orientation.z;

	AK::SoundEngine::RegisterGameObj(id);
	AK::SoundEngine::SetPosition(id, l_SoundPosition);
}

void CSoundManagerImplementation::UnregisterSpeaker(const C3DElement* _speaker)
{
	auto it = m_GameObjectSpeakers.find(_speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		AK::SoundEngine::UnregisterGameObj(it->second);
		m_FreeObjectsIDs.push_back(it->second);
		m_GameObjectSpeakers.erase(it);
	}
	else
	{
		assert(false);
	}
}

bool CSoundManagerImplementation::Load(const std::string &soundbanks_filename, const std::string &speakers_filename)
{
	m_SoundBanksFilename = soundbanks_filename;
	m_SpeakersFilename = speakers_filename;

	bool l_IsOk = true;
	l_IsOk = LoadSoundBanksXML();
	l_IsOk &= LoadSpeakersXML();

	return l_IsOk;
}

bool CSoundManagerImplementation::Reload()
{
	Clean();

	bool l_IsOk = true;
	l_IsOk = LoadSoundBanksXML();
	l_IsOk &= LoadSpeakersXML();

	return l_IsOk;
}

bool CSoundManagerImplementation::InitBanks()
{
	// load initialization and main soundbanks
	AkOSChar *path;
	AKRESULT retValue;
	CONVERT_CHAR_TO_OSCHAR(m_Path.c_str(), path);
	retValue = AK::SOUNDENGINE_DLL::SetBasePath(path);
	retValue = AK::StreamMgr::SetCurrentLanguage(L"English(US)");

	AkBankID bankID;
	retValue = AK::SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID);
	if (retValue != AK_Success)
	{
		return false;
	}
	return true;
}

bool CSoundManagerImplementation::LoadSoundBank(const std::string &bank)
{
	AkBankID bankID;
	AKRESULT retValue;

	retValue = AK::SoundEngine::LoadBank(bank.c_str(), AK_DEFAULT_POOL_ID, bankID);

	if (retValue != AK_Success)
	{
		return false;
	}
	return true;
}

bool CSoundManagerImplementation::UnloadSoundBank(const std::string &bank)
{
	//AkBankID bankID;
	AKRESULT retValue;

	retValue = AK::SoundEngine::UnloadBank(bank.c_str(), nullptr);
	if (retValue != AK_Success)
	{
		return false;
	}
	return true;
}

bool CSoundManagerImplementation::LoadSoundBanksXML()
{
	InitBanks();

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile((m_Path + m_SoundBanksFilename).c_str()))
	{
		CXMLTreeNode l_SoundBanks = l_XML["SoundBanks"];
		if (l_SoundBanks.Exists())
		{
			for (int i = 0; i < l_SoundBanks.GetNumChildren(); ++i)
			{
				std::string l_Name = l_SoundBanks(i).GetPszProperty("name", "");
				LoadSoundBank(l_Name);
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool CSoundManagerImplementation::LoadSpeakersXML()
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile((m_Path + m_SpeakersFilename).c_str()))
	{
		CXMLTreeNode l_Speakers = l_XML["Speakers"];
		if (l_Speakers.Exists())
		{
			for (int i = 0; i < l_Speakers.GetNumChildren(); ++i)
			{
				std::string l_Name = l_Speakers(i).GetPszProperty("name", "");
				Vect3f l_Position = l_Speakers(i).GetVect3fProperty("position", Vect3f(0.f, 0.f, 0.f));
				Vect3f l_Orientation = l_Speakers(i).GetVect3fProperty("orientation", Vect3f(0.f, 0.f, 0.f));

				AkSoundPosition l_SoundPosition = {};

				l_SoundPosition.Position.X = l_Position.x;
				l_SoundPosition.Position.Y = l_Position.y;
				l_SoundPosition.Position.Z = l_Position.z;

				l_SoundPosition.Orientation.X = l_Orientation.x;
				l_SoundPosition.Orientation.Y = l_Orientation.y;
				l_SoundPosition.Orientation.Z = l_Orientation.z;

				AkGameObjectID id = GenerateObjectID();
				m_NamedSpeakers[l_Name] = id;
				AK::SoundEngine::RegisterGameObj(id);
				AK::SoundEngine::SetPosition(id, l_SoundPosition);
			}
		}
	}
	else
	{
		return false;
	}
	return true;

}

void CSoundManagerImplementation::Terminate()
{
	AK::SoundEngine::ClearBanks();  // Limpiamos cualquier bank
	AK::SoundEngine::UnregisterAllGameObj();

	AK::SOUNDENGINE_DLL::Term();
}

void CSoundManagerImplementation::Clean()
{
	AK::SoundEngine::ClearBanks();

	for (auto it : m_NamedSpeakers)
	{
		AK::SoundEngine::UnregisterGameObj(it.second);
		m_FreeObjectsIDs.push_back(it.second);
	}
	m_NamedSpeakers.clear();
}

void CSoundManagerImplementation::Update(const CCamera *camera)
{
	for (auto it : m_GameObjectSpeakers)
	{
		Vect3f l_Position = it.first->GetPosition();
		Quatf l_Rotation = it.first->GetRotation();
		Vect3f l_Orientation = l_Rotation.GetForwardVector();

		AkSoundPosition l_SoundPosition = {};

		l_SoundPosition.Position.X = l_Position.x;
		l_SoundPosition.Position.Y = l_Position.y;
		l_SoundPosition.Position.Z = l_Position.z;

		l_SoundPosition.Orientation.X = l_Orientation.x;
		l_SoundPosition.Orientation.Y = l_Orientation.y;
		l_SoundPosition.Orientation.Z = l_Orientation.z;

		AK::SoundEngine::SetPosition(it.second, l_SoundPosition);
	}
	SetListenerPosition(camera);
	AK::SOUNDENGINE_DLL::Tick();
}

void CSoundManagerImplementation::SetListenerPosition(const CCamera *camera)
{
	Vect3f l_Position = camera->GetPosition();
	Vect3f l_Orientation = camera->GetLookAt();
	Vect3f l_VectorUp = camera->GetUp();

	AkListenerPosition l_ListenerPosition = {};

	l_ListenerPosition.Position.X = l_Position.x;
	l_ListenerPosition.Position.Y = l_Position.y;
	l_ListenerPosition.Position.Z = l_Position.z;

	l_ListenerPosition.OrientationFront.X = l_Orientation.x;
	l_ListenerPosition.OrientationFront.Y = l_Orientation.y;
	l_ListenerPosition.OrientationFront.Z = l_Orientation.z;

	l_ListenerPosition.OrientationTop.X = l_VectorUp.x;
	l_ListenerPosition.OrientationTop.Y = l_VectorUp.y;
	l_ListenerPosition.OrientationTop.Z = l_VectorUp.z;

	AK::SoundEngine::SetListenerPosition(l_ListenerPosition);
}

void CSoundManagerImplementation::PlayEvent(const SoundEvent &_event, const AkGameObjectID &id)
{
	AK::SoundEngine::PostEvent(_event.eventName.c_str(), id);
}

void CSoundManagerImplementation::PlayEvent(const SoundEvent &_event)
{
	PlayEvent(_event, m_DefaultSpeakerId);
}

void CSoundManagerImplementation::PlayEvent(const SoundEvent &_event, const std::string &_speaker)
{
	auto it = m_NamedSpeakers.find(_speaker);
	if (it != m_NamedSpeakers.end())
	{
		PlayEvent(_event, it->second);
	}
	else
	{
		assert(false);
	}
}

void CSoundManagerImplementation::PlayEvent(const SoundEvent &_event, const C3DElement* _speaker)
{
	auto it = m_GameObjectSpeakers.find(_speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		PlayEvent(_event, it->second);
	}
	else
	{
		assert(false);
	}
}

void CSoundManagerImplementation::SetSwitch(const SoundSwitchValue &switchValue, const AkGameObjectID &id)
{
	AKRESULT res = AK::SoundEngine::SetSwitch(switchValue.soundSwitch.switchName.c_str(), switchValue.valueName.c_str(), id);
	assert(res);
}

void CSoundManagerImplementation::SetSwitch(const SoundSwitchValue &switchValue)
{
	SetSwitch(switchValue, m_DefaultSpeakerId);
}

void CSoundManagerImplementation::SetSwitch(const SoundSwitchValue &switchValue, const std::string &_speaker)
{
	auto it = m_NamedSpeakers.find(_speaker);
	if (it != m_NamedSpeakers.end())
	{
		SetSwitch(switchValue, it->second);
	}
	else
	{
		assert(false);
	}
}

void CSoundManagerImplementation::SetSwitch(const SoundSwitchValue &switchValue, const C3DElement* _speaker)
{
	auto it = m_GameObjectSpeakers.find(_speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		SetSwitch(switchValue, it->second);
	}
	else
	{
		assert(false);
	}
}

void CSoundManagerImplementation::SetRTPCValue(const SoundRTPC &_rtpc, float value, const AkGameObjectID &id)
{
	AKRESULT res = AK::SoundEngine::SetRTPCValue(_rtpc.RTPCName.c_str(), (AkRtpcValue)value, id);
	assert(res);
}

void CSoundManagerImplementation::SetRTPCValue(const SoundRTPC &_rtpc, float value)
{
	SetRTPCValue(_rtpc, m_DefaultSpeakerId);
}

void CSoundManagerImplementation::SetRTPCValue(const SoundRTPC &_rtpc, float value, const std::string &_speaker)
{
	auto it = m_NamedSpeakers.find(_speaker);
	if (it != m_NamedSpeakers.end())
	{
		SetRTPCValue(_rtpc, it->second);
	}
	else
	{
		assert(false);
	}
}

void CSoundManagerImplementation::SetRTPCValue(const SoundRTPC &_rtpc, float value, const C3DElement* _speaker)
{
	auto it = m_GameObjectSpeakers.find(_speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		SetRTPCValue(_rtpc, it->second);
	}
	else
	{
		assert(false);
	}
}

void CSoundManagerImplementation::BroadcastRTPCValue(const SoundRTPC &_rtpc, float value)
{
	AKRESULT res = AK::SoundEngine::SetRTPCValue(_rtpc.RTPCName.c_str(), (AkRtpcValue)value);
	assert(res);
}

void CSoundManagerImplementation::BroadcastState(const SoundStateValue &_state)
{
	AKRESULT res = AK::SoundEngine::SetState(_state.soundState.stateName.c_str(), _state.valueName.c_str());
	assert(res);
}