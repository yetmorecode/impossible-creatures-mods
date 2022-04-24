/////////////////////////////////////////////////////////////////////
// File    : DirectionMap.cpp
// Desc    : 
// Created : 
// Author  : jrehn
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

/////////////////////////////////////////////////////////////////////
//	Class	: DirectionMap
//
class DirectionMap
{
// types
public:

	// This will fit into 2 bits.
	enum Dir
	{
		n = 0,
		e,
		s,
		w,
		max_dirs
	};

// interface
public:
	DirectionMap( const int x, const int z );
	~DirectionMap();

	inline void			SetCellFrom( const size_t x, const size_t z, Dir from );
	inline const Dir	GetCellFrom( const size_t x, const size_t z ) const;

	size_t				GetMemUsage() const;

// implementation
private:

// data
private:
	unsigned long*	m_cells;
	size_t			m_width;
	size_t			m_height;
	size_t			m_pitch;
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: sets the 'from' direction of a cell
//	Param	: x, z - the cell coord
//			  from - the direction 'from' at the cell
//	Author	: dswinerd
//
inline void DirectionMap::SetCellFrom( const size_t x, const size_t z, DirectionMap::Dir from )
{
	dbAssert( x < m_width );
	dbAssert( z < m_height );

	// rowData points to the first long in the row of data
	unsigned long *rowData = &m_cells[ m_pitch * z ];
	
	int longIndex = x / 16;

	// ul points to the unsigned long where our value is
	unsigned long* ul = &rowData[longIndex];

	int subIndex = x & 15;

	unsigned long flong = (unsigned long)from;

	unsigned long shiftIndex = subIndex * 2;
	unsigned long val		 = flong << shiftIndex;
	unsigned long mask		 = 3     << shiftIndex;

	// zero the bits we are about to set
	*ul = ( *ul & ~mask ) | val;

	dbAssert( GetCellFrom( x, z) == from );
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: gets the 'from' direction of a cell
//  Result	: returns the direction 'from' at the cell
//	Param	: x, z - the cell coord
//	Author	: dswinerd
//
inline const DirectionMap::Dir DirectionMap::GetCellFrom( const size_t x, const size_t z ) const
{
	dbAssert( x < m_width );
	dbAssert( z < m_height );

	// rowData points to the first long in the row of data
	unsigned long *rowData = &m_cells[ m_pitch * z ];
	
	int longIndex = x / 16;

	// ul points to the unsigned long where our value is
	unsigned long ul = rowData[longIndex];

	int subIndex = x & 15;

	unsigned long val = ( ul >> (subIndex<<1) ) & 3;

	return (DirectionMap::Dir)val;
}
