// Matrix4.h

#pragma once

#ifndef _MATRIX4_H
#define _MATRIX4_H

// Include Files.
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix3.h"
#include "Matrix43.h"



// Forward Declarations.
class Quatf;


class Matrix4f
{
	public:

		union
		{
			//	We don't provide the e[] and e2[][] members since Matrix4f has a non-2d array storage format
			struct
			{
				// Defined in a weird order so that we can return references to matrix3's for
				//	rotations etc...

				// m-row-column.
				float m00, m01, m02;	//	x x x
				float m10, m11, m12;	//	y y y
				float m20, m21, m22;	//	z z z
				float m30, m31, m32;	//	tx ty tz
				float m03, m13, m23, m33;	// last column.
			};
			struct
			{
				union
				{
					struct
					{
						union
						{
							struct
							{
								Vec3f X_axis;
								Vec3f Y_axis;
								Vec3f Z_axis;
							};
							struct
							{
								Matrix3f R;
							};
						};
						Vec3f T;
					};
					struct
					{
						Matrix43f X;
					};
				};
				Vec4f W;
			};
		};


		// Constructors.
		explicit inline Matrix4f();
		explicit inline Matrix4f(const float a);
		explicit inline Matrix4f(const float _m00, const float _m01, const float _m02, const float _m03, 
							const float _m10, const float _m11, const float _m12, const float _m13, 
							const float _m20, const float _m21, const float _m22, const float _m23, 
							const float _m30, const float _m31, const float _m32, const float _m33);
		explicit inline Matrix4f(const Vec3f &Xaxis, const Vec3f &Yaxis, const Vec3f &Zaxis, const Vec3f &_T);
		explicit inline Matrix4f(const Matrix3f Rot, const Vec3f &_T);
		explicit inline Matrix4f(const Quatf &q);
		explicit inline Matrix4f(const Quatf &q, const Vec3f &_T);


		// Access Operations.
		// Note: Access the specific rows by Matrix4f.row[i], or Matrix4f.X_axis.

		inline Vec4f GetRow(size_t i) const;

		// Operators.

		// Matrix4f - float operations.
		inline Matrix4f operator *(const float a) const;
		inline Matrix4f &operator *=(const float f);
		friend inline Matrix4f operator *(const float a, const Matrix4f &b);
		inline Matrix4f operator -() const;

		// Matrix4f - Matrix4f operations.
		inline Matrix4f operator +(const Matrix4f &a) const;
		inline Matrix4f &operator +=(const Matrix4f &);
		inline Matrix4f operator -(const Matrix4f &a) const;
		inline Matrix4f &operator -=(const Matrix4f &);
		inline Matrix4f operator *(const Matrix4f &a) const;
		inline Matrix4f &operator *=(const Matrix4f &);

		// Matrix43f - VecXf operations
		friend Vec4f operator *(const Vec4f &v, const Matrix4f &a);
		friend Vec3f operator *(const Vec3f &v, const Matrix4f &a);
		friend Vec4f &operator *=(Vec4f &v, const Matrix4f &a);
		friend Vec3f &operator *=(Vec3f &v, const Matrix4f &a);


		// Member Utility functions.
		void		IdentitySelf();

		void		TransposeSelf();
		Matrix4f	TransposeClone() const;

		void		MakeRotateX(const float rads);
		void		RotateXSelf(const float rads);
		Matrix4f	RotateXClone(const float rads) const;

		void		MakeRotateY(const float rads);
		void		RotateYSelf(const float rads);
		Matrix4f	RotateYClone(const float rads) const;

		void		MakeRotateZ(const float rads);
		void		RotateZSelf(const float rads);
		Matrix4f	RotateZClone(const float rads) const;

		void		MakeRotateAboutAxis(const Vec3f &v, const float rads);
		void		RotateAboutAxisSelf(const Vec3f &v, const float rads);
		Matrix4f	RotateAboutAxisClone(const Vec3f &v, const float rads) const;

