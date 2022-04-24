/////////////////////////////////////////////////////////////////////
// File  : Vec4.h
// Author: Jrehn
// Date  : 2000-11-15
//      (c) relic entertainment inc.2000
// 
// *
// 

#pragma once

#include "Vec3.h"
#include "FastMath.h"


#define USE_FAST_INV_SQRT 1



// Forward Declaration.
class Quatf;


//***************************************************************************
//	Class Definition: Vec4f
//***************************************************************************

class Vec4f
{
	public:
	    union
	    {
			struct
			{
				float x, y, z, w;
			};
			float e[4];
			struct
			{
				float r, g, b, a;
			};
			struct
			{
				Vec3f v;
				float w;
			};
	    };


		// Constructors
		explicit Vec4f();
		explicit Vec4f(const float f);
		explicit Vec4f(const float _x, const float _y, const float _z, const float _w);
		explicit Vec4f(const Vec3f &_v, const float _w);


		// Access operations.
		float operator[] (int i) const {return e[i];}
		float &operator[] (int i) {return e[i];}


		// Operators.

		// Vec4f - float operations.
		friend Vec4f operator *(const float f, const Vec4f &b);

		Vec4f&	operator += (const float f);
		Vec4f&	operator -= (const float f);
		Vec4f&	operator *= (const float f);
		Vec4f&	operator /= (const float f);

		Vec4f	operator -  () const;
		Vec4f	operator +  (const float f) const;
		Vec4f	operator -  (const float f) const;
		Vec4f	operator *  (const float f) const;
		Vec4f	operator /  (const float f) const;

		// Vec4f - Vec4f operations.
		Vec4f&	operator += (const Vec4f& a);
		Vec4f&	operator -= (const Vec4f& a);
		Vec4f&	operator *= (const Vec4f& a);

		Vec4f	operator +  (const Vec4f& a) const;
		Vec4f	operator -  (const Vec4f& a) const;
		Vec4f	operator *  (const Vec4f& a) const;

		// Dot product.
		float	operator %  (const Vec4f& a) const;

		// Comparison Operators.
		//	Note: Be careful using these when using these comparison operators on calculated values.
		int		operator==(const Vec4f &a) const;
		int		operator!=(const Vec4f &a) const;
		int		operator!() const;


		// Member Utility functions.
		float	Length() const;
		float	LengthSqr() const;

		Vec4f	NormalizeClone() const;
		void	NormalizeSelf();
		Vec4f	NormalizeFastClone() const;
		void	NormalizeFastSelf();

		void Set( const float f);
		void Set( const float _x, const float _y, const float _z, const float _w);

		Vec4f	ClampComponentsClone(const float min, const float max) const;
		void	ClampComponentsSelf(const float min, const float max);

		Vec4f RotateClone(const Quatf &q) const;
		void RotateSelf(const Quatf &q);
};

//***************************************************************************
// Constructors.
//***************************************************************************

inline Vec4f::Vec4f()
{
}


inline Vec4f::Vec4f(const float a)
{
	x = a;
	y = a;
	z = a;
	w = a;
}


inline Vec4f::Vec4f(const float _x, const float _y, const float _z, const float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}


inline Vec4f::Vec4f(const Vec3f &_v, const float _w)
{
	v = _v;
	w = _w;
}


//***************************************************************************
//	Vec4f - float operations.
//***************************************************************************

inline Vec4f Vec4f::operator +(const float f) const
{
	return Vec4f(x+f,y+f,z+f,w+f);
}


inline Vec4f &Vec4f::operator +=(const float f)
{
	x += f;
	y += f;
	z += f;
	w += f;

	return *this;
}


inline Vec4f Vec4f::operator-() const
{
	return Vec4f(-x, -y, -z, -w);
}


inline Vec4f Vec4f::operator-(const float f) const
{
	return Vec4f(x-f, y-f, z-f, w-f);
}


inline Vec4f &Vec4f::operator-=(const float f)
{
	x-=f;
	y-=f;
	z-=f;
	w-=f;
	return *this;
}


inline Vec4f Vec4f::operator *(const float a) const
{
	return Vec4f(a*x, a*y, a*z, a*w);
}


inline Vec4f operator *(const float a, const Vec4f &b)
{
	return Vec4f(a*b.x, a*b.y, a*b.z, a*b.w);
}


inline Vec4f &Vec4f::operator *=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;

	return *this;
}


inline Vec4f Vec4f::operator /(const float f) const
{
	float Inv = 1.0f/f;
	return Vec4f(x*Inv, y*Inv, z*Inv, w*Inv);
}


inline Vec4f &Vec4f::operator /=(const float f)
{
	float Inv = 1.0f/f;
	return (*this) *= Inv;
}


