/////////////////////////////////////////////////////////////////////
// File  : Memorylib.h
// Author: Dominic Mathieu
// Date  : 2000-12-20
//      (c) relic entertainment inc.2000
//
// *
//

#pragma once

#include "mmExports.h"

#pragma warning( push, 3 )
	#include <malloc.h>
	#include <memory>
	#include <new>
#pragma warning( pop )

/////////////////////////////////////////////////////////////////////
// types definition

typedef struct _HMEMPOOL	 { int reserved; } *HMEMPOOL;
typedef struct _HMEMPOOLSHOT { int reserved; } *HMEMPOOLSHOT;
typedef struct _HMEMPOOLWALK { int reserved; } *HMEMPOOLWALK;

const int MEMCALLSTACKLEN = 32;

enum MEMBLOCKTYPE
{
	MMBT_NEW			= 0,
	MMBT_NEW_ARRAY		= 1,
	MMBT_MALLOC			= 2,
	MMBT_REALLOC		= 3,
	MMBT_STL			= 4,
	MMBT_POOLALLOC		= 5,
	MMBT_STRING			= 6
};

enum MEMFREETYPE
{
	MMFT_DELETE			= 10,
	MMFT_DELETE_ARRAY   = 11,
	MMFT_FREE			= 12,
	MMBT_STLFREE		= 13,
	MMFT_POOLFREE		= 14,
	MMFT_STRINGFREE		= 15
};

struct MEMBLOCKINFO
{
	const void*		address;
	unsigned long	size;
	unsigned long	unique;
	MEMBLOCKTYPE	type;
	long			callstack[ MEMCALLSTACKLEN ];
};

typedef void ( *MEMCBALLOCPTR )( const HMEMPOOL&, const MEMBLOCKINFO& );

/////////////////////////////////////////////////////////////////////
// pool functions
//
// this is the main workforce of this lib

// this is the dafult memory pool, used by the C runtime
	// the name is "DEFAULT"
MMAPI HMEMPOOL 		MemPoolDefault();

// all memory functions need a pool, this creates a memory pool
	// poolName must be unique
MMAPI HMEMPOOL 		MemPoolCreate			( const char* poolName, size_t initialSize );

// destroy a memory pool previously created
MMAPI void 			MemPoolDestroy			( HMEMPOOL handle );

// return the name of the specified pool, or null if the pool does not exist
MMAPI const char*	MemPoolName             ( HMEMPOOL handle );

// this is the main allocation function
MMAPI void* 		MemPoolAlloc			( HMEMPOOL handle, size_t size );

// release the memory
MMAPI void			MemPoolFree				( HMEMPOOL handle, void* pointer );

// Available in Debug only would not work in release
#ifdef _DEBUG
	// check for alloc at pointer, return true if i have a record for an allocation at that location
	MMAPI bool		MemPoolAllocExist		( HMEMPOOL handle, const MEMBLOCKINFO& );
#endif

// while locked, a pool disallows alloc or free
MMAPI void			MemPoolLock				( HMEMPOOL handle );
MMAPI void			MemPoolUnlock			( HMEMPOOL handle );

// validates underflow and overflow buffers, and verify that if there has been
// no writes to free'd buffers
MMAPI void			MemPoolValidate			( HMEMPOOL handle );

/////////////////////////////////////////////////////////////////////
// pool debug functions
//
// these are only valid if 'MEMINFOTRACKING' is defined

// calculates the amount of memory used by the pool.
MMAPI void			MemPoolSize				( HMEMPOOL handle, size_t& allocInUse, size_t& allocMax );

// takes a 'snapshot' of the state of the pool
	// this snapshot can be used for walking the memory blocks in the pool
	// the snapshot must be released when no longer needed
MMAPI HMEMPOOLSHOT	MemPoolSnapshot			( HMEMPOOL handle );
MMAPI void			MemPoolSnapshotDestroy	( HMEMPOOLSHOT handle );

// compare 2 snapshots of the same pool
	// this function creates a new snapshot, which must also be destroyed
	// this snapshot is special, as it only contains the difference between the 2 snapshot parms
	// newly allocated blocks will have positive size
	// freed blocks will have negative size
	// blocks that didn't change are not listed
	// NOTE: both snapshot parms must be from the same memory pool
MMAPI HMEMPOOLSHOT	MemPoolSnapshotDiff		( HMEMPOOLSHOT handle1, HMEMPOOLSHOT handle2 );

// walk blocks inside heap
	// the return value is the handle for this walk. pass it to 'MemPoolWalkNext',
	// and release the associated memory by calling 'MemPoolWalkDestroy'
	// NOTE: destroy all 'HHEAPWALK' before destroying the 'HHEAPSHOT' they reference
