/////////////////////////////////////////////////////////////////////
// File    : MemorySmall.h
// Desc    : 
// Created : Wednesday, April 04, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "Memorylib.h" 

#include <debug/ctAssert.h>
#include <debug/db.h>

///////////////////////////////////////////////////////////////////// 
// MemPoolSmall

class MemPoolSmall
{
// types
public:
	enum { SIZETHRESHOLD = 128 };

// construction
public:
	MMAPI  explicit MemPoolSmall( const char* name );
	MMAPI ~MemPoolSmall();

	MMAPI  MemPoolSmall( const MemPoolSmall& );
	MMAPI  MemPoolSmall& operator= ( const MemPoolSmall& );

// interface
public:
	MMAPI void*	Alloc( size_t size );
	MMAPI void	Free ( void* p, size_t size );

	MMAPI void	Shrink();	//	Deallocates unused pages

// fields
private:
	// total encapsulation
	void* m_internals;
};
