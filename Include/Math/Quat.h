// Quat.h
//
// A quaternion is just a colapsed form of an orthonormal 3x3 matrix

#pragma once

#ifndef _MATH_QUAT_H
#define _MATH_QUAT_H

#include "Vec3.h"
#include "Matrix3.h"


class Quatf 
{
	public :
		
		union
		{
			float e[4];
			struct
			{
				float	x, y, z, s;
			};
			struct
			{
				Vec3f v;
				float s;
			};
		};

		
		// Constructors
		explicit inline Quatf();
		explicit inline Quatf(const float a);
		explicit inline Quatf(const float a, const float b, const float c, const float d);
		explicit inline Quatf(const Vec3f& b, float f);
		explicit inline Quatf(const Matrix3f& a);

		inline void		IdentitySelf();

		// assignment
		inline void Set( const float a);
		inline void Set( const float a, const float b, const float c, const float d);
		inline void Set( const Vec3f& b, float f);
		void Set( const Matrix3f& a);
		inline Quatf& operator =( const float a);
		inline Quatf& operator =( const Matrix3f& a);

		// Member Overloaded Operators
		inline Quatf& operator +=(const Quatf&);
		inline Quatf& operator -=(const Quatf&);

		// Member Utility Functions
		inline Quatf Normalize() const;
		inline void	NormalizeSelf();
		inline float Length() const;
		
		inline Quatf operator -() const;
		inline Quatf operator -(const Quatf& a) const;
		inline Quatf operator +(const Quatf& a) const;
		inline Quatf operator *(const Quatf& a) const;
		inline Quatf& operator *=(const Quatf& a);


		inline Quatf operator *(const float f) const;
		friend inline Quatf operator *(const float f, const Quatf& b);
		inline Quatf& operator *=(const float f);

		inline void InverseSelf();
		inline Quatf InverseClone() const;

		// Returns the rotation delta between quats, so they can later be concatinated
		//     ie. delta = a.DeltaClone( b);
		//         (delta * b) == a
		// Essentially a divide, but that's just weird conceptually
		inline Quatf& DeltaSelf( const Quatf& a);
		inline Quatf DeltaClone( const Quatf& a) const;

		Quatf ScaleRotClone( const float f) const;
		Quatf& ScaleRotSelf( const float f) { *this = ScaleRotClone( f); return *this; }


		Quatf Slerp(const Quatf& a, const Quatf& b, const float f);
};

// Function prototype.
Quatf Slerp(const Quatf& a, const Quatf& b, const float f);



inline Quatf::Quatf()
{
}

inline void Quatf::Set(const float a)
{
	x = a;
	y = a;
	z = a;
	s = a;
}

inline void Quatf::Set(const float a, const float b, const float c, const float d)
{
	x = a;
	y = b;
	z = c;
	s = d;
}

inline void Quatf::Set(const Vec3f& a, const float f)
{
	x = a.x;
	y = a.y;
	z = a.z;
	s = f;
}



inline Quatf::Quatf(const float a)
{
	Set( a);
}

inline Quatf::Quatf(const float a, const float b, const float c, const float d)
{
	Set( a, b, c, d);
}

inline Quatf::Quatf(const Vec3f& a, const float f)
{
	Set( a, f);
}

inline Quatf::Quatf( const Matrix3f& a)
{
	Set( a);
}



inline Quatf& Quatf::operator =( const float a)
{
	Set( a);
	return *this;
}

inline Quatf& Quatf::operator =( const Matrix3f& a)
{
	Set( a);
	return *this;
}



inline Quatf& Quatf::operator +=(const Quatf& a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	s += a.s;

	return *this;
}

inline Quatf& Quatf::operator -=(const Quatf& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	s -= a.s;

	return *this;
}

inline void Quatf::IdentitySelf()
{
	x = 0;
	y = 0;
	z = 0;
	s = 1;
}

inline float Quatf::Length() const
{
	return sqrtf(x*x + y*y + z*z + s*s);
}

inline Quatf Quatf::Normalize() const
{
	float InvLength;

	InvLength = 1.0f / this->Length();

	return Quatf(InvLength * x, InvLength * y, InvLength * z, InvLength * s );
}

inline void Quatf::NormalizeSelf()
{
	float InvLength;

	InvLength = 1.0f / this->Length();

	x *= InvLength;
	y *= InvLength;
	z *= InvLength;
	s *= InvLength;
}

inline Quatf Quatf::operator -() const
{
	return Quatf(-x,-y,-z,-s);
}

inline Quatf Quatf::operator +(const Quatf& a) const
{
	return Quatf(x+a.x, y+a.y, z+a.z, s+a.s);
}

inline Quatf Quatf::operator -(const Quatf& a) const
{
	return Quatf(x-a.x, y-a.y, z-a.z, s-a.s);
}

inline Quatf Quatf::operator *(const Quatf& a) const
{
	Quatf r;

	r.x = + a.s*x + a.x*s + a.y*z - a.z*y;
	r.y = + a.s*y - a.x*z + a.y*s + a.z*x;
	r.z = + a.s*z + a.x*y - a.y*x + a.z*s;
	r.s = + a.s*s - a.x*x - a.y*y - a.z*z;

	return r;
}

inline Quatf& Quatf::operator *=(const Quatf& a)
{
	Quatf r;

	r.x = + a.s*x + a.x*s + a.y*z - a.z*y;
	r.y = + a.s*y - a.x*z + a.y*s + a.z*x;
	r.z = + a.s*z + a.x*y - a.y*x + a.z*s;
	r.s = + a.s*s - a.x*x - a.y*y - a.z*z;

	*this = r;

	return *this;
}


inline Quatf Quatf::operator *(const float a) const
{
	return Quatf(a*x, a*y, a*z, a*s);
}


inline Quatf operator *(const float a, const Quatf& b)
{
	return Quatf(a*b.x, a*b.y, a*b.z, a*b.s);
}


inline Quatf& Quatf::operator *=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	s *= f;

	return *this;
}

inline void Quatf::InverseSelf()
{
	s = -s;
}

inline Quatf Quatf::InverseClone() const
{
	return Quatf( v, -s);
}


inline Quatf& Quatf::DeltaSelf( const Quatf& a)
{
	Quatf r;

	r.x = - a.s*x + a.x*s + a.y*z - a.z*y;
	r.y = - a.s*y - a.x*z + a.y*s + a.z*x;
	r.z = - a.s*z + a.x*y - a.y*x + a.z*s;
	r.s = - a.s*s - a.x*x - a.y*y - a.z*z;

	*this = r;

	return *this;
}

inline Quatf Quatf::DeltaClone( const Quatf& a) const
{
	Quatf r;

	r.x = - a.s*x + a.x*s + a.y*z - a.z*y;
	r.y = - a.s*y - a.x*z + a.y*s + a.z*x;
	r.z = - a.s*z + a.x*y - a.y*x + a.z*s;
	r.s = - a.s*s - a.x*x - a.y*y - a.z*z;

	return r;
}


#endif	// _MATH_QUAT_H

