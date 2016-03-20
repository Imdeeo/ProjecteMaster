#ifndef MATERIAL_PARAMETER_H
#define MATERIAL_PARAMETER_H

#include "Utils\Named.h"
#include "Utils\CEmptyPointerClass.h"
#include <string>

class CXMLTreeNode;
class CMaterial;

class CMaterialParameter : public CNamed
{
public:
	enum TMaterialType
	{
		FLOAT = 0,
		VECT2F,
		VECT3F,
		VECT4F
	};
protected:
	TMaterialType m_MaterialType;
public:
	CMaterialParameter(CMaterial *Material, CXMLTreeNode &TreeNode, CMaterialParameter::TMaterialType MaterialType) :CNamed(TreeNode),m_MaterialType(MaterialType){}
	virtual ~CMaterialParameter(){}
	virtual void Apply() = 0;
	virtual void * GetValueAddress() const = 0;
	TMaterialType getMaterialType()const{ return m_MaterialType;}
	virtual CEmptyPointerClass * GetValueLuaAddress() const {
		return (CEmptyPointerClass *)GetValueAddress();
	}

	static TMaterialType GetTypeFromString(std::string _SType);
	static std::string GetStringFromType(TMaterialType _Type);

	virtual void Save(FILE* _File, size_t _NTabs = 0);

	virtual const char* GetDescription() const = 0;
};

#endif //MATERIAL_PARAMETER_H