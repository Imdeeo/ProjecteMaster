//-----------------------------------------------------------------------
// CColor class
/// Describe un color. Internamente implementado como un Vect4f que almacena los valores de rojo verde azul y
/// alfa como valores en coma flotante. Cada uno de ellos con valores de 0.0f a 1.0f. En el caso de las
/// componentes rojo verde y azul, un 0.0f indica ausencia de esa componente y de 1.0f indica la cantidad
/// maxima. Un alfa de 1.0f indica color opaco y uno de .0f indica que es completamente transparente.
//-----------------------------------------------------------------------
#ifndef COLOR_H_
#define COLOR_H_

#include "Vector4.h"

class CColor : public Vect4f
{

public:
  // ------------
  // Construcci??n
  // ------------
  explicit CColor()															:Vect4f(1.0f, 1.0f, 1.0f, 1.0f)  {}
  explicit CColor (const Vect4f& vect)										:Vect4f(vect)                    {}
  explicit CColor(float red, float green, float blue, float alpha = 1.0f)	:Vect4f(red, green, blue, alpha) {}
	//inline   CColor (uint32 argb);

  // --------------------
  // Acceso a componentes
  // --------------------

  inline float         GetRed         ()                   const { return x; }
  inline void          SetRed         (float red)                { x = red; }

  inline float         GetGreen       ()                   const { return y; }
  inline void          SetGreen       (float green)              { y = green; }

  inline float         GetBlue        ()                   const { return z; }
  inline void          SetBlue        (float blue)               { z = blue; }

  inline float         GetAlpha       ()                   const { return w; }
  inline void          SetAlpha       (float alpha)              { w = alpha; }

  inline const Vect4f& GetArgb        ()                   const { return *this; }
  inline void          SetArgb(const Vect4f& argb)       { *this = (CColor)argb; }

  inline uint32_t        GetUint32Abgr()										const;
  inline uint32_t        GetUint32Argb()										const;

  inline void          SetUint32Argb(int32_t argb);
  inline void          SetUint32Abgr(int32_t argb);

	inline CColor&			 Clamp();

	inline CColor&			 Lerp(const CColor& other, float f);

	// Operadores aritméticos   
	inline        CColor  operator + (const CColor& otro)	const;
	inline        CColor  operator - (const CColor& otro)	const;
	inline        CColor  operator * (const CColor& otro)	const;
	inline        CColor  operator * (float escalar)			const;

	// Operadores aritméticos de actualización   
	inline CColor& operator += (const CColor& otro);
	inline CColor& operator -= (const CColor& otro);
	inline CColor& operator *= (const CColor& otro);
	inline CColor& operator *= (float escalar);

private:
	inline uint8_t FloatToByte(float fValue) const;
	inline float ByteToFloat(uint8_t uValue) const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// incluimos las funciones inline
#include "Color.inl"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// CONSTANTES DE COLOR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const CColor colBLACK;
extern const CColor colWHITE;
extern const CColor colRED;
extern const CColor colGREEN;
extern const CColor colBLUE;
extern const CColor colMAGENTA;
extern const CColor colYELLOW;
extern const CColor colCYAN;


#endif
