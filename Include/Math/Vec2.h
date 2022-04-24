/////////////////////////////////////////////////////////////////////
// File    : Vec2Template.h
// Desc    : 
// Created : Monday, October 22, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// * This files defines 2 classes
// * -Vec2f
// * -Vec2i

#pragma once

#include "FastMath.h"

#define USE_FAST_INV_SQRT 1

///////////////////////////////////////////////////////////////////// 
// Vec2Template

template< class T >
class Vec2Template
{
// fields
public:
	union
	{
		struct
		{
			T	x, y;
		};
		struct
		{
			T	e[2];
		};
		struct
		{
			T	u, v;
		};
	};

// constructors
public:
	explicit inline Vec2Template();
	explicit inline Vec2Template(const T f);
	explicit inline Vec2Template(const T _x, const T _y);

// interface
public:
	// Access operations.
	T		operator[] (size_t i) const;
	T&		operator[] (size_t i);

	void	Set( const T f);
	void	Set( const T _x, const T _y);

	// Operators.

	// Vec2Template - float operations.
	Vec2Template  operator+  (const T f) const;
	Vec2Template& operator+= (const T f);
	Vec2Template  operator-  (const T f) const;
	Vec2Template& operator-= (const T f);
	Vec2Template  operator*  (const T f) const;
	Vec2Template& operator*= (const T f);

	// Vec2Template - Vec2Template operations.
	Vec2Template  operator-  () const;
	Vec2Template  operator+  (const Vec2Template& a) const;
	Vec2Template& operator+= (const Vec2Template& a);
	Vec2Template  operator-  (const Vec2Template& a) const;
	Vec2Template& operator-= (const Vec2Template& a);
	Vec2Template  operator*  (const Vec2Template& a) const;
	Vec2Template& operator*= (const Vec2Template& a);

	Vec2Template	ClampComponentsClone(const T min, const T max) const;
	void			ClampComponentsSelf(const T min, const T max);

	Vec2Template	ClampMagnitudeClone(const T min, const T max) const;
	void			ClampMagnitudeSelf(const T min, const T max);

	// Comparison Operators.
	//	Note: Be careful using these when using these comparison operators on calculated values.
	bool  operator== (const Vec2Template& a) const;
	bool  operator!= (const Vec2Template& a) const;
	bool  operator!  () const;

	//*************************************
	//* These are NOT available on non-float types

	Vec2Template  operator/  (const T f) const;
	Vec2Template& operator/= (const T f);

	// Dot product.
	T		      operator%  (const Vec2Template& a) const;

	// Member Utility functions.
	T			  Length() const;
	T			  LengthSqr() const;

	Vec2Template  Normalize() const;
	Vec2Template& NormalizeSelf();
	Vec2Template  NormalizeFast() const;
	Vec2Template& NormalizeSelfFast();

	Vec2Template  Rotate(const float rads) const;
	Vec2Template& RotateSelf(const float rads);

	static T	  DistSqr(const Vec2Template& pt1, const Vec2Template& pt2);
};

///////////////////////////////////////////////////////////////////// 
// 

typedef Vec2Template<float>	Vec2f;
typedef Vec2Template<int>	Vec2i;

///////////////////////////////////////////////////////////////////// 
// Vec2Template implementation

template< class T > inline Vec2Template<T>::Vec2Template()
{
}

template< class T > inline Vec2Template<T>::Vec2Template(const T a)
	: x(a),
	  y(a)
{
}

template< class T > inline Vec2Template<T>::Vec2Template(const T _x, const T _y)
	: x(_x),
	  y(_y)
{
}

template< class T > inline T Vec2Template<T>::operator[] (size_t i) const 
{
	return e[i];
}

template< class T > inline T& Vec2Template<T>::operator[] (size_t i)
{
	return e[i];
}

template< class T > inline void Vec2Template<T>::Set( const T f)
{
	x = f;
	y = f;
}

template< class T > inline void Vec2Template<T>::Set( const T _x, const T _y)
{
	x = _x;
	y = _y;
}

template< class T > inline Vec2Template<T>& Vec2Template<T>::operator+= (const T f)
{
	x += f;
	y += f;

	return *this;
}

template< class T > inline Vec2Template<T> Vec2Template<T>::operator+ (const T f) const
{
	return Vec2Template(*this)+= f;
}

template< class T > inline Vec2Template<T>& Vec2Template<T>::operator-= (const T f)
{
	x-=f;
	y-=f;
	return *this;
}

template< class T > inline Vec2Template<T> Vec2Template<T>::operator- (const T f) const
{
	return Vec2Template(*this)-= f;
}

template< class T > inline Vec2Template<T>& Vec2Template<T>::operator*= (const T f)
{
	x*=f;
	y*=f;
	return *this;
}

template< class T > inline Vec2Template<T> Vec2Template<T>::operator* (const T f) const
{
	return Vec2Template(*this)*= f;
}

template< class T > inline Vec2Template<T> operator* (const float a, const Vec2Template<T>& b)
{
	return Vec2Template<T>(b)*= a;
}

