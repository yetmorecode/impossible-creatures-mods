// CollisionObject.h

#pragma once

#include <math/Vec3.h>
#include <math/Vec2.h>

class CollCircle
{
public:
//	Vec3f m_Pos;		// Position.
	float m_Radius;		// Size.
};

class CollRect
{
public:
//	Vec3f m_Pos;		// Position.
	float m_x, m_z;		// Extents.

//	float m_rads;		// Orientation.

	// Or Define orientation as the "forwards in z, vector, and the right vector"
	Vec2f X_axis;
	Vec2f Z_axis;

};


enum CollisionObjectType {CollisionCircle = 0, CollisionRect};

class CollisionObject
{

// Data.
public:
	CollisionObjectType m_CType;

	Vec3f m_Pos;		// Position.

	union
	{
		// Circle
		struct
		{
			CollCircle m_circ;
		};
		// Rectangle
		struct
		{
			CollRect m_rect;
		};
	};

private:

// Functions.
public:
	// collision test routine.
	bool IsColliding(CollisionObject &obj);

private:

};


