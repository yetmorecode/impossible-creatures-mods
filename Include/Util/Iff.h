/////////////////////////////////////////////////////////////////////
//	File	: Iff.h
//	Desc.	: rTool iff loading/saving
//		(c)2000 relic entertainment inc.
//

#pragma once


#include "UtilExports.h"

#include <Assist/TypeMagic.h>

#include <Debug/ctAssert.h>

// forward declarations
class ByteStream;
class ChunkNode;
class IFF;

///////////////////////////////////////////////////////////////////// 
// 

enum IFFChunkType
{
	Type_Normal			= 0,
	Type_NormalVers		= 'NRML',
	Type_Form			= 'FORM',
	Type_ForceDWORD		= ULONG_MAX,
};

const unsigned long Size_Unknown = 0xffffffff;

/////////////////////////////////////////////////////////////////////
// These data types are for handling the data handlers we use when calling the
// recursive Scan routine

// return !0 to indicate error

typedef unsigned long (ChunkHandlerFunc)(IFF &iff, ChunkNode *pChunk, void *pContext1, void *pContext2);

typedef ChunkHandlerFunc* ChunkHandlerPtr;

/////////////////////////////////////////////////////////////////////
// IFF

// * IFF specifies that only FORM chunks are allowed to contain nested chunks.  
// * this class implements this using the first argument of the PushChunk() function.  
// * if you intend to include other chunks within this one, pass Type_Form as the type, 
// * otherwise use Type_Normal.

class IFF
{
// construction
public:
	UTIL_API  IFF();
	UTIL_API ~IFF();

// interface
public:
	// Associates a stream object with the IFF object.  This is necessary
	// before doing anything else, and the stream must be set for either
	// reading or writing, depending on what you plan to do.
	UTIL_API bool			Attach(ByteStream *pNewStream, bool byteswap=false);
	UTIL_API ByteStream*	GetByteStream( );

	// Validate the File
	UTIL_API bool			Validate( );

	// Read operations
		// returns 0 on success
	UTIL_API unsigned long	Parse( bool skipStandard = true );
	UTIL_API void			AddParseHandler
								(
								ChunkHandlerPtr func, 
								IFFChunkType	type, 
								unsigned long	id, 
								void*			context1, 
								void*			context2
								);
	UTIL_API void			SetDefaultHandler
								(
								ChunkHandlerPtr func, 
								void*			context1 = 0, 
								void*			context2 = 0
								);

	// Write
	// NOTE: the version parameter is only used if the ChunkType is Type_NormalVers
	UTIL_API bool			PushChunk
								(
								IFFChunkType Type, 
								unsigned long ID, 
								unsigned long Version = 0
								);
	UTIL_API bool			PopChunk();

	// Navigation
	UTIL_API bool			IsByteSwapped(void);

	UTIL_API long			GetPos(void);
	UTIL_API long			ChunkRemain(void);
	UTIL_API void			Skip(unsigned long NumBytes);

	// Standard chunks: NAME and VERS
	UTIL_API bool			WriteNameChunk( const char* );
	UTIL_API const char*	GetName       ( void ) const;

	UTIL_API bool			WriteVersionChunk( unsigned long version );
	UTIL_API unsigned long	GetVersion		( void ) const;
	UTIL_API unsigned long	GetNormalVersion( void ) const;

	UTIL_API bool			WriteCommentChunk( const char* );

	//
	//	File version (old version system)
	//
	UTIL_API void			SetFileVersion(unsigned long Ver);
	UTIL_API unsigned long	GetFileVersion(void);

// fields
private:
	// pimpl idiom
	class Data;
	Data* m_pimpl;

// implementation
private:
	bool InternalParse( bool skipStandard );
	bool InternalPushChunk(IFFChunkType Type, unsigned long ID, unsigned long Version);
	bool InternalPopChunk();
	bool InternalValidateRecurse( size_t maxpos );

public:
	// even though these are public, don't EVER use them

	// obsolete functions
	bool						OldRead ( void *ptr, unsigned long len );
	bool						OldWrite( const void *ptr, unsigned long len );

	// internal functions
	UTIL_API bool				_Write( const void *ptr, unsigned long c, unsigned long size );
	UTIL_API bool				_Read ( void *ptr, unsigned long c, unsigned long size );

// copy -- do not define
private:
	IFF( const IFF& );
	IFF& operator= ( const IFF& );
};

///////////////////////////////////////////////////////////////////// 
// Read/Write 

template< class T > bool IFFWrite     ( IFF& iff, const T& o );
template< class T > bool IFFRead	  ( IFF& iff, T& o );
template< class T > bool IFFWriteArray( IFF& iff, const T p[], unsigned long c );
template< class T > bool IFFReadArray ( IFF& iff, T p[], unsigned long c );

UTIL_API bool IFFWriteString( IFF& iff, const char* str );
UTIL_API bool IFFReadString( IFF& iff, char* str, size_t maxlen );

UTIL_API bool IFFWriteString( IFF& iff, const wchar_t* str );
UTIL_API bool IFFReadString( IFF& iff, wchar_t* str, size_t maxlen );

// use this one ONLY if IFF*/IFF*Array can't do the job
UTIL_API bool IFFWriteBinary( IFF& iff, const void* p, unsigned long c );
UTIL_API bool IFFReadBinary( IFF& iff, void* p, unsigned long c );

/////////////////////////////////////////////////////////////////////
// ChunkNode

class ChunkNode
{
	friend class IFF;

// fields
private:
	IFFChunkType		Type;
	unsigned long		Size;
	unsigned long		ID;

