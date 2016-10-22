#include "ParticleManager.h"
#include "XML\tinyxml2.h"
#include "Engine\UABEngine.h"
#include "LevelManager\LevelManager.h"

CParticleManager::CParticleManager(void){}

CParticleManager::~CParticleManager(void)
{
	Destroy();
}


void CParticleManager::Load(const std::string &Filename, CLevel *_Level)
{
	m_Filename = Filename;
	m_LevelName = _Level->GetName();

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(Filename.c_str());

	tinyxml2::XMLElement* l_Element;

	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("particle_systems");
		if (l_Element!=NULL)
		{
			l_Element = l_Element->FirstChildElement();
			while (l_Element != NULL)
			{
				CParticleSystemType *l_ParticleSystemType = new CParticleSystemType(l_Element, m_LevelName);
				//if (!AddResource(l_ParticleSystemType->GetName(), l_ParticleSystemType, "CParticleManager"))
				if (!AddResource(l_ParticleSystemType->GetName(), l_ParticleSystemType, m_LevelName))
				{
					CHECKED_DELETE(l_ParticleSystemType);
				}
				l_Element = l_Element->NextSiblingElement();
			}
		}
	}
}

void CParticleManager::Reload()
{
	Destroy();
	Load(m_Filename, UABEngine.GetLevelManager()->GetResource(m_LevelName));
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