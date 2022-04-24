/////////////////////////////////////////////////////////////////////
// File    : SparseGridEntry.h
// Desc    : 
// Created : Monday, January 21, 2002
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//


#pragma once

#include "PathTypes.h"

#include <Assist/StlExSmallVector.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class GroundDynamics;
class SparseGridEntry;

/////////////////////////////////////////////////////////////////////
//	struct	: SparseGridSpan
//
struct SparseGridSpan
{
	int					CalcValue( int x ) const;

	int					start;	// the x value that this span starts at (inclusive)
	int					end;	// the x value that this span starts at (inclusive)

	SparseGridSpan*		pNext;	// doubly-linked goodness
	SparseGridSpan*		pPrev;	// doubly-linked goodness

	int					y;
	int					value;	//
	SparseGridEntry*	pEntry;	// pointer back to the owning SparseGridEntry

	int					pad;	// to round to 8 ints
};

/////////////////////////////////////////////////////////////////////
//	Class	: SparseGridEntry
//
class SparseGridEntry
{
	friend class SparseGrid;

// Methods
public:
	SparseGridEntry( size_t entitySize );								// ctor
	~SparseGridEntry();													// dtor

	inline void				SetGroundDynamics( GroundDynamics *pGD );	// sets the pointer back to the owner
	inline const GroundDynamics*
							GetGroundDynamics() const;

	inline void				SetGroundLevel( int groundLevel );			// set the ground level the entity is at
	inline int				GetGroundLevel( ) const;					// get the ground level the entity is at

	inline void				SetIsMoving( bool bIsMoving );				// set whether or not the entity is moving
	inline bool				GetIsMoving( ) const;						// gets whether or not the entity is moving

	inline bool				IsAdded() const;							// has been added?
	inline int				GetSize() const;							// returns the size
	inline int				GetEntitySize() const;						// returns the entity size
	inline SparseGridSpan*	GetSpan( size_t index );					// gets a span at the given index

// Methods
private:
	inline void				SetAdded( bool tf );						// sets whether or not it has been added
	
	void					InitSpans( );
	void					FreeSpans( );

// Types:
private:
	
	typedef std::smallvector<SparseGridSpan*, MAX_ENTITY_SIZE+MAX_ENTITY_DIAMETER*2> Spans;

// Data
private:

	GroundDynamics*	m_pGD;				// point back to the owning GroundDynamics
	bool			m_bAdded;			// has this enity already been added
	int				m_entitySize;		//
	size_t			m_size;				// the number of spans vertically

	// ground parameters
	int				m_groundLevel;		// what ground level the entity is on. 0 is above ground, 1 is dug in.
	bool			m_bIsMoving;		// is the entity moving


//	SparseGridSpan* m_spans;			// the span array
	Spans			m_spans;
};

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void SparseGridEntry::SetGroundDynamics( GroundDynamics *pGD )
{
	dbAssert(pGD);
	m_pGD = pGD;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const GroundDynamics* SparseGridEntry::GetGroundDynamics() const
{
	return m_pGD;
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int SparseGridEntry::GetSize() const
{
	return m_size;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int SparseGridEntry::GetEntitySize() const
{
	return m_entitySize;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline SparseGridSpan* SparseGridEntry::GetSpan( size_t index )
{
	return( m_spans[index] );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline bool SparseGridEntry::IsAdded() const
{
	return m_bAdded;
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void SparseGridEntry::SetAdded( bool tf )
{
	m_bAdded = tf;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: sets the ground level
//
inline void	SparseGridEntry::SetGroundLevel( int groundLevel )
{
	m_groundLevel = groundLevel;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the ground level
//
inline int SparseGridEntry::GetGroundLevel( ) const
{
	return m_groundLevel;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: sets the ground level
//
inline void	SparseGridEntry::SetIsMoving( bool bIsMoving )
{
	m_bIsMoving = bIsMoving;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the ground level
//
inline bool SparseGridEntry::GetIsMoving( ) const
{
	return m_bIsMoving;
}
