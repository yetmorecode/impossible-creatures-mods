/////////////////////////////////////////////////////////////////////
//	File	: obb.h
//	Desc.	: Oriented bounding boxes
//	Created	: Apr 1 2000, Yann Cleroux
//		(c) 2000 relic entertainment inc.
//
#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix43.h>


/////////////////////////////////////////////////////////////////////
//	Desc.	: Used for highly optimized obb collision detection
//
class OBB3f
{
public:
	OBB3f();

	void Build( const Vec3f *verts, long nVerts );
	void Build( const Vec3f &min, const Vec3f &max );

	void Transform( const Matrix43f &transform );

	// find minimum distance between box and point
	float GetMinDistance(const Vec3f &) const;

	// create box vertices and store result into pv
	void GetVertices(Vec3f *pv) const;

	inline float GetXScale(void) const { return m_scale.x; }
	inline float GetYScale(void) const { return m_scale.y; }
	inline float GetZScale(void) const { return m_scale.z; }

	// Find the intersection of the ray through this obb.
	bool IntersectRay( const Vec3f &point, const Vec3f &ray, Vec3f &intersect ) const;
	bool IntersectPoint( const Vec3f &point ) const;

	// Return the centre of the obb
	inline const Vec3f&		GetCentre() const				{	return m_transform.T;	}
	inline Vec3f&			GetCentre()						{	return m_transform.T;	}

	// Return the OBB orientation
	inline const Matrix43f& GetTransform() const			{	return m_transform;	}
	inline Matrix43f&		GetTransform()					{	return m_transform;	}
	void					SetTransform( const Matrix43f& );

	//	Return the scale
	inline const Vec3f&		GetScale() const				{	return m_scale;	}
	void					SetScale( const Vec3f& n );

	float					GetSurfaceArea() const;

	const OBB3f&	operator +=( const OBB3f& rhs );
	bool			operator ==( const OBB3f& rhs );
	bool			operator !=( const OBB3f& rhs );

private:
	Matrix43f		m_transform;			// box transform
	Vec3f			m_scale;				// box size (radius in each 6 (3) direction)
};

inline bool OBB3f::operator ==( const OBB3f& rhs )
{
	return ( (m_scale==rhs.m_scale) && (m_transform==rhs.m_transform) );
}

inline bool OBB3f::operator !=( const OBB3f& rhs )
{
	return ( (m_scale!=rhs.m_scale) || (m_transform!=rhs.m_transform) );
}