#include "CinematicObject.h"
#include "Cinematics\CinematicObjectKeyFrame.h"
#include "RenderableObjects\RenderableObject.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "Layers\LayerManager.h"

#include "LevelManager\LevelManager.h"

CCinematicObject::CCinematicObject(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId ) :m_RenderableObject(nullptr)
{
	m_RenderableObject=UABEngine.GetLevelManager()->GetResource(_LevelId)->GetLayerManager()->GetLayer(TreeNode)->GetResource(TreeNode->GetPszProperty("resource"));

	//m_PivotPosition = m_RenderableObject->GetPosition();
	//m_PivotRotation = m_RenderableObject->GetRotation();
	//m_PivotScale = m_RenderableObject->GetScale();
	Quatf l_qi = m_RenderableObject->GetRotation().conjugate();
	//Quatf l_qlast = l_qi;
	//Quatf l_qf2;
	//l_qi.conjugate();
	Quatf l_qf, l_qAux;
	//bool l_xIgual, l_wIgual;
	tinyxml2::XMLElement *l_Element = TreeNode->FirstChildElement();
	while (l_Element != NULL)
	{
		if (l_Element->Name() == std::string("cinematic_object_key_frame"))
		{
			CCinematicObjectKeyFrame* l_CinematicKeyFrame = new CCinematicObjectKeyFrame(l_Element);
			l_qf = l_CinematicKeyFrame->GetRotation();
			
			l_qAux = l_qf*l_qi;
			l_CinematicKeyFrame->SetRotation(l_qAux);
			AddCinematicObjectKeyFrame(l_CinematicKeyFrame);
			//l_qlast = l_qf;
		}
		l_Element = l_Element->NextSiblingElement();
	}
	if (IsOk())
	{
		m_CurrentTime = 0.0f;
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
			m_RenderableObject->SetIsCinematic(false);
			m_CurrentTime = 0;
		}
		else
		{
			GetCurrentKey();

			Vect3f l_pI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition();
			Vect3f l_pF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetPosition();

			float l_tI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime();
			float l_tF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetKeyFrameTime();

			Quatf l_RI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRotation();
			Quatf l_RF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame + 1]->GetRotation();

			Vect3f l_scaleI = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale();
			Vect3f l_scaleF = m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetScale();

			Vect3f l_RelPos = (((l_pF - l_pI)*(m_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_pI;
			Quatf l_RelRot = l_RI.slerpJU(l_RF, ((m_CurrentTime - l_tI) / (l_tF - l_tI)));
			Vect3f l_RelSca = (((l_scaleF - l_scaleI)*(m_CurrentTime - l_tI)) / (l_tF - l_tI)) + l_scaleI;
			
			m_RenderableObject->SetIsCinematic(true);

			m_RenderableObject->SetAnimatedPosition(l_RelPos);
			m_RenderableObject->SetAnimatedScale(l_RelSca);
			m_RenderableObject->SetAnimatedRotation(l_RelRot);
			
			//m_RenderableObject->SetPosition(m_PivotPosition+l_RelPos);
			//m_RenderableObject->SetRotation(l_RelRot*m_PivotRotation);
			//m_RenderableObject->SetScale(Vect3f(m_PivotScale.x*l_RelSca.x, m_PivotScale.y*l_RelSca.y, m_PivotScale.z*l_RelSca.z));
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
	m_CurrentTime = 0.0f;
	GetCurrentKey();
	m_Playing=true;
}

void CCinematicObject::GetCurrentKey()
{
	m_CurrentKeyFrame = 0;
	for (size_t i = 0; i < m_CinematicObjectKeyFrames.size(); i++){
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

bool CCinematicObject::IsFinished()
{
	return m_CurrentTime >= m_Duration;
}