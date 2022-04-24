/////////////////////////////////////////////////////////////////////
// File    : BitArray.h
// Desc    : 
// Created : Tuesday, August 28, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <Util/Bitplane.h>

///////////////////////////////////////////////////////////////////// 
// MarkingBitfield

class MarkingBitfield
{
// fields
private:
	BitPlane m_bits;

// construction
public:
	 MarkingBitfield( int width, int height )
	 {
		m_bits.SetSize( width, height );
	 }

	~MarkingBitfield()
	{
	}

// interface
public:
	void	Clear();

	void	ClearBit( int x, int z );

	void	SetBitNoRangeCheck( int x, int z );
	bool	GetBitNoRangeCheck( int x, int z ) const;

	void	SetBit  ( int x, int z );
	bool	GetBit  ( int x, int z ) const;



	size_t	GetMemUsage() const;
};

///////////////////////////////////////////////////////////////////// 
// MarkingBitfield inlines

#define TESTBOUNDS(x,y) \
	((x) >= 0 && (y) >= 0 && (x) < int(m_bits.GetWidth()) && (y) < int(m_bits.GetHeight()) )

inline void MarkingBitfield::Clear()
{
	m_bits.SetAll( 0 );
}

inline void MarkingBitfield::ClearBit( int x, int z )
{
	if( TESTBOUNDS(x,z) )
	{
		m_bits.Clear( x, z );
	}
}

inline void MarkingBitfield::SetBitNoRangeCheck( int x, int z )
{
	m_bits.Set( x, z );
}

inline void MarkingBitfield::SetBit( int x, int z )
{
	if( TESTBOUNDS(x,z) )
	{
		m_bits.Set( x, z );
	}
}

inline bool	MarkingBitfield::GetBitNoRangeCheck( int x, int z ) const
{
	return m_bits.Get( x, z );
}

inline bool MarkingBitfield::GetBit( int x, int z ) const
{
	if( TESTBOUNDS(x,z) )
	{
		return m_bits.Get( x, z );
	}
	else
	{
		return true;
	}
}

inline size_t MarkingBitfield::GetMemUsage() const
{
	return m_bits.GetMemUsage();
}

#undef TESTBOUNDS
