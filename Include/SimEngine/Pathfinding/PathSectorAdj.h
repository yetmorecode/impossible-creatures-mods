/////////////////////////////////////////////////////////////////////
// File    : PathSectorAdj.h
// Desc    : 
// Created : Thursday, July 04, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h"
#include "PathTypes.h"

#include <Memory/MemoryAlloc.h>

/////////////////////////////////////////////////////////////////////
//	Class	: PathSectorAdj
//
class SIM_API PathSectorAdj
{
// types
public:

	typedef unsigned short AdjacencySize;
	typedef std::pair< PathSubCell, AdjacencySize > AdjacencyPair;

	typedef std::vector< AdjacencyPair, mempool_alloc< AdjacencyPair > > AdjacencyVector;

	typedef AdjacencyVector::iterator			AdjacencyVectorI;
	typedef AdjacencyVector::const_iterator		AdjacencyVectorCI;

	typedef AdjacencyVectorI  iterator;
	typedef AdjacencyVectorCI const_iterator;

// interface
public:

	PathSectorAdj();
	~PathSectorAdj();

	// clears the adjacency information
	void Clear();							
	
	// adds an adjacent sector, returns true on success
	bool Add( const PathSubCell adjacent, AdjacencySize size );		

	// removes an adjacent sector, returns true on success
	bool Remove( const PathSubCell adjacent );							

	// tests if the sector is adjacent, given a size
	bool IsAdjacent( const PathSubCell adjacent, AdjacencySize size ) const;	

	// returns the adjacency size if any
	bool GetAdjacentSize( const PathSubCell adj, AdjacencySize& size ) const;

	// returns the max connectivity size
	AdjacencySize GetMaxSize( ) const	{ return m_maxAdjacencySize; }

	// iterators
	iterator		begin()				{ return m_adj.begin(); };
	const_iterator	begin() const		{ return m_adj.begin(); };
	iterator		end()				{ return m_adj.end();	};
	const_iterator	end() const			{ return m_adj.end();	};
	size_t			size() const		{ return m_adj.size();	};


// data
private:
	
	AdjacencyVector		m_adj;
	AdjacencySize		m_maxAdjacencySize;			// the largest adjacency connection
};