	unsigned long		Version;
	char				Name[128];

	unsigned long		Pos;

// construction
public:
	ChunkNode()
		: Pos( 0 ), Size( 0 ), ID( 0 ), Version( 0 ), Type( Type_Normal )
	{
		Name[0] = '\0';
	}

	ChunkNode(IFFChunkType NewType, unsigned long NewID)
		: Pos( 0 ), Size( 0 ), ID( NewID ), Version( 0 ), Type( NewType )
	{
		Name[0] = '\0';
	}

// interface
public:
	unsigned long	GetPos		() const			{return Pos;}
	unsigned long	GetSize		() const			{return Size;}
	unsigned long	GetID		() const			{return ID;}
	IFFChunkType	GetType		() const			{return Type;}

	unsigned long	GetVersion	() const			{return Version;}
	void			SetVersion	(unsigned long v)	{Version = v;}

	const char*		GetName		() const			{return Name;}
	void			SetName		(const char* p)		{strcpy(Name, p);}
};

///////////////////////////////////////////////////////////////////// 
// implementation

inline bool IFFWriteString( IFF& iff, const std::string& str )							
{	
	return IFFWriteString( iff, str.c_str() );
}

inline bool IFFReadString( IFF& iff, std::string& str )
{
	char buffer[ 512 ];

	const bool success = 
		IFFReadString( iff, buffer, LENGTHOF(buffer) );

	str = buffer;

	return success;
}

inline bool IFFWriteString( IFF& iff, const std::wstring& str )
{	
	return IFFWriteString( iff, str.c_str() );
}

inline bool IFFReadString( IFF& iff, std::wstring& str )
{
	wchar_t buffer[ 512 ];

	const bool success = 
		IFFReadString( iff, buffer, LENGTHOF(buffer) );

	str = buffer;

	return success;
}

#define VALIDATEIFFTYPE(T) \
	ctAssert( TypeMagic::IsBuiltIn<T>::result )

template< class T > inline bool IFFWriteArray( IFF& iff, const T p[], unsigned long c )	
{	
	// validate template parm
	VALIDATEIFFTYPE(T);

	return iff._Write( p, c, sizeof(T) );
}

template< class T > inline bool IFFReadArray( IFF& iff, T p[], unsigned long c )			
{	
	// validate template parm
	VALIDATEIFFTYPE(T);

	return iff._Read( p, c, sizeof(T) );
}

#undef VALIDATEIFFTYPE

template< class T > inline bool IFFWrite( IFF& iff, const T& o )							
{	
	return IFFWriteArray( iff, &o, 1 );
}

template< class T > inline bool IFFRead( IFF& iff, T &o )								
{	
	return IFFReadArray( iff, &o, 1 );
}

template<> inline bool IFFWriteArray<bool>( IFF& iff, const bool p[], unsigned long c )	
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		const unsigned short d = 
			static_cast<unsigned short>( p[i] ? 0xffff : 0 );

		success &= IFFWrite( iff, d );
	}

	return success;
}

template<> inline bool IFFReadArray<bool>( IFF& iff, bool p[], unsigned long c )	
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		unsigned short d = 0;

		success &= IFFRead( iff, d );

		p[i] = (d != 0);
	}

	return success;
}

template<> inline bool IFFWriteArray<std::string>( IFF& iff, const std::string p[], unsigned long c )	
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		success &= IFFWriteString( iff, p[i] );
	}

	return success;
}

template<> inline bool IFFReadArray<std::string>( IFF& iff, std::string p[], unsigned long c )	
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		success &= IFFReadString( iff, p[i] );
	}

	return success;
}

template<> inline bool IFFWriteArray<std::wstring>( IFF& iff, const std::wstring p[], unsigned long c )	
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		success &= IFFWriteString( iff, p[i] );
	}

	return success;
}

template<> inline bool IFFReadArray<std::wstring>( IFF& iff, std::wstring p[], unsigned long c )	
{
	bool success = true;

	for( unsigned long i = 0; i != c; ++i )
	{
		success &= IFFReadString( iff, p[i] );
	}

	return success;
}

///////////////////////////////////////////////////////////////////// 
// 

template< class T > struct IFFTemplateChunk
{
	static unsigned long handler( IFF& iff, ChunkNode *pChunk, void *pContext1, void *pContext2 )
	{
		UNREF_P( pContext2 );

		dbAssert( pChunk && pContext1 && !pContext2 );

		if ( pChunk->GetSize() != sizeof(T) )
		{
			dbBreak();
			return 1;
		}
		
		T* t = static_cast<T*>(pContext1);

		if ( !IFFRead( iff, *t ) )
			return 1;

		return 0;
	}
};

// happy struct{} chunk assistants

template< class T > void IFFParseChunk( IFF& iff, unsigned long ID, T& data )
{
	iff.AddParseHandler( IFFTemplateChunk<T>::handler, Type_Normal, ID, &data, NULL );
}

template< class T > bool IFFWriteChunk( IFF& iff, unsigned long ID, const T& data )
{
	bool success = true;

	iff.PushChunk( Type_Normal, ID );
		success = IFFWrite( iff, data );
	iff.PopChunk();

	return success;
}

///////////////////////////////////////////////////////////////////// 
// inlines

inline bool IFF::OldRead( void *ptr, unsigned long len )			
{	
	return _Read(ptr,len,1);	
}

inline bool IFF::OldWrite( const void *ptr, unsigned long len )	
{	
	return _Write(ptr,len,1);	
}

//	eof
