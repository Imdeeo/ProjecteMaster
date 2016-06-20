

#include <map>
#include <vector>

class CLayerManager;

class CLevelManager
{
private:
	std::map<std::string, std::string> m_LevelDirectories;
	CLayerManager* m_LayerManagerArray;

	std::string m_LevelsFileName;
public:
	CLevelManager();
	virtual ~CLevelManager();

	void LoadFile(const std::string &_LevelsFilename);
	void LoadLevel(const std::string &_LevelName);
	void ReloadLevel(const std::string &_LevelName);
	void UnloadLevel(const std::string &_LevelName);
};

