

#include <map>
#include <vector>

class CLayerManager;

class CLevelManager
{
private:
	CLayerManager* m_LayerManagerArray;
public:
	CLevelManager();
	virtual ~CLevelManager();

	void LoadFile();
};

