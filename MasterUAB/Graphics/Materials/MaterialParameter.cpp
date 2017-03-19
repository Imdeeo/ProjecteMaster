#include "MaterialParameter.h"

#include "Math\Vector4.h"
#include "Math\Color.h"

CMaterialParameter::TMaterialType CMaterialParameter::GetTypeFromString(std::string _SType)
{
	if(_SType == "float2")
	{
		return VECT2F;
	}
	if(_SType == "float3")
	{
		return VECT3F;
	}
	if(_SType == "float4")
	{
		return VECT4F;
	}
	if(_SType == "float")
	{
		return FLOAT;
	}
	if (_SType == "color")
	{
		return COLOR;
	}

	return FLOAT;
}

std::string CMaterialParameter::GetStringFromType(CMaterialParameter::TMaterialType _Type)
{
	if (_Type == VECT2F)
	{
		return "float2";
	}
	if (_Type == VECT3F)
	{
		return "float3";
	}
	if (_Type == VECT4F)
	{
		return "float4";
	}
	if (_Type == FLOAT)
	{
		return "float";
	}
	if (_Type == COLOR)
	{
		return "color";
	}

	return "float";
}


void CMaterialParameter::Save(FILE* _File, size_t _NTabs)
{
	for (size_t i = 0; i < _NTabs; i++)
	{
		fprintf_s(_File, "\t");
	}
	if (m_MaterialType == VECT2F)
	{
		fprintf_s(_File, "<parameter type=\"float2\" name=\"%s\" value=\"%f %f\" description=\"%s\"/>\n"
			,m_Name.c_str()
			,((Vect2f*)GetValueAddress())->x
			,((Vect2f*)GetValueAddress())->y
			,GetDescription());
	}
	if (m_MaterialType == VECT3F)
	{
		fprintf_s(_File, "<parameter type=\"float3\" name=\"%s\" value=\"%f %f %f\" description=\"%s\"/>\n", m_Name.c_str()
			, ((Vect3f*)GetValueAddress())->x
			, ((Vect3f*)GetValueAddress())->y
			, ((Vect3f*)GetValueAddress())->z
			, GetDescription());
	}
	if (m_MaterialType == VECT4F)
	{
		fprintf_s(_File, "<parameter type=\"float4\" name=\"%s\" value=\"%f %f %f %f\" description=\"%s\"/>\n", m_Name.c_str()
			, ((Vect4f*)GetValueAddress())->x
			, ((Vect4f*)GetValueAddress())->y
			, ((Vect4f*)GetValueAddress())->z
			, ((Vect4f*)GetValueAddress())->w
			, GetDescription());
	}
	if (m_MaterialType == FLOAT)
	{
		fprintf_s(_File, "<parameter type=\"float\" name=\"%s\" value=\"%f\" description=\"%s\"/>\n", m_Name.c_str()
			, *((float*)GetValueAddress())
			, GetDescription());
	}
	if (m_MaterialType == COLOR)
	{
		fprintf_s(_File, "<parameter type=\"color\" name=\"%s\" value=\"%f %f %f %f\" description=\"%s\"/>\n", m_Name.c_str()
			, ((CColor*)GetValueAddress())->x
			, ((CColor*)GetValueAddress())->y
			, ((CColor*)GetValueAddress())->z
			, ((CColor*)GetValueAddress())->w
			, GetDescription());
	}
}