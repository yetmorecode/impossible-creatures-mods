// Matrix3.h
#pragma once

// Matrix3f is defined to perform rotations by post multiplying with a row vector.
//	Note: The inverse of a Rotation Matrix (orthonormal with respect to rows, i.e rows have
//	length of 1.0f and dot product with each other to 0) can be calculated by simply
//	taking the transpose of the matrix.


// Include Files.
#include "vec3.h"


// Forward Declarations.
class Quatf;

//***************************************************************************
//	Class Definition: Matrix3f
//***************************************************************************

class Matrix3f
{
	public:

		union
		{
			float e[9];
			float e2[3][3];
			struct
			{
				// m-row-column.
				float m00, m01, m02;	//	x x x
				float m10, m11, m12;	//	y y y
				float m20, m21, m22;	//	z z z
			};
			struct
			{
				Vec3f X_axis;
				Vec3f Y_axis;
				Vec3f Z_axis;
			};
		};


		// Constructors.
		explicit Matrix3f();
		explicit Matrix3f(const float a);
		explicit Matrix3f(float _m00, float _m01, float _m02, float _m10, float _m11, float _m12, float _m20, float _m21, float _m22);
		explicit Matrix3f(const Vec3f &Xaxis,const Vec3f &Yaxis,const Vec3f &Zaxis);
		explicit inline Matrix3f(const Quatf &q);

		void Set( const Quatf &q);
		inline Matrix3f &operator =( const Quatf &q);



		// Access Operations.
		// Note: Access the specific rows by Matrix3f.row[i], or Matrix3f.X_axis.
		float& operator[](int i) { return e[i]; }
		float operator[](int i) const { return e[i]; }

		inline const Vec3f& GetRow(size_t i) const;
		inline Vec3f& GetRow(size_t i);

		inline Vec3f GetColumnClone(const int Col) const;


		// Operators.

		// Matrix3f - float operations.
		Matrix3f operator *(const float a) const;
		Matrix3f &operator *=(const float f);
		friend Matrix3f operator *(const float a, const Matrix3f &b);
		Matrix3f operator -() const;

		// Matrix3f - Matrix3f operations.
		Matrix3f operator +(const Matrix3f &a) const;
		Matrix3f &operator +=(const Matrix3f &);
		Matrix3f operator -(const Matrix3f &a) const;
		Matrix3f &operator -=(const Matrix3f &);
		Matrix3f operator *(const Matrix3f &a) const;
		Matrix3f &operator *=(const Matrix3f &);
//		inline Vec3f operator *(const Vec3f &v) const;
		friend Vec3f operator *(const Vec3f &v, const Matrix3f &a);
		friend Vec3f &operator *=(Vec3f &v, const Matrix3f &a);


		// Member Utility functions.
		//	Make*	- Fills entire matrix with this operation
		//	*Self	- Performs this operation as a pre-multiplied matrix
		//	*Clone	- Returns this matrix pre-multiplied by this operation
		void		IdentitySelf();

		void		TransposeSelf();
		Matrix3f	TransposeClone() const;

		void		MakeRotateX(const float rads);
		void		RotateXSelf(const float rads);
		Matrix3f	RotateXClone(const float rads) const;

		void		MakeRotateY(const float rads);
		void		RotateYSelf(const float rads);
		Matrix3f	RotateYClone(const float rads) const;

		void		MakeRotateZ(const float rads);
		void		RotateZSelf(const float rads);
		Matrix3f	RotateZClone(const float rads) const;

		void		MakeRotateAboutAxis(const Vec3f &v, const float rads);
		void		RotateAboutAxisSelf(const Vec3f &v, const float rads);
		Matrix3f	RotateAboutAxisClone(const Vec3f &v, const float rads) const;

		void		MakeAlignXAxis( const Vec3f& xAxis );
		void		MakeAlignYAxis( const Vec3f& yAxis );
		void		MakeAlignZAxis( const Vec3f& zAxis );

		void		MakeLookAt( const Vec3f& origin, const Vec3f& target, const Vec3f& up);

		void		MakeScale(const Vec3f &v);
		void		ScaleSelf(const Vec3f &v);
		Matrix3f	ScaleClone(const Vec3f &v) const;

		float		Det() const;
		void		InverseSelf();
		Matrix3f	InverseClone() const;
};


//***************************************************************************
// Constructors.
//***************************************************************************

inline Matrix3f::Matrix3f()
:	X_axis(), Y_axis(), Z_axis()
{
}


