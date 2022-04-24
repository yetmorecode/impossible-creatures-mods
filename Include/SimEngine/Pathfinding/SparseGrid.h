/////////////////////////////////////////////////////////////////////
// File    : SparseGrid.h
// Desc    : 
// Created : Monday, January 21, 2002
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h"


#include <Memory/MemoryAlloc.h>
#include <Memory/MemorySmall.h>
#include "SparseGridEntry.h"

#include <SimEngine/GroundDynamics.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class GroundDynamics;
class GroundDynamics;


/////////////////////////////////////////////////////////////////////
//	Class	:  SparseGrid
//
class SIM_API SparseGrid
{
public:
	SparseGrid();																			// ctor
	~SparseGrid();																			// dtor

	static void* operator new   ( size_t );													// operator new
	static void  operator delete( void* );													// operator delete	

	void			SetSize  ( int width, int height );										// sets the size of the grid

	inline int		GetWidth () const;														// returns the width
	inline int		GetHeight() const;														// returns the height

	int				GetValue( int x, int y ) const;											// returns the value at the given coord

	void			Add		( SparseGridEntry *pEntry, const Vec2i& center );				// adds the Entry at center
	void			Remove	( SparseGridEntry *pEntry );									// removes the given entry
	void			MoveTo	( SparseGridEntry *pEntry, const Vec2i& center );				// moves the entity to

#if 0
	bool			IsOccupied( const Vec2i& min, const Vec2i& max, bool bOnlyStopped = false );
#endif

	void			GetGroundDynamicsInRect( const Vec2i& min, const Vec2i& max, std::set<const GroundDynamics*>& dynamics ) const;

	void			FillData( std::vector<unsigned char>& data ) const;
	void			Dump( const char *filename ) const;

	bool			CanStand( const Vec2i& center, const GroundAttributes& attributes, unsigned long ignoreFlags ) const;

	size_t			GetMemUsage() const;

private:

	inline SparseGridSpan*			GetHead( int bucket, int y );										// returns the head at y-coord
	inline SparseGridSpan*			GetTail( int bucket, int y );										// returns the tail at y-coord

	inline const SparseGridSpan*	GetHead( int bucket, int y ) const;									// returns the head at y-coord (const)
	inline const SparseGridSpan*	GetTail( int bucket, int y ) const;									// returns the tail at y-coord (const)

	void					AddSpan( SparseGridSpan *pSpan, int bucket, int start, int end, int y );	// adds the given span at the given location
	void					RemoveSpan( SparseGridSpan *pSpan );										// removes the given span

	SparseGridSpan*			FindPrevInBucket( int bucket, int start, int end, int y );					// finds the span previous to the given location

	const SparseGridSpan*	FindSpanThatMightOverlap( int bucket, int x, int y ) const;

	void					InsertAfter( SparseGridSpan *pInsert, SparseGridSpan *pPrev );	// inserts the pInsert span after the pPrev span

	bool					DoCanStandBucket( int bucket, const Vec2i& center, const GroundAttributes& attributes, unsigned long ignoreFlags ) const;

	const SparseGridSpan*	FindSpanThatMightOverlap_Head( int bucket, int x, int y ) const;
	const SparseGridSpan*	FindSpanThatMightOverlap_Tail( int bucket, int x, int y ) const;

	int						GetValueFromBucket( int bucket, int x, int y ) const;
	int						GetNearBucketEdge( int bucket, int x ) const;
	void					GetGroundDynamicsInRect_Bucket( int bucket, const Vec2i& min, const Vec2i& max, std::set<const GroundDynamics*>& dynamics ) const;

	// bucket helpers
	inline int				FindBucket( int x ) const;
	inline int				FindBucketStart( int bucket ) const;
	inline int				FindBucketEnd( int bucket ) const;
	inline int				FindBucketCenter( int bucket ) const;

private:

	enum
	{
		BUCKET_SIZE			= 64,
		BUCKET_SIZE_OVER2	= 32,
		BUCKET_SIZE_SHIFT	= 6
	};

	int m_width, m_height;

	int m_numBuckets;

	typedef std::vector< SparseGridSpan /*, mempool_alloc<SparseGridSpan>*/ > HeadTailer;

	HeadTailer*	m_heads;										// doubly-linked list heads
	HeadTailer*	m_tails;										// doubly-linked list tails
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the width of the SparseGrid
//	Result	: int
//	Author	: dswinerd
//
inline int SparseGrid::GetWidth() const
{
	return m_width;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the height of the SparseGrid
//	Result	: int
//	Author	: dswinerd
//
inline int SparseGrid::GetHeight() const
{
	return m_height;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the linked-list head at y-coord
//	Result	: returns SparseGridSpan*
//	Param.	: y
//	Author	: dswinerd
//
inline SparseGridSpan* SparseGrid::GetHead( int bucket, int y )
{
	dbAssert( bucket >= 0 && bucket < m_numBuckets );
	dbAssert( y >= -MAX_ENTITY_DIAMETER && y < m_height+MAX_ENTITY_DIAMETER );

	y += MAX_ENTITY_DIAMETER;
	return &m_heads[bucket][y];
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the linked-list tail at y-coord
//	Result	: returns SparseGridSpan*
//	Param.	: y
//	Author	: dswinerd
//
inline SparseGridSpan* SparseGrid::GetTail( int bucket, int y )
{
	dbAssert( bucket >= 0 && bucket < m_numBuckets );
	dbAssert( y >= -MAX_ENTITY_DIAMETER && y < m_height+MAX_ENTITY_DIAMETER );

	y += MAX_ENTITY_DIAMETER;
	return &m_tails[bucket][y];
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the linked-list head at y-coord
//	Result	: returns const SparseGridSpan*
//	Param.	: y
//	Author	: dswinerd
//
inline const SparseGridSpan* SparseGrid::GetHead( int bucket, int y ) const
{
	dbAssert( bucket >= 0 && bucket < m_numBuckets );
	dbAssert( y >= -MAX_ENTITY_DIAMETER && y < m_height+MAX_ENTITY_DIAMETER );

	y += MAX_ENTITY_DIAMETER;
	return &m_heads[bucket][y];
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the linked-list tail at y-coord
//	Result	: returns const SparseGridSpan*
//	Param.	: y
//	Author	: dswinerd
//
inline const SparseGridSpan* SparseGrid::GetTail( int bucket, int y ) const
{
	dbAssert( bucket >= 0 && bucket < m_numBuckets );
	dbAssert( y >= -MAX_ENTITY_DIAMETER && y < m_height+MAX_ENTITY_DIAMETER );

	y += MAX_ENTITY_DIAMETER;
	return &m_tails[bucket][y];
}


inline int SparseGrid::FindBucket( int x ) const
{
	if ( x < 0 )
	{
		x = 0;
	}
	if ( x >= m_width )
	{
		x = m_width - 1;
	}
	return x >> BUCKET_SIZE_SHIFT;
}

inline int SparseGrid::FindBucketStart( int bucket ) const
{
	return bucket * BUCKET_SIZE;
}

inline int SparseGrid::FindBucketEnd( int bucket ) const
{
	return FindBucketStart( bucket+1 ) - 1;
}

inline int SparseGrid::FindBucketCenter( int bucket ) const
{
	return ( bucket * BUCKET_SIZE ) + BUCKET_SIZE_OVER2;
}
