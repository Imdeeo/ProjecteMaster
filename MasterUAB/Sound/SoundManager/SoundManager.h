#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <string>
class C3DElement;

class CCameraController;

struct SoundEvent
{
	std::string eventName;
};

struct SoundSwitch
{
	std::string switchName;
};

struct SoundSwitchValue
{
	SoundSwitch soundSwitch;
	std::string valueName;
};

struct SoundRTPC
{
	std::string RTPCName;
};

struct SoundState
{
	std::string stateName;
};

struct SoundStateValue
{
	SoundState soundState;
	std::string valueName;
};

class ISoundManager
{
public:
	static ISoundManager* InstantiateSoundManager();

	virtual ~ISoundManager() {}

protected:
	ISoundManager()
	{}

	std::string m_Path;

public:
	void SetPath(const std::string &path) { m_Path = path; }

	virtual bool Init() = 0;

	virtual void Update(const CCameraController *camera) = 0;

	virtual bool Load(const std::string &soundbandks_filename, const std::string &speakers_filename) = 0;

	virtual bool Reload() = 0;

	virtual bool LoadSoundBank(const std::string &bank) = 0;
	virtual bool UnloadSoundBank(const std::string &bank) = 0;

	virtual void RegisterSpeaker(const C3DElement* _speaker) = 0;
	virtual void UnregisterSpeaker(const C3DElement* _speaker) = 0;

	virtual void PlayEvent(const SoundEvent &_event) = 0;
	virtual void PlayEvent(const SoundEvent &_event, const std::string &_speaker) = 0;
	virtual void PlayEvent(const SoundEvent &_event, const C3DElement* _speaker) = 0;

	virtual void SetSwitch(const SoundSwitchValue &switchValue) = 0;
	virtual void SetSwitch(const SoundSwitchValue &switchValue, const std::string &_speaker) = 0;
	virtual void SetSwitch(const SoundSwitchValue &switchValue, const C3DElement* _speaker) = 0;

	virtual void BroadcastRTPCValue(const SoundRTPC &_rtpc, float value) = 0;
	virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value) = 0;
	virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const std::string &_speaker) = 0;
	virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const C3DElement* _speaker) = 0;

	virtual void BroadcastState(const SoundStateValue &_state) = 0;
};

#endif