MMAPI HMEMPOOLWALK	MemPoolWalkFirst		( HMEMPOOLSHOT handle, MEMBLOCKINFO& info );

  // continue walking the memory blocks, stop when it returns 'false'
MMAPI bool			MemPoolWalkNext			( HMEMPOOLWALK handle, MEMBLOCKINFO& info );

  // release the memory walking handle
MMAPI void			MemPoolWalkDestroy		( HMEMPOOLWALK handle );

/////////////////////////////////////////////////////////////////////
// debug related functions
//
// these are only valid if 'MEMINFOTRACKING' is defined

// return number of memory pool currently in use
MMAPI size_t		MemPoolCount			();

// fill the 'pools' parm with the handles of all memory pools currently in use
	// the 'pools' array must be big enough to hold 'MemPoolCount()' handles
MMAPI size_t		MemPoolList				( HMEMPOOL pools[], size_t maxlen );

// cumulative of 'MemPoolSize()'
MMAPI void			MemSize					( size_t& allocInUse, size_t& allocMax );

// user-defined callback
MMAPI void			MemCBAlloc              ( MEMCBALLOCPTR user );

// for tracing memory calls
MMAPI bool			MemConvertAddress		( const long codeAddress, char str[ 256 ] );

// when tracing memory calls, if this file and line appears in the callstack it will be skipped
MMAPI void			MemAddIgnoreFileLine	( const char* file, unsigned long codeLine );
MMAPI void			MemRemIgnoreFileLine	( const char* file, unsigned long codeLine );
MMAPI void			MemClearIgnoreFileLine	( );

// to get the memory manager to barf out all leaks when it shuts down
MMAPI void			MemWriteLeakFile		( const char* logfile );

/////////////////////////////////////////////////////////////////////
// memory usage

MMAPI void			MemVMShrink  ();
MMAPI int			MemVMMap     ( const char* folder );
MMAPI void			MemVMDumpPool( const char* file, const char* poolname );

/////////////////////////////////////////////////////////////////////
// utilities

#define DELETEZERO(p)		delete   p, p = NULL
#define DELETEARRAYZERO(p)	delete[] p, p = NULL

// this class is useful for deleting all objects in a stl container
	// sample use: std::for_each( container.begin(), container.end(), DELETEITEM() );
class DELETEITEM
{
public:
	template< class T > void operator()( T*& t ) const
		{ DELETEZERO( t ); }
};

#ifndef RELIC_TOOL

/////////////////////////////////////////////////////////////////////
// C runtime replacements

MMAPI void* __stdcall MemCRTAlloc   ( size_t size, int type );
MMAPI void* __stdcall MemCRTRealloc ( void* pointer, size_t newsize, int type );
MMAPI void __stdcall	MemCRTFree	  ( void* pointer, int type );
MMAPI void* __stdcall	MemStringAlloc( size_t size, size_t sizeOfOne );
MMAPI void __stdcall	MemStringFree ( void* pointer );

//lint -e{1771}
__forceinline void* __cdecl operator new( size_t size ) //_THROW1(std::bad_alloc)
{
	return MemCRTAlloc( size, MMBT_NEW );
}

//lint -e{1771}
__forceinline void* __cdecl operator new[]( size_t size ) //_THROW1(std::bad_alloc)
{
	return MemCRTAlloc( size, MMBT_NEW_ARRAY );
}

//lint -e{1771}
__forceinline void __cdecl operator delete( void* pointer ) //_THROW0()
{
	if( pointer )
		MemCRTFree( pointer, MMFT_DELETE );
}

//lint -e{1771}
__forceinline void __cdecl operator delete[]( void* pointer ) //_THROW0()
{
	if( pointer )
		MemCRTFree( pointer, MMFT_DELETE_ARRAY );
}

//lint -esym(652,malloc) -esym(683,malloc)
#define malloc( size )				MemCRTAlloc( size, MMBT_MALLOC )

//lint -esym(652,free) -esym(683,free)
#define free( pointer )				MemCRTFree( pointer, MMFT_FREE )

//lint -esym(652,realloc) -esym(683,realloc)
#define realloc( pointer, newsize )	MemCRTRealloc( pointer, newsize, MMBT_REALLOC )

//lint -esym(652,calloc) -esym(683,calloc)
#define calloc( num, size )			relic_mem_calloc_compile_error

//lint -esym(652,_strdup)
#define _strdup( str )				relic_mem_strdup_compile_error

#endif // RELIC_TOOL