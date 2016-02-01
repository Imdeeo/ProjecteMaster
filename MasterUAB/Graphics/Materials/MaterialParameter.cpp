#include "MaterialParameter.h"

CMaterialParameter::TMaterialType CMaterialParameter::GetTypeFromString(std::string _SType)
{
	if(_SType == "Vector2")
	{
		return VECT2F;
	}
	if(_SType == "Vector3")
	{
		return VECT3F;
	}
	if(_SType == "Vector4")
	{
		return VECT4F;
	}
	if(_SType == "float")
	{
		return FLOAT;
	}
	return FLOAT;
}
