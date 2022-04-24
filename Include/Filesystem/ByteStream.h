/////////////////////////////////////////////////////////////////////
// File  : ByteStream.h
// Author: Drew Dunlop
// Date  : 2000-12-8
//      (c) relic entertainment inc.2000
// 
// * Base class for read/write operations on byte streams
// 
// * VERY IMPORTANT NOTE
// * this class (and derived classes) must be thread-safe, 
// * in the sense that simultaneous accesses to **distinct** streams must be safe

#pragma once

#include "FilesystemExports.h"

#include <Debug/ctAssert.h>

#include <Assist/FixedString.h>
#include <Assist/TypeMagic.h>

///////////////////////////////////////////////////////////////////// 
// stream read modes

enum 
{
	BS_ReadBit  = 1 << 0,
	BS_WriteBit = 1 << 1,
};

enum StreamMode
{
	Mode_None  = 0,
	Mode_Read  = BS_ReadBit,
	Mode_Write = BS_WriteBit,
	Mode_Both  = BS_ReadBit | BS_WriteBit,
};

/////////////////////////////////////////////////////////////////////
// ByteStream

class FILESYSTEM_API ByteStream
{
// fields
private:
	StreamMode			m_mode;
	fstring<_MAX_PATH>	m_name;

// construction
public:
	ByteStream();
	virtual ~ByteStream();

// interface
public:
	virtual void			SetStreamName( const char* name);

	// Read NumBytes from the stream into the buffer at *pDest
	// Number returned is # bytes successfully read
	virtual	unsigned long	Read(void *pDest, unsigned long NumBytes) = 0;

	// Write NumBytes from the buffer at *pSrc into the stream
	// Number returned is # bytes successfully written
	virtual unsigned long	Write(const void *pSrc, unsigned long NumBytes) = 0;

	// Flush unwritten data to the stream - Returns when all buffered data has been output
		// NOTE: some derived classes might not allow this operation
	virtual void			Flush() = 0;

	// Set the read/write position in the stream
	// Returns 1 on success, 0 on failure
		// NOTE: some derived classes might not allow this operation
	virtual unsigned long	SetPos(unsigned long Position) = 0;

	// Return the current read/write position of the stream
		// NOTE: some derived classes might not allow this operation
	virtual unsigned long	GetPos() = 0;

	// return the current length of the stream
	virtual unsigned long	GetLen() = 0;

public:
	// Return a nice name for the stream
	const char*				GetStreamName() const;

	// StreamMode limits the type of operation that can be done on a stream
	StreamMode				GetStreamMode() const;

// for derived classes only
protected:
	// Any classes deriving from ByteStream must set the stream mode before using the stream.
	// Before performing read/write operations in your derived class, make sure the mode
	// is set correctly, and return an error if it isn't.
	virtual void	SetStreamMode(StreamMode NewMode);
};

///////////////////////////////////////////////////////////////////// 
// ByteStreamProxy

// auto deleting byte stream pointer

class ByteStreamProxy
{
// fields
private:
	ByteStream* m_ptr;

// construction
public:
	 ByteStreamProxy( ByteStream* p = 0 );
 	 ByteStreamProxy( const ByteStreamProxy& rhs );
	~ByteStreamProxy();
		
	 ByteStreamProxy& operator= ( const ByteStreamProxy& rhs );

// interface
public:
	ByteStream*			operator->();

	const ByteStream*	operator->() const;

	ByteStream&			operator* ();

	const ByteStream*	get() const;

	ByteStream*			get();
	
	// to close the stream
	void				reset();

	// to release the stream without closing it
	ByteStream*			release();

	// to test if open
	operator const void*() const;
};

/////////////////////////////////////////////////////////////////////
// ByteStream utility functions

FILESYSTEM_API bool BSWriteText  ( ByteStream& stream, const char* s, bool addNewline );
FILESYSTEM_API bool BSPrintf     ( ByteStream& stream, const char* format, ... );

FILESYSTEM_API bool BSWriteString( ByteStream& stream, const char* str );
FILESYSTEM_API bool BSReadString ( ByteStream& stream, char* str, size_t maxlen );

FILESYSTEM_API bool BSWriteString( ByteStream& stream, const wchar_t* str );
FILESYSTEM_API bool BSReadString ( ByteStream& stream, wchar_t* str, size_t maxlen );

/////////////////////////////////////////////////////////////////////
// ByteStream utility functions

#define VALIDATEBYTESTREAMTYPE(T) \
	ctAssert( TypeMagic::IsBuiltIn<T>::result )

template < class T >
	inline bool BSWriteArray( ByteStream& s, const T *p, unsigned long c )	
{	
	// validate template parm
	VALIDATEBYTESTREAMTYPE(T);

	return( s.Write( p, sizeof(T) * c ) == ( sizeof(T) * c ) );
}

