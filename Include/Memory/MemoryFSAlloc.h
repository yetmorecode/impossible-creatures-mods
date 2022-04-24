/////////////////////////////////////////////////////////////////////
// File    : MemoryFSAlloc.h
// Desc    : 
// Created : Wednesday, March 14, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//
// * STL allocator that works with a memory pool
// * this allocator is optimized for serving fixed size blocks
// 

// IMPORTANT NOTE
// DO NOT USE THIS ALLOCATOR WITH VECTORS

// IMPORTANT NOTE
// although this class is defined in the memory library, 
// it MUST NOT be used in this library

#pragma once

#include "MemorySmall.h"

/////////////////////////////////////////////////////////////////////
// mempool_fs_alloc

template< class T >
	class mempool_fs_alloc 
{
// fields
public:
	MemPoolSmall m_pool;

// types
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

// construction
public:
    explicit mempool_fs_alloc( const char* name = "" )
		: m_pool( name )
	{
	}

    explicit mempool_fs_alloc( const MemPoolSmall& p )
		: m_pool( p )
	{
	}

	template<class P> mempool_fs_alloc( const mempool_fs_alloc<P>& rhs ) 
		: m_pool( rhs.m_pool )
	{ 
	}

    ~mempool_fs_alloc()
	{
	}

	mempool_fs_alloc& operator= (const mempool_fs_alloc& rhs ) 
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
	    typedef mempool_fs_alloc<P> other;
	};

// interface    
public:
	template <class _Tp1, class _Tp2>
		inline mempool_fs_alloc<_Tp2>& __stl_alloc_rebind(mempool_fs_alloc<_Tp1>& __a, const _Tp2*) 
	{
		return (mempool_fs_alloc<_Tp2>&)(__a);
	}

	template <class _Tp1, class _Tp2>
		inline mempool_fs_alloc<_Tp2> __stl_alloc_create(const mempool_fs_alloc<_Tp1>&, const _Tp2*) 
	{
		return mempool_fs_alloc<_Tp2>();
	}

	void construct( pointer p, const T& val )
	{ 
		new(p) T(val); 
	}

	void destroy( pointer p )
	{
		p->~T(); 
	}

	pointer allocate( size_type n, const void* = 0 )
	{
		if ( n != 0 )
		{
			return static_cast< pointer >( m_pool.Alloc( sizeof( T ) * n ) );
		}
		else
		{
			return 0;
		}
	}

	void deallocate( pointer p, size_type n )
	{
		if ( p )
		{
			m_pool.Free( p, sizeof( T ) * n );
		}
	}
};
