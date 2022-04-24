/////////////////////////////////////////////////////////////////////
// File    : BitPlane.h
// Desc    : 
// Created : Friday, June 22, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// * 2D bit array

#pragma once

#include "UtilExports.h"

class IFF;

///////////////////////////////////////////////////////////////////// 
// BitPlane

class BitPlane
{
// construction
public:
	UTIL_API  BitPlane();
	UTIL_API  BitPlane( const BitPlane& );
	UTIL_API ~BitPlane();

	UTIL_API  BitPlane& operator= ( const BitPlane& );

// interface
public:
	unsigned int	GetWidth () const;
	unsigned int	GetHeight() const;

	UTIL_API void	SetSize  ( unsigned int w, unsigned int h );

	bool			Get		 ( unsigned int x, unsigned int y ) const;
	void			Set		 ( unsigned int x, unsigned int y );
	void			Set		 ( unsigned int x, unsigned int y, bool b );
	void			Clear    ( unsigned int x, unsigned int y );
	UTIL_API void	SetAll	 ( bool b );

	UTIL_API unsigned long	GetSpan ( unsigned int x, unsigned int y, unsigned int width ) const;
	UTIL_API void			FillSpan( unsigned int x, unsigned int y, unsigned int width );

	UTIL_API bool	IsBlockSet  ( unsigned int x, unsigned int y, unsigned int width, unsigned int height );
	UTIL_API bool	IsBlockClear( unsigned int x, unsigned int y, unsigned int width, unsigned int height );

	UTIL_API size_t	GetMemUsage() const;

	UTIL_API void	Save	 ( IFF& iff ) const;
	UTIL_API void	Load	 ( IFF& iff );

// fields
private:
	unsigned long*	m_pData;
	size_t			m_width, m_pitch, m_height;
};

///////////////////////////////////////////////////////////////////// 
// inlines

inline unsigned BitPlane::GetWidth() const
{
	return m_width;
}

inline unsigned BitPlane::GetHeight() const
{
	return m_height;
}

inline void BitPlane::Clear( unsigned x, unsigned y )
{
	dbAssert( x < m_width );
	dbAssert( y < m_height );

	unsigned long mask = ( 1<<(x&0x1f) );
	unsigned long& data = m_pData[ m_pitch*y + ( x >> 5 ) ];
	data &= ~mask;

	return;
}

inline bool BitPlane::Get( unsigned x, unsigned y ) const
{
	dbAssert( x < m_width );
	dbAssert( y < m_height );

	return !!( m_pData[ m_pitch*y + x/32 ] & ( 1<<(x&0x1f) ) );
}

inline void BitPlane::Set( unsigned x, unsigned y )
{
	dbAssert( x < m_width );
	dbAssert( y < m_height );

	unsigned long mask = ( 1<<(x&0x1f) );
	unsigned long& data = m_pData[ m_pitch*y + ( x >> 5 ) ];
	data |= mask;

	return;
}

inline void BitPlane::Set( unsigned x, unsigned y, bool b )
{
	if ( b )
		Set( x, y );
	else
		Clear( x, y );
}
