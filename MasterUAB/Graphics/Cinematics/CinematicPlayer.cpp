#include "CinematicPlayer.h"

void CCinematicPlayer::Init(float _Duration)
{
	m_Duration = _Duration;
	m_CurrentTime = 0.0f;
	m_Playing = false;
	m_Cycle = false;
}

void CCinematicPlayer::Update(float _ElapsedTime)
{
	m_CurrentTime += _ElapsedTime;
}

void CCinematicPlayer::Stop()
{
	m_Playing = false;
	m_CurrentTime = 0.0f;
}

void CCinematicPlayer::Play(bool _Cycle)
{
	m_Cycle = _Cycle;
	m_Playing = true;	
}

void CCinematicPlayer::Pause()
{
	m_Playing = false;
	m_CurrentTime = 0;
}

float CCinematicPlayer::GetDuration()
{
	return m_Duration;
}

float CCinematicPlayer::GetCurrentT()
{
	return m_CurrentTime;
}