		void		MakeLookAt( const Vec3f& origin, const Vec3f& target, const Vec3f& up);

		void		MakeScale(const Vec3f &v);
		void		ScaleSelf(const Vec3f &v);
		Matrix4f	ScaleClone(const Vec3f &v) const;

		void		MakeTranslate(const Vec3f &v);
		void		TranslateSelf(const Vec3f &v);
		Matrix4f	TranslateClone(const Vec3f &v) const;

		float		Det() const;
		void		InverseSelf();
		Matrix4f	InverseClone() const;

		void		Multiply( const Matrix4f& a, const Matrix4f& b );
};


//***************************************************************************
// Constructors.
//***************************************************************************

inline Matrix4f::Matrix4f()
:	X_axis(), Y_axis(), Z_axis(), R(), T(), X(), W()
{
}


inline Matrix4f::Matrix4f(const float a)
{
	m00 = a;
	m01 = 0.0f;
	m02 = 0.0f;
	m03 = 0.0f;
	m10 = 0.0f;
	m11 = a;
	m12 = 0.0f;
	m13 = 0.0f;
	m20 = 0.0f;
	m21 = 0.0f;
	m22 = a;
	m23 = 0.0f;
	m30 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;
	m33 = a;
}


inline Matrix4f::Matrix4f(const float _m00, const float _m01, const float _m02, const float _m03, 
							const float _m10, const float _m11, const float _m12, const float _m13,
							const float _m20, const float _m21, const float _m22, const float _m23,
							const float _m30, const float _m31, const float _m32, const float _m33)
{ 
	m00 = _m00;
	m01 = _m01;
	m02 = _m02;
	m03 = _m03;
	m10 = _m10;
	m11 = _m11;
	m12 = _m12;
	m13 = _m13;
	m20 = _m20;
	m21 = _m21;
	m22 = _m22;
	m23 = _m23;
	m30 = _m30;
	m31 = _m31;
	m32 = _m32;
	m33 = _m33;
}


inline Matrix4f::Matrix4f(const Vec3f &Xaxis,const Vec3f &Yaxis,const Vec3f &Zaxis, const Vec3f &_T)
{
	X_axis = Xaxis;
	Y_axis = Yaxis;
	Z_axis = Zaxis;
	T = _T;

	W[0] = 0;
	W[1] = 0;
	W[2] = 0;
	W[3] = 1;
}


inline Matrix4f::Matrix4f(const Matrix3f Rot, const Vec3f &_T)
{
	R = Rot;
	T = _T;

	W[0] = 0;
	W[1] = 0;
	W[2] = 0;
	W[3] = 1;
}


inline Matrix4f::Matrix4f(const Quatf &q)
{
	R = q;
	T.Set( 0.0f);

	W[0] = 0;
	W[1] = 0;
	W[2] = 0;
	W[3] = 1;
}


inline Matrix4f::Matrix4f(const Quatf &q, const Vec3f &_T)
{
	R = q;
	T = _T;

	W[0] = 0;
	W[1] = 0;
	W[2] = 0;
	W[3] = 1;
}


Vec4f Matrix4f::GetRow(size_t i) const
{
	dbAssert( i <= 4 );
	return Vec4f( X.GetRow(i), W[i] );
}


//***************************************************************************
// Matrix4f - float operations.
//***************************************************************************

//Matrix4f operator *(const Matrix4f &b, const float a)
Matrix4f Matrix4f::operator *(const float f) const
{
	// Matrix multiplied by a float.
	return Matrix4f(f*m00, f*m01, f*m02, f*m03, f*m10, f*m11, f*m12, f*m13, f*m20, f*m21, f*m22, f*m23, f*m30, f*m31, f*m32, f*m33);
}

