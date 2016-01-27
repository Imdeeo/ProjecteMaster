#ifndef MATERIAL_PARAMETER_H
#define MATERIAL_PARAMETER_H

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
	CMaterialParameter(CMaterial *Material, CXMLTreeNode &TreeNode,
		CMaterialParameter::TMaterialType MaterialType);
	virtual ~CMaterialParameter();
	virtual void Apply() = 0;
	virtual void * GetValueAddress() const = 0;
	UAB_GET_STANDARD_PROPERTY(TMaterialType, MaterialType);
	virtual CEmptyPointerClass * GetValueLuaAddress() const {
		return
			(CEmptyPointerClass *)GetValueAddress();
	}
};

#endif //MATERIAL_PARAMETER_H