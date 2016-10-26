#ifndef SOUNDMANAGERIMPLEMENTATION_H
#define SOUNDMANAGERIMPLEMENTATION_H

#include "SoundManager.h"
#include <vector>
#include <unordered_map>

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/IBytes.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AkFilePackageLowLevelIOBlocking.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>

#include <AkSoundEngineDLL.h>

#pragma comment(lib, "AkSoundEngineDLL.lib")
//#pragma comment(lib, "AkSoundEngine.lib")
//#pragma comment(lib, "AkMemoryMgr.lib")
//#pragma comment(lib, "AkStreamMgr.lib")
//#pragma comment(lib, "AkMusicEngine.lib")
//#pragma comment(lib, "CommunicationCentral.lib")


class CSoundManagerImplementation : public ISoundManager
{
private:
	AkGameObjectID m_LastGameObjectID; // note: only > 0
	std::vector<AkGameObjectID> m_FreeObjectsIDs;

	AkGameObjectID m_DefaultSpeakerId;
	std::unordered_map<std::string, AkGameObjectID> m_NamedSpeakers;
	std::unordered_map<const C3DElement*, AkGameObjectID> m_GameObjectSpeakers;

	AkGameObjectID GenerateObjectID();

	std::string m_SoundBanksFilename;
	std::string m_SpeakersFilename;

	bool LoadSoundBanksXML();
	bool LoadSpeakersXML();
	bool InitBanks();
	void Terminate();
	void Clean();
	void SetListenerPosition(const CCameraController *camera);
	void PlayEvent(const SoundEvent &_event, const AkGameObjectID &id);
	void SetSwitch(const SoundSwitchValue &switchValue, const AkGameObjectID &id);
	void SetRTPCValue(const SoundRTPC &_rtpc, float value, const AkGameObjectID &id);

public:
	CSoundManagerImplementation();
	virtual ~CSoundManagerImplementation();
	bool Init();
	void RegisterSpeaker(const C3DElement* _speaker);
	void UnregisterSpeaker(const C3DElement* _speaker);
	bool Load(const std::string &soundbanks_filename, const std::string &speakers_filename);
	bool Reload();
	bool LoadSoundBank(const std::string &bank);
	bool UnloadSoundBank(const std::string &bank);
	void Update(const CCameraController *camera);

	void PlayEvent(const SoundEvent &_event);
	void PlayEvent(const SoundEvent &_event, const std::string &_speaker);
	void PlayEvent(const SoundEvent &_event, const C3DElement* _speaker);

	void SetSwitch(const SoundSwitchValue &switchValue);
	void SetSwitch(const SoundSwitchValue &switchValue, const std::string &_speaker);
	void SetSwitch(const SoundSwitchValue &switchValue, const C3DElement* _speaker);

	void BroadcastRTPCValue(const SoundRTPC &_rtpc, float value);
	void SetRTPCValue(const SoundRTPC &_rtpc, float value);
	void SetRTPCValue(const SoundRTPC &_rtpc, float value, const std::string &_speaker);
	void SetRTPCValue(const SoundRTPC &_rtpc, float value, const C3DElement* _speaker);

	void BroadcastState(const SoundStateValue &_state);

	
	
};

#endif