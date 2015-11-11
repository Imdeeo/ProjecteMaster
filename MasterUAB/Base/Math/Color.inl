#include "MathUtils.h"

inline CColor& CColor::Clamp()
{
	x = mathUtils::Min(mathUtils::Max(x, 0.0f), 1.0f);
	y = mathUtils::Min(mathUtils::Max(y, 0.0f), 1.0f);
	z = mathUtils::Min(mathUtils::Max(z, 0.0f), 1.0f);
	w = mathUtils::Min(mathUtils::Max(w, 0.0f), 1.0f);

	return (*this);
}

///
/// Construye un uint32 que codifica el color como 0xAARRGGBB (PC)
///
inline uint32_t CColor::GetUint32Argb() const
{
	return  (uint32_t)(FloatToByte((*this)[3]) << 24)
		+ (FloatToByte((*this)[0])<<16)
		+ (FloatToByte((*this)[1])<<8) 
		+  FloatToByte((*this)[2]);
}

///
/// Construye un uint32 que codifica el color como 0xAABBGGRR (PC)
///
inline uint32_t CColor::GetUint32Abgr() const
{
	return  (uint32_t)(FloatToByte((*this)[3]) << 24)
		+ (FloatToByte((*this)[2])<<16)
		+ (FloatToByte((*this)[1])<<8) 
		+  FloatToByte((*this)[0]);
}

///
/// Asigna el color a partir de un uint32 en formato  0xAARRGGBB
///
inline void	CColor::SetUint32Argb(int32_t argb)
{
	(*this)[3] = ByteToFloat(static_cast<uint8_t>((argb >> 24))); // alpha
	(*this)[0] = ByteToFloat(static_cast<uint8_t>(0xff & (argb >> 16))); // red
	(*this)[1] = ByteToFloat(static_cast<uint8_t>(0xff & (argb >> 8))); // green
	(*this)[2] = ByteToFloat(static_cast<uint8_t>(0xff & (argb))); // blue
}

///
/// Asigna el color a partir de un uint32 en formato  0xAABBGGRR
///
inline void	CColor::SetUint32Abgr(int32_t abgr)
{
	(*this)[3] = ByteToFloat(static_cast<uint8_t>((abgr >> 24))); // alpha
	(*this)[2] = ByteToFloat(static_cast<uint8_t>(0xff & (abgr >> 16))); // blue
	(*this)[1] = ByteToFloat(static_cast<uint8_t>(0xff & (abgr >> 8))); // green
	(*this)[0] = ByteToFloat(static_cast<uint8_t>(0xff & (abgr))); // red
}


// Funci??n de utilidad para convertir una componente de color en coma flotante a su versi??n de un byte (0..255)
inline uint8_t CColor::FloatToByte(float fValue) const
{ 
  int i = static_cast<int>(0xff * fValue); 
	i = mathUtils::Clamp(i, 0, 0xff); 

	return static_cast<uint8_t>(i);
}

// Funci??n de utilidad para convertir un byte con una componente de color (0..255) a componente float (0.0..1.0)
inline float CColor::ByteToFloat(uint8_t uValue) const
{ 
  return static_cast<float>(uValue)/255.0f; 
}

inline CColor& CColor::Lerp(const CColor& other, float f)
{
	Vect4f::Lerp(other.GetArgb(), f);

	return *this;
}

// Operadores aritméticos   
inline CColor CColor::operator + (const CColor& otro)	const
{
	return CColor((Vect4f)otro + (Vect4f)(*this));
}

inline CColor CColor::operator - (const CColor& otro)	const
{
	return CColor((Vect4f)(*this) - (Vect4f)otro);
}

inline CColor CColor::operator * (const CColor& otro)	const
{
	return CColor(otro.x * x,
				  otro.y * y,
				  otro.z * z,
				  otro.w * w);
}

inline CColor CColor::operator * (float escalar) const
{
	return CColor((Vect4f)(*this) * escalar);
}

// Operadores aritméticos de actualización   
inline CColor& CColor::operator += (const CColor& otro)
{
	(Vect4f)(*this) += (Vect4f)otro;
	
	return (*this);
}

inline CColor& CColor::operator -= (const CColor& otro)
{
	(Vect4f)(*this) -= (Vect4f)otro;

	return (*this);
}

inline CColor& CColor::operator *= (const CColor& otro)
{
	x *= otro.x;
	y *= otro.y;
	z *= otro.z;
	w *= otro.w;

	return (*this);
}

inline CColor& CColor::operator *= (float escalar)
{
	(Vect4f)(*this) *= escalar;

	return (*this);
}
