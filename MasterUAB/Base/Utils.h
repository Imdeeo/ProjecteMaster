﻿#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>

#include "Math\Quatn.h"
#include "Math\MathUtils.h"
#include "no_sillywarnings_please.h"

#define MAXBONES 150
#define MAX_LIGHTS_BY_SHADER 4
#define UAB_ID_SHADOW_MAP 0
#define MAX_PARTICLE_PER_INSTANCE 200
#define MAX_BILBOARDS_PER_INSTANCE 50
#define MAX_LINE_LENGHT 20
#define MAX_MANCHAS 10
#define MAX_VERTICES_PER_CALL 200
#define RIGHT_OBJECT_BONE_ID 39
#define LEFT_OBJECT_BONE_ID 20
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define HALF_WIDTH 640
#define HALF_HEIGHT 360

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
#ifdef _DEBUG 
	OutputDebugString(("LOG: " + text + "\n").c_str());
#endif
}

static void UtilsLogV2(const Vect2f& vect)
{
#ifdef _DEBUG 
	OutputDebugString(("LOG: Vector2(" + std::to_string(vect.x) + ", " + std::to_string(vect.y) + ")\n").c_str());
#endif
}

static void UtilsLogV3(const Vect3f& vect)
{
#ifdef _DEBUG 
	OutputDebugString(("LOG: Vector3(" + std::to_string(vect.x) + ", " + std::to_string(vect.y) + ", " + std::to_string(vect.z) + ")\n").c_str());
#endif
}

static void UtilsLogQ(const Quatf& quat)
{
#ifdef _DEBUG 
	OutputDebugString(("LOG: Quaternion(" + std::to_string(quat.x) + ", " + std::to_string(quat.y) + ", " + std::to_string(quat.z) + ", " + std::to_string(quat.w) + ")\n").c_str());
#endif
}

std::vector<std::string> &SplitString(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> SplitString(const std::string &s, char delim);

static void InfoMessage(const char * format, ...)
{
	va_list args;
	char* buffer;

	va_start(args, format);
	int len = _vscprintf(format, args) + 1;

	buffer = (char*)malloc(len*sizeof(char));
	vsprintf(buffer, format, args);

	::MessageBox(NULL, buffer, "ATTENTION", MB_OK);

	free(buffer);
}

#endif //_UTILS_H