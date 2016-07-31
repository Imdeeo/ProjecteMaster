

#include <map>
#include <vector>
#include <string>

class CLayerManager;

typedef struct SLevelInfo
{
	bool		m_Loaded;
	std::string	m_Directory;
} TLevelInfo;

class CLevelManager
{
private:
	std::map<std::string, TLevelInfo> m_LevelsInfo;
	CLayerManager* m_LayerManagerArray;

	std::string m_LevelsFileName;
public:
	CLevelManager();
	virtual ~CLevelManager();

	void LoadFile(const std::string &_LevelsFilename);
	void LoadLevel(const std::string &_LevelName);
	void ReloadLevel(const std::string &_LevelName);
	void UnloadLevel(const std::string &_LevelName);

	void ReloadAllLua();
};

