/////////////////////////////////////////////////////////////////////
// File	: Typedef.h
//		(c)1999 relic entertainment inc.
//
// * Common typedefs that people are using, cause they are bitches...

#pragma once

#ifndef RELIC_LIB
    #ifdef PLATFORM_EXPORTS
	    #define PLATFORM_API __declspec(dllexport)
    #else
	    #define PLATFORM_API __declspec(dllimport)
    #endif
#else
    #define PLATFORM_API /**/
#endif // RELIC_LIB

///////////////////////////////////////////////////////////////////// 
// 

// byte, dword
typedef unsigned char byte;
typedef unsigned long DWORD;

//	longlong - a 64bit word
#ifdef WIN32
	typedef __int64 longlong;
#else
	#error No definition of a 64bit word
#endif

///////////////////////////////////////////////////////////////////// 
// transform an intel 32 value to a network neutral 32 bit value

#ifdef WIN32
	#define DWORD2NETWORKDWORD(v) \
		( ( ( v >>  0 ) & 0xff ) << 24 ) | \
		( ( ( v >>  8 ) & 0xff ) << 16 ) | \
		( ( ( v >> 16 ) & 0xff ) <<  8 ) | \
		( ( ( v >> 24 ) & 0xff ) <<  0 )
#else			
	#error 
#endif

#define MAKEBYTE(v0,v1,v2,v3,v4,v5,v6,v7) \
	((unsigned char)\
		(\
		( ( (v0) & 0x01 ) << 7 ) | 	\
		( ( (v1) & 0x01 ) << 6 ) | 	\
		( ( (v2) & 0x01 ) << 5 ) | 	\
		( ( (v3) & 0x01 ) << 4 ) | 	\
		( ( (v4) & 0x01 ) << 3 ) | 	\
		( ( (v5) & 0x01 ) << 2 ) | 	\
		( ( (v6) & 0x01 ) << 1 ) | 	\
		( ( (v7) & 0x01 ) << 0 )	\
		))

///////////////////////////////////////////////////////////////////// 
// GUID -- unique key -- ripped out from windows.h (wtypes.h)

#ifndef GUID_DEFINED
#define GUID_DEFINED

	typedef struct _GUID
	    {
	    unsigned long	Data1;
	    unsigned short	Data2;
	    unsigned short	Data3;
	    unsigned char	Data4[ 8 ];
	    } GUID;

#endif // !GUID_DEFINED

bool PLATFORM_API GUIDCompare   ( const GUID& l, const GUID& r );
bool PLATFORM_API GUIDFromString( GUID& out, const char* in );
bool PLATFORM_API GUIDToString  ( char* out, size_t outlen, const GUID& in, bool curlies = true );
GUID PLATFORM_API GUIDGenerate  ();
GUID PLATFORM_API GUIDNil       ();
