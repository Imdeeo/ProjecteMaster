#include "Utils.h"
#include <sstream>

static void UtilsLog(const std::string& text)
{
	// TODO: OPCIONAL esto va bi�n tenerlo escribiendo en un fichero
	OutputDebugString(("LOG: " + text).c_str());
}


std::vector<std::string> &SplitString(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> SplitString(const std::string &s, char delim) {
    std::vector<std::string> elems;
    SplitString(s, delim, elems);
    return elems;
}