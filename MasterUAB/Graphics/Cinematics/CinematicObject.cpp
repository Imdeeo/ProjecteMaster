#include "CinematicObject.h"
#include "Cinematics\CinematicObjectKeyFrame.h"
#include "RenderableObjects\RenderableObject.h"
#include "Engine\UABEngine.h"

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

			float l_pitchI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch();
			float l_pitchF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetPitch();

			float l_rollI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll();
			float l_rollF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetRoll();

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

void CCinematicObject::OnRestartCycle()
{
	m_CurrentTime = 0;
	GetCurrentKey();
	m_Playing=true;
}

void CCinematicObject::GetCurrentKey()
{
	for (size_t i = 0; i < m_CinematicObjectKeyFrames.size(); i++){
		if (m_CurrentTime >= m_CinematicObjectKeyFrames[i]->GetKeyFrameTime()){ m_CurrentKeyFrame = i; }
	}	
}