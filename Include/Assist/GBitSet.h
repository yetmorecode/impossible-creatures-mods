/////////////////////////////////////////////////////////////////////
// File    : GBitSet.h
// Desc    : 
// Created : Monday, April 16, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// * boolean array, useful for enumerations (eg: TypedBitSet<SomeEnum,SE_Count> someBits;)
//
#pragma once

///////////////////////////////////////////////////////////////////// 
// TypedBitSet

template< class T, size_t N >
class TypedBitSet
{
// fields
private:
	std::bitset< N > m_bits;

// construction
public:
	TypedBitSet()
	{
	}

	TypedBitSet( const TypedBitSet& rhs )
		: m_bits( rhs.m_bits )
	{
	}

	TypedBitSet& operator= ( const TypedBitSet& rhs )
	{
		m_bits = rhs.m_bits;

		return *this;
	}

// interface
public:
	bool	tst		( T t ) const	{ return m_bits.test( t ); }

	bool	any		() const		{ return m_bits.any(); }
	bool	none	() const		{ return m_bits.none(); }

	void	set		( T t )			{ m_bits.set( t, 1 ); }
	void	set		( T t, bool b )	{ m_bits.set( t, b ); }
	void	clr		( T t )			{ m_bits.set( t, 0 ); }

	void	clear	()				{ m_bits.reset(); }

	TypedBitSet&	operator|= ( T t )		{ m_bits.set( t, 1 ); return *this;	}

	TypedBitSet&	operator|= ( const TypedBitSet& s )
									{ m_bits |= s.m_bits; return *this; }

	TypedBitSet&	operator&= ( const TypedBitSet& s )
									{ m_bits &= s.m_bits; return *this; }


	bool	operator== ( const TypedBitSet& rhs ) const
									{ return m_bits == rhs.m_bits; }

	size_t			to_size_t() const	{	return m_bits.to_ulong();	}
};
