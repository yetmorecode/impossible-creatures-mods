/////////////////////////////////////////////////////////////////////
// File    : AstarPool.h
// Desc    : 
// Created : Friday, November 16, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "PathFindingPool.h" 

#include <Assist/TypeMagic.h>

// forward declarations:
class AstarGridNode;

/////////////////////////////////////////////////////////////////////
//	AstarPool
//
template< class T > 
	class AstarPool
{
// construction
public:
 	 AstarPool( size_t initialSize );
	~AstarPool();

// interface
public:
	T*		GetNode();

	void	Reset();

	size_t	GetMemUsage() const;
	
// fields
private:
	const size_t		m_allocPerPage;
	const size_t		m_allocMemPage;

	std::vector<void*>	m_allocPages;
	size_t				m_allocNdxNode;
	size_t				m_allocNdxPage;

	size_t				m_allocCount;
	size_t				m_allocMax;

// copy -- do not define
private:
	AstarPool( const AstarPool& );
	AstarPool& operator= ( const AstarPool& );
};

///////////////////////////////////////////////////////////////////// 
// implementation

template<class T> inline AstarPool<T>::AstarPool( size_t initialSize )
	: m_allocPerPage( initialSize ),
	  m_allocMemPage( initialSize * sizeof(T) ),
	  m_allocNdxNode( 0 ),
	  m_allocNdxPage( 0 ),
	  m_allocCount	( 0 ),
	  m_allocMax    ( 0 )
{
}

template<class T> inline AstarPool<T>::~AstarPool()
{
	// release each page
	size_t i = 0;
	size_t e = m_allocPages.size();

	for( ; i != e; ++i )
	{
		PathMemFree( m_allocPages[i] );
		m_allocPages[i] = 0;
	}

	return;
}

template<class T> inline void AstarPool<T>::Reset()
{
	// we only need to reset the next index
	m_allocNdxNode = 0;
	m_allocNdxPage = 0;
	m_allocCount   = 0;	
}

template<class T> inline T* AstarPool<T>::GetNode()
{
	// check if we need to allocate a new page
	if( m_allocNdxPage == m_allocPages.size() )
	{
		// a new page is needed
		m_allocPages.push_back( PathMemAlloc( m_allocMemPage ) );
	}

	// retrieve that block
	T* page = static_cast<T*>( m_allocPages[m_allocNdxPage] );

	// 
	T* node = &page[m_allocNdxNode];

	// increment indexes
	m_allocCount++;
	m_allocNdxNode++;

	if( m_allocNdxNode == m_allocPerPage )
	{
		m_allocNdxNode = 0;
		m_allocNdxPage++;
	}

	if( m_allocMax < m_allocCount )
	{
		m_allocMax = m_allocCount;
	}

	return node;
}

template<class T> inline size_t AstarPool<T>::GetMemUsage() const
{
	return m_allocPages.size() * m_allocMemPage;
}
