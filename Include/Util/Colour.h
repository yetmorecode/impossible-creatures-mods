/////////////////////////////////////////////////////////////////////
//	File	: Colour.h
//	Desc.	: 32bit ARGB colour class
//		(c)2000 relic entertainment inc.
//

#pragma once

#include "UtilExports.h"

#include <Math/Vec4.h>

///////////////////////////////////////////////////////////////////// 
// Colour

class Colour
{
public:
	enum PreDefinedColour
	{
		White	= 0xffffffff,
		Black	= 0xff000000,
		Red		= 0xffff0000,
		Green	= 0xff00ff00,
		Blue	= 0xff0000ff,
	};

	// allow 32bit long to share space with the 4 colour elements
	union	
	{
		struct
		{
			unsigned char b, g, r, a;	//	Note: D3D colour format
		};

		unsigned long c;				//	Note: We will need to watch for byte order on this...
	};

public:
	Colour()	
		{ ; }

	explicit Colour( unsigned long _c )
		{ c = _c; }

	explicit Colour( unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 0xff )
		{ r = _r, g = _g, b = _b, a = _a; }
		
	Colour( enum PreDefinedColour _c )
		{ c = unsigned long(_c); }

	Colour& operator = ( unsigned long _c )
		{ c = _c; return *this; };

	Colour& operator = ( enum PreDefinedColour _c )
		{ c = unsigned long(_c); return *this; };

	static Colour BuildFrom( float _r, float _g, float _b, float _a )
		{ 
			return Colour
				( 
				static_cast< unsigned char >( __max( 0, __min( 255, _r * 255.99f ) ) ), 
				static_cast< unsigned char >( __max( 0, __min( 255, _g * 255.99f ) ) ), 
				static_cast< unsigned char >( __max( 0, __min( 255, _b * 255.99f ) ) ), 
				static_cast< unsigned char >( __max( 0, __min( 255, _a * 255.99f ) ) )
				); 
		}

	static Colour BuildFrom( const Vec4f& v )
			{ return BuildFrom( v.r, v.g, v.b, v.a ); }

	static Colour BuildFrom( const Vec3f& v, float a )
			{ return BuildFrom( v.r, v.g, v.b, a ); }

	static Vec4f AsVec4f(const Colour& c)
			{ return Vec4f( c.r/float(0xff),
							c.g/float(0xff),
							c.b/float(0xff),
							c.a/float(0xff) ); }

	//	Linear [0.f,1.f] blend from 'a' to 'b'
	static Colour Blend( const Colour& a, const Colour& b, float blend )
			{
				dbAssert( blend >= 0.f );
				dbAssert( blend <= 1.f );
				
				return Colour(
					a.r + unsigned char((b.r-a.r)*blend),
					a.g + unsigned char((b.g-a.g)*blend),
					a.b + unsigned char((b.b-a.b)*blend),
					a.a + unsigned char((b.a-a.a)*blend)
					);
			}
};

///////////////////////////////////////////////////////////////////// 
// Colour utilities

inline bool operator== ( const Colour& lhs, const Colour& rhs )
{
	return lhs.c == rhs.c;
}

inline bool operator!= ( const Colour& lhs, const Colour& rhs )
{
	return !( lhs == rhs );
}

inline bool operator== ( const Colour& lhs, const Colour::PreDefinedColour& rhs )
{
	return lhs == Colour(rhs);
}


//	eof
