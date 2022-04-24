/////////////////////////////////////////////////////////////////////
// File  : MemoryAlloc.h
// Author: Dominic Mathieu
// Date  : 2001-1-3
//      (c) relic entertainment inc.2001
// 
// * STL allocator that works with a memory pool
// 

// IMPORTANT NOTE
// although this class is defined in the memory library, 
// it MUST NOT be used in this library

#pragma once

#include "MemoryLib.h"

/////////////////////////////////////////////////////////////////////
// mempool_alloc

template< class T >
	class mempool_alloc 
{
public:
	HMEMPOOL m_pool;

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
    explicit mempool_alloc( HMEMPOOL p )
		: m_pool( p )
	{ 
	}
    
	template<class U> explicit mempool_alloc( const mempool_alloc<U>& t ) 
		: m_pool( t.m_pool )
	{ 
	}

    ~mempool_alloc()
	{
	}
    
	mempool_alloc<T>& operator= (const mempool_alloc<T>& rhs ) 
	{
		if( this != &rhs )
		{
			m_pool = rhs.m_pool;
		}

		return *this;
	}

public:
	template<class P> struct rebind
	{
	    typedef mempool_alloc<P> other;
	};

public:
	void construct(pointer p, const T& val)
	{ 
		new(p) T(val); 
	}

	void destroy(pointer p)
	{ 
		p->~T(); 
	}

	pointer allocate( size_type n, const void* = 0 )
	{
		if( n != 0 )
		{
			return static_cast< pointer >( MemPoolAlloc( m_pool, n * sizeof( T ) ) );
		}
		else
		{
			return 0;
		}
	}

	void deallocate(pointer p,size_type)
	{
		if( p )
		{
			MemPoolFree( m_pool, p );
		}
	}

	template <class _Tp1, class _Tp2>
		inline mempool_alloc<_Tp2>& __stl_alloc_rebind(mempool_alloc<_Tp1>& __a, const _Tp2*) 
	{
		return (mempool_alloc<_Tp2>&)(__a);
	}

	template <class _Tp1, class _Tp2>
		inline mempool_alloc<_Tp2> __stl_alloc_create(const mempool_alloc<_Tp1>&, const _Tp2*) 
	{
		return mempool_alloc<_Tp2>();
	}
};
