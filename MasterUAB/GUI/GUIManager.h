#ifndef H_GUI_MANAGER_H
#define H_GUI_MANAGER_H

#include <string>
#include <stdlib.h>
//#include "Utils\TemplatedVectorMapManager.h"

class CGUIManager 
{
private:
	std::string m_ActiveItem;
	std::string m_HotItem;
public:
	void SetActive(const std::string& id);
	void SetNotActive(const std::string& id);
	void SetHot(const std::string& id);
	void SetNotHot(const std::string& id);
};

#endif //H_GUI_MANAGER_H