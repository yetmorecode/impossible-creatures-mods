// Vec3.h

#pragma once

#ifndef _VEC3_H
#define _VEC3_H

#include "FastMath.h"

#define USE_FAST_INV_SQRT 1


// Forward Declarations.
class Quatf;


//***************************************************************************
//	Class Definition: Vec3f
//***************************************************************************

class Vec3f
{
	public:
	    union
	    {
			struct
			{
				float x, y, z;
			};
			float e[3];
			struct
			{
				float r, g, b;
			};
	    };


		// Constructors
		explicit Vec3f();
		explicit Vec3f(const float f);
		explicit Vec3f(const float _x, const float _y, const float _z);

		// assignment
		void Set( const float f);
		void Set( const float _x, const float _y, const float _z);
		Vec3f& operator =( const float f);

		// Access operations.
		float operator[] (int i) const {return e[i];}
		float &operator[] (int i) {return e[i];}


		// Operators.

		// Vec3f - float operations.
		Vec3f operator +(const float f) const;
		Vec3f &operator +=(const float f);
		Vec3f operator-(const float f) const;
		Vec3f &operator-=(const float f);
		Vec3f operator *(const float f) const;
		friend Vec3f operator *(const float f, const Vec3f &b);
		Vec3f &operator *=(const float f);
		Vec3f operator /(const float f);
		Vec3f &operator /=(const float f);

		// Vec3f - Vec3f operations.
		Vec3f operator +(const Vec3f &a) const;
		Vec3f &operator +=(const Vec3f &a);
		Vec3f operator-() const;
		Vec3f operator -(const Vec3f &a) const;
		Vec3f &operator -=(const Vec3f &a);

		// NOTE: this is NOT a cross product (use the ^ operator)
		Vec3f operator *(const Vec3f &a) const;
		Vec3f &operator *=(const Vec3f &a);

		// Dot product.
		float operator %(const Vec3f &a) const;

		// Cross product.
		Vec3f operator ^(const Vec3f &a) const;
		Vec3f &operator ^=(const Vec3f &a);


		// Comparison Operators.
		//	Note: Be careful using these when using these comparison operators on calculated values.
		int operator==(const Vec3f &a) const;
		int operator!=(const Vec3f &a) const;
		int operator!() const;


		// Member Utility functions.
		float Length() const;
		float LengthSqr() const;

		Vec3f	NormalizeClone() const;
		void    NormalizeSelf();
		Vec3f	NormalizeFastClone() const;
		void    NormalizeFastSelf();

		Vec3f	ClampComponentsClone(const float min, const float max) const;
		void	ClampComponentsSelf(const float min, const float max);

		Vec3f	ClampMagnitudeClone(const float min, const float max) const;
		void	ClampMagnitudeSelf(const float min, const float max);

		Vec3f RotateClone(const Quatf &q) const;
		void RotateSelf(const Quatf &q);
};


//***************************************************************************
// Constructors.
//***************************************************************************

inline Vec3f::Vec3f()
{
}


inline Vec3f::Vec3f(const float a)
	: x( a ),
	  y( a ),
	  z( a )
{
}


inline Vec3f::Vec3f(const float _x, const float _y, const float _z)
	: x( _x ),
	  y( _y ),
	  z( _z )
{
}


//***************************************************************************
//	Vec3f - float operations.
//***************************************************************************

inline Vec3f Vec3f::operator +(const float f) const
{
	return Vec3f(x+f,y+f,z+f);
}


inline Vec3f &Vec3f::operator +=(const float f)
{
	x += f;
	y += f;
	z += f;

	return *this;
}

inline Vec3f Vec3f::operator-(const float f) const
{
	return Vec3f(x-f, y-f, z-f);
}


inline Vec3f &Vec3f::operator-=(const float f)
{
	x-=f;
	y-=f;
	z-=f;
	return *this;
}


inline Vec3f Vec3f::operator *(const float a) const
{
	return Vec3f(a*x, a*y, a*z);
}


inline Vec3f operator *(const float a, const Vec3f &b)
{
	return Vec3f(a*b.x, a*b.y, a*b.z);
}


inline Vec3f &Vec3f::operator *=(const float f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}


inline Vec3f Vec3f::operator /(const float f)
{
	float Inv = 1.0f/f;
	return Vec3f(x*Inv, y*Inv, z*Inv);
}


inline Vec3f &Vec3f::operator /=(const float f)
{
	float Inv = 1.0f/f;
	return (*this) *= Inv;
}


//***************************************************************************
// Vec3f - Vec3f operations.
//***************************************************************************

inline Vec3f Vec3f::operator +(const Vec3f &a) const
{
	return Vec3f(x+a.x, y+a.y, z+a.z);
}


inline Vec3f &Vec3f::operator +=(const Vec3f &a)
{
	x += a.x;
	y += a.y;
	z += a.z;

	return *this;
}


inline Vec3f Vec3f::operator-() const
{
	return Vec3f(-x, -y, -z);
}


inline Vec3f Vec3f::operator -(const Vec3f &a) const
{
	return Vec3f(x-a.x, y-a.y, z-a.z);
}


