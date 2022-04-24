/////////////////////////////////////////////////////////////////////
// File  : CRC.h
// Author: 
// Date  : 2000-12-8
//      (c) relic entertainment inc.2000
// 
// * This file contains a class that will let you continually add data to a stream
// * that can then be collapsed (using a CRC) into one long.
// 

#pragma once

#include <Debug/ctAssert.h>

#include "Filesystem/ByteStream.h"
#include "FilesystemExports.h"

/////////////////////////////////////////////////////////////////////
// CRC

// for performance reason, the CRC is calculated when values are added

class CRC
{
// construction
public:
	 CRC();
	~CRC();

// interface
public:
	// reset
	void			ResetStream();

	// current crc value
	unsigned long	GetCRC() const;

	// qty of value added to generate this crc
	size_t			GetValueQty() const;

	// add one or more values
	void FILESYSTEM_API	AddValues( const void* data, size_t size );

	// add the content of the file
	void FILESYSTEM_API	AddFile( FILE* f );
	void FILESYSTEM_API	AddFile( ByteStream* s );

	// add any var
	template < class T > 
	inline void		AddVar( const T& t )
					{
						AddValues( &t, sizeof( T ) );
					}

	//	Compare two CRC values
	bool			operator ==( const CRC& ) const;
	bool			operator !=( const CRC& ) const;

// fields
private:
	unsigned long	m_crc;

	size_t			m_qty;
};

/////////////////////////////////////////////////////////////////////
// CRCStream

// this class can be used to quickly generate a crc from something that can
// be streamed

class FILESYSTEM_API CRCStream : public ByteStream
{
// fields
private:
	CRC* const		m_crc;
	
	unsigned long	m_size;

// construction
public:
	CRCStream( CRC* p );

// inherited -- ByteStream
public:
	// Get the Name of the stream
	virtual const char*		GetStreamName() const { return "CRCStream"; }

	// NOTE: will always return 0
	virtual	unsigned long	Read(void*, unsigned long );

	// will calculate crc on the values passed
	virtual unsigned long	Write(const void* src, unsigned long numbytes);

	// NOTE: will always fail
	virtual void			Flush();

	// NOTE: will always fail
	virtual unsigned long	SetPos(unsigned long);

	// NOTE: will always fail
	virtual unsigned long	GetPos();

	// return the current length of the stream
	virtual unsigned long	GetLen();

protected:
	virtual void			SetStreamMode(StreamMode NewMode);

// copy -- do not define
private:
	CRCStream( const CRCStream& );
	CRCStream& operator= ( const CRCStream& );
};

///////////////////////////////////////////////////////////////////// 
// CRC inline implementation

inline void CRC::ResetStream()		
{	
	m_crc=0;
	m_qty=0;	
}

inline CRC::CRC()
{	
	ResetStream();	
}

inline CRC::~CRC()			
{
}

inline unsigned long CRC::GetCRC() const		
{	
	return m_crc;		
}

inline size_t CRC::GetValueQty() const	
{	
	return m_qty;		
}

inline bool CRC::operator== ( const CRC& rhs ) const
{
	return( m_crc == rhs.m_crc ) && ( m_qty == rhs.m_qty );
}

inline bool CRC::operator!= ( const CRC& rhs ) const
{
	return !operator == ( rhs );
}
