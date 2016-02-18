#include "CinematicObject.h"
#include "Cinematics\CinematicObjectKeyFrame.h"
#include "RenderableObjects\RenderableObject.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectsManager.h"

CCinematicObject::CCinematicObject(CXMLTreeNode &TreeNode):m_RenderableObject(nullptr)
{
	m_RenderableObject=UABEngine.GetLayerManager()->GetLayer(TreeNode)->GetResource(TreeNode.GetPszProperty("resource"));
	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{		
		CXMLTreeNode l_Element = TreeNode(i);
		if (l_Element.GetName() == std::string("cinematic_object_key_frame"))
		{
			AddCinematicObjectKeyFrame(new CCinematicObjectKeyFrame(l_Element));
		}
	}
	if (IsOk())
	{
		m_CurrentTime=0;
		m_Duration = m_CinematicObjectKeyFrames[m_CinematicObjectKeyFrames.size()-1]->GetKeyFrameTime();
		GetCurrentKey();
	}
}

CCinematicObject::~CCinematicObject()
{
	for (size_t i=0; i < m_CinematicObjectKeyFrames.size(); ++i)
	{
		CHECKED_DELETE(m_CinematicObjectKeyFrames[i]);
	}
	m_CinematicObjectKeyFrames.clear();
}

bool CCinematicObject::IsOk()
{
	if (m_RenderableObject != nullptr && m_CinematicObjectKeyFrames.size()>1)
	{
			return true;
	}
	return false; 
}

void CCinematicObject::AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame)
{
	m_CinematicObjectKeyFrames.push_back(CinematicObjectKeyFrame);
}

void CCinematicObject::Update(float _ElapsedTime)
{
	if(m_Playing)
	{
		m_CurrentTime += _ElapsedTime;
		if(IsFinished())
		{
			m_Playing = false;
		}
		else
		{
			GetCurrentKey();

			Vect3f l_pI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition();
			Vect3f l_pF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetPosition();

			float l_tI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime();
			float l_tF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetKeyFrameTime();

			float l_yawI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetYaw();
			float l_yawF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetYaw();
			l_yawF = calculateBestAngle(l_yawI, l_yawF);

			float l_pitchI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch();
			float l_pitchF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetPitch();
			l_pitchF = calculateBestAngle(l_pitchI, l_pitchF);

			float l_rollI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll();
			float l_rollF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetRoll();
			l_rollF = calculateBestAngle(l_rollI, l_rollF);

			Vect3f l_scaleI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale();
			Vect3f l_scaleF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetScale();
			
			m_RenderableObject->SetPosition((((l_pF - l_pI)*(m_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_pI);	

			m_RenderableObject->SetYaw((((l_yawF - l_yawI)*(m_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_yawI);	

			m_RenderableObject->SetPitch((((l_pitchF - l_pitchI)*(m_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_pitchI);	

			m_RenderableObject->SetRoll((((l_rollF - l_rollI)*(m_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_rollI);

			m_RenderableObject->SetScale((((l_scaleF - l_scaleI)*(m_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_scaleI);		
		}
	}


}

float CCinematicObject::calculateBestAngle(float _Current, float _Next)
{
	if (_Current == _Next){
		return _Next;
	}
	float l_best = _Next;
	if (_Current > _Next){
		while (_Current > l_best)
		{
			l_best = l_best + 2*UAB_PI;
		}
		_Next = l_best - 2 * UAB_PI;
	}
	else
	{
		while (_Current < l_best)
		{
			l_best = l_best - 2*UAB_PI;
		}
		_Next = l_best + 2 * UAB_PI;
	}

	return (abs(_Current - l_best) < abs(_Current - _Next)) ? l_best : _Next;

}

void CCinematicObject::OnRestartCycle()
{
	m_CurrentTime = 0;
	GetCurrentKey();
	m_Playing=true;
}

void CCinematicObject::GetCurrentKey()
{
	m_CurrentKeyFrame = 0;
	for (size_t i = 0; i < m_CinematicObjectKeyFrames.size()-1; i++){
		if (m_CurrentTime >= m_CinematicObjectKeyFrames[i]->GetKeyFrameTime())
		{
			m_CurrentKeyFrame = i;
		}
		else
		{
			return;
		}
	}	
}