// Matrix4.h

#pragma once

#ifndef _MATRIX43_H
#define _MATRIX43_H

// Include Files.
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix3.h"


// Forward Declarations.
class Quatf;


class Matrix43f
{
	public:

		union
		{
			float e[12];
			float e2[4][3];
			struct
			{
				// Defined in a weird order so that we can return references to matrix3's for
				//	rotations etc...

				// m-row-column.
				float m00, m01, m02;	//	x x x
				float m10, m11, m12;	//	y y y
				float m20, m21, m22;	//	z z z
				float m30, m31, m32;	//	tx ty tz
			};
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
		};


		// Constructors.
		explicit inline Matrix43f();
		explicit inline Matrix43f(const float a);
		explicit inline Matrix43f(const float _m00, const float _m01, const float _m02, 
						const float _m10, const float _m11, const float _m12, 
						const float _m20, const float _m21, const float _m22, 
						const float _m30, const float _m31, const float _m32);
		explicit inline Matrix43f(const Vec3f &Xaxis, const Vec3f &Yaxis, const Vec3f &Zaxis, const Vec3f &_T);
		explicit inline Matrix43f(const Matrix3f Rot, const Vec3f &_T);
		explicit inline Matrix43f(const Quatf &q);
		explicit inline Matrix43f(const Quatf &q, const Vec3f &_T);

		// assignment
		inline void Set(const float a);
		inline void Set(const float _m00, const float _m01, const float _m02, 
						const float _m10, const float _m11, const float _m12, 
						const float _m20, const float _m21, const float _m22, 
						const float _m30, const float _m31, const float _m32);
		inline void Set(const Vec3f &Xaxis,const Vec3f &Yaxis,const Vec3f &Zaxis, const Vec3f &_T);
		inline void Set(const Matrix3f Rot, const Vec3f &_T);
		inline void Set(const Quatf &q);
		inline void Set(const Quatf &q, const Vec3f &_T);

		Matrix43f& Matrix43f::operator =(const float a);
		Matrix43f& Matrix43f::operator =(const Quatf &q);


		// Access Operations.
		// Note: Access the specific rows by Matrix43f.row[i], or Matrix43f.X_axis.
		float& operator[](int i) { return e[i]; }
		float operator[](int i) const { return e[i]; }

		inline const Vec3f& GetRow(size_t i) const;
		inline Vec3f& GetRow(size_t i);

		inline Vec4f GetColumnClone(const int Col) const;

		// Operators.

		// Matrix43f - float operations.
		Matrix43f operator *(const float a) const;
		Matrix43f &operator *=(const float f);
		friend Matrix43f operator *(const float a, const Matrix43f &b);
		Matrix43f operator -() const;

		// Matrix43f - Matrix43f operations.
		Matrix43f operator +(const Matrix43f &a) const;
		Matrix43f &operator +=(const Matrix43f &);
		Matrix43f operator -(const Matrix43f &a) const;
		Matrix43f &operator -=(const Matrix43f &);
		Matrix43f operator *(const Matrix43f &a) const;
		Matrix43f &operator *=(const Matrix43f &);

		// Matrix43f - VecXf operations
		friend Vec4f operator *(const Vec4f &v, const Matrix43f &a);
		friend Vec3f operator *(const Vec3f &v, const Matrix43f &a);
		friend Vec4f &operator *=(Vec4f &v, const Matrix43f &a);
		friend Vec3f &operator *=(Vec3f &v, const Matrix43f &a);

		//	binary comparison, no tollerance for differences
		bool operator ==(const Matrix43f& a) const;
		bool operator !=(const Matrix43f& a) const;


		// Member Utility functions.
		void		IdentitySelf();

		void		TransposeSelf();
		Matrix43f	TransposeClone() const;

		void		MakeRotateX(const float rads);
		void		RotateXSelf(const float rads);
		Matrix43f	RotateXClone(const float rads) const;

		void		MakeRotateY(const float rads);
		void		RotateYSelf(const float rads);
		Matrix43f	RotateYClone(const float rads) const;

		void		MakeRotateZ(const float rads);
		void		RotateZSelf(const float rads);
		Matrix43f	RotateZClone(const float rads) const;

		void		MakeRotateAboutAxis(const Vec3f &v, const float rads);
		void		RotateAboutAxisSelf(const Vec3f &v, const float rads);
		Matrix43f	RotateAboutAxisClone(const Vec3f &v, const float rads) const;

		void		MakeLookAt( const Vec3f& origin, const Vec3f& target, const Vec3f& up);

		void		MakeScale(const Vec3f &v);
		void		ScaleSelf(const Vec3f &v);
		Matrix43f	ScaleClone(const Vec3f &v) const;

