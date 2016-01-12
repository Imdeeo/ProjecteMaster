#include "CinematicPlayer.h"

void CCinematicPlayer::Init(float Duration)
{
	m_Duration = Duration;
	m_CurrentTime = 0;
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
	m_CurrentTime = 0;
}

void CCinematicPlayer::Play(bool Cycle)
{
	m_Cycle = Cycle;
	m_Playing = true;	
}

void CCinematicPlayer::Pause()
{
	m_Playing = false;
}
