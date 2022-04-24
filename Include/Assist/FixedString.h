/////////////////////////////////////////////////////////////////////
// File  : FixedString.h
// Author: Dominic Mathieu
// Date  : 2001-1-5
//      (c) relic entertainment inc.2001
// 
// *
// 

#pragma once

#include "StlExString.h" 

#include "TypeMagic.h" 

/////////////////////////////////////////////////////////////////////
// fixedstring

template< size_t N, typename TYPE = relicstring_trait<char> >
	class fixedstring
{
// types
public:
    typedef TYPE::char_type		char_type;
    typedef TYPE::size_type		size_type;
    typedef TYPE::stl_string	stl_string;
    typedef const char_type*	const_iterator;

	enum { LEN = N };

// fields
private:
	char_type m_internal[ LEN + 1 ];

// construction
public:
	fixedstring();
	explicit fixedstring( const char_type* );
	explicit fixedstring( const stl_string& );

	fixedstring& operator = ( const char_type* );
	fixedstring& operator = ( const stl_string& );

	~fixedstring();

	template< size_t U, typename T > explicit fixedstring( const fixedstring<U,T>& rhs )
		{
		*this = rhs.c_str();
		}

	template< size_t U, typename T > fixedstring& operator = ( const fixedstring<U,T>& rhs )
		{
		return *this = rhs.c_str();
		}
		
// interface
public:	
	const_iterator		begin() const;
	const_iterator		end  () const;

	char_type			at( size_type pos ) const;

	char_type&			operator[]( size_type pos );
	const char_type&	operator[]( size_type pos ) const;

	const char_type*	c_str() const;

    size_type			length  () const;
    size_type			size    () const;
    size_type			max_size() const;
	size_type			capacity() const;

    bool				empty() const;    

	void				erase();

	// same as strcmp
	int					compare( const char_type* rhs ) const;

public:
	static const fixedstring& 
					fakestring( const char_type* );
};

// invalid specialization
template<> class fixedstring< 0, relicstring_trait<char   > > {};
template<> class fixedstring< 1, relicstring_trait<char   > > {};

template<> class fixedstring< 0, relicstring_trait<wchar_t> > {};
template<> class fixedstring< 1, relicstring_trait<wchar_t> > {};

///////////////////////////////////////////////////////////////////// 
// fstring

// this is just a short name for fixedstring< N, relicstring_trait<char> >

template< size_t N >
	class fstring : public fixedstring< N, relicstring_trait<char> >
{
// construction
public:
	 fstring();
	 fstring( const char_type*  str );
	 fstring( const stl_string& str );

	 fstring& operator = ( const char_type*  str );
	 fstring& operator = ( const stl_string& str );

	~fstring();
};

///////////////////////////////////////////////////////////////////// 
// fwstring

// this is just a short name for fixedstring< N, relicstring_trait<wchar_t> >

template< size_t N >
	class fwstring : public fixedstring< N, relicstring_trait<wchar_t> >
{
// construction
public:
	 fwstring();
	 fwstring( const char_type*  str );
	 fwstring( const stl_string& str );

	 fwstring& operator = ( const char_type*  str );
	 fwstring& operator = ( const stl_string& str );

	~fwstring();
};

/////////////////////////////////////////////////////////////////////
// fixedstring operation

template< size_t N, typename T >
	inline bool operator == ( const fixedstring<N,T>& l, const fixedstring<N,T>::char_type* r )
		{
		return l.compare( r ) == 0;
		}

template< size_t N, typename T >
	inline bool operator == ( const fixedstring<N,T>& l, const fixedstring<N,T>::stl_string& r )
		{
		return l == r.c_str();
		}

template< size_t N, typename T >
	inline bool operator == ( const fixedstring<N,T>& l, const fixedstring<N,T>& r )
		{
		return l == r.c_str();
		}

template< size_t N, typename T >
	inline bool operator != ( const fixedstring<N,T>& l, const fixedstring<N,T>& r )
		{
		return !( l == r );
		}

template< size_t N, typename T >
	inline bool operator < ( const fixedstring<N,T>& l, const fixedstring<N,T>::char_type* r )
		{
		return l.compare( r ) < 0;
		}

template< size_t N, typename T >
	inline bool operator < ( const fixedstring<N,T>& l, const fixedstring<N,T>& r )
		{
		return l < r.c_str();
		}

template< size_t N, typename T >
	inline bool operator < ( const fixedstring<N,T>& l, const fixedstring<N,T>::stl_string& r )
		{
		return l < r.c_str();
		}

/////////////////////////////////////////////////////////////////////
// fixedstring implementation

template< size_t N, typename T >
	inline fixedstring<N,T>::fixedstring()
		{
		m_internal[ 0 ] = 0;
		}

template< size_t N, typename T >
	inline fixedstring<N,T>::fixedstring( const char_type* rhs )
		{
		*this = rhs;
		}

template< size_t N, typename T >
	inline fixedstring<N,T>::fixedstring( const stl_string& rhs )
		{
		*this = rhs;
		}

