/////////////////////////////////////////////////////////////////////
// File    : FloodFill.h
// Desc    : 
// Created : Tuesday, February 19, 2002
// Author  : Dave Swinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include <Memory/MemoryAlloc.h>

class FloodFillDeque
{
// construction
public:
	FloodFillDeque( size_t initialSize );
	~FloodFillDeque();

// interface
public:

	bool			Empty( ) const;

	void			Clear( );

	void			Push( const Vec2i& cell );

	const Vec2i&	Top( ) const;
	void			Pop( );

// types
private:

	typedef std::vector< Vec2i, mempool_alloc<Vec2i> > ArrayType;

// implementation	
private:

	size_t		m_head;
	size_t		m_tail;
	ArrayType	m_array;
};

/////////////////////////////////////////////////////////////////////
//	Class	: FloodFillPackage
//	Desc.	: A base class that should be derived from to implement necessary floodfill tests
//
class FloodFillPackage
{
// types
public: 
	enum OnMatchAction
	{
		ACTION_Nothing,
		ACTION_Exit
	};

// interface
public:
	const Vec2i&			GetMin() const;
	const Vec2i&			GetMax() const;

public:
	virtual void			SetRange( const Vec2i& min, const Vec2i& max );		// sets the acceptable range of values
	virtual bool			InRange( int x, int y ) const;

	virtual bool			CanGoUp	  ( int y ) const;
	virtual bool			CanGoDown ( int y ) const;
	virtual bool			CanGoLeft ( int x ) const;
	virtual bool			CanGoRight( int x ) const;

	virtual bool			IsFindNextMatch( int x, int y ) const;

public:

	virtual bool			IsMatch( int x, int y ) const = 0;
	virtual OnMatchAction	OnMatch( int x, int y, int fillCount ) = 0;

// fields
protected:
	Vec2i m_rangeMin;
	Vec2i m_rangeMax;
};

//typedef std::deque<Vec2i> FloodFillStack;

int FloodFillPump( int x, int y, FloodFillDeque& floodStack, FloodFillPackage *pPackage );
bool FloodFillFindOpenPump( int& x, int& y, FloodFillPackage *pPackage );

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Vec2i& FloodFillPackage::GetMin() const
{
	return(m_rangeMin);
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Vec2i& FloodFillPackage::GetMax() const
{
	return(m_rangeMax);
}