inline Matrix4f& Matrix4f::operator *=(const float f)
{
	m00 *= f;
	m01 *= f;
	m02 *= f;
	m03 *= f;

	m10 *= f;
	m11 *= f;
	m12 *= f;
	m13 *= f;

	m20 *= f;
	m21 *= f;
	m22 *= f;
	m23 *= f;

	m30 *= f;
	m31 *= f;
	m32 *= f;
	m33 *= f;

	return *this;
}


Matrix4f operator *(const float f, const Matrix4f &a)
{
	// Matrix multiplied by a float.
	return Matrix4f(f*a.m00, f*a.m01, f*a.m02, f*a.m03, f*a.m10, f*a.m11, f*a.m12, f*a.m13, f*a.m20, f*a.m21, f*a.m22, f*a.m23, f*a.m30, f*a.m31, f*a.m32, f*a.m33);
}


inline Matrix4f Matrix4f::operator -() const
{
	// Return a Matrix multiplied by -1.
	return Matrix4f(-m00, -m01, -m02, -m03, -m10, -m11, -m12, -m13, -m20, -m21, -m22, -m23, -m30, -m31, -m32, -m33);

}


//***************************************************************************
// Matrix4f - Matrix4f operations.
//***************************************************************************

inline Matrix4f Matrix4f::operator +(const Matrix4f &a) const
{
	// Add one matrix to another elementwise.
	Matrix4f tmp;

	tmp.m00 = m00 + a.m00;
	tmp.m01 = m01 + a.m01;
	tmp.m02 = m02 + a.m02;
	tmp.m03 = m03 + a.m03;

	tmp.m10 = m10 + a.m10;
	tmp.m11 = m11 + a.m11;
	tmp.m12 = m12 + a.m12;
	tmp.m13 = m13 + a.m13;

	tmp.m20 = m20 + a.m20;
	tmp.m21 = m21 + a.m21;
	tmp.m22 = m22 + a.m22;
	tmp.m23 = m23 + a.m23;

	tmp.m30 = m30 + a.m30;
	tmp.m31 = m31 + a.m31;
	tmp.m32 = m32 + a.m32;
	tmp.m33 = m33 + a.m33;

	return tmp;
}


inline Matrix4f &Matrix4f::operator +=(const Matrix4f &a)
{
	// Add one matrix to another elementwise.

	m00 += a.m00;
	m01 += a.m01;
	m02 += a.m02;
	m03 += a.m03;

	m10 += a.m10;
	m11 += a.m11;
	m12 += a.m12;
	m13 += a.m13;

	m20 += a.m20;
	m21 += a.m21;
	m22 += a.m22;
	m23 += a.m23;

	m30 += a.m30;
	m31 += a.m31;
	m32 += a.m32;
	m33 += a.m33;

	return *this;
}


inline Matrix4f Matrix4f::operator -(const Matrix4f &a) const
{
	// subtract one matrix from another elementwise.
	Matrix4f tmp;

	tmp.m00 = m00 - a.m00;
	tmp.m01 = m01 - a.m01;
	tmp.m02 = m02 - a.m02;
	tmp.m03 = m03 - a.m03;

	tmp.m10 = m10 - a.m10;
	tmp.m11 = m11 - a.m11;
	tmp.m12 = m12 - a.m12;
	tmp.m13 = m13 - a.m13;

	tmp.m20 = m20 - a.m20;
	tmp.m21 = m21 - a.m21;
	tmp.m22 = m22 - a.m22;
	tmp.m23 = m23 - a.m23;

	tmp.m30 = m30 - a.m30;
	tmp.m31 = m31 - a.m31;
	tmp.m32 = m32 - a.m32;
	tmp.m33 = m33 - a.m33;

	return tmp;
}


