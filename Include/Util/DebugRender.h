/////////////////////////////////////////////////////////////////////
//	File	: DebugRender.h
//	Desc.	: 
//		(c)2000 relic entertainment inc.
//
#pragma once

#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <SurfVol/Sphere.h>
#include <SurfVol/OBB3.h>

#include "Colour.h"

/***
Usage notes:

	DebugRender::Draw( DebugRender::Tri( a, b, c, Colour::Red ) );

Implementation notes:
- still performs the Draw() calls, even though it will short circuit out early.
***/

class DebugRenderProxy;

/////////////////////////////////////////////////////////////////////
//	Name	: 
//	Desc.	: 
//	Notes	: 
//
class UTIL_API DebugRender
{
public:
/////////////////////////////////////////////////////////////////////
//
//	Proxy members
//
	static void	Register( DebugRenderProxy* );
	static void	Unregister( DebugRenderProxy* );

/////////////////////////////////////////////////////////////////////
//
//	Management methods
//
	static void Display();
	static void List( std::vector<const char*>& );

	static void SetDisplay( const char* frame, bool bDisplay );
	static bool GetDisplay( const char* frame );
	static void SetAutoClear( const char* frame, bool bClear );
	static bool GetAutoClear( const char* frame );
	static void Clear( const char* frame );

	static const char* GlobalFrame();

/////////////////////////////////////////////////////////////////////
//
//	Member data structures
//
	enum {	k_stringSize = 512	};

	struct Point2D
	{
		Point2D() {}
		Point2D( const Vec2f& _p, Colour _c )	{	p=_p,c=_c;	}

		Vec2f	p;
		Colour	c;
	};

	struct Line2D
	{
		Line2D() {}
		Line2D( const Vec2f& p1, const Vec2f& p2, Colour _c )	{	p[0]=p1,p[1]=p2,c=_c;	}

		Vec2f	p[2];
		Colour	c;
	};

	struct Tri2D
	{
		Tri2D() {}
		Tri2D( const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, Colour _c )	{	p[0]=p1,p[1]=p2,p[2]=p3,c=_c;	}

		Vec2f	p[3];
		Colour	c;
	};

	struct RectOutline
	{
		RectOutline() {}
		RectOutline(  const Vec2f& p1, const Vec2f& p2, Colour _c )		{	p[0]=p1,p[1]=p2,c=_c;	}

		Vec2f	p[2];
		Colour	c;
	};

    struct OvalOutline
	{
		OvalOutline() {}
		OvalOutline( const Vec2f& _p, float _rx, float _ry, Colour _c )	{	p=_p,rx=_rx,ry=_ry,c=_c;	}

		Vec2f	p;
		float	rx, ry;
		Colour	c;
	};

	struct Point
	{
		Point() {}
		Point( const Vec3f& _p, Colour _c )	{	p=_p,c=_c;	}

		Vec3f	p;
		Colour	c;
	};

	struct Line
	{
		Line() {}
		Line( const Vec3f& p1, const Vec3f& p2, Colour _c )	{	p[0]=p1,p[1]=p2,c=_c;	}

		Vec3f	p[2];
		Colour	c;
	};

	struct Tri
	{
		Tri() {}
		Tri( const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, Colour _c )	{	p[0]=p1,p[1]=p2,p[2]=p3,c=_c;	}

		Vec3f	p[3];
		Colour	c;
	};

	struct Cube
	{
		Cube() {}
		Cube( const OBB3f& obb, Colour _c )		{	obb.GetVertices( p ),c=_c;	}

		Vec3f	p[8];
		Colour	c;
	};

	struct Sphere
	{
		Sphere() {}
		Sphere( const Spheref& s, Colour _c )			{	p=s.GetCentre(),r=s.GetRadius(),c=_c;	}
		Sphere( const Vec3f& _p, float _r, Colour _c )	{	p=_p,r=_r,c=_c;	}

		Vec3f	p;
		float	r;
		Colour	c;
	};

	struct Text2D
	{
		Text2D()	{}
		Text2D( Vec2f v, const char* _str, Colour _c )	{ p=v; strncpy(str, _str, k_stringSize-1); str[k_stringSize-1] = '\0'; c=_c; }

		Vec2f		p;
		char	str[k_stringSize];
		Colour		c;
	};

	struct Text3D
	{
		Text3D()	{}
		Text3D( Vec3f v, const char* _str, Colour _c )	{ p=v; strncpy(str, _str, k_stringSize-1); str[k_stringSize-1] = '\0'; c=_c; }

		Vec3f		p;
		char	str[k_stringSize];
		Colour		c;
	};

/////////////////////////////////////////////////////////////////////
//
//	Drawing methods
//
	static void	Draw( const Point2D&, const char* frame=GlobalFrame() );
	static void	Draw( const Line2D&, const char* frame=GlobalFrame() );
	static void	Draw( const Tri2D&, const char* frame=GlobalFrame() );
	static void	Draw( const RectOutline&, const char* frame=GlobalFrame() );
	static void	Draw( const OvalOutline&, const char* frame=GlobalFrame() );
	static void	Draw( const Point&, const char* frame=GlobalFrame() );
	static void	Draw( const Line&, const char* frame=GlobalFrame() );
	static void	Draw( const Tri&, const char* frame=GlobalFrame() );
	static void	Draw( const Cube&, const char* frame=GlobalFrame() );
	static void	Draw( const Sphere&, const char* frame=GlobalFrame() );
	static void	Draw( const Text2D&, const char* frame=GlobalFrame() );
	static void	Draw( const Text3D&, const char* frame=GlobalFrame() );

private:
	static DebugRenderProxy*	s_pProxy;
};

/////////////////////////////////////////////////////////////////////
//	Name	: DebugRenderProxy
//	Desc.	: Performs the drawing of the data
//	Notes	: Implemented in RenderAnim, detaches spooge dependancy
//
class UTIL_API DebugRenderProxy
{
public:

	virtual ~DebugRenderProxy();

	virtual void	Display() = 0;
	virtual void	List( std::vector<const char*>& ) = 0;

	virtual void	SetDisplay( const char* frame, bool bDisplay ) = 0;
	virtual bool	GetDisplay( const char* frame ) = 0;
	virtual void	SetAutoClear( const char* frame, bool bClear ) = 0;
	virtual bool	GetAutoClear( const char* frame ) = 0;
	virtual void	Clear( const char* frame ) = 0;

	virtual void	Draw( const DebugRender::Point2D&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::Line2D&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::Tri2D&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::RectOutline&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::OvalOutline&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::Point&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::Line&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::Tri&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::Cube&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::Sphere&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::Text2D&, const char* frame ) = 0;
	virtual void	Draw( const DebugRender::Text3D&, const char* frame ) = 0;
};

//	eof