template < class T >
	inline bool BSReadArray( ByteStream& s, T *p, unsigned long c )
{	
	// validate template parm
	VALIDATEBYTESTREAMTYPE(T);

	return( s.Read ( p, sizeof(T) * c ) == ( sizeof(T) * c ) );	
}

#undef VALIDATEBYTESTREAMTYPE

template < class T >
	inline bool BSWrite( ByteStream& s, const T &o )
{	
	return BSWriteArray( s, &o, 1 );
}


template < class T >
	inline bool BSRead( ByteStream& s, T &o )
{	
	return BSReadArray( s, &o, 1 );
}

template<>
	inline bool BSWriteArray<bool>( ByteStream& s, const bool* p, unsigned long c )	
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		const unsigned char d = 
			static_cast<unsigned char>( p[i] ? 1 : 0 );

		success &= BSWrite( s, d );
	}

	return success;
}

template<>
	inline bool BSReadArray<bool>( ByteStream& s, bool* p, unsigned long c )	
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		unsigned char d = 0;

		success &= BSRead( s, d );

		p[i] = (d == 1);
	}

	return success;
}

template<> 
	inline bool BSWriteArray<std::string>( ByteStream& s, const std::string* p, unsigned long c )
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		success &= BSWriteString( s, p[i].c_str() );
	}

	return success;
}

template<>
	inline bool BSReadArray<std::string>( ByteStream& s, std::string* p, unsigned long c )	
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		char buffer[ 2048 ];
		buffer[0] = '\0';

		success &= BSReadString( s, buffer, LENGTHOF(buffer) );

		p[i] = buffer;
	}

	return success;
}

template<> 
	inline bool BSWriteArray<std::wstring>( ByteStream& s, const std::wstring* p, unsigned long c )
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		success &= BSWriteString( s, p[i].c_str() );
	}

	return success;
}

template<>
	inline bool BSReadArray<std::wstring>( ByteStream& s, std::wstring* p, unsigned long c )	
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		wchar_t buffer[ 2048 ];
		buffer[0] = '\0';

		success &= BSReadString( s, buffer, LENGTHOF(buffer) );

		p[i] = buffer;
	}

	return success;
}

///////////////////////////////////////////////////////////////////// 
// 

template < class T >
	inline bool WriteArray( ByteStream& s, const T *p, unsigned long c )
{
	return BSWriteArray( s, p, c );
}

template < class T >
	inline bool Write( ByteStream& s, const T &o )
{	
	return BSWrite( s, o );
}

template < class T >
	inline bool ReadArray( ByteStream& s, T *p, unsigned long c )
{	
	return BSReadArray( s, p, c );
}

template < class T >
	inline bool Read( ByteStream& s, T &o )
{	
	return BSRead( s, o );
}

/////////////////////////////////////////////////////////////////////
// ByteStream inlines

inline StreamMode ByteStream::GetStreamMode() const
{
	return m_mode;
}

/////////////////////////////////////////////////////////////////////
// ByteStreamProxy inlines

inline void ByteStreamProxy::reset() 
{ 
	if( m_ptr )
	{
		DELETEZERO( m_ptr );
	}
}

inline ByteStream* ByteStreamProxy::release() 
{ 
	ByteStream* p = m_ptr;
	m_ptr = 0;

	return p;
}

inline ByteStreamProxy& ByteStreamProxy::operator= ( const ByteStreamProxy& rhs ) 
{ 
	if( this != &rhs )
	{
		//
		reset();

		//
		std::swap( m_ptr, const_cast< ByteStreamProxy& >( rhs ).m_ptr );
	}

	return *this;
}

inline ByteStreamProxy::ByteStreamProxy( ByteStream* p ) 
	: m_ptr( p ) 
{
}

inline ByteStreamProxy::ByteStreamProxy( const ByteStreamProxy& rhs ) 
	: m_ptr( 0 )
{
	*this = rhs;
}

inline ByteStreamProxy::~ByteStreamProxy() 
{ 
	reset();
}
	
inline ByteStream* ByteStreamProxy::operator->() 
{ 
	return m_ptr; 
}

inline const ByteStream* ByteStreamProxy::operator->() const 
{ 
	return m_ptr; 
}

inline ByteStream& ByteStreamProxy::operator*() 
{ 
	dbAssert( m_ptr );
	return *m_ptr; 
}

inline const ByteStream* ByteStreamProxy::get() const 
{ 
	return m_ptr; 
}

inline ByteStream* ByteStreamProxy::get() 
{ 
	return m_ptr; 
}

inline ByteStreamProxy::operator const void*() const
{
	return m_ptr;
}
