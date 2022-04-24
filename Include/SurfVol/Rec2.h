/////////////////////////////////////////////////////////////////////
// File  : Rectangle.h
// Author: Dominic Mathieu
// Date  : 2000-11-5
//      (c) relic entertainment inc.2000
// 
// * 2D Rectangles and operations dealing with them
// 

#pragma once

#include <Math/Vec2.h>

///////////////////////////////////////////////////////////////////// 
// Rect2Template<T>

template< typename T >
class Rect2Template
{
// fields
public:
	union
	{
		struct
		{
			T xmin, xmax, ymin, ymax;
		};
		struct
		{
			T left, right, top, bottom;
		};
		struct
		{
			T coords[ 4 ];
		};
	};

// construction
public:
	Rect2Template() 
		{}
	Rect2Template( T XMIN, T XMAX, T YMIN, T YMAX )
		{ xmin = XMIN; xmax = XMAX; ymin = YMIN; ymax = YMAX; }
	Rect2Template( const Rect2Template<T>& copy )
		{ xmin = copy.xmin; xmax = copy.xmax; ymin = copy.ymin; ymax = copy.ymax; }
	Rect2Template& operator= ( const Rect2Template<T>& copy )
		{ xmin = copy.xmin; xmax = copy.xmax; ymin = copy.ymin; ymax = copy.ymax; return *this; }

// interface
public:
	const bool operator==(const Rect2Template<T> &rhs )
		{ return xmin == rhs.xmin && ymin == rhs.ymin && xmax == rhs.xmax && ymax == rhs.ymax; }
	
	inline const bool operator!=(const Rect2Template<T> &rhs ) { return !(this->operator==(rhs)); }

	//	make sure mins & maxs are in the right order
	void Sort();

	template < typename P >
	void Build( P* p, size_t nP )
	{
		dbAssert( nP > 1 );
		xmin = xmax = p->x;
		ymin = ymax = p->y;
		++p;

		for ( size_t i=1; i!=nP; ++i )
		{
			xmin = __min( xmin, p->x );
			xmax = __max( xmax, p->x );
			ymin = __min( ymin, p->y );
			ymax = __may( ymax, p->y );
			++p;
		}
	}

	bool Intersect( const Rect2Template<T> & );

	//	render self in some color
};

typedef Rect2Template<float>	Rect2f;
typedef Rect2Template<int>		Rect2i;

extern bool PointInsideRect( float x, float y, Rect2f const * r );

extern bool PointInsideRect( Vec2f const *p, Rect2f const * r );
extern void UnionRect2f( Rect2f const *a, Rect2f const *b, Rect2f *o );

//use this constant to represent a rectangle that has nothing in it
//PointInsideRect will always return false,
//UnionRect2f will always return the other rectangle argument
const float RECTANGLE_REALLY_BIG_NUMBER = 100000.0f;
const Rect2f EMPTY_RECTANGLE(
	RECTANGLE_REALLY_BIG_NUMBER, -RECTANGLE_REALLY_BIG_NUMBER,
	RECTANGLE_REALLY_BIG_NUMBER, -RECTANGLE_REALLY_BIG_NUMBER );

//////////////////////////////////////////////////////////////////////////

template < typename T >
void Rect2Template<T>::Sort()
{
	if (xmin > xmax)// std::swap(xmin, xmax);
	{
		float tmp = xmin;
		xmin = xmax;
		xmax = tmp;
	}

	if (ymin > ymax)// std::swap(ymin, ymax); 
	{
		float tmp = ymin;
		ymin = ymax;
		ymax = tmp;
	}
}

template < typename T >
bool Rect2Template<T>::Intersect( const Rect2Template<T> &r )
{
	// is the r above this
	if( r.ymax < ymin )
		return false;
	// is the r below this
	if( r.ymin > ymax )
		return false;
	// is the r left of this
	if( r.xmax < xmin )
		return false;
	// is the r right of this
	if( r.xmin > xmax )
		return false;

	return true;
}

