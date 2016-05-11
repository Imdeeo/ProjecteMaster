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
			//if (!AddResource(l_ParticleSystemType->GetName(), l_ParticleSystemType, "CParticleManager"))
			if (!AddResource(l_ParticleSystemType->GetName(), l_ParticleSystemType))
			{
				CHECKED_DELETE(l_ParticleSystemType);
			}
		}
	}
}

void CParticleManager::Reload()
{
	Destroy();
	Load(m_Filename);
}

void CParticleManager::Save()
{
	FILE* l_File;
	if (!fopen_s(&l_File, m_Filename.c_str(), "w"))
	{
		fprintf_s(l_File, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
		fprintf_s(l_File, "<particle_systems>\n");

		typedef TMapResource::iterator it_type;
		for (it_type iterator = m_Resources.begin(); iterator != m_Resources.end(); iterator++)
		{
			iterator->second->Save(l_File);
		}

		fprintf_s(l_File, "</particle_systems>\n");
		fclose(l_File);
	}
}