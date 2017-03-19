#ifndef H_CINEMATIC_PLAYER_H
#define H_CINEMATIC_PLAYER_H

class CCinematicPlayer
{
protected:
	bool m_Playing;
	float m_CurrentTime;
	float m_Duration;
	bool m_Cycle;
public:
	CCinematicPlayer(){};
	virtual ~CCinematicPlayer(){};
	void Init(float _Duration);
	virtual void Update(float _ElapsedTime);
	virtual void Stop();
	virtual void Play(bool _Cycle);
	virtual void Pause();
	float GetDuration();
	float GetCurrentT();
	virtual void OnRestartCycle(){};
};

#endif H_CINEMATIC_PLAYER_H