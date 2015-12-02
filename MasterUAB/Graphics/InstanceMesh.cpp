#include "InstanceMesh.h"
#include "UABEngine.h"

#include <d3d11.h>
#include "Math\Matrix44.h"


CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName)
{
	SetName(Name);
	m_StaticMesh = UABEngine.GetStaticMeshManager()->GetResource(CoreName);
}


CInstanceMesh::~CInstanceMesh(void)
{

}

void CInstanceMesh::Render(CRenderManager *RM)
{
	if(GetVisible())
	{
		Mat44f world;

		world.SetIdentity();
		world.SetIdentity();
		world.SetFromPitchRollYaw(Vect3f(GetPitch(),GetRoll(),GetYaw()));
		world.SetFromPos(GetPosition());
		world.SetScale(GetScale());
		RM->GetContextManager()->SetWorldMatrix(world);
		m_StaticMesh->Render(RM);
	}
}