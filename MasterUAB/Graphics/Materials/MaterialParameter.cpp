#include "MaterialParameter.h"

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
	return FLOAT;
}
