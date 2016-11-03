#include "VideoManagerImplementation.h"
#include "Utils.h"
#include "Engine\UABEngine.h"
#include <theoraplayer/TheoraPlayer.h>
#include <theoraplayer/TheoraDataSource.h>

CVideoManagerImplementation::CVideoManagerImplementation()
{
	mgr = new TheoraVideoManager();
	m_ActualClip = nullptr;
	m_Path = "Data/Video/";
}

CVideoManagerImplementation::~CVideoManagerImplementation()
{
	CHECKED_DELETE(mgr);
}

TheoraVideoClip* CVideoManagerImplementation::LoadVideoClip(const std::string &l_ClipFile, bool _Restart)
{
	TheoraVideoClip* l_Clip = mgr->createVideoClip(m_Path + l_ClipFile, TH_RGBX);
	if (l_Clip == nullptr)
		return nullptr;
	l_Clip->setAutoRestart(_Restart);
	l_Clip->stop();
	return l_Clip;
}

bool CVideoManagerImplementation::LoadClip(const std::string &l_ClipFile, bool _Restart)
{
	//clip = mgr->createVideoClip("Data/Video/bunny.ogv", TH_RGBX);
	TheoraVideoClip* l_Clip = LoadVideoClip(l_ClipFile, _Restart);
	if (l_Clip == nullptr)
		return false;
	return true;
}
 
TheoraVideoClip* CVideoManagerImplementation::GetClip(const std::string &l_ClipName)
{
	return mgr->getVideoClipByName(m_Path + l_ClipName);
}

bool CVideoManagerImplementation::ClearClip(const std::string &l_ClipName)
{
	TheoraVideoClip* l_Clip = mgr->getVideoClipByName(m_Path+l_ClipName);
	if (l_Clip == nullptr)
		return false;
	mgr->destroyVideoClip(l_Clip);
	return true;
}

void CVideoManagerImplementation::Update(float _ElapsedTime)
{
	mgr->update(_ElapsedTime);
	if (m_ActualClip != nullptr)
	{
		char logstr[256] = "";
		sprintf_s(logstr, "TimePosition: %f, Duration: %f, AutoRestart: %s", m_ActualClip->getTimePosition(), m_ActualClip->getDuration(), m_ActualClip->getAutoRestart()?"true":"false");
		UtilsLog(logstr);
		if (m_ActualClip->getTimePosition() >= m_ActualClip->getDuration() && !m_ActualClip->getAutoRestart())
		{			
			UABEngine.SetVideoPause(false);
			m_ActualClip = nullptr;
		}
	}
}

TheoraVideoClip* CVideoManagerImplementation::GetActualClip()
{
	return m_ActualClip;
}

bool CVideoManagerImplementation::PlayClip(const std::string &l_ClipName)
{
	TheoraVideoClip* l_Clip = mgr->getVideoClipByName(m_Path+l_ClipName);
	if (l_Clip == nullptr)
		return false; 
	l_Clip->play();
	return true;
}

bool CVideoManagerImplementation::RenderSceenClip(const std::string &l_ClipName)
{
	m_ActualClip = mgr->getVideoClipByName(m_Path+l_ClipName);
	if (m_ActualClip == nullptr)
		return false;
	m_ActualClip->play();
	UABEngine.SetVideoPause(true);
	return true;
}

void CVideoManagerImplementation::ReloadClip(const std::string &l_ClipName)
{
	TheoraVideoClip* l_ActualClip = mgr->getVideoClipByName(m_Path + l_ClipName);
	l_ActualClip->stop();
}
