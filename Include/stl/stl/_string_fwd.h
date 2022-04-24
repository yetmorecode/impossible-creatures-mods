/*
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

#ifndef _STLP_STRING_FWD_H
#define _STLP_STRING_FWD_H

#ifndef _STLP_IOSFWD
# include <iosfwd>
#endif

_STLP_BEGIN_NAMESPACE

/////////////////////////////////////////////////////////////////////
// Sigma -- Modification for Sigma, all std::strings are allocated
// through a different default allocator, different heap for strings

template< class T >
	class allocator_string 
{
public:
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    
    typedef T* pointer;
    typedef const T* const_pointer;
    
    typedef T& reference;
    typedef const T& const_reference;
    
    pointer address(reference r) const { return &r; }
    const_pointer address(const_reference r) const {return &r;}

public:
	template<class P> struct rebind
	{
	    typedef allocator_string<P> other;
	};
    
public:
	inline void construct(pointer p, const T& val)
	{ 
		new(p) T(val); 
	}

	inline void destroy(pointer p)
	{ 
		p->~T(); 
	}

	inline pointer allocate( size_type n, const void* = 0 )	// space for n Ts
	{
		return static_cast< T* >( mallocString( n, sizeof( T ) ) );
	}

	void deallocate(pointer p,size_type) // deallocate n Ts, don't destroy
	{
		if( p )
		{
			freeString( p );
		}
	}

	template <class _Tp1, class _Tp2>
		inline allocator_string<_Tp2>& __stl_alloc_rebind(allocator_string<_Tp1>& __a, const _Tp2*) 
	{
		return (allocator_string<_Tp2>&)(__a);
	}

	template <class _Tp1, class _Tp2>
		inline allocator_string<_Tp2> __stl_alloc_create(const allocator_string<_Tp1>&, const _Tp2*) 
	{
		return allocator_string<_Tp2>();
	}
};
  
template<class T>bool operator==(const allocator_string<T>&, const allocator_string<T>&);
template<class T>bool operator!=(const allocator_string<T>&, const allocator_string<T>&);

/////////////////////////////////////////////////////////////////////

# if !defined (_STLP_LIMITED_DEFAULT_TEMPLATES)
template <class _CharT, 
          class _Traits = char_traits<_CharT>, 
          class _Alloc = allocator<_CharT> >
class basic_string;
# else
template <class _CharT, 
          class _Traits, 
          class _Alloc>
class basic_string;
# endif /* _STLP_LIMITED_DEFAULT_TEMPLATES */

typedef basic_string<char, char_traits<char>, allocator_string<char> > string;

#  ifdef _STLP_HAS_WCHAR_T
typedef basic_string<wchar_t, char_traits<wchar_t>, allocator_string<wchar_t> > wstring;
#  endif

# ifdef _STLP_OWN_IOSTREAMS
_STLP_DECLSPEC const char*  _STLP_CALL
__get_c_string(const string& __str);
# else
template <class _CharT, class _Traits, class _Alloc>
const char* _STLP_CALL
__get_c_string(const basic_string<_CharT, _Traits, _Alloc>& __str);
# endif

_STLP_END_NAMESPACE

#endif /* _STLP_STRING_FWD_H */

// Local Variables:
// mode:C++
// End:
