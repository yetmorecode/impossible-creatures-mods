/////////////////////////////////////////////////////////////////////
// File    : AstarSector.cpp
// Desc    : 
// Created : 
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h"
#include "PathTypes.h"
#include "AstarSectorNode.h"
#include "AstarPool.h"

#include "PathfindingPool.h"

#include "BitVector.h"

#include <Assist/StlExSet.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class PathSectorMap;
class PathSector;
class BrokenSectorConnections;
class AstarPathfindingResult;
class SimHighPath;

/////////////////////////////////////////////////////////////////////
//
// Notes: This could be cleaned up by using a pimpl, but since this class
//			is currently only ever used within another class's pimpl, 
//			it's not too bad.
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
//	Class	:  SortNodePredicate
//
class AstarSectorSortNodePredicate : 
	public std::binary_function< const AstarSectorNode*, const AstarSectorNode*, bool >
{
public:

	/////////////////////////////////////////////////////////////////////
	//	Desc.	: To compare two nodes, we compare the `f' value, which is the
	//				sum of the g and h values.
	//				Since priority_queue::pop returns the largest element and we want the
	//				smallest we actually return operator >.  Yes, that is confusing.
	//
	bool operator() ( const AstarSectorNode* a, const AstarSectorNode* b ) const
	{
		return ( (a->gval + a->hval) > (b->gval + b->hval) );
	}
};


/////////////////////////////////////////////////////////////////////
//	Class	: AstarSector
//  Desc	: this class runs A* on the sectors in a PathSectorMap.
//				It is used to find a highlevel path across a map
//
class SIM_API AstarSector
{
// Types
public:

	// the type of path that will be returned by the A*
	typedef std::vector<PathSubCell> SectorPath;								

// Methods
public:
	AstarSector( AstarPool<AstarSectorNode> *pPool, const PathSectorMap* psm );			// ctor
	virtual ~AstarSector();																// dtor

	void* operator new   ( size_t );													// operator new
	void  operator delete( void* );														// operator delete

	// initialize the number of sectors in the graph
	void	SetNumSectors( size_t numSectors );									

	// find a path across the sectors
	bool	DoFindPath( SimHighPath& sectorPath, AstarSectorPathResult& pathResult, 
						PathSubCell startSector, PathSubCell goalSector,
						bool bToBuilding );		

	// ===== stat functions =====

	// resets the num searched counter
	void						ResetNumSearched();													

	// gets the number of nodes searched since last reset
	int							GetNumSearched();														

// Pure virtual functions
protected:

	// given a PathSubCell, return the PathSector* 
	virtual const PathSector*	GetPathSector( PathSubCell sector ) const = 0;

	// determines the heuristic cost of travelling from sector to goalSector
	virtual float				HeuristicCost( PathSubCell sector ) const = 0;				

	// determines the actual cost of travelling from pCurrent to pNext
	virtual float				ActualCost( const PathSector* pCurrent, const PathSector* pNext, const unsigned short adjSize ) const = 0;	

	// can the entity traverse the hither -> yon connection
	virtual bool				CanTraverse( int adjSize, PathSubCell hitherSector, PathSubCell yonSector ) const = 0;

// Virtual Functions
public:

	virtual size_t				GetMemUsage() const;

protected:

	// allow derived class initialization
	virtual void				DoFindPathInitialize() {};

	// returns true if the given sector satisfies the exit condition
	virtual bool				TestExitCondition( PathSubCell sector ) const;

	// returns true if the given sector is passible
	virtual bool				PassibleSector( const PathSector* pSector ) const;					

// Methods
private:

	// pops the top node off the open list
	void						GetFirstNode( AstarSectorNode* & node );								


// Types
private:
	typedef std::priority_queue<AstarSectorNode*, std::vector<AstarSectorNode*>, AstarSectorSortNodePredicate > NodeContainer;

	class BitVectorAllocator
	{
	public:

		void Free( unsigned long* p, unsigned long numLongs )
		{
			UNREF_P( numLongs );

			PathMemFree( p );
		}

		unsigned long* Alloc( unsigned long numLongs )
		{
			return reinterpret_cast< unsigned long* >( PathMemAlloc( sizeof(unsigned long) * numLongs ) );
		}
	};

	typedef BitVector< BitVectorAllocator > MarkingBits;

	typedef std::pair< PathSubCell, PathSubCell > ConnectionPair;
	typedef std::setVector< ConnectionPair >	  MarkedConnections;

// Data
protected:

	AstarPool<AstarSectorNode>* m_pool;								// the pool of nodes

	const PathSectorMap*		m_pathSectorMap;					// pointer to the PathSectorMap we will search

	NodeContainer				m_open;								// list of open nodes
	MarkingBits					m_marking;							// list of nodes that have been visited
	MarkedConnections			m_markedConnections;				// set of connections that have been visited

	size_t						m_maxNodes;							// the limit to the number of nodes that we will search

	const PathSubCellSet*		m_goalSectors;

	int							m_statNumSearched;					// accumulates the number of sectors searched
};
