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

inline CColor CColor::HsvToRgb(HsvColor hsv)
{
	CColor rgb;
	unsigned char region, remainder, p, q, t;

	if (hsv.s == 0)
	{
		rgb.SetRed(hsv.v);
		rgb.SetGreen(hsv.v);
		rgb.SetBlue(hsv.v);
		return rgb;
	}

	region = hsv.h / 43;
	remainder = (hsv.h - (region * 43)) * 6;

	p = (hsv.v * (255 - hsv.s)) >> 8;
	q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
	t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

	rgb.SetAlpha(hsv.alpha);

	switch (region)
	{
	case 0:
		rgb.SetRed(hsv.v);
		rgb.SetGreen(t);
		rgb.SetBlue(p);
		break;
	case 1:
		rgb.SetRed(q);
		rgb.SetGreen(hsv.v);
		rgb.SetBlue(p);
		break;
	case 2:
		rgb.SetRed(p);
		rgb.SetGreen(hsv.v);
		rgb.SetBlue(t);
		break;
	case 3:
		rgb.SetRed(p);
		rgb.SetGreen(q);
		rgb.SetBlue(hsv.v);
		break;
	case 4:
		rgb.SetRed(t);
		rgb.SetGreen(p);
		rgb.SetBlue(hsv.v);
		break;
	default:
		rgb.SetRed(hsv.v);
		rgb.SetGreen(p);
		rgb.SetBlue(q);
		break;
	}

	rgb.SetRed(rgb.GetRed() > 0 ? rgb.GetRed() / 255 : 0);
	rgb.SetGreen(rgb.GetGreen() > 0 ? rgb.GetGreen() / 255 : 0);
	rgb.SetBlue(rgb.GetBlue() > 0 ? rgb.GetBlue() / 255 : 0);
	rgb.SetAlpha(rgb.GetAlpha() > 0 ? rgb.GetAlpha() / 255 : 0);

	return rgb;
}

inline CColor::HsvColor CColor::RgbToHsv(CColor rgb)
{
	HsvColor hsv;
	unsigned char rgbMin, rgbMax;

	int l_R = (int)rgb.GetRed() * 255;
	int l_G = (int)rgb.GetGreen() * 255;
	int l_B = (int)rgb.GetBlue() * 255;
	int l_Alpha = (int)rgb.GetAlpha() * 255;

	rgbMin = l_R < l_G ? (l_R < l_B ? l_R : l_B) : (l_G < l_B ? l_G : l_B);
	rgbMax = l_R > l_G ? (l_R > l_B ? l_R : l_B) : (l_G > l_B ? l_G : l_B);

	hsv.v = rgbMax;
	if (hsv.v == 0)
	{
		hsv.h = 0;
		hsv.s = 0;
		return hsv;
	}

	hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
	if (hsv.s == 0)
	{
		hsv.h = 0;
		return hsv;
	}

	if (rgbMax == l_R)
		hsv.h = 0 + 43 * (l_G - l_B) / (rgbMax - rgbMin);
	else if (rgbMax == l_G)
		hsv.h = 85 + 43 * (l_B - l_R) / (rgbMax - rgbMin);
	else
		hsv.h = 171 + 43 * (l_R - l_G) / (rgbMax - rgbMin);

	hsv.alpha = l_Alpha;

	return hsv;
}

inline CColor CColor::interpolate(CColor a, CColor b, float t)
{
	HsvColor ca = RgbToHsv(a);
	HsvColor cb = RgbToHsv(b);
	HsvColor final;

	final.h = linear(ca.h, cb.h, t);
	final.s = linear(ca.s, cb.s, t);
	final.v = linear(ca.v, cb.v, t);

	return HsvToRgb(final);
}

inline int CColor::linear(int a, int b, float t)
{
	return (int)(a * (1 - t) + b * t);
}