inline Matrix4f &Matrix4f::operator -=(const Matrix4f &a)
{
	// subtract one matrix from another elementwise.

	m00 -= a.m00;
	m01 -= a.m01;
	m02 -= a.m02;
	m03 -= a.m03;

	m10 -= a.m10;
	m11 -= a.m11;
	m12 -= a.m12;
	m13 -= a.m13;

	m20 -= a.m20;
	m21 -= a.m21;
	m22 -= a.m22;
	m23 -= a.m23;

	m30 -= a.m30;
	m31 -= a.m31;
	m32 -= a.m32;
	m33 -= a.m33;

	return *this;
}


inline Matrix4f Matrix4f::operator *(const Matrix4f &a) const
{
	Matrix4f tmp;
	tmp.Multiply( *this, a );

	return tmp;
}


inline Matrix4f &Matrix4f::operator *=(const Matrix4f &a)
{
	Matrix4f tmp = *this;
	Multiply( tmp, a );

	return *this;
}


inline Vec4f operator *(const Vec4f &v, const Matrix4f &a)
{
	// Matrix pre-multiplied by a row-vector.
	Vec4f	tmp;

	tmp.x = v.x*a.m00 + v.y*a.m10 + v.z*a.m20 + v.w*a.m30;
	tmp.y = v.x*a.m01 + v.y*a.m11 + v.z*a.m21 + v.w*a.m31;
	tmp.z = v.x*a.m02 + v.y*a.m12 + v.z*a.m22 + v.w*a.m32;
	tmp.w = v.x*a.m03 + v.y*a.m13 + v.z*a.m23 + v.w*a.m33;

	return tmp;
}

inline Vec3f operator *(const Vec3f &v, const Matrix4f &a)
{
	return v*a.X;
}

inline Vec4f &operator *=(Vec4f &v, const Matrix4f &a)
{
	v = v * a;
	return v;
}

inline Vec3f &operator *=(Vec3f &v, const Matrix4f &a)
{
	v = v * a;
	return v;
}


//***************************************************************************
// Member Utility functions.
//***************************************************************************

inline void Matrix4f::TransposeSelf()
{
	// Transpose the current Matrix4f.
	float tmp;

	tmp = m01;
	m01 = m10;
	m10 = tmp;

	tmp = m20;
	m20 = m02;
	m02 = tmp;

	tmp = m03;
	m03 = m30;
	m30 = tmp;

	tmp = m12;
	m12 = m21;
	m21 = tmp;

	tmp = m13;
	m13 = m31;
	m31 = tmp;

	tmp = m32;
	m32 = m23;
	m23 = tmp;

}

inline Matrix4f Matrix4f::TransposeClone() const
{
	Matrix4f dup( *this );
	dup.TransposeSelf();

	return dup;
}

inline void Matrix4f::IdentitySelf()
{
	// Make the current Matrix4f an identity matrix.

	m00 = 1.0f;
	m11 = 1.0f;
	m22 = 1.0f;
	m33 = 1.0f;

	m01 = 0.0f;
	m02 = 0.0f;

	m10 = 0.0f;
	m12 = 0.0f;

	m20 = 0.0f;
	m21 = 0.0f;

	m30 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;

	m03 = 0.0f;
	m13 = 0.0f;
	m23 = 0.0f;
}


inline float Matrix4f::Det() const
{
	// Calculate the determinant of the Matrix4f.
	return R.Det();
}


inline void Matrix4f::MakeRotateX(const float rads)
{
	R.MakeRotateX( rads );
	T = Vec3f( 0 );
	W = Vec4f( 0,0,0,1 );
}


inline void Matrix4f::RotateXSelf(const float rads)
{
	// Rotate vector by r (in radians).
	Matrix4f tmp;
	tmp.MakeRotateX( rads );

	(*this) *= tmp;
}


inline Matrix4f Matrix4f::RotateXClone(const float rads) const
{
	// Rotate vector by r (in radians).
	Matrix4f tmp( *this );
	tmp.RotateXSelf(rads);

	return tmp;
}


inline void Matrix4f::MakeRotateY(const float rads)
{
	R.MakeRotateY( rads );
	T = Vec3f( 0 );
	W = Vec4f( 0,0,0,1 );
}