		void		MakeTranslate(const Vec3f &v);
		void		TranslateSelf(const Vec3f &v);
		Matrix43f	TranslateClone(const Vec3f &v) const;

		float		Det() const;
		void		InverseSelf();
		Matrix43f	InverseClone() const;

		void		Multiply( const Matrix43f& a, const Matrix43f& b );
};


//***************************************************************************
// Constructors.
//***************************************************************************

inline Matrix43f::Matrix43f()
:	X_axis(), Y_axis(), Z_axis(), R(), T()
{
}


inline void Matrix43f::Set(const float a)
{
	m00 = a;
	m01 = 0.0f;
	m02 = 0.0f;
	m10 = 0.0f;
	m11 = a;
	m12 = 0.0f;
	m20 = 0.0f;
	m21 = 0.0f;
	m22 = a;
	m30 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;
}


inline void Matrix43f::Set( const float _m00, const float _m01, const float _m02, 
							const float _m10, const float _m11, const float _m12,
							const float _m20, const float _m21, const float _m22,
							const float _m30, const float _m31, const float _m32)
{ 
	m00 = _m00;
	m01 = _m01;
	m02 = _m02;
	m10 = _m10;
	m11 = _m11;
	m12 = _m12;
	m20 = _m20;
	m21 = _m21;
	m22 = _m22;
	m30 = _m30;
	m31 = _m31;
	m32 = _m32;
}


inline void Matrix43f::Set(const Vec3f &Xaxis,const Vec3f &Yaxis,const Vec3f &Zaxis, const Vec3f &_T)
{
	X_axis = Xaxis;
	Y_axis = Yaxis;
	Z_axis = Zaxis;
	T = _T;
}


inline void Matrix43f::Set(const Matrix3f Rot, const Vec3f &_T)
{
	R = Rot;
	T = _T;
}


inline void Matrix43f::Set(const Quatf &q)
{
	R = q;
	T.Set( 0.0f);
}


inline void Matrix43f::Set(const Quatf &q, const Vec3f &_T)
{
	R = q;
	T = _T;
}



inline Matrix43f::Matrix43f(const float a)
{
	Set( a);
}


inline Matrix43f::Matrix43f(const float _m00, const float _m01, const float _m02, 
							const float _m10, const float _m11, const float _m12,
							const float _m20, const float _m21, const float _m22,
							const float _m30, const float _m31, const float _m32)
{ 
	m00 = _m00;
	m01 = _m01;
	m02 = _m02;
	m10 = _m10;
	m11 = _m11;
	m12 = _m12;
	m20 = _m20;
	m21 = _m21;
	m22 = _m22;
	m30 = _m30;
	m31 = _m31;
	m32 = _m32;
}


inline Matrix43f::Matrix43f(const Vec3f &Xaxis,const Vec3f &Yaxis,const Vec3f &Zaxis, const Vec3f &_T)
{
	Set( Xaxis, Yaxis, Zaxis, _T);
}


inline Matrix43f::Matrix43f(const Matrix3f Rot, const Vec3f &_T)
{
	Set( Rot, _T);
}


inline Matrix43f::Matrix43f(const Quatf &q)
{
	Set( q);
}


inline Matrix43f::Matrix43f(const Quatf &q, const Vec3f &_T)
{
	Set( q, _T);
}


inline Matrix43f& Matrix43f::operator =(const float a)
{
	Set( a);
	return *this;
}


inline Matrix43f& Matrix43f::operator =(const Quatf &q)
{
	Set( q);
	return *this;
}


const Vec3f& Matrix43f::GetRow(size_t i) const
{
	dbAssert( i < 4 );
	return *( &X_axis + i );
}

Vec3f& Matrix43f::GetRow(size_t i)
{
	dbAssert( i < 4 );
	return *( &X_axis + i );
}

Vec4f Matrix43f::GetColumnClone(const int Col) const
{
	dbAssert( Col < 3 );
	return Vec4f(e2[0][Col], e2[1][Col], e2[2][Col], e2[2][Col]);
}

//***************************************************************************
// Matrix43f - float operations.
//***************************************************************************

//Matrix43f operator *(const Matrix43f &b, const float a)
inline Matrix43f Matrix43f::operator *(const float f) const
{
	// Matrix multiplied by a float.
	return Matrix43f(f*m00, f*m01, f*m02, f*m10, f*m11, f*m12, f*m20, f*m21, f*m22, f*m30, f*m31, f*m32);
}

inline Matrix43f& Matrix43f::operator *=(const float f)
{
	m00 *= f;
	m01 *= f;
	m02 *= f;

	m10 *= f;
	m11 *= f;
	m12 *= f;

	m20 *= f;
	m21 *= f;
	m22 *= f;

	m30 *= f;
	m31 *= f;
	m32 *= f;

	return *this;
}


