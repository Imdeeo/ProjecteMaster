#include "LayerManager.h"
#include "RenderableObjects\RenderableObjectManager.h"
#include "Utils.h"

CLayerManager::CLayerManager(){}

CLayerManager::~CLayerManager()
{
	Destroy();
}

void CLayerManager::Destroy()
{
	CHECKED_DELETE(m_DefaultLayer);
}

void CLayerManager::Load(const std::string &Filename)
{

}

void CLayerManager::Reload()
{
	Destroy();
	Load(m_Filename);
}

void CLayerManager::Update(float ElapsedTime)
{
}

void CLayerManager::Render(CRenderManager &RenderManager)
{
}

void CLayerManager::Render(CRenderManager &RenderManager, const std::string &LayerName)
{
}