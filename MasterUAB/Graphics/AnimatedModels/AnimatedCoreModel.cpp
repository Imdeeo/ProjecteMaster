#include "AnimatedCoreModel.h"
#include "Materials\Material.h"
#include "Engine\UABEngine.h"

#include "XML\XMLTreeNode.h"

CAnimatedCoreModel::CAnimatedCoreModel():CNamed("")
{
	m_CalCoreModel = nullptr;
}
CAnimatedCoreModel::~CAnimatedCoreModel()
{
}

bool CAnimatedCoreModel::Load(const std::string &Path)
{
	bool result = true;
	m_Path = Path;
	std::string l_Filename = Path;
	
	std::string l_MeshFilename = Path;
	std::string l_SkeletonFilename = Path;
	std::string l_AnimationFilename = Path;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(l_Filename.append("actor.xml").c_str()))
	{
		CXMLTreeNode l_Input = l_XML["actor"];
		if (l_Input.Exists())
		{
			SetName(l_Input.GetPszProperty("name"));
			m_CalCoreModel = new CalCoreModel(m_Name);
			m_BSRadius = l_Input.GetFloatProperty("radius_bs");
			m_BSPosition = l_Input.GetVect3fProperty("pos_bs",Vect3f(0.f,0.f,0.f),true);
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("material"))
				{
					result = result & UABEngine.GetMaterialManager()->AddResource(l_Element.GetPszProperty("name"),new CMaterial(l_Element));
					m_Materials.push_back(UABEngine.GetMaterialManager()->GetResource(l_Element.GetPszProperty("name")));
				}
				if (l_Element.GetName() == std::string("skeleton"))
				{
					result = result & LoadSkeleton(l_SkeletonFilename.append(l_Input.GetPszProperty("filename")));
				}
				if (l_Element.GetName() == std::string("mesh"))
				{
					result = result & LoadMesh(l_MeshFilename.append(l_Input.GetPszProperty("filename")));
				}
				if (l_Element.GetName() == std::string("animation"))
				{
					result = result & LoadAnimation(l_Input.GetPszProperty("name"),
						l_AnimationFilename.append(l_Input.GetPszProperty("filename")));
				}
			}
		}
	}


	return result;
}

bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
	if(m_CalCoreModel->loadCoreMesh(Filename) == -1)
    {
		CalError::printLastError();
		return false;
    }
}

bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
	if(!m_CalCoreModel->loadCoreSkeleton(Filename))
    {
    CalError::printLastError();
    return false;
    }
	return true;
}

bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
	
	m_AnimationsIds[Name] = m_CalCoreModel->loadCoreAnimation(Filename);
    if(m_AnimationsIds[Name] == -1)
    {
		CalError::printLastError();
		return false;
    }
}