inline Matrix3f::Matrix3f(const float a)
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
}


inline Matrix3f::Matrix3f(float _m00, float _m01, float _m02, float _m10, float _m11, float _m12, float _m20, float _m21, float _m22)
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
}


inline Matrix3f::Matrix3f(const Vec3f &Xaxis,const Vec3f &Yaxis,const Vec3f &Zaxis)
{
	X_axis = Xaxis;
	Y_axis = Yaxis;
	Z_axis = Zaxis;
}


inline Matrix3f::Matrix3f( const Quatf &q)
{
	Set( q);
}


inline Matrix3f& Matrix3f::operator =( const Quatf &q)
{
	Set( q);
	return *this;
}

const Vec3f& Matrix3f::GetRow(size_t i) const
{
	dbAssert( i < 3 );
	return *( &X_axis + i );
}

Vec3f& Matrix3f::GetRow(size_t i)
{
	dbAssert( i < 3 );
	return *( &X_axis + i );
}

Vec3f Matrix3f::GetColumnClone(const int Col) const
{
	dbAssert( Col < 3 );
	return Vec3f(e2[0][Col], e2[1][Col], e2[2][Col]);
}


//***************************************************************************
// Matrix3f - float operations.
//***************************************************************************

//Matrix3f operator *(const Matrix3f &b, const float a)
inline Matrix3f Matrix3f::operator *(const float f) const
{
	// Matrix multiplied by a float.
	return Matrix3f(f*m00, f*m01, f*m02, f*m10, f*m11, f*m12, f*m20, f*m21, f*m22);
}

inline Matrix3f& Matrix3f::operator *=(const float f)
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

	return *this;
}


inline Matrix3f operator *(const float f, const Matrix3f &a)
{
	// Matrix multiplied by a float.
	return Matrix3f(f*a.m00, f*a.m01, f*a.m02, f*a.m10, f*a.m11, f*a.m12, f*a.m20, f*a.m21, f*a.m22);
}


inline Matrix3f Matrix3f::operator -() const
{
	// Return a Matrix multiplied by -1.
	return Matrix3f(-m00, -m01, -m02, -m10, -m11, -m12, -m20, -m21, -m22);

}


//***************************************************************************
// Matrix3f - Matrix3f operations.
//***************************************************************************

inline Matrix3f Matrix3f::operator +(const Matrix3f &a) const
{
	// Add one matrix to another elementwise.
	Matrix3f tmp;

	tmp.m00 = m00 + a.m00;
	tmp.m01 = m01 + a.m01;
	tmp.m02 = m02 + a.m02;

	tmp.m10 = m10 + a.m10;
	tmp.m11 = m11 + a.m11;
	tmp.m12 = m12 + a.m12;

	tmp.m20 = m20 + a.m20;
	tmp.m21 = m21 + a.m21;
	tmp.m22 = m22 + a.m22;

	return tmp;
}


inline Matrix3f &Matrix3f::operator +=(const Matrix3f &a)
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

	return *this;
}


inline Matrix3f Matrix3f::operator -(const Matrix3f &a) const
{
	// subtract one matrix from another elementwise.
	Matrix3f tmp;

	tmp.m00 = m00 - a.m00;
	tmp.m01 = m01 - a.m01;
	tmp.m02 = m02 - a.m02;

	tmp.m10 = m10 - a.m10;
	tmp.m11 = m11 - a.m11;
	tmp.m12 = m12 - a.m12;

	tmp.m20 = m20 - a.m20;
	tmp.m21 = m21 - a.m21;
	tmp.m22 = m22 - a.m22;

	return tmp;
}


inline Matrix3f &Matrix3f::operator -=(const Matrix3f &a)
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

	return *this;
}


inline Matrix3f Matrix3f::operator *(const Matrix3f &a) const
{
	Matrix3f tmp;

	tmp.m00 = m00*a.m00 + m01*a.m10 + m02*a.m20;
	tmp.m01 = m00*a.m01 + m01*a.m11 + m02*a.m21;
	tmp.m02 = m00*a.m02 + m01*a.m12 + m02*a.m22;

	tmp.m10 = m10*a.m00 + m11*a.m10 + m12*a.m20;
	tmp.m11 = m10*a.m01 + m11*a.m11 + m12*a.m21;
	tmp.m12 = m10*a.m02 + m11*a.m12 + m12*a.m22;

	tmp.m20 = m20*a.m00 + m21*a.m10 + m22*a.m20;
	tmp.m21 = m20*a.m01 + m21*a.m11 + m22*a.m21;
	tmp.m22 = m20*a.m02 + m21*a.m12 + m22*a.m22;

	return tmp;
}


