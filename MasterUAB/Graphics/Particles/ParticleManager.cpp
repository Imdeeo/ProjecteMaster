#include "ParticleManager.h"
#include "XML\XMLTreeNode.h"

CParticleManager::CParticleManager(void){}

CParticleManager::~CParticleManager(void)
{
	Destroy();
}


void CParticleManager::Load(const std::string &Filename)
{
	CXMLTreeNode l_File;
	m_Filename = Filename;

	if (l_File.LoadFile(m_Filename.c_str()))
	{
		CXMLTreeNode l_Meshes = l_File["particle_systems"];
		for (int i = 0; i < l_Meshes.GetNumChildren(); ++i)
		{
			CParticleSystemType *l_ParticleSystemType = new CParticleSystemType(l_Meshes(i));
/*			if (!AddResource(l_ParticleSystemType->GetName(), l_ParticleSystemType, "CParticleManager"))
			{
				CHECKED_DELETE(l_ParticleSystemType);
			}*/
		}
	}
}

void CParticleManager::Reload()
{
	Destroy();
	Load(m_Filename);
}