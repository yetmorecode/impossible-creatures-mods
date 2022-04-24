/////////////////////////////////////////////////////////////////////
// File    : IffMath.h
// Desc    : 
// Created : Friday, October 12, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "Iff.h"

#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <Math/Vec4.h>

#include <Math/Matrix2.h>
#include <Math/Matrix3.h>
#include <Math/Matrix4.h>

#include <Math/Matrix43.h>

#include <Math/Quat.h>

#include <Debug/ctAssert.h>

///////////////////////////////////////////////////////////////////// 
// 

template<> inline bool IFFWriteArray<Vec2f>( IFF& iff, const Vec2f p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFWriteArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFReadArray<Vec2f>( IFF& iff, Vec2f p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFReadArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFWriteArray<Vec2i>( IFF& iff, const Vec2i p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFWriteArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFReadArray<Vec2i>( IFF& iff, Vec2i p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFReadArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFWriteArray<Vec3f>( IFF& iff, const Vec3f p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFWriteArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFReadArray<Vec3f>( IFF& iff, Vec3f p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFReadArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFWriteArray<Vec4f>( IFF& iff, const Vec4f p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFWriteArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFReadArray<Vec4f>( IFF& iff, Vec4f p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFReadArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFWriteArray<Matrix3f>( IFF& iff, const Matrix3f p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFWriteArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFReadArray<Matrix3f>( IFF& iff, Matrix3f p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFReadArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFWriteArray<Matrix43f>( IFF& iff, const Matrix43f p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFWriteArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFReadArray<Matrix43f>( IFF& iff, Matrix43f p[], unsigned long c )
{
	COMPILE_TIME_ASSERT_FAILED< sizeof(p[0]) == ( LENGTHOF(p[0].e) * sizeof(p[0].e[0]) ) >();

	return IFFReadArray( iff, p[0].e, LENGTHOF(p[0].e)*c );
}

template<> inline bool IFFWriteArray<Matrix4f>( IFF& iff, const Matrix4f p[], unsigned long c )
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		success &= IFFWrite( iff, p[i].X_axis );
		success &= IFFWrite( iff, p[i].Y_axis );
		success &= IFFWrite( iff, p[i].Z_axis );
		success &= IFFWrite( iff, p[i].W );
	}

	return success;
}

template<> inline bool IFFReadArray<Matrix4f>( IFF& iff, Matrix4f p[], unsigned long c )
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		success &= IFFRead( iff, p[i].X_axis );
		success &= IFFRead( iff, p[i].Y_axis );
		success &= IFFRead( iff, p[i].Z_axis );
		success &= IFFRead( iff, p[i].W );
	}

	return success;
}