//***************************************************************************
// Vec4f - Vec4f operations.
//***************************************************************************

inline Vec4f Vec4f::operator +(const Vec4f &a) const
{
	return Vec4f(x+a.x, y+a.y, z+a.z, w+a.w);
}


inline Vec4f &Vec4f::operator +=(const Vec4f &a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;

	return *this;
}


inline Vec4f Vec4f::operator -(const Vec4f &a) const
{
	return Vec4f(x-a.x, y-a.y, z-a.z, w-a.w);
}


inline Vec4f &Vec4f::operator -=(const Vec4f &a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;

	return *this;
}


inline Vec4f Vec4f::operator *(const Vec4f &a) const
{
	return Vec4f(x*a.x, y*a.y, z*a.z, w*a.w);
}

inline Vec4f &Vec4f::operator *=(const Vec4f &a)
{
	x*=a.x;
	y*=a.y;
	z*=a.z;
	w*=a.w;

	return *this;
}


//***************************************************************************
// Dot product.
//***************************************************************************

inline float Vec4f::operator %(const Vec4f &a) const
{
	// Dot Product operator. Calculate and return the dot product for the 2 vectors (a,b).
	return (x*a.x + y*a.y + z*a.z + w*a.w);
}


//***************************************************************************
// Comparison Operators.
//	Note: Be careful using these when using these comparison operators on calculated values.
//***************************************************************************

// Comparison Operators
inline int Vec4f::operator==(const Vec4f &a) const
{
	// Test for 2 Vec4f's to be EXACTLY equivalent.
	//	Return 1 if vectors are the same, return 0 if vectors are the different.
	return ((x==a.x) && (y==a.y) && (z==a.z) && (w==a.w));
}


inline int Vec4f::operator!=(const Vec4f &a) const
{
	// Test for 2 Vec4f's to be not equal (even if they are VERY close).
	//	Return 1 if vectors are different, return 0 if vectors are the same.
	return ((x!=a.x) || (y!=a.y) || (z!=a.z) || (w!=a.w));
}


inline int Vec4f::operator!() const
{
	// Test for a zero vector (vector of length zero).
	//	Return 1 if zero vector, return 0 otherwise.
	return ((x == 0.0f) && (y == 0.0f) && (z == 0.0f) && (w == 0.0f));
}


//***************************************************************************
// Member Utility functions.
//***************************************************************************

inline float Vec4f::Length() const
{
	// Return the length of the vector.
	return sqrtf(x*x + y*y + z*z + w*w);
}


inline float Vec4f::LengthSqr() const
{
	// Return the squared length of a vector.
	return x*x + y*y + z*z + w*w;
}


inline Vec4f Vec4f::NormalizeClone() const
{
	// Normalise the vector.
	//	This is done in this manner so that the contents of the vector being normalised
	//	are left in tact. As is often required when normalising vectors.

	float InvLength;

	InvLength = 1.0f / Length();
	return InvLength * (*this);

}


inline void Vec4f::NormalizeSelf()
{
	// Normalise the current Vec4f.

	float InvLength;

	InvLength = 1.0f / Length();
	(*this) *= InvLength;
}


inline Vec4f Vec4f::NormalizeFastClone() const
{
	// Use the fast Normalise routine
	float InvLength;

#if USE_FAST_INV_SQRT
	InvLength = InvSqrt(LengthSqr());
#else
	InvLength = sqrtf(Length());
#endif

	return InvLength * (*this);
}


inline void Vec4f::NormalizeFastSelf()
{
	float InvLength;

#if USE_FAST_INV_SQRT
	InvLength = InvSqrt(LengthSqr());
#else
	InvLength = sqrtf(Length());
#endif

	(*this)*=(InvLength);
}


inline void Vec4f::Set( const float f)
{
	x = f;
	y = f;
	z = f;
	w = f;
}


inline void Vec4f::Set( const float _x, const float _y, const float _z, const float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}


inline void Vec4f::ClampComponentsSelf(const float min, const float max)
{
	// Ensure that a vector's elements are between the range of min and max.

	if(x > max)
		x = max;
	if(x < min)
		x = min;

	if(y < min)
		y = min;
	if(y > max)
		y = max;

	if(z < min)
		z = min;
	if(z > max)
		z = max;

	if(w < min)
		w = min;
	if(w > max)
		w = max;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns a clone of this, with clamped components
//	Param.	: min & max - the range to clamp to
//
inline Vec4f Vec4f::ClampComponentsClone(const float min, const float max) const
{
	// Normalise the vector.
	Vec4f dup(*this);
	dup.ClampComponentsSelf( min, max );
	
	return dup;
}

