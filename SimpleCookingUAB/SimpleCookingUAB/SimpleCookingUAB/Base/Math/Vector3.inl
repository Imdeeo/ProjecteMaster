//-----------------------------------------------------------------------
// Vector3 inline
/// Definiciones de funciones inline de la clase 'Vector3'
/// Este fichero es realmente parte de la cabecera 'Vector3.h' 
//-----------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versi�n 1
/// Construcci�n sin inicializaci�n de par�metros  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>::Vector3 ()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versi�n 2
/// Constructor de copia
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>::Vector3 (const Vector3<T>& otro)
{
  x = otro.x;
  y = otro.y;
  z = otro.z;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versi�n 5
/// A partir de 3 escalares
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>::Vector3 (const T tx, const T ty, const T tz)
{
  x = tx;
  y = ty;
  z = tz;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versi�n 6
/// A partir de 1 escalar, que se establecer� en las 3 componentes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>::Vector3 (const T escalar)
{
  x = escalar;
  y = escalar;
  z = escalar;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: suma de vectores
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::operator + (const Vector3<T>& otro) const
{   
  return (Vector3<T>(x + otro.x,
                     y + otro.y,
                     z + otro.z));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: resta de vectores
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::operator - (const Vector3<T>& otro) const
{   
  return (Vector3<T>(x - otro.x,
                     y - otro.y,
                     z - otro.z));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: producto vector x escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::operator * (const T escalar) const
{
  return (Vector3<T>(x * escalar,
                     y * escalar,
                     z * escalar));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa operador aritm�tico: producto escalar x vector 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> operator * (const T escalar, const Vector3<T>& otro)
{
  return (Vector3<T>(otro.x * escalar,
                     otro.y * escalar,
                     otro.z * escalar));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: cociente vector / escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::operator / (const T escalar) const
{ 
#ifdef CHECK_MATH_SINGULARITY
  //--------------------------------------------------<<<
  // Con chequeo de divisi�n por cero
  Vector3<T> retVect(*this);

  ASSERTMSG(escalar != Zero<T>(), "Divisi�n por cero en cociente vector-escalar");
  if(escalar != Zero<T>())
  {
    T inv_escalar = One<T>() / escalar;
    retVect.x *= inv_escalar;
    retVect.y *= inv_escalar;
    retVect.z *= inv_escalar;
  }

  return retVect;
  //-------------------------------------------------->>>
#else
  //--------------------------------------------------<<<
  // Sin chequeo
  T inv_escalar = One<T>() / escalar;

  return (Vector3<T>(x * inv_escalar,
                     y * inv_escalar,
                     z * inv_escalar));
  //--------------------------------------------------<<<
#endif 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa operador aritm�tico: cociente escalar / vector
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> operator / (const T escalar, const Vector3<T>& otro)
{ 
#ifdef CHECK_MATH_SINGULARITY
  //--------------------------------------------------<<<
  // Con chequeo de divisi�n por cero
  Vector3<T> retVect(escalar);

  ASSERTMSG(otro.x != Zero<T>() &&
            otro.y != Zero<T>() &&
            otro.z != Zero<T>(),
            "Divisi�n por cero en cociente escalar-vector");
  if(otro.x != Zero<T>())
  {
    retVect.x /= otro.x;
  }
  if(otro.y != Zero<T>())
  {
    retVect.y /= otro.y;
  }
  if(otro.z != Zero<T>())
  {
    retVect.z /= otro.z;
  }

  return retVect;
  //-------------------------------------------------->>>
#else
  //--------------------------------------------------<<<
  // Sin chequeo
  return (Vector3<T>(escalar / otro.x,
                     escalar / otro.y,
                     escalar / otro.z)); 
  //-------------------------------------------------->>>
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: suma de un escalar a las tres componentes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::operator + (const T escalar) const
{
  return Vector3<T>(x + escalar,
                    y + escalar,
                    z + escalar);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa operador aritm�tico: suma de un escalar
/// a las tres componentes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> operator + (const T escalar, const Vector3<T>& vector)
{
  return Vector3<T>(vector.x + escalar,
                    vector.y + escalar,
                    vector.z + escalar);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: resta de un escalar a las tres componentes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::operator - (const T escalar) const
{
  return Vector3<T>(x - escalar,
                    y - escalar,
                    z - escalar);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa operador aritm�tico: resta de las componentes a
/// un escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> operator - (const T escalar, const Vector3<T>& vector)
{
  return Vector3<T>(escalar - vector.x,
                    escalar - vector.y,
                    escalar - vector.z);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: cambio de signo
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::operator - () const
{   
  return (Vector3<T>(-x, -y, -z));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: signo positivo
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::operator + ()
{
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: signo positivo (versi�n const)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Vector3<T>& Vector3<T>::operator + () const
{
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: incremento
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::operator += (const Vector3<T>& otro)
{ 
  x += otro.x;
  y += otro.y;
  z += otro.z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: decremento
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::operator -= (const Vector3<T>& otro)
{
  x -= otro.x;
  y -= otro.y;
  z -= otro.z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: vector x escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::operator *= (const T escalar)
{ 
  x *= escalar;
  y *= escalar;
  z *= escalar;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: vector / escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::operator /= (const T escalar)
{ 
#ifdef CHECK_MATH_SINGULARITY
  //------------------------------------------<<<
  // Con chequeo de divisi�n por cero
  ASSERTMSG(escalar != Zero<T>(), "Divisi�n por cero en cociente vector-escalar");
  if(escalar != Zero<T>())
  {
    T inv_escalar = One<T>() / escalar;
    x *= inv_escalar;
    y *= inv_escalar;
    z *= inv_escalar;
  }
  //------------------------------------------>>>
#else
  //------------------------------------------<<<
  // Sin chequeo
  T inv_escalar = One<T>() / escalar;
  x *= inv_escalar;
  y *= inv_escalar;
  z *= inv_escalar;
  //------------------------------------------>>>
#endif

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: vector + escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::operator += (const T escalar)
{
  x += escalar;
  y += escalar;
  z += escalar;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: vector - escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::operator -= (const T escalar)
{
  x -= escalar;
  y -= escalar;
  z -= escalar;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de asignaci�n de componentes par�ntesis
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::operator () (const T tx, const T ty, const T tz)
{   
  x = tx;
  y = ty;
  z = tz;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n de asignaci�n de componentes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Vector3<T>::Set (const T tx, const T ty, const T tz)
{
  x = tx;
  y = ty;
  z = tz;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece el vector [0, 0, 0]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Vector3<T>::SetZero ()
{
  x = Zero<T>();
  y = Zero<T>();
  z = Zero<T>();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece el vector [0, 0, 0]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline float Vector3<T>::GetModule()
{
	return x*x + y*y + z*z;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece el valor del vector a partir de coordenadas polares
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Vector3<T>::SetFromPolar(const T longitude, const T latitude, const T length)
{
  T cos_lat;
  mathUtils::SinCos(latitude, z, cos_lat);
  mathUtils::SinCos(longitude, y, x);
  
  T k_length = length * cos_lat;
 
  x *= k_length;
  y *= k_length;
  z *= length;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene los datos del vector en forma de coordenadas polares
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Vector3<T>::GetPolar(T& longitude, T& latitude, T& length) const
{
  length    = Length();
  latitude  = mathUtils::ASin(z / length);
  longitude = (T)atan2(y, x);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador vectorial: producto escalar (dot product)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Vector3<T>::operator * (const Vector3<T>& otro) const
{
  return (x * otro.x + y * otro.y + z * otro.z);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador vectorial: producto vectorial (cross product)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::operator ^ (const Vector3<T>& otro) const
{
  return Vector3<T>((y * otro.z) - (z * otro.y),
                    (z * otro.x) - (x * otro.z),
                    (x * otro.y) - (y * otro.x));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de igualdad absoluto
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Vector3<T>::operator == (const Vector3<T>& otro) const
{   
  return (x == otro.x &&
          y == otro.y &&
          z == otro.z);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de desigualdad absoluto
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Vector3<T>::operator != (const Vector3<T>& otro) const
{   
    return (x != otro.x ||
            y != otro.y ||
            z != otro.z );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n de igualdad con epsilon global
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Vector3<T>::IsEqualEpsilon (const Vector3<T>& otro, const T Epsilon) const
{
  return (mathUtils::EqualEpsilon(x, otro.x, Epsilon) &&
          mathUtils::EqualEpsilon(y, otro.y, Epsilon) &&
          mathUtils::EqualEpsilon(z, otro.z, Epsilon));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n de desigualdad con epsilon global
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool Vector3<T>::IsNotEqualEpsilon (const Vector3<T>& otro, const T Epsilon) const
{
  return (!mathUtils::EqualEpsilon(x, otro.x, Epsilon) ||
          !mathUtils::EqualEpsilon(y, otro.y, Epsilon) ||
          !mathUtils::EqualEpsilon(z, otro.z, Epsilon));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Producto por componentes (escalado). Versi�n que modifica el vector (no const).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::Scale (const Vector3<T>& otro)
{
  x *= otro.x;
  y *= otro.y;
  z *= otro.z;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Producto por componentes (escalado). Versi�n que devuelve un vector modificado (const).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::GetScaled (const Vector3<T>& otro) const
{
  return Vector3<T>(x * otro.x, y * otro.y, z * otro.z);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Cambia cada componente del vector actual por la correspondiente del vector pasado en el par�metro si
/// esta es menor (su tipo devuelve 'true' al operador '<')
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Vector3<T>& Vector3<T>::SetIfMinComponents (const Vector3<T>& otro)
{
  if(otro.x < x)
  {
    x = otro.x;
  }
  if(otro.y < y)
  {
    y = otro.y;
  }
  if(otro.z < z)
  {
    z = otro.z;
  }

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Cambia cada componente del vector actual por la correspondiente del vector pasado en el par�metro si
/// esta es mayor (su tipo devuelve 'true' al operador '>')
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Vector3<T>& Vector3<T>::SetIfMaxComponents (const Vector3<T>& otro)
{
  if(otro.x > x)
  {
    x = otro.x;
  }
  if(otro.y > y)
  {
    y = otro.y;
  }
  if(otro.z > z)
  {
    z = otro.z;
  }

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de acceso []
/// (Versi�n const)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Vector3<T>::operator [] (int i) const
{
  return ((T*)this)[i];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de acceso []
/// (Versi�n no const)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T& Vector3<T>::operator [] (int i)
{
  return ((T*)this)[i];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Proyecci�n sobre el plano XY
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> Vector3<T>::GetProjXY () const
{
  return (Vector2<T>(x, y));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Proyecci�n sobre el plano YZ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> Vector3<T>::GetProjYZ () const
{
  return (Vector2<T>(y, z));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Proyecci�n sobre el plano XZ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> Vector3<T>::GetProjZX () const
{
  return (Vector2<T>(z, x));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Longitud
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Vector3<T>::Length () const
{
  return ((T)sqrt(x * x + y * y + z * z));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Longitud al cuadrado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Vector3<T>::SquaredLength () const
{
  return (x * x + y * y + z * z);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Vector3<T>::Distance(const Vector3<T>& rhs) const
{
  Vector3<T> diff(*this - rhs);
  return diff.Length();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Vector3<T>::SqDistance(const Vector3<T>& rhs) const
{
  Vector3<T> diff(*this - rhs);
  return diff.SquaredLength();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n de normalizaci�n
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::Normalize (const T tk)
{
	#ifdef CHECK_MATH_SINGULARITY
		//------------------------------------------<<<
		// Con chequeo de divisi�n por cero
		T length = Length();

		ASSERTMSG(length != Zero<T>(), "Divisi�n por cero en normalizaci�n de vector");
		if(length != Zero<T>())
		{
			T aux = tk / length;
			x *= aux;
			y *= aux;
			z *= aux;
		}
		//------------------------------------------>>>
	#else
		//------------------------------------------<<<
		// Sin chequeo
		T aux = tk / Length();
		x *= aux;
		y *= aux;
		z *= aux;
		//------------------------------------------>>>
	#endif    

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve el vector normalizado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::GetNormalized (const T tk) const
{
	#ifdef CHECK_MATH_SINGULARITY
		//-----------------------------------------------------<<<
		// Con chequeo de divisi�n por cero
		Vector3<T> retVect(*this);

		T length = Length();
		ASSERTMSG(length != Zero<T>(), "Divisi�n por cero en normalizaci�n de vector");
		if(length != Zero<T>())
		{
			T aux = tk / length;
			retVect.x *= aux;
			retVect.y *= aux;
			retVect.z *= aux;
		}

		return retVect;
		//----------------------------------------------------->>>
	#else
		//-----------------------------------------------------<<<
		// Sin chequeo
		T aux = tk / Length();
		return (Vector3<T>(x * aux,
											 y * aux,
											 z * aux));
		//----------------------------------------------------->>>
	#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Rotaci�n del vector alrededor del eje X
/// Se usa la regla del pulgar de la mano derecha (con el pulgar en la direcci�n del eje de rotaci�n,
/// la palma rodeando al eje indica el sentido de la rotaci�n positiva).\n
/// En este caso, los �ngulos ir�n desde el eje Y al Z por el lado corto.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::RotateX (const T angle)
{
  Vector2<T> auxVector = GetProjYZ().Rotate(angle);
  y = auxVector.x;
  z = auxVector.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector rotado alrededor del eje X respecto a este
/// (ver comentario en RotateX)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::GetRotatedX (const T angle) const
{
  return (Vector3<T>(*this).RotateX(angle));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Rotaci�n del vector alrededor del eje Y
/// Se usa la regla del pulgar de la mano derecha (con el pulgar en la direcci�n del eje de rotaci�n,
/// la palma rodeando al eje indica el sentido de la rotaci�n positiva).\n
/// En este caso, los �ngulos ir�n desde el eje Z al X por el lado corto.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::RotateY (const T angle)
{
  Vector2<T> auxVector = GetProjZX().Rotate(angle);
  z = auxVector.x;
  x = auxVector.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector rotado alrededor del eje Y respecto a este
/// (Ver comentario en RotateY)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::GetRotatedY (const T angle) const
{
  return (Vector3<T>(*this).RotateY(angle));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector rotado alrededor del eje Z respecto a este
/// Se usa la regla del pulgar de la mano derecha (con el pulgar en la direcci�n del eje de rotaci�n,
/// la palma rodeando al eje indica el sentido de la rotaci�n positiva).\n
/// En este caso, los �ngulos ir�n desde el eje X al Y por el lado corto.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::RotateZ (const T angle)
{
  Vector2<T> auxVector = GetProjXY().Rotate(angle);
  x = auxVector.x;
  y = auxVector.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector rotado alrededor del eje Z respecto a este
/// (Ver comentario en RotateZ)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::GetRotatedZ (const T angle) const
{
  return (Vector3<T>(*this).RotateZ(angle));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el angulo del vector alrededor del eje X. Origen (0,1,0), sentido antihorario ---> 'inclinaci�n'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Vector3<T>::GetAngleX (void) const
{ 
  return GetProjYZ().GetAngle();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el angulo del vector alrededor del eje Y. Origen (0,0,1), sentido antihorario ---> 'alabeo'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Vector3<T>::GetAngleY (void) const
{
  return GetProjZX().GetAngle();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene el angulo del vector alrededor del eje Z. Origen (1,0,0), sentido antihorario ---> 'giro'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Vector3<T>::GetAngleZ (void) const
{
  return GetProjXY().GetAngle();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene los 3 angulos del vector con los ejes:
/// X = el angulo del vector alrededor del eje X. Origen (0,1,0), sentido antihorario         ---> 'inclinaci�n'
/// Y = Obtiene el angulo del vector alrededor del eje Y. Origen (0,0,1), sentido antihorario ---> 'alabeo'
/// Z = Obtiene el angulo del vector alrededor del eje Z. Origen (1,0,0), sentido antihorario ---> 'giro'
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void Vector3<T>::GetAngles(T& angX, T& angY, T& angZ) const
{
  angX = GetAngleX();
  angY = GetAngleY();
  angZ = GetAngleZ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Interpolaci�n lineal entre el vector actual y el pasado en el par�metro
/// usando t como 'lambda', teniendo que si:
///    t=0 --> actual
///    t=1 --> otro
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T>& Vector3<T>::Lerp (const Vector3<T>& otro, const T t)
{
  x += (otro.x - x) * t;
  y += (otro.y - y) * t;
  z += (otro.z - z) * t;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Interpolaci�n lineal entre el vector actual y el pasado en el par�metro
/// usando t como 'lambda', teniendo que si:
///    t=0 --> actual
///    t=1 --> otro
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> Vector3<T>::GetLerp (const Vector3<T>& otro, const T t) const
{
  return Vector3<T>(*this).Lerp(otro, t);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa: Devuelve un vector con las componentes m�nimas de
/// los pasados en los par�metros
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> VectorMinComponents (const Vector3<T>& uno, const Vector3<T>& otro)
{
  return Vector3<T>(mathUtils::Min(uno.x, otro.x),
                    mathUtils::Min(uno.y, otro.y),
                    mathUtils::Min(uno.z, otro.z));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa: Devuelve un vector con las componentes m�ximas de
/// los pasados en los par�metros
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector3<T> VectorMaxComponents (const Vector3<T>& uno, const Vector3<T>& otro)
{
  return Vector3<T>(mathUtils::Max(uno.x, otro.x),
                    mathUtils::Max(uno.y, otro.y),
                    mathUtils::Max(uno.z, otro.z));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