inline Matrix43f operator *(const float f, const Matrix43f &a)
{
	// Matrix multiplied by a float.
	return Matrix43f(f*a.m00, f*a.m01, f*a.m02, f*a.m10, f*a.m11, f*a.m12, f*a.m20, f*a.m21, f*a.m22, f*a.m30, f*a.m31, f*a.m32);
}


inline Matrix43f Matrix43f::operator -() const
{
	// Return a Matrix multiplied by -1.
	return Matrix43f(-m00, -m01, -m02, -m10, -m11, -m12, -m20, -m21, -m22, -m30, -m31, -m32);

}


//***************************************************************************
// Matrix43f - Matrix43f operations.
//***************************************************************************

inline Matrix43f Matrix43f::operator +(const Matrix43f &a) const
{
	// Add one matrix to another elementwise.
	Matrix43f tmp;

	tmp.m00 = m00 + a.m00;
	tmp.m01 = m01 + a.m01;
	tmp.m02 = m02 + a.m02;

	tmp.m10 = m10 + a.m10;
	tmp.m11 = m11 + a.m11;
	tmp.m12 = m12 + a.m12;

	tmp.m20 = m20 + a.m20;
	tmp.m21 = m21 + a.m21;
	tmp.m22 = m22 + a.m22;

	tmp.m30 = m30 + a.m30;
	tmp.m31 = m31 + a.m31;
	tmp.m32 = m32 + a.m32;

	return tmp;
}


inline Matrix43f &Matrix43f::operator +=(const Matrix43f &a)
{
	// Add one matrix to another elementwise.

	m00 += a.m00;
	m01 += a.m01;
	m02 += a.m02;

	m10 += a.m10;
	m11 += a.m11;
	m12 += a.m12;

	m20 += a.m20;
	m21 += a.m21;
	m22 += a.m22;

	m30 += a.m30;
	m31 += a.m31;
	m32 += a.m32;

	return *this;
}


inline Matrix43f Matrix43f::operator -(const Matrix43f &a) const
{
	// subtract one matrix from another elementwise.
	Matrix43f tmp;

	tmp.m00 = m00 - a.m00;
	tmp.m01 = m01 - a.m01;
	tmp.m02 = m02 - a.m02;

	tmp.m10 = m10 - a.m10;
	tmp.m11 = m11 - a.m11;
	tmp.m12 = m12 - a.m12;

	tmp.m20 = m20 - a.m20;
	tmp.m21 = m21 - a.m21;
	tmp.m22 = m22 - a.m22;

	tmp.m30 = m30 - a.m30;
	tmp.m31 = m31 - a.m31;
	tmp.m32 = m32 - a.m32;

	return tmp;
}


inline Matrix43f &Matrix43f::operator -=(const Matrix43f &a)
{
	// subtract one matrix from another elementwise.

	m00 -= a.m00;
	m01 -= a.m01;
	m02 -= a.m02;

	m10 -= a.m10;
	m11 -= a.m11;
	m12 -= a.m12;

	m20 -= a.m20;
	m21 -= a.m21;
	m22 -= a.m22;

	m30 -= a.m30;
	m31 -= a.m31;
	m32 -= a.m32;

	return *this;
}


inline Matrix43f Matrix43f::operator *(const Matrix43f &a) const
{
	Matrix43f tmp;
	tmp.Multiply( *this, a );

	return tmp;
}


inline Matrix43f &Matrix43f::operator *=(const Matrix43f &a)
{
	Matrix43f tmp = *this;
	Multiply( tmp, a );

	return *this;
}


inline Vec4f operator *(const Vec4f &v, const Matrix43f &a)
{
	// Matrix pre-multiplied by a row-vector.
	Vec4f	tmp;

	tmp.x = v.x*a.m00 + v.y*a.m10 + v.z*a.m20 + v.w*a.m30;
	tmp.y = v.x*a.m01 + v.y*a.m11 + v.z*a.m21 + v.w*a.m31;
	tmp.z = v.x*a.m02 + v.y*a.m12 + v.z*a.m22 + v.w*a.m32;
	tmp.w = v.w;

	return tmp;
}


inline Vec3f operator *(const Vec3f &v, const Matrix43f &a)
{
	// Matrix pre-multiplied by a row-vector.
	Vec3f tmp;

	tmp.x = v.x*a.m00 + v.y*a.m10 + v.z*a.m20 + a.m30;
	tmp.y = v.x*a.m01 + v.y*a.m11 + v.z*a.m21 + a.m31;
	tmp.z = v.x*a.m02 + v.y*a.m12 + v.z*a.m22 + a.m32;

	return tmp;
}

