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
	float hh, p, q, t, ff;
	long i;
	CColor rgb;

	rgb.SetAlpha(hsv.alpha);

	if (hsv.s <= 0.0) {       // < is bogus, just shuts up warnings
		rgb.SetRed(hsv.v);
		rgb.SetGreen(hsv.v);
		rgb.SetBlue(hsv.v);
		return rgb;
	}
	hh = hsv.h;
	if (hh >= 360.0) hh = 0.0f;
	hh /= 60.0f;
	i = (long)hh;
	ff = hh - i;
	p = hsv.v * (1.0f - hsv.s);
	q = hsv.v * (1.0f - (hsv.s * ff));
	t = hsv.v * (1.0f - (hsv.s * (1.0f - ff)));

	switch (i) {
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
	case 5:
	default:
		rgb.SetRed(hsv.v);
		rgb.SetGreen(p);
		rgb.SetBlue(q);
		break;
	}
	return rgb;
}

inline CColor::HsvColor CColor::RgbToHsv(CColor rgb)
{
	HsvColor hsv;
	float min, max, delta;

	hsv.alpha = rgb.GetAlpha();
	
	min = rgb.GetRed() < rgb.GetGreen() ? rgb.GetRed() : rgb.GetGreen();
	min = min  < rgb.GetBlue() ? min : rgb.GetBlue();

	max = rgb.GetRed() > rgb.GetGreen() ? rgb.GetRed() : rgb.GetGreen();
	max = max  > rgb.GetBlue() ? max : rgb.GetBlue();

	hsv.v = max;                                // v
	delta = max - min;
	if (delta < 0.00001)
	{
		hsv.s = 0.0f;
		hsv.h = 0.0f; // undefined, maybe nan?
		return hsv;
	}
	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		hsv.s = (delta / max);                  // s
	}
	else {
		// if max is 0, then r = g = b = 0              
		// s = 0, v is undefined
		hsv.s = 0.0f;
		hsv.h = NAN;                            // its now undefined
		return hsv;
	}
	if (rgb.GetRed() >= max)                           // > is bogus, just keeps compilor happy
		hsv.h = (rgb.GetGreen() - rgb.GetBlue()) / delta;        // between yellow & magenta
	else
		if (rgb.GetGreen() >= max)
			hsv.h = 2.0f + (rgb.GetBlue() - rgb.GetRed()) / delta;  // between cyan & yellow
		else
			hsv.h = 4.0f + (rgb.GetRed() - rgb.GetGreen()) / delta;  // between magenta & cyan

	hsv.h *= 60.0f;                              // degrees

	if (hsv.h < 0.0)
		hsv.h += 360.0f;

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
	final.alpha = linear(ca.alpha, cb.alpha, t);

	return HsvToRgb(final);
}

inline float CColor::linear(float a, float b, float t)
{
	return (a * (1 - t) + b * t);
}