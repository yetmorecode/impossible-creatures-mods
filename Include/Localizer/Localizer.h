/////////////////////////////////////////////////////////////////////
// File  : Localizer.h
// Author: Dominic Mathieu
// Date  : 2001-1-11
//      (c) relic entertainment inc.2001
// 
// *
// 

#pragma once

#ifndef RELIC_LIB
    #if defined( LOCALIZERDLL_EXPORTS )
	    #define LOCALIZERDLL_API __declspec(dllexport)
    #else
	    #define LOCALIZERDLL_API __declspec(dllimport)
    #endif
#else
    #define LOCALIZERDLL_API /**/
#endif // RELIC_LIB

#define OUTPARAM

// we include this for the very useful LENGTHOF()
#include <Assist/TypeMagic.h>

/////////////////////////////////////////////////////////////////////
// Localizer

class LOCALIZERDLL_API Localizer
{
// types
public:
	// use this class with stl
	class LessString;

	enum CharacterType
	{
		CT_Alpha = 0x0100,
		CT_Digit = 0x0004,
		CT_Space = 0x0008,
		CT_Punct = 0x0010,
	};

// construction
public:
	static bool		Initialize( void );
	static void		Shutdown( void );

// interface
public:
	// get the localized string using a key
		// NOTE; key ZERO is always en empty string
	static std::wstring	GetString				( int key );
	static bool			GetString				( wchar_t* out, size_t outlen, int key );
	
	// convert from an std::string to a locString
	static std::string	LocString2String		( const std::wstring& s );
	static std::string	LocString2String		( const wchar_t* s );
	static void			LocString2String		( char* out, size_t outlen, const wchar_t* s );

	static std::wstring	String2LocString		( const std::string& s );
	static std::wstring	String2LocString		( const char* s );
	static void			String2LocString		( wchar_t* out, size_t outlen, const char* s );

	// localized conversions
	static bool			ConvertNumber2Localized	( wchar_t* out, size_t outlen, long n );

	static bool			GetLocalizedDate		( wchar_t* out, size_t outlen );
	static bool			GetLocalizedTime		( wchar_t* out, size_t outlen );

	// This is a localized string id, that is embedded in a string.
	// the format is as follows:
	// "$nnnnnnnnn"
	// where nnnnnnnnn is the number of the id, e.g. "$10023"
	static bool			ConvertLocIDToLocalized	( const wchar_t* locid, wchar_t* out, size_t outlen );
	static bool			ConvertWStringToLocID	( const wchar_t* locid, int& id );
	static bool			ConvertLocIDToWString	( int id, wchar_t* out, size_t outlen );

	// format a string, similar to sprintf
	static bool			FormatText( wchar_t* out, size_t outlen, int formatkey, ... );

	// locale friendly case insensitive string compare this is very good for sorting,
	// for Equality use CompareEqual
	static int			CompareSort( const std::wstring& l, const std::wstring& r );
	static int			CompareSort( const wchar_t* l, const wchar_t* r );

	// locale friendly case insensitive string compare this is very good equality, 
	// for sorting use CompareSort
	static bool			CompareEqual( const std::wstring& l, const std::wstring& r );
	static bool			CompareEqual( const wchar_t* l, const wchar_t* r );

	// strict case sensitive string compare
	// for equavalence use CompareEqual, for sorting use CompareSort
	static bool			CompareStrict( const std::wstring& l, const std::wstring& r );
	static bool			CompareStrict( const wchar_t* l, const wchar_t* r );

	// validate a the characters in a string according to the specified mask
	static bool			CheckString   ( const wchar_t* str, unsigned long mask, const wchar_t* allowed );

public:
	// this will load the .dat or .dll, whichever is newest
	static bool	ResLoad  ( const char* fileName, int rangeMin, int rangeMax );
	static bool	ResUnload( const char* fileName );

	static bool	DllLoad  ( const char* fileName, int rangeMin, int rangeMax );
	static bool	DllUnload( const char* fileName );

	static bool	TxtLoad  ( const char* fileName, int rangeMin, int rangeMax );
	static bool	TxtUnload( const char* fileName );

public:
	// saves all the strings,ranges,comments to filename.txt
	static bool			TxtSave( const char* txtResName, const char* filename );

	// NOTE: call this before adding strings to a specified range, it will cause all strings
	// that don't get re-added to be deleted, otherwise old strings won't release their ids
	static bool			TxtResetRange( const int rangeMin, const int rangeMax, const char* txtResName );

	// returns a min and max sub range within the ranges of the specified file if it is loaded
	static bool			TxtAddRange( int& rangeMin, int& rangeMax, const size_t rangeSize, const char* txtResName, const char* comment = NULL );

	// add/update a string to a sub range of the specified file if it is loaded, takes a suggestion for the ID
	// returns the new id of the string if successful otherwise it returns 0
	static int			TxtAdd
							(
							const wchar_t* text,
							const int rangeMin,
							const int rangeMax,
							const char* txtResName,
							const int suggestion = 0	// default 0 == no suggestion
							);

public:
	//	Font stuff	
	typedef int	FontID;
	enum
	{
		InvalidFont = -1,
	};

	static FontID		FontNewFont( const wchar_t* pName, size_t size, bool bold, unsigned char charSet, OUTPARAM size_t& ascent, OUTPARAM size_t& descent );
	static void			FontDeleteFont( FontID );
	static bool			FontQueryGlyphSize( FontID, wchar_t ch, size_t& width, size_t& height );
	static bool			FontBuildGlyphImage( FontID, wchar_t ch, unsigned char* pByteImage );	//	Must be (width*height) sized, packed

// copy -- do not define
private:
	Localizer();
	Localizer( const Localizer& );
	Localizer& operator= ( const Localizer& );
};

///////////////////////////////////////////////////////////////////// 
// 

class Localizer::LessString
{
public:
	bool operator() ( const wchar_t* l, const wchar_t* r ) const
	{
		return Localizer::CompareSort( l, r ) < 0;
	}

	bool operator() ( const std::wstring& l, const wchar_t* r ) const
	{
		return Localizer::CompareSort( l.c_str(), r ) < 0;
	}

	bool operator() ( const std::wstring& l, const std::wstring& r ) const
	{
		return Localizer::CompareSort( l.c_str(), r.c_str() ) < 0;
	}
};
