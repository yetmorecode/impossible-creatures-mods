/////////////////////////////////////////////////////////////////////
// File    : Matrix2.h
// Desc    : 
// Created : Tuesday, October 23, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

//	Mathlib Vectors are row vectors, Matrix2f is defined to perform rotations
//	 by post multiplying with a row vector. Rotation matrices are defined thusly:
//
//	m = [x x] , so to create a rotation matrices we use m = [ cos(rads) sin(rads)]
//		[y y]												[-sin(rads) cos(rads)]
//

#include "Vec2.h"

//***************************************************************************
//	Class Definition: Matrix2f
//***************************************************************************

class Matrix2f
{
	public:

		union
		{
			struct
			{
				// m-row-column.
				float m00, m01;	//	x x 
				float m10, m11;	//	y y 
			};
			struct
			{
				Vec2f X_axis;
				Vec2f Y_axis;
			};
		};


		// Constructors.
		explicit Matrix2f();
		explicit Matrix2f(const float a);
		explicit Matrix2f(float _m00, float _m01, float _m10, float _m11);
		explicit Matrix2f(const Vec2f &Xaxis,const Vec2f &Yaxis);

		inline const Vec2f& GetRow(size_t i) const;
		inline Vec2f& GetRow(size_t i);

		// Operators.

		// Matrix2f - float operations.
		Matrix2f operator *(const float a) const;
		friend Matrix2f operator *(const float a, const Matrix2f &b);
		Matrix2f operator -() const;
		Matrix2f &operator *=(const float a);

		// Matrix2f - Matrix2f operations.
		Matrix2f operator +(const Matrix2f &a) const;
		Matrix2f &operator +=(const Matrix2f &);
		Matrix2f operator -(const Matrix2f &a) const;
		Matrix2f &operator -=(const Matrix2f &);
		Matrix2f operator *(const Matrix2f &a) const;
		Matrix2f &operator *=(const Matrix2f &);
		friend Vec2f operator *(const Vec2f &v, const Matrix2f &a);
		friend Vec2f &operator *=(Vec2f &v, const Matrix2f &a);


		// Member Utility functions.
		void		IdentitySelf();

		Matrix2f	TransposeClone() const;
		void		TransposeSelf();

		Matrix2f	InverseClone() const;
		void		InverseSelf();

		float		Det() const;
};


//***************************************************************************
// Constructors.
//***************************************************************************

inline Matrix2f::Matrix2f()
:	X_axis(), Y_axis()
{
}


inline Matrix2f::Matrix2f(const float a)
{
	m00 = a;
	m01 = 0.0f;
	m10 = 0.0f;
	m11 = a;
}


inline Matrix2f::Matrix2f(float _m00, float _m01, float _m10, float _m11)
{ 
	m00 = _m00;
	m01 = _m01;
	m10 = _m10;
	m11 = _m11;
}


inline Matrix2f::Matrix2f(const Vec2f &Xaxis,const Vec2f &Yaxis)
{
	X_axis = Xaxis;
	Y_axis = Yaxis;
}


const Vec2f& Matrix2f::GetRow(size_t i) const
{
	dbAssert( i < 2 );
	return *( &X_axis + i );
}

Vec2f& Matrix2f::GetRow(size_t i)
{
	dbAssert( i < 2 );
	return *( &X_axis + i );
}

//***************************************************************************
// Matrix2f - float operations.
//***************************************************************************

//Matrix2f operator *(const Matrix2f &b, const float a)
inline Matrix2f Matrix2f::operator *(const float f) const
{
	// Matrix multiplied by a float.
	return Matrix2f(f*m00, f*m01, f*m10, f*m11);
}


inline Matrix2f operator *(const float f, const Matrix2f &a)
{
	// Matrix multiplied by a float.
	return Matrix2f(f*a.m00, f*a.m01, f*a.m10, f*a.m11);
}


inline Matrix2f Matrix2f::operator -() const
{
	// Return a Matrix multiplied by -1.
	return Matrix2f(-m00, -m01, -m10, -m11);

}

