#ifndef MATERIAL_PARAMETER_H
#define MATERIAL_PARAMETER_H

#include "Utils\Named.h"
#include "Utils\CEmptyPointerClass.h"
#include "XML\tinyxml2.h"
#include <string>

class CMaterial;

class CMaterialParameter : public CNamed
{
public:
	enum TMaterialType
	{
		FLOAT = 0,
		VECT2F,
		VECT3F,
		VECT4F,
		COLOR
	};
protected:
	TMaterialType m_MaterialType;
public:
	CMaterialParameter(CMaterial *Material, tinyxml2::XMLElement* TreeNode, CMaterialParameter::TMaterialType MaterialType) :CNamed(TreeNode), m_MaterialType(MaterialType){}
	virtual ~CMaterialParameter(){}
	virtual void Apply() = 0;
	virtual void * GetValueAddress(int index = 0) const = 0;
	TMaterialType getMaterialType()const{ return m_MaterialType;}
	virtual CEmptyPointerClass * GetValueLuaAddress(int index = 0) const {
		return (CEmptyPointerClass *)GetValueAddress(index);
	}

	static TMaterialType GetTypeFromString(std::string _SType);
	static std::string GetStringFromType(TMaterialType _Type);

	virtual void Save(FILE* _File, size_t _NTabs = 0);

	virtual const char* GetDescription() const = 0;
};

#endif //MATERIAL_PARAMETER_H