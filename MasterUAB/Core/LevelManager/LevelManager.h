
#include <map>
#include <vector>
#include <string>

#include "Level.h"
#include "Utils\TemplatedVectorMapManager.h"

class CRenderManager;
class CLayerManager;
class CRenderableObjectsManager;
class CSceneRendererCommandManager;

typedef struct SLevelInfo
{
	std::string m_ID;
	bool m_Loaded;
	std::string	m_Directory;
	std::string m_LevelInitLuaFunction;
} TLevelInfo;

typedef struct SLevelLayers
{
	bool* m_Visible;
	CRenderableObjectsManager* m_Layer;
} TLevelLayers;

class CLevelManager: public CTemplatedVectorMapManager<CLevel>
{
private:
	std::map<std::string, TLevelInfo> m_LevelsInfo;

	std::map<std::string,std::vector<TLevelLayers*>> m_LayersMap;

	std::string m_LevelSceneRenderCommands;

	std::map<std::string, CSceneRendererCommandManager*> m_SceneRenderCommandsManager;

	std::string m_LevelsFileName;
public:
	CLevelManager();
	virtual ~CLevelManager();

	TLevelInfo GetLevelInfo(const std::string &_LevelName);

	void ChooseSceneRenderLevel(const std::string &_LevelId);
	void AddSceneCommandsManager(const std::string &_LevelId, CSceneRendererCommandManager* _SceneRendererCommandManager);
	CSceneRendererCommandManager * GetSceneRendererCommandManager(const std::string _levelId);
	void ExecuteSceneCommands(CRenderManager* _RenderManager);

	void LoadFile(const std::string &_LevelsFilename);
	void LoadLevel(const std::string &_LevelName);
	void ReloadLevel(const std::string &_LevelName);
	void UnloadLevel(const std::string &_LevelName);

	std::vector<TLevelLayers*>* GetCompleteLayer(const std::string &_LayerName);

	void ReloadAllLua();

	void Update(float _ElapsedTime);

	UAB_BUILD_GET_SET(std::string, ActualLevel);
};