/////////////////////////////////////////////////////////////////////
//	File	: Sphere.h
//	Desc.	: 3d Sphere bounding volume
//		(c)2000 relic entertainment inc.
//
#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix43.h>

class Spheref
{
public:
	Spheref()	:	c(0), r(0)							{}
	Spheref( const Vec3f &c, float r )					{	this->c = c; this->r = r;	}
	Spheref( const Vec3f* pVerts, int nVerts )			{	Build( pVerts, nVerts );	}

	void			Build( const Vec3f* pVerts, int nVerts );

	void			Transform( const Matrix43f& m )		{	c *= m;	}

	const Vec3f&	GetCentre() const				{	return c;	}
	void			SetCentre( const Vec3f& v )		{	c = v;		}
	float			GetRadius() const				{	return r;	}
	void			SetRadius( float f )			{	r = f;		}

	const Spheref&	operator+=( const Spheref& rhs );

private:
	Vec3f	c;
	float	r;
};

//	eof
