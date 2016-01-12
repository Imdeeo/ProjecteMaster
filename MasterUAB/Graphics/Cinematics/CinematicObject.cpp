#include "CinematicObject.h"
#include "Cinematics\CinematicObjectKeyFrame.h"
#include "RenderableObjects\RenderableObject.h"
#include "Engine\UABEngine.h"

CCinematicObject::CCinematicObject(CXMLTreeNode &TreeNode):m_RenderableObject(nullptr)
{
	m_RenderableObject=UABEngine.GetRenderableObjectsManager()->GetResource(TreeNode.GetPszProperty("resource"));
	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{		
		CXMLTreeNode l_Element = TreeNode(i);
		if (l_Element.GetName() == std::string("cinematic_object_key_frame"))
		{
			AddCinematicObjectKeyFrame(new CCinematicObjectKeyFrame(l_Element));
		}
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
	return (m_RenderableObject != nullptr); //No se que hay que mirar exactamente.
}

void CCinematicObject::AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame)
{
	m_CinematicObjectKeyFrames.push_back(CinematicObjectKeyFrame);
}

void CCinematicObject::Update(float _ElapsedTime)
{
	m_CurrentTime += _ElapsedTime;
	GetCurrentKey();
	//interpolar
}

void CCinematicObject::OnRestartCycle()
{
	m_CurrentTime = 0;
}

void CCinematicObject::GetCurrentKey()
{
	for (size_t i = 0; i < m_CinematicObjectKeyFrames.size(); i++){
		if (m_CurrentTime >= m_CinematicObjectKeyFrames[i]->GetKeyFrameTime()){ m_CurrentKeyFrame = i; }
	}	
}