template< size_t N, typename T >
	inline fixedstring<N,T>& fixedstring<N,T>::operator = ( const char_type* rhs )
		{
		dbAssert( rhs );

		if( rhs != m_internal )
		{
			// validate that we are not silently cutting a part of the parm
			dbAssert( T::length( rhs ) <= LEN );
		
			//
			T::copyN( m_internal, rhs, LENGTHOF(m_internal) );
		}

		return *this;
		}		

template< size_t N, typename T >
	inline fixedstring<N,T>& fixedstring<N,T>::operator = ( const stl_string& rhs )
		{
		return *this = rhs.c_str();
		}

template< size_t N, typename T >
	inline fixedstring<N,T>::~fixedstring()
		{
		}
	
template< size_t N, typename T >
	inline fixedstring<N,T>::const_iterator fixedstring<N,T>::begin() const
		{
		return m_internal;
		}

template< size_t N, typename T >
	inline fixedstring<N,T>::const_iterator fixedstring<N,T>::end() const
		{
		return m_internal + length();
		}

template< size_t N, typename T >
	inline fixedstring<N,T>::char_type fixedstring<N,T>::at( size_type pos ) const
		{
		dbAssert( pos < length() );

		return m_internal[ pos ];
		}

template< size_t N, typename T >
	inline const fixedstring<N,T>::char_type& fixedstring<N,T>::operator[]( size_type pos ) const
		{
		dbAssert( pos < length() );

		return m_internal[ pos ];
		}

template< size_t N, typename T >
	inline fixedstring<N,T>::char_type& fixedstring<N,T>::operator[]( size_type pos )
		{
		// NOTE: it is valid to take the address of the first character in the string
		dbAssert( (pos < length()) || (pos == 0 && length() == 0) );

		return m_internal[ pos ];
		}

template< size_t N, typename T >
	inline const fixedstring<N,T>::char_type* fixedstring<N,T>::c_str() const
		{
		return m_internal;
		}

template< size_t N, typename T >
	inline fixedstring<N,T>::size_type fixedstring<N,T>::length() const
		{
		return T::length( m_internal );
		}
    		
template< size_t N, typename T >
	inline fixedstring<N,T>::size_type fixedstring<N,T>::size() const
		{
		return length();
		}

template< size_t N, typename T >
	inline fixedstring<N,T>::size_type fixedstring<N,T>::max_size() const
		{
		return LEN;
		}

template< size_t N, typename T >
	inline fixedstring<N,T>::size_type fixedstring<N,T>::capacity() const
		{
		return LENGTHOF(m_internal);
		}

template< size_t N, typename T >
	inline bool fixedstring<N,T>::empty() const
		{
		return m_internal[ 0 ] == 0;
		}

template< size_t N, typename T >
	inline void fixedstring<N,T>::erase()
		{
		m_internal[ 0 ] = 0;
		}

template< size_t N, typename T >
	inline const fixedstring<N,T>& fixedstring<N,T>::fakestring( const char_type* str )
		{
		return( *reinterpret_cast< const fixedstring<N,T>* >( str ) );
		}

template< size_t N, typename T >
	inline int fixedstring<N,T>::compare( const char_type* rhs ) const
		{
		return T::compareN( m_internal, rhs, LEN );
		}

///////////////////////////////////////////////////////////////////// 
// fstring implementation

template< size_t N >
	inline fstring<N>::fstring() 
		{
		}

template< size_t N >
	inline fstring<N>::fstring( const char_type* str ) 
		: fixedstring< N, relicstring_trait<char> >( str ) 
		{
		}

template< size_t N >
	inline fstring<N>::fstring( const stl_string& str ) 
		: fixedstring< N, relicstring_trait<char> >( str ) 
		{
		}

template< size_t N >
	inline fstring<N>& fstring<N>::operator = ( const char_type*  str ) 
		{ 
		fixedstring< N, relicstring_trait<char> >::operator= ( str ); 
		return *this; 
		}

template< size_t N >
	inline fstring<N>& fstring<N>::operator = ( const stl_string& str ) 
		{ 
		fixedstring< N, relicstring_trait<char> >::operator= ( str ); 
		return *this; 
		}

template< size_t N >
	inline fstring<N>::~fstring() 
		{
		}

///////////////////////////////////////////////////////////////////// 
// fwstring implementation

template< size_t N >
	inline fwstring<N>::fwstring() 
		{
		}

template< size_t N >
	inline fwstring<N>::fwstring( const char_type* str ) 
		: fixedstring< N, relicstring_trait<wchar_t> >( str ) 
		{
		}

template< size_t N >
	inline fwstring<N>::fwstring( const stl_string& str ) 
		: fixedstring< N, relicstring_trait<wchar_t> >( str ) 
		{
		}

template< size_t N >
	inline fwstring<N>& fwstring<N>::operator = ( const char_type*  str ) 
		{ 
		fixedstring< N, relicstring_trait<wchar_t> >::operator= ( str ); 
		return *this; 
		}

template< size_t N >
	inline fwstring<N>& fwstring<N>::operator = ( const stl_string& str ) 
		{ 
		fixedstring< N, relicstring_trait<wchar_t> >::operator= ( str ); 
		return *this; 
		}

template< size_t N >
	inline fwstring<N>::~fwstring() 
		{
		}
