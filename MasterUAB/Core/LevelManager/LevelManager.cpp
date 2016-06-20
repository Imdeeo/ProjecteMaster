#include "LevelManager.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

#include "Materials\MaterialManager.h"
#include "StaticMesh\StaticMeshManager.h"
#include "Lights\LightManager.h"
#include "Layers\LayerManager.h"
#include "ScriptManager\ScriptManager.h"
#include "Camera\CameraControllerManager.h"
#include "Cinematics\Cinematic.h"
#include "Particles\ParticleManager.h"

CLevelManager::CLevelManager()
{
}


CLevelManager::~CLevelManager()
{
}

void CLevelManager::LoadFile(const std::string &_LevelsFilename)
{
	m_LayerManagerArray = UABEngine.GetLayerManager();

	m_LevelsFileName = _LevelsFilename;
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(_LevelsFilename.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["levels"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("level"))
				{
					std::string l_LevelName = l_Element.GetPszProperty("name");
					std::string l_LevelDirectory = l_Element.GetPszProperty("directory");

					m_LevelDirectories[l_LevelName] = l_LevelName;
				}
			}
		}
	}
}

void CLevelManager::LoadLevel(const std::string &_LevelName)
{
	std::string l_LevelDirectory = m_LevelDirectories[_LevelName];
	UABEngine.GetMaterialManager()->Load(l_LevelDirectory + "\\materials.xml", "Data\\default_effect_materials.xml");
	UABEngine.GetParticleManager()->Load(l_LevelDirectory + "\\particles.xml");
	UABEngine.GetStaticMeshManager()->Load(l_LevelDirectory + "\\static_meshes.xml");
	UABEngine.GetLayerManager()->Load(l_LevelDirectory + "\\renderable_objects.xml");
	UABEngine.GetLightManager()->Load(l_LevelDirectory + "\\lights.xml");
	UABEngine.GetCinematic()->LoadXML(l_LevelDirectory + "\\cinematic.xml");
	UABEngine.GetCameraControllerManager()->Load(l_LevelDirectory + "\\cameras.xml");
	UABEngine.GetScriptManager()->RunCode("mainLua(\"" + l_LevelDirectory + "\")");
}

void CLevelManager::ReloadLevel(const std::string &_LevelName)
{

}
void CLevelManager::UnloadLevel(const std::string &_LevelName)
{

}