template< class T > inline Vec2Template<T>& Vec2Template<T>::operator+= (const Vec2Template& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

template< class T > inline Vec2Template<T> Vec2Template<T>::operator+ (const Vec2Template& rhs) const
{
	return Vec2Template(*this)+= rhs;
}

template< class T > inline Vec2Template<T>& Vec2Template<T>::operator-= (const Vec2Template& rhs)
{
	x-=rhs.x;
	y-=rhs.y;
	return *this;
}

template< class T > inline Vec2Template<T> Vec2Template<T>::operator- (const Vec2Template& rhs) const
{
	return Vec2Template(*this)-= rhs;
}

template< class T > inline Vec2Template<T> Vec2Template<T>::operator- () const
{
	return Vec2Template(-x, -y);
}

template< class T > inline Vec2Template<T>& Vec2Template<T>::operator*= (const Vec2Template& rhs)
{
	x*=rhs.x;
	y*=rhs.y;
	return *this;
}

template< class T > inline Vec2Template<T> Vec2Template<T>::operator* (const Vec2Template& rhs) const
{
	return Vec2Template(*this)*= rhs;
}

template< class T > inline bool Vec2Template<T>::operator== (const Vec2Template& rhs) const
{
	// Test for 2 Vec2f's to be EXACTLY equivalent.
	return ((x==rhs.x) && (y==rhs.y));
}

template< class T > inline bool Vec2Template<T>::operator!= (const Vec2Template& rhs) const
{
	return !operator==(rhs);
}

template< class T > inline bool Vec2Template<T>::operator! () const
{
	return ((x == 0) && (y == 0));
}

template< class T > inline void Vec2Template<T>::ClampComponentsSelf(const T min, const T max)
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
}

template< class T > inline Vec2Template<T> Vec2Template<T>::ClampComponentsClone(const T min, const T max) const
{
	// Ensure that a vector's elements are between the range of min and max.
	// Normalise the vector.
	T dup(*this);
	dup.ClampComponentsSelf( min, max );
	
	return dup;
}

template< class T > inline void Vec2Template<T>::ClampMagnitudeSelf(const T min, const T max)
{
	// Ensure that a vector's length is between the range of min and max.

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

template< class T > inline Vec2Template<T> Vec2Template<T>::ClampMagnitudeClone(const T min, const T max) const
{
	// Ensure that a vector's elements are between the range of min and max.
	// Normalise the vector.
	T dup(*this);
	dup.ClampMagnitudeSelf(min, max);
	
	return dup;
}

//***************************************************************************
// specialization
//***************************************************************************

template<> inline Vec2Template<float>& Vec2Template<float>::operator/= (const float f)
{
	const float Inv = 1.0f/f;
	x*=Inv;
	y*=Inv;
	return *this;
}

template<> inline Vec2Template<float> Vec2Template<float>::operator/ (const float f) const
{
	return Vec2Template<float>(*this)/= f;
}

template<> inline float Vec2Template<float>::operator% (const Vec2Template<float>& a) const
{
	// Dot Product operator. Calculate and return the dot product for the 2 vectors (a,b).
	return (x*a.x + y*a.y);
}

template<> inline float Vec2Template<float>::LengthSqr() const
{
	return x*x + y*y;
}

template<> inline float Vec2Template<float>::Length() const
{
	return sqrtf(x*x + y*y);
}

template<> inline Vec2Template<float>& Vec2Template<float>::NormalizeSelf()
{
	// Normalise the current Vec2f.
	const float InvLength = 1.0f / Length();
	return (*this) *= InvLength;
}

template<> inline Vec2Template<float> Vec2Template<float>::Normalize() const
{
	// Normalise the vector.
	//	This is done in this manner so that the contents of the vector being normalised
	//	are left in tact. As is often required when normalising vectors.
	return Vec2Template<float>(*this).NormalizeSelf();
}

template<> inline Vec2Template<float>& Vec2Template<float>::NormalizeSelfFast()
{
	float InvLength;

#if USE_FAST_INV_SQRT
	InvLength = InvSqrt(LengthSqr());
#else
	InvLength = sqrtf(Length());
#endif

	return (*this) *= InvLength;
}

template<> inline Vec2Template<float> Vec2Template<float>::NormalizeFast() const
{
	return Vec2Template<float>(*this).NormalizeSelfFast();
}

template<> inline Vec2Template<float>& Vec2Template<float>::RotateSelf(const float rads)
{
	// Rotate vector by r (in radians).

	const float cosr = cosf(rads);
	const float sinr = sinf(rads);

	const float _x = x * cosr - y * sinr;
	const float _y = x * sinr + y * cosr;

	x = _x; y = _y;

	return *this;
}

template<> inline Vec2Template<float> Vec2Template<float>::Rotate(const float rads) const
{
	return Vec2Template<float>(*this).RotateSelf(rads);
}

template<> inline float Vec2Template<float>::DistSqr
	(
	const Vec2Template<float>& pt1, 
	const Vec2Template<float>& pt2
	)
{
	// return distance square of 2 points.
	float xdist = pt1.x - pt2.x;
	float ydist = pt1.y - pt2.y;
	return (xdist*xdist) + (ydist*ydist);
}

