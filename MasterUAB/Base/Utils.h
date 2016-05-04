#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include <Windows.h>
#include <string>
#include <vector>

#include "Math\Quatn.h"
#include "Math\MathUtils.h"

#define MAXBONES 40
#define MAX_LIGHTS_BY_SHADER 4
#define UAB_ID_SHADOW_MAP 0
#define MAX_PARTICLE_PER_INSTANCE 200
#define MAX_VERTICES_PER_CALL 200

#define UAB_PI 3.14159265359f

#define UAB_BUILD_GET_SET(Type, Variable) \
protected: \
	Type			m_##Variable##; \
public: \
	void Set##Variable##(Type Variable) {m_##Variable##=Variable;} \
	Type Get##Variable##() const {return m_##Variable##;}

#define UAB_BUILD_GET_SET_AND_GET_ADDRESS(Type, Variable) \
protected: \
	Type			m_##Variable##; \
public: \
	void Set##Variable##(Type Variable) { m_##Variable## = Variable; } \
	Type Get##Variable##() const { return m_##Variable##; } \
	Type Get##Variable##Address() const { return &m_##Variable##; }

#define UAB_BUILD_GET_BY_REFERENCE(Type, Variable) \
protected: \
	Type			m_##Variable##; \
public: \
	const Type & Get##Variable##() const { return m_##Variable##; }

#define UAB_BUILD_GET_SET_BY_REFERENCE(Type, Variable) \
protected: \
	Type			m_##Variable##; \
public: \
	void Set##Variable##(const Type &Variable) {m_##Variable##=Variable;} \
	const Type & Get##Variable##() const {return m_##Variable##;}

#define UAB_GET_PROPERTY(Type, Variable) \
public: \
	Type Get##Variable##() const {return m_##Variable##;}

#define UAB_GET_PROPERTY_POINTER(Type, Variable) \
public: \
	Type * Get##Variable##() const { return m_##Variable##; }

#define UAB_GET_PROPERTY_H(Class, Type, Variable) \
	public:\
	Type Class##::Get##Variable##() const;

#define UAB_GET_PROPERTY_CPP(Class, Type, Variable) \
	Type Class##::Get##Variable##() const {return m_##Variable##;}

#define CHECKED_RELEASE(x) if(x) {x->Release(); x = 0;}
#define CHECKED_DELETE(x) if(x) {delete x; x = 0;}
#define CHECKED_DELETE_ARRAY(x) if(x) {delete[] x; x = 0;}

#define DEG2RAD(x) (x * (float)(3.14159265358979323846 / 180.0))

static void UtilsLog(const std::string& text)
{
// TODO: OPCIONAL esto va bien tenerlo escribiendo en un fichero
	OutputDebugString(("LOG: " + text + "\n").c_str());
}

std::vector<std::string> &SplitString(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> SplitString(const std::string &s, char delim);

#endif //_UTILS_H