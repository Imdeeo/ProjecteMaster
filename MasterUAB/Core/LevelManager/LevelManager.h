
#include <map>
#include <vector>
#include <string>

#include "Level.h"
#include "Utils\TemplatedVectorMapManager.h"

class CLayerManager;
class CRenderableObjectsManager;

typedef struct SLevelInfo
{
	std::string m_ID;
	bool m_Loaded;
	std::string	m_Directory;
} TLevelInfo;

class CLevelManager: CTemplatedVectorMapManager<CLevel>
{
private:
	std::map<std::string, TLevelInfo> m_LevelsInfo;
	CLayerManager* m_LayerManagerArray;

	std::string m_LevelsFileName;
public:
	CLevelManager();
	virtual ~CLevelManager();

	TLevelInfo GetLevelInfo(std::string &_LevelName);

	void LoadFile(const std::string &_LevelsFilename);
	void LoadLevel(const std::string &_LevelName);
	void ReloadLevel(const std::string &_LevelName);
	void UnloadLevel(const std::string &_LevelName);

	std::vector<CRenderableObjectsManager*>GetCompleteLayer(const std::string &_LayerName);

	void ReloadAllLua();

	void Update(float _ElapsedTime);
};

