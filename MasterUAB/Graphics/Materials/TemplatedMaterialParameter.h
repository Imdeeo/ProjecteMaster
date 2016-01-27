#ifndef TEMPLATED_MATERIAL_PARAMETER_H
#define TEMPLATED_MATERIAL_PARAMETER_H

template<typename T>
class CTemplatedMaterialParameter : public CMaterialParameter
{
private:
	T m_Value;
	void *m_EffectAddress;
public:
	CTemplatedMaterialParameter(CMaterial *Material, CXMLTreeNode &TreeNode,
		const T &Value, CMaterialParameter::TMaterialType MaterialType);
	virtual ~CTemplatedMaterialParameter();
	void Apply();
	void * GetValueAddress() const;
};

#endif //TEMPLATED_MATERIAL_PARAMETER_H