inline void Matrix4f::RotateYSelf(const float rads)
{
	// Rotate vector by r (in radians).
	Matrix4f tmp;
	tmp.MakeRotateY( rads );

	(*this) *= tmp;
}


inline Matrix4f Matrix4f::RotateYClone(const float rads) const
{
	// Rotate vector by r (in radians).
	Matrix4f tmp( *this );
	tmp.RotateYSelf(rads);

	return tmp;
}


inline void Matrix4f::MakeRotateZ(const float rads)
{
	R.MakeRotateZ( rads );
	T = Vec3f( 0 );
	W = Vec4f( 0,0,0,1 );
}


inline void Matrix4f::RotateZSelf(const float rads)
{
	// Rotate vector by r (in radians).
	Matrix4f tmp;
	tmp.MakeRotateZ( rads );

	(*this) *= tmp;
}


inline Matrix4f Matrix4f::RotateZClone(const float rads) const
{
	// Rotate vector by r (in radians).
	Matrix4f tmp( *this );
	tmp.RotateZSelf(rads);

	return tmp;
}

inline void Matrix4f::RotateAboutAxisSelf(const Vec3f &v, const float rads)
{
	// Taken from "Real-Time Rendering", pg 42.

	Matrix4f tmp;

	float cosr = cosf(rads);
	float sinr = sinf(rads);

	float one_minus_cosr = 1.0f - cosr;

	float xy_one_minus_cosr = v.x*v.y * one_minus_cosr;
	float xz_one_minus_cosr = v.x*v.z * one_minus_cosr;
	float yz_one_minus_cosr = v.y*v.z * one_minus_cosr;

	tmp.m00 = cosr + one_minus_cosr * v.x * v.x;
	tmp.m10 = xy_one_minus_cosr - v.z*sinr;
	tmp.m20 = xz_one_minus_cosr + v.y*sinr;

	tmp.m01 = xy_one_minus_cosr + v.z*sinr;
	tmp.m11 = cosr + one_minus_cosr * v.y * v.y;
	tmp.m21 = yz_one_minus_cosr - v.x*sinr;

	tmp.m02 = xz_one_minus_cosr - v.y*sinr;
	tmp.m12 = yz_one_minus_cosr + v.x*sinr;
	tmp.m22 = cosr + one_minus_cosr * v.z * v.z;

	tmp.m33 = 1.0f;

	(*this) *= tmp;
}

inline Matrix4f Matrix4f::RotateAboutAxisClone(const Vec3f &v, const float rads) const
{
	Matrix4f dup( *this );
	dup.RotateAboutAxisSelf( v, rads );

	return dup;
}

inline void Matrix4f::MakeLookAt( const Vec3f& origin, const Vec3f& target, const Vec3f& up)
{
	X.MakeLookAt( origin, target, up);
	W.Set( 0.0f);
}


inline void Matrix4f::MakeScale( const Vec3f& v )
{
	X.MakeScale(v);
	W = Vec4f(0,0,0,1);
}

inline void Matrix4f::ScaleSelf( const Vec3f& v )
{
	X.ScaleSelf( v );
}

inline Matrix4f Matrix4f::ScaleClone( const Vec3f& v ) const
{
	Matrix4f tmp( *this );
	tmp.X.ScaleSelf( v );
	return tmp;
}

inline void Matrix4f::MakeTranslate( const Vec3f& v )
{
	X.MakeTranslate(v);
	W = Vec4f(0,0,0,1);
}

inline void Matrix4f::TranslateSelf( const Vec3f& v )
{
	X.TranslateSelf( v );
}

inline Matrix4f Matrix4f::TranslateClone( const Vec3f& v ) const
{
	Matrix4f tmp( *this );
	tmp.TranslateSelf( v );

	return tmp;
}

#endif // _MATRIX4_H