inline Vec4f &operator *=(Vec4f &v, const Matrix43f &a)
{
	// Matrix pre-multiplied by a row-vector.
	Vec4f	tmp;

	tmp.x = v.x*a.m00 + v.y*a.m10 + v.z*a.m20 + v.w*a.m30;
	tmp.y = v.x*a.m01 + v.y*a.m11 + v.z*a.m21 + v.w*a.m31;
	tmp.z = v.x*a.m02 + v.y*a.m12 + v.z*a.m22 + v.w*a.m32;
	tmp.w = v.w;

	v = tmp;

	return v;
}

inline Vec3f &operator *=(Vec3f &v, const Matrix43f &a)
{
	// Matrix pre-multiplied by a row-vector.
	Vec3f tmp;

	tmp.x = v.x*a.m00 + v.y*a.m10 + v.z*a.m20 + a.m30;
	tmp.y = v.x*a.m01 + v.y*a.m11 + v.z*a.m21 + a.m31;
	tmp.z = v.x*a.m02 + v.y*a.m12 + v.z*a.m22 + a.m32;

	v = tmp;

	return v;
}


//***************************************************************************
// Member Utility functions.
//***************************************************************************

inline void Matrix43f::IdentitySelf()
{
	// Make the current Matrix43f an identity matrix.

	R.IdentitySelf();
	T = Vec3f(0.0f);
}

inline void		Matrix43f::TransposeSelf()
{
	R.TransposeSelf();
	//T = Vec3f(0.0f);
}


inline void Matrix43f::MakeTranslate(const Vec3f &v)
{
	R.IdentitySelf();
	T = v;
}


inline void Matrix43f::TranslateSelf(const Vec3f &v)
{
	T += v;
}

inline Matrix43f Matrix43f::TranslateClone(const Vec3f &v) const
{
	Matrix43f dup( *this );
	dup.TranslateSelf( v );

	return dup;
}

inline void Matrix43f::MakeRotateX(const float rads)
{
	R.MakeRotateX(rads);
	T = Vec3f(0.0f);
}


inline Matrix43f Matrix43f::RotateXClone(const float rads) const
{
	// Rotate vector by r (in radians).
	Matrix43f tmp;

	tmp.MakeRotateX(rads);
	return (*this) * tmp;
}


inline void Matrix43f::RotateXSelf(const float rads)
{
	(*this) = this->RotateXClone(rads);
}


inline void Matrix43f::MakeRotateY(const float rads)
{
	R.MakeRotateY(rads);
	T = Vec3f(0.0f);
}


inline Matrix43f Matrix43f::RotateYClone(const float rads) const
{
	Matrix43f tmp;
	
	tmp.MakeRotateY(rads);
	return (*this) * tmp;
}


inline void Matrix43f::RotateYSelf(const float rads)
{
	(*this) = this->RotateYClone(rads);
}


inline void Matrix43f::MakeRotateZ(const float rads)
{
	R.MakeRotateZ(rads);
	T = Vec3f(0.0f);
}


inline Matrix43f Matrix43f::RotateZClone(const float rads) const
{
	Matrix43f tmp;

	tmp.MakeRotateZ(rads);
	return (*this) * tmp;
}


inline void Matrix43f::RotateZSelf(const float rads)
{
	(*this) = this->RotateZClone(rads);
}


inline void Matrix43f::MakeRotateAboutAxis(const Vec3f &v, const float rads)
{
	R.MakeRotateAboutAxis(v,rads);
	T = Vec3f(0.0f);
}


inline Matrix43f Matrix43f::RotateAboutAxisClone(const Vec3f &v, const float rads) const
{
	Matrix43f tmp;

	tmp.MakeRotateAboutAxis(v,rads);
	return (*this) * tmp;
}


inline void Matrix43f::RotateAboutAxisSelf(const Vec3f &v, const float rads)
{
	(*this) = this->RotateAboutAxisClone(v,rads);
}


inline void Matrix43f::MakeLookAt( const Vec3f& origin, const Vec3f& target, const Vec3f& up)
{
	R.MakeLookAt( origin, target, up);
	T = origin;
}

inline void Matrix43f::MakeScale(const Vec3f &v)
{
	R.MakeScale(v);
	T = Vec3f(0.0f);
}

inline void Matrix43f::ScaleSelf(const Vec3f &v)
{
	R.ScaleSelf(v);
	T *= v;
}

inline Matrix43f Matrix43f::ScaleClone(const Vec3f &v) const
{
	Matrix43f dup( *this );
	dup.ScaleSelf( v );

	return dup;
}

inline float Matrix43f::Det() const
{
	// Calculate the determinant of the Matrix43f.
	return R.Det();
}

#endif // _MATRIX43_H