inline Vec3f &Vec3f::operator -=(const Vec3f &a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;

	return *this;
}


// NOTE: this is NOT a cross product (use the ^ operator)
inline Vec3f Vec3f::operator *(const Vec3f &a) const
{
	return Vec3f(x*a.x, y*a.y, z*a.z);
}


// NOTE: this is NOT a cross product (use the ^ operator)
inline Vec3f &Vec3f::operator *=(const Vec3f &a)
{
	x *= a.x;
	y *= a.y;
	z *= a.z;

	return *this;
}


//***************************************************************************
// Dot product.
//***************************************************************************

inline float Vec3f::operator %(const Vec3f &a) const
{
	// Dot Product operator. Calculate and return the dot product for the 2 vectors (a,b).
	return (x*a.x + y*a.y + z*a.z);
}


//***************************************************************************
// Cross product.
//***************************************************************************

inline Vec3f Vec3f::operator ^(const Vec3f &a) const
{
	// Cross Product operator. Calculate and return the cross product of the two vectors (a,b).
	return Vec3f(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x);
}


inline Vec3f &Vec3f::operator ^=(const Vec3f &a)
{
	(*this) = Vec3f(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x);
	return *this;
}


//***************************************************************************
// Comparison Operators.
//	Note: Be careful using these when using these comparison operators on calculated values.
//***************************************************************************

// Comparison Operators
inline int Vec3f::operator==(const Vec3f &a) const
{
	// Test for 2 Vec3f's to be EXACTLY equivalent.
	//	Return 1 if vectors are the same, return 0 if vectors are the different.
	return ((x==a.x) && (y==a.y) && (z==a.z));
}


inline int Vec3f::operator!=(const Vec3f &a) const
{
	// Test for 2 Vec3f's to be not equal (even if they are VERY close).
	//	Return 1 if vectors are different, return 0 if vectors are the same.
	return ((x!=a.x) || (y!=a.y) || (z!=a.z));
}



inline int Vec3f::operator!() const
{
	// Test for a zero vector (vector of length zero).
	//	Return 1 if zero vector, return 0 otherwise.
	return ((x == 0.0f) && (y == 0.0f) && (z == 0.0f));
}


//***************************************************************************
// Member Utility functions.
//***************************************************************************

inline float Vec3f::Length() const
{
	// Return the length of the vector.
	return sqrtf(x*x + y*y + z*z);
}


inline float Vec3f::LengthSqr() const
{
	// Return the squared length of a vector.
	return x*x + y*y + z*z;
}

inline void Vec3f::NormalizeSelf()
{
	// Normalise the current Vec3f.

	float length = Length();
	dbAssert( length > 0 );

	float InvLength;

	InvLength = 1.0f / length;
	(*this) *= InvLength;

	return;
}

inline Vec3f Vec3f::NormalizeClone() const
{
	// Normalise the vector.
	Vec3f dup(*this);
	dup.NormalizeSelf();
	
	return dup;
}


inline void Vec3f::NormalizeFastSelf()
{
	float InvLength;

#if USE_FAST_INV_SQRT
	float l2 = LengthSqr();
	if ( l2 == 0.f )
		return;
	InvLength = InvSqrt(l2);
#else
	float l = Length();
	if ( l == 0.f )
		return;
	InvLength = 1/l;
#endif

	(*this)*=(InvLength);

	return;
}


inline Vec3f Vec3f::NormalizeFastClone() const
{
	Vec3f dup(*this);
	dup.NormalizeFastSelf();
	
	return dup;
}


inline void Vec3f::Set( const float f)
{
	x = f;
	y = f;
	z = f;
}


inline void Vec3f::Set( const float _x, const float _y, const float _z)
{
	x = _x;
	y = _y;
	z = _z;
}


inline Vec3f& Vec3f::operator =( const float f)
{
	x = f;
	y = f;
	z = f;
	return *this;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: clamps the components (x, y, z) to be within the given range (inclusive)
//	Result	: may modify x, y, z
//	Param.	: min & max - the range to clamp to
//
inline void Vec3f::ClampComponentsSelf(const float min, const float max)
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
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns a clone of this, with clamped components
//	Param.	: min & max - the range to clamp to
//
inline Vec3f Vec3f::ClampComponentsClone(const float min, const float max) const
{
	// Normalise the vector.
	Vec3f dup(*this);
	dup.ClampComponentsSelf( min, max );
	
	return dup;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: clamps the length of the vector to be within the given range (inclusive)
//	Result	: may modify x, y, z
//	Param.	: min & max - the range to clamp to
//
inline void Vec3f::ClampMagnitudeSelf(const float min, const float max)
{
	float l = Length();

	if( l < min )
	{
		NormalizeSelf();
		operator*=(min);
	}
	else if( l > max )
	{
		NormalizeSelf();
		operator*=(max);
	}
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns a clone of this, with clamped length
//	Param.	: min & max - the range to clamp to
//
inline Vec3f Vec3f::ClampMagnitudeClone(const float min, const float max) const
{
	// Normalise the vector.
	Vec3f dup(*this);
	dup.ClampMagnitudeSelf( min, max );
	
	return dup;
}


#endif	// _VEC3_H