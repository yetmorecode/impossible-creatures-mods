/////////////////////////////////////////////////////////////////////
// File    : StlExString.h
// Desc    : 
// Created : Wednesday, September 19, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// * utility classes to use with std::string

#pragma once

///////////////////////////////////////////////////////////////////// 
// 

template< typename T > class relicstring_trait {};

template<> class relicstring_trait<char>
{
// types
public:
	typedef char		char_type;
	typedef size_t		size_type;
	typedef std::string stl_string;

// interface
public:
	static inline size_t length( const char_type* str )
	{
		return strlen( str );
	}

	static inline void copyN( char_type* dst, const char_type* src, size_t maxlen )
	{
		// validate parm
		dbAssert( dst );
		dbAssert( src );
		dbAssert( maxlen > 0 );

		// copy
		const size_t lenOrg = strlen(src);
		const size_t lenCpy = __min( lenOrg, maxlen - 1 );
		memcpy( dst, src, lenCpy );

		dst[ lenCpy ] = 0;

		return;
	}

	static inline int compareN( const char_type* l, const char_type* r, size_t maxlen )
	{
		return strncmp( l, r, maxlen );
	}

	static inline int compareI( const char_type* l, const char_type* r )
	{
		return stricmp( l, r );
	}

	static inline int compare( const char_type* l, const char_type* r )
	{
		return strcmp( l, r );
	}
};

template<> class relicstring_trait<wchar_t>
{
// types
public:
	typedef wchar_t			char_type;
	typedef size_t			size_type;
	typedef std::wstring	stl_string;

// interface
public:
	static inline size_t length( const char_type* str )
	{
		return wcslen( str );
	}

	static inline void copyN( char_type* dst, const char_type* src, size_t maxlen )
	{
		// validate parm
		dbAssert( dst );
		dbAssert( src );
		dbAssert( maxlen > 0 );

		// copy
		wcsncpy( dst, src, maxlen );
		dst[ maxlen - 1 ] = 0;

		return;
	}

	static inline int compareN( const char_type* l, const char_type* r, size_t maxlen )
	{
		return wcsncmp( l, r, maxlen );
	}

	static inline int compareI( const char_type* l, const char_type* r )
	{
		return wcsicmp( l, r );
	}

	static inline int compare( const char_type* l, const char_type* r )
	{
		return wcscmp( l, r );
	}
};

///////////////////////////////////////////////////////////////////// 
// 

namespace std
{
	template< class T >
	class basicstring_less
	{
	public:
		typedef relicstring_trait<T> trait;

	public:
		bool operator()( const trait::char_type* l, const trait::char_type* r ) const 
		{
			return trait::compare( l, r ) < 0;
		}

		bool operator()( const trait::stl_string& l, const trait::char_type* r ) const 
		{
			return trait::compare( l.c_str(), r ) < 0;
		}

		bool operator()( const trait::char_type* l, const trait::stl_string& r ) const 
		{
			return trait::compare( l, r.c_str() ) < 0;
		}

		bool operator()( const trait::stl_string& l, const trait::stl_string& r ) const 
		{
			return trait::compare( l.c_str(), r.c_str() ) < 0;
		}
	};

	typedef basicstring_less<char>		string_less;
	typedef basicstring_less<wchar_t>	wstring_less;

	template< class T >
	class basicstring_less_insensitive
	{
	public:
		typedef relicstring_trait<T> trait;

	public:
		bool operator()( const trait::char_type* l, const trait::char_type* r ) const 
		{
			return trait::compareI( l, r ) < 0;
		}

		bool operator()( const trait::stl_string& l, const trait::char_type* r ) const 
		{
			return trait::compareI( l.c_str(), r ) < 0;
		}

		bool operator()( const trait::char_type* l, const trait::stl_string& r ) const 
		{
			return trait::compareI( l, r.c_str() ) < 0;
		}

		bool operator()( const trait::stl_string& l, const trait::stl_string& r ) const 
		{
			return trait::compareI( l.c_str(), r.c_str() ) < 0;
		}
	};

	typedef basicstring_less_insensitive<char>		string_less_insensitive;
	typedef basicstring_less_insensitive<wchar_t>	wstring_less_insensitive;

	inline void string_tolower( std::string& s )
	{
		size_t i = 0;
		size_t e = s.length();

		for( ; i != e; ++i )
		{
			s[ i ] = static_cast<char>( tolower( s[ i ] ) );
		}
	}

	inline void string_toupper( std::string& s )
	{
		size_t i = 0;
		size_t e = s.length();

		for( ; i != e; ++i )
		{
			s[ i ] = static_cast<char>( toupper( s[ i ] ) );
		}
	}
}

///////////////////////////////////////////////////////////////////// 
// 

template<class T> inline size_t relicstring_length( const T* str )
{
	return relicstring_trait<T>::length( str );
}

template<class T> inline void relicstring_copyN( T* dst, const T* src, size_t maxlen )
{
	relicstring_trait<T>::copyN( dst, src, maxlen );
}

template<class T> inline void relicstring_catN( T* dst, const T* src, size_t maxlen )
{
	const size_t len = relicstring_length( dst );
	dbAssert( len < maxlen );

	relicstring_trait<T>::copyN( dst + len, src, maxlen - len );
}

template<class T> inline int relicstring_compareN( const T* l, const T* r, size_t maxlen )
{
	return relicstring_trait<T>::compareN( l, r, maxlen );
}

template<class T> inline int relicstring_compareI( const T* l, const T* r )
{
	return relicstring_trait<T>::compareI( l, r );
}

template<class T> inline int relicstring_compare( const T* l, const T* r )
{
	return relicstring_trait<T>::compare( l, r );
}
