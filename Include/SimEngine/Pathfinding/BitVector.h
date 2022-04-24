/////////////////////////////////////////////////////////////////////
// File    : BitVector.h
// Desc    : 
// Created : 
// Author  : 
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once


///////////////////////////////////////////////////////////////////// 
// BitVectorNewAllocator

class BitVectorNewAllocator
{
public:
	void Free( unsigned long* p, unsigned long numLongs )
	{
		UNREF_P( numLongs );

		DELETEARRAYZERO( p );
	}

	unsigned long* Alloc( unsigned long numLongs )
	{
		return new unsigned long[ numLongs ];
	}
};


/////////////////////////////////////////////////////////////////////
//	Class	: BitVector
//

template< class Allocator = BitVectorNewAllocator >
	class BitVector : private Allocator
{
// construction
public:

	// array will be unitialized
	BitVector( );
	~BitVector( );

// interface
public:
	void			SetSize  ( size_t maxElems );
	size_t			GetSize  () const;

	bool			GetValue ( unsigned long index ) const;

	void			SetValue ( unsigned long index, bool tf );

	void			FillValue( const bool );

	size_t			GetMemUsage() const;

// hidden
private:

	BitVector( const BitVector& );
	BitVector& operator= ( const BitVector& );

// data
private:

	size_t			m_maxElems;

	unsigned long	m_numLongs;
	unsigned long*	m_array;
};

/////////////////////////////////////////////////////////////////////
//	Desc.	: BitVector constructor
//
template< class Allocator >
	BitVector<Allocator>::BitVector()
		: m_array( NULL ),
		  m_maxElems( 0 ),
		  m_numLongs( 0 )
{
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: BitVector destructor
//
template< class Allocator >
	BitVector<Allocator>::~BitVector()
{
	SetSize( 0 );
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
template< class Allocator >
	void BitVector<Allocator>::SetSize( size_t maxElems )
{
	unsigned long longs = ( maxElems + 31 ) / 32;		// + 31 is to round up

	if ( longs != m_numLongs )
	{
		// need to resize

		if ( m_array != 0 )
		{
			Allocator::Free( m_array, m_numLongs );
		}

		//
		m_numLongs = longs;

		// allocate memory
		if ( m_numLongs > 0 )
		{
			m_array = Allocator::Alloc( m_numLongs );
		}
	}

	m_maxElems = maxElems;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the max number of elements the BitVector will hold
//	Result	: returns unsigned long
//
template< class Allocator >
	size_t BitVector<Allocator>::GetSize( ) const		
{	
	return m_maxElems;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the value at the given index
//	Result	: returns bool
//
template< class Allocator >
	bool BitVector<Allocator>::GetValue( unsigned long index ) const
{
	dbAssert( index < m_maxElems );

	unsigned long ul = m_array[ index / 32 ];

	return !!( ul & ( 1 << ( index & 31 ) ) );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
template< class Allocator >
	void BitVector<Allocator>::SetValue( unsigned long index, bool tf )
{
	dbAssert( index < m_maxElems );

	unsigned long* ul = &m_array[ index / 32 ];

	unsigned long mask = ( 1 << ( index & 31 ) );

	// fix: this would be faster without the branch
	if ( tf )
	{
		// set the bit
		*ul |= mask;
	}
	else
	{
		// clear the bit
		*ul &= ~mask;
	}

	dbAssert( GetValue( index ) == tf );

	return;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
template< class Allocator >
	void BitVector<Allocator>::FillValue( const bool tf )
{
	memset( m_array, tf, m_numLongs * sizeof(unsigned long) );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the number of bytes used by this instance
//
template< class Allocator >
	size_t BitVector<Allocator>::GetMemUsage() const
{
	return m_numLongs * sizeof(unsigned long) + sizeof(*this);
}