inline Matrix2f &Matrix2f::operator *=(const float a)
{
	m00 *= a;
	m01 *= a;
	m10 *= a;
	m11 *= a;

	return *this;
}


//***************************************************************************
// Matrix2f - Matrix2f operations.
//***************************************************************************

inline Matrix2f Matrix2f::operator +(const Matrix2f &a) const
{
	// Add one matrix to another elementwise.
	Matrix2f tmp;

	tmp.m00 = m00 + a.m00;
	tmp.m01 = m01 + a.m01;
	
	tmp.m10 = m10 + a.m10;
	tmp.m11 = m11 + a.m11;

	return tmp;
}


inline Matrix2f &Matrix2f::operator +=(const Matrix2f &a)
{
	// Add one matrix to another elementwise.

	m00 += a.m00;
	m01 += a.m01;
	m10 += a.m10;
	m11 += a.m11;

	return *this;
}


inline Matrix2f Matrix2f::operator -(const Matrix2f &a) const
{
	// subtract one matrix from another elementwise.
	Matrix2f tmp;

	tmp.m00 = m00 - a.m00;
	tmp.m01 = m01 - a.m01;

	tmp.m10 = m10 - a.m10;
	tmp.m11 = m11 - a.m11;

	return tmp;
}


inline Matrix2f &Matrix2f::operator -=(const Matrix2f &a)
{
	// subtract one matrix from another elementwise.

	m00 -= a.m00;
	m01 -= a.m01;
	m10 -= a.m10;
	m11 -= a.m11;

	return *this;
}


inline Matrix2f Matrix2f::operator *(const Matrix2f &a) const
{
	Matrix2f tmp;

	tmp.m00 = m00*a.m00 + m01*a.m10;
	tmp.m01 = m00*a.m01 + m01*a.m11;

	tmp.m10 = m10*a.m00 + m11*a.m10;
	tmp.m11 = m10*a.m01 + m11*a.m11;

	return tmp;
}


inline Matrix2f &Matrix2f::operator *=(const Matrix2f &a)
{
	Matrix2f tmp = *this;
	*this = tmp * a;

	return *this;
}


inline Vec2f operator *(const Vec2f &v, const Matrix2f &a)
{
	// Matrix pre-multiplied by a row-vector.
	Vec2f	tmp;

	tmp.x = v.x*a.m00 + v.y*a.m10;
	tmp.y = v.x*a.m01 + v.y*a.m11;

	return tmp;
}


inline Vec2f &operator *=(Vec2f &v, const Matrix2f &a)
{
	// Matrix pre-multiplied by a row-vector.
	Vec2f	tmp;

	tmp.x = v.x*a.m00 + v.y*a.m10;
	tmp.y = v.x*a.m01 + v.y*a.m11;

	v = tmp;

	return v;
}


//***************************************************************************
// Member Utility functions.
//***************************************************************************

inline void Matrix2f::TransposeSelf()
{
	// Transpose the current Matrix2f.
	float tmp;

	tmp = m01;
	m01 = m10;
	m10 = tmp;
}


inline Matrix2f Matrix2f::TransposeClone() const
{
	Matrix2f dup( *this );
	dup.TransposeSelf();

	return dup;
}

inline void Matrix2f::IdentitySelf()
{
	// Make the current Matrix2f an identity matrix.
	m00 = 1.0f;
	m11 = 1.0f;

	m01 = 0.0f;
	m10 = 0.0f;
}


inline float Matrix2f::Det() const
{
	// Return the determinant.
	return m00*m11 - m01*m10;
}

inline void Matrix2f::InverseSelf()
{
	// Invert myself.

	float InvDet = 1.0f/Det();
	float tmp;
	
	tmp = m11 * InvDet;
	m11 = m00 * InvDet;
	m00 = tmp;

	m01 *= -InvDet;
	m10 *= -InvDet;
}

inline Matrix2f Matrix2f::InverseClone() const
{
	Matrix2f dup( *this );
	dup.InverseSelf();

	return dup;
}

