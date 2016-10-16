#include "LoadScreenManager.h"
#include "XML\tinyxml2.h"
#include "Effects\EffectManager.h"
#include "Texture\Texture.h"
#include "ContextManager\ContextManager.h"
#include "RenderManager\RenderManager.h"
#include "Utils.h"
#include "Engine\UABEngine.h"

#include <Windows.h>

CLoadScreenManager::CLoadScreenManager(const std::string &Filename) : m_Filename(Filename),  m_Count(0), m_Timer(0.0f), m_Angle(0), m_LoadTechnique(nullptr), m_Loading(true) {
	m_Texture = nullptr;
	m_EffectAddress = CEffectManager::AddMaterialParameter(CMaterialParameter::TMaterialType::FLOAT);
}

CLoadScreenManager::~CLoadScreenManager(void)
{
	Destroy();
}

void CLoadScreenManager::Load()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(m_Filename.c_str());

	tinyxml2::XMLElement* l_Element;

	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		TLoadScreen* l_screen;

		l_Element = doc.FirstChildElement("load_screens")->FirstChildElement();
		while (l_Element != NULL)
		{
			if (l_Element->Name() == std::string("screen"))
			{
				l_screen = new TLoadScreen();
				l_screen->name = l_Element->GetPszProperty("name", "");
				l_screen->time = l_Element->GetFloatProperty("time", 1.0);
				l_screen->file = l_Element->GetPszProperty("file", "");
				l_screen->animated = l_Element->GetBoolProperty("animated", false);

				m_LoadScreens.push_back(l_screen);
			}
			l_Element = l_Element->NextSiblingElement();
		}
	}

	LoadScreen("Data\\effects.xml");
}


void CLoadScreenManager::LoadScreen(const std::string _FileName)
{
	std::string l_EffectName;
	CEffectVertexShader* l_EffectVertexShader;
	CEffectPixelShader* l_EffectPixelShader;

	tinyxml2::XMLDocument doc;
	doc.LoadFile(_FileName.c_str());

	tinyxml2::XMLElement* titleElement;

	titleElement = doc.FirstChildElement("load_screen")->FirstChildElement("vertex_shader");
	l_EffectVertexShader = new CEffectVertexShader(titleElement);
	l_EffectVertexShader->Load();

	titleElement = doc.FirstChildElement("load_screen")->FirstChildElement("pixel_shader");
	l_EffectPixelShader = new CEffectPixelShader(titleElement);
	l_EffectPixelShader->Load();

	titleElement = doc.FirstChildElement("load_screen")->FirstChildElement("effect_technique");
	l_EffectName = titleElement->Attribute("name");
	m_LoadTechnique = new CEffectTechnique(l_EffectVertexShader, l_EffectPixelShader, nullptr, l_EffectName);

	RenderLoadScreen();
}

void CLoadScreenManager::RenderLoadScreen()
{
	m_Texture = new CTexture();
	CRenderManager * _RenderManager = UABEngine.GetRenderManager();
	_RenderManager->EnableAlphaBlendState();
	CContextManager* l_ContextManager = _RenderManager->GetContextManager();
	m_Texture->Load(m_LoadScreens[m_Count]->file);
	//RENDER DEL LOADSCREEN		
	l_ContextManager->BeginRender();
	_RenderManager->SetMatrixViewProjection();
	_RenderManager->Clear(true, true);
	_RenderManager->GetContextManager()->SetWorldMatrix(m44fIDENTITY);
	CEffectManager::SetSceneConstants(m_LoadTechnique);
	_RenderManager->DrawScreenQuad(m_LoadTechnique, m_Texture, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
	l_ContextManager->EndRender();

	SYSTEMTIME st;
	GetSystemTime(&st);
	float time_s = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
	float time_s2;
	
	while (m_Loading)
	{
		GetSystemTime(&st);
		time_s2 = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
		m_Timer = time_s2 - time_s;
		if (m_Timer > m_LoadScreens[m_Count]->time && m_Count < m_LoadScreens.size()-1)
		{
			++m_Count;
			m_Texture->Load(m_LoadScreens[m_Count]->file);
			l_ContextManager->BeginRender();
			_RenderManager->SetMatrixViewProjection();
			_RenderManager->Clear(true, true);
			_RenderManager->GetContextManager()->SetWorldMatrix(m44fIDENTITY);
			_RenderManager->DrawScreenQuad(m_LoadTechnique, m_Texture, 0.0, 0.0, 1.0, 1.0, CColor(1.f, 1.f, 1.f, 1.f));
			l_ContextManager->EndRender();
		}
	}
	_RenderManager->EnableAlphaBlendState();
}

void CLoadScreenManager::Reload()
{
	Destroy();
	Load();
}

void CLoadScreenManager::Destroy()
{
	for (size_t i = 0; i < m_LoadScreens.size(); ++i)
	{
		CHECKED_DELETE(m_LoadScreens[i]);
	}

	m_LoadScreens.clear();
	CHECKED_DELETE(m_LoadTechnique);
	CHECKED_DELETE(m_Texture);
}