inline Matrix3f &Matrix3f::operator *=(const Matrix3f &a)
{
	Matrix3f tmp = *this;
	*this = tmp * a;

	return *this;
}


inline Vec3f operator *(const Vec3f &v, const Matrix3f &a)
{
	// Matrix pre-multiplied by a row-vector.
	Vec3f	tmp;

	tmp.x = v.x*a.m00 + v.y*a.m10 + v.z*a.m20;
	tmp.y = v.x*a.m01 + v.y*a.m11 + v.z*a.m21;
	tmp.z = v.x*a.m02 + v.y*a.m12 + v.z*a.m22;

	return tmp;
}

inline Vec3f &operator *=(Vec3f &v, const Matrix3f &a)
{
	// Matrix pre-multiplied by a row-vector.
	Vec3f	tmp;

	tmp.x = v.x*a.m00 + v.y*a.m10 + v.z*a.m20;
	tmp.y = v.x*a.m01 + v.y*a.m11 + v.z*a.m21;
	tmp.z = v.x*a.m02 + v.y*a.m12 + v.z*a.m22;

	v = tmp;

	return v;
}

//***************************************************************************
// Member Utility functions.
//***************************************************************************

inline void Matrix3f::TransposeSelf()
{
	// Transpose the this Matrix3f.
	//	The this Matrix is replaced by the new Matrix.

	float tmp;

	tmp = m01;
	m01 = m10;
	m10 = tmp;

	tmp = m12;
	m12 = m21;
	m21 = tmp;

	tmp = m20;
	m20 = m02;
	m02 = tmp;
}

inline Matrix3f Matrix3f::TransposeClone() const
{
	Matrix3f dup( *this );
	dup.TransposeSelf();

	return dup;
}

inline void Matrix3f::IdentitySelf()
{
	// Make the this Matrix3f an identity matrix.

	m00 = 1.0f;
	m11 = 1.0f;
	m22 = 1.0f;

	m01 = 0.0f;
	m02 = 0.0f;

	m10 = 0.0f;
	m12 = 0.0f;

	m20 = 0.0f;
	m21 = 0.0f;
}


inline float Matrix3f::Det() const
{
	// Calculate the determinant of the Matrix3f.
	return (float)(m00 * (m11*m22 - m12*m21) - m01 * (m10*m22 - m12*m20) + m02 * (m10*m21 - m11*m20));
}


inline void Matrix3f::MakeRotateX(const float rads)
{
	// Create a Rotation Matrix that is a rotation about the X axis by angle rads.
	float cosr, sinr;

	// Build the rotation Matrix.
	cosr = cosf(rads);
	sinr = sinf(rads);

	m00 = 1.0f;
	m01 = 0.0f;
	m02 = 0.0f;

	m10 = 0.0f;
	m11 = cosr;
	m12 = sinr;

	m20 = 0.0f;
	m21 = -sinr;
	m22 = cosr;
}


inline Matrix3f Matrix3f::RotateXClone(const float rads) const
{
	// Return the this Matrix, rotated by rads along the X axis.
	//	Note: The this Matrix is not altered.
	//	Note: Angle to rotate is specified in Radians - NOT degrees.

	Matrix3f tmp;

	tmp.MakeRotateX(rads);

	// Create the new Matrix.
	return (*this) * tmp;
}


inline void Matrix3f::RotateXSelf(const float rads)
{
	// Rotate the this Matrix along the X axis.
	//	Note: The this Matrix is replaced by this Matrix.
	//	Note: Angle to rotate is specified in Radians - NOT degrees.
	(*this) = this->RotateXClone(rads);
}


inline void Matrix3f::MakeRotateY(const float rads)
{
	float cosr, sinr;

	// Build the rotation Matrix.
	cosr = cosf(rads);
	sinr = sinf(rads);

	m00 = cosr;
	m01 = 0.0f;
	m02 = -sinr;

	m10 = 0.0f;
	m11 = 1.0f;
	m12 = 0.0f;

	m20 = sinr;
	m21 = 0.0f;
	m22 = cosr;
}

