#ifndef TEMPLATED_MATERIAL_PARAMETER_H
#define TEMPLATED_MATERIAL_PARAMETER_H

#include <string.h>
#include "Materials\MaterialParameter.h"

template<typename T>
class CTemplatedMaterialParameter : public CMaterialParameter
{
private:
	T m_Value;
	void *m_EffectAddress;
	std::string m_Description;
public:
	CTemplatedMaterialParameter(CMaterial *Material, CXMLTreeNode &TreeNode, const T &Value, CMaterialParameter::TMaterialType MaterialType, std::string _Description):CMaterialParameter(Material,TreeNode,MaterialType)
	{
		m_EffectAddress = CEffectManager::AddMaterialParameter(MaterialType);
		m_Value = Value;
		m_Description = _Description;
	}
	virtual ~CTemplatedMaterialParameter(){}
	void Apply()
	{
		memcpy(m_EffectAddress,&m_Value,sizeof(T));
	}
	void * GetValueAddress(int index= 0) const { return (void*)((float*)&m_Value + index); }
	void SetValue(T _value) { m_Value = _value; }
	T GetValue() { return m_Value; }
	const char* GetDescription() const { return m_Description.c_str(); };
};

#endif //TEMPLATED_MATERIAL_PARAMETER_H