inline Matrix3f Matrix3f::RotateYClone(const float rads) const
{
	// Return the this Matrix, rotated by rads along the X axis.
	//	Note: The this Matrix is not altered.
	//	Note: Angle to rotate is specified in Radians - NOT degrees.

	Matrix3f tmp;

	tmp.MakeRotateY(rads);

	// Create the new Matrix.
	return (*this) * tmp;
}


inline void Matrix3f::RotateYSelf(const float rads)
{
	// Rotate the this Matrix along the Y axis.
	//	Note: The this Matrix is replaced by this Matrix.
	//	Note: Angle to rotate is specified in Radians - NOT degrees.
	(*this) = this->RotateYClone(rads);
}


inline void Matrix3f::MakeRotateZ(const float rads)
{
	float cosr, sinr;

	// Create the Rotation Matrix.
	cosr = cosf(rads);
	sinr = sinf(rads);

	m00 = cosr;
	m01 = sinr;
	m02 = 0.0f;

	m10 = -sinr;
	m11 = cosr;
	m12 = 0.0f;

	m20 = 0.0f;
	m21 = 0.0f;
	m22 = 1.0f;
}

inline Matrix3f Matrix3f::RotateZClone(const float rads) const
{
	// Return the this Matrix, rotated by rads along the Z axis.
	//	Note: The this Matrix is not altered.
	//	Note: Angle to rotate is specified in Radians - NOT degrees.
	Matrix3f tmp;

	tmp.MakeRotateZ(rads);
	return (*this) * tmp;
}


inline void Matrix3f::RotateZSelf(const float rads)
{
	// Rotate the this Matrix along the Z axis.
	//	Note: The this Matrix is replaced by this Matrix.
	//	Note: Angle to rotate is specified in Radians - NOT degrees.

	(*this) = this->RotateZClone(rads);
}

inline void Matrix3f::MakeRotateAboutAxis(const Vec3f &v, const float rads)
{
	// Create the Rotation Matrix.
	float cosr = cosf(rads);
	float sinr = sinf(rads);

	float one_minus_cosr = 1.0f - cosr;

	float xy_one_minus_cosr = v.x*v.y * one_minus_cosr;
	float xz_one_minus_cosr = v.x*v.z * one_minus_cosr;
	float yz_one_minus_cosr = v.y*v.z * one_minus_cosr;

	m00 = cosr + one_minus_cosr * v.x * v.x;
	m10 = xy_one_minus_cosr - v.z*sinr;
	m20 = xz_one_minus_cosr + v.y*sinr;

	m01 = xy_one_minus_cosr + v.z*sinr;
	m11 = cosr + one_minus_cosr * v.y * v.y;
	m21 = yz_one_minus_cosr - v.x*sinr;

	m02 = xz_one_minus_cosr - v.y*sinr;
	m12 = yz_one_minus_cosr + v.x*sinr;
	m22 = cosr + one_minus_cosr * v.z * v.z;
}

inline Matrix3f Matrix3f::RotateAboutAxisClone(const Vec3f &v, const float rads) const
{
	// Taken from "Real-Time Rendering", pg 42.

	// Rotate the this Matrix around the axis v, by the angle rads.
	//	Note: The this Matrix is not altered.
	//	Note: Angle to rotate is specified in Radians - NOT degrees.

	// todo: This function is untested.

	Matrix3f tmp;

	// Create the Rotation Matrix.
	tmp.MakeRotateAboutAxis(v,rads);

	// Create the new rotated Matrix.
	return (*this) * tmp;
}


inline void Matrix3f::RotateAboutAxisSelf(const Vec3f &v, const float rads)
{
	// Taken from "Real-Time Rendering", pg 42.

	// Rotate the this Matrix around the axis v, by the angle rads.
	//	Note: The this Matrix is replaced by the new Matrix.
	//	Note: Angle to rotate is specified in Radians - NOT degrees.
	(*this) = this->RotateAboutAxisClone(v,rads);
}


inline void Matrix3f::MakeScale(const Vec3f &v)
{
	m00 = v.x;	m01 = 0;	m02 = 0;
	m10 = 0;	m11 = v.y;	m12 = 0;
	m20 = 0;	m21 = 0;	m22 = v.z;
}

inline void Matrix3f::ScaleSelf(const Vec3f &v)
{
	X_axis *= v;
	Y_axis *= v;
	Z_axis *= v;
}

inline Matrix3f Matrix3f::ScaleClone(const Vec3f &v) const
{
	Matrix3f dup( *this );
	dup.ScaleSelf( v );

	return dup;
}

//	eof