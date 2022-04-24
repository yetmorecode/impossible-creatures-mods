/////////////////////////////////////////////////////////////////////
// File    : AstarBase.h
// Desc    : 
// Created : 
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//


#pragma once

#include "DirectionMap.h"
#include "BitArray.h"	// for Markingbitfield.
#include "AstarGridNode.h"
#include "AstarPool.h"

#include <Memory/MemoryAlloc.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations
//
class AstarGridNode;
class AstarPool;
class AstarPathfindingResult;
class AstarBaseFindPathPred;
class AstarBaseFindOpenPred;
class AstarBaseFindValidPred;
class AstarBaseFindBestPred;

/////////////////////////////////////////////////////////////////////
//	Class	:  SortNodePredicate
//
class SortNodePredicate : 
	public std::binary_function< const AstarGridNode*, const AstarGridNode*, bool >
{
public:

	/////////////////////////////////////////////////////////////////////
	//	Desc.	: To compare two nodes, we compare the `f' value, which is the
	//				sum of the g and h values.
	//				Since priority_queue::pop returns the largest element and we want the
	//				smallest we actually return operator >.  Yes, that is confusing.
	//
	bool operator() ( const AstarGridNode* a, const AstarGridNode* b ) const
	{
//		return ( (a->gval + a->hval) > (b->gval + b->hval) );
		return ( a->fval > b->fval );
	}
};

/////////////////////////////////////////////////////////////////////
//	Class	: AstarBase
//
class AstarBase
{
// Types.
public:
	typedef std::vector<Vec2i, mempool_alloc<Vec2i> > GridPath;										// the grid locations returned by a path

	enum FloodDir
	{
		n, e, s, w
	};
	
// Functions.
public:
	AstarBase( AstarPool<AstarGridNode> *pPool, int width, int height );		// ctor
	virtual ~AstarBase();														// dtor

	void* operator new   ( size_t );											// operator new
	void  operator delete( void* );												// operator delete

	void SetMaxNodes( size_t max );												// sets the maximum number of nodes that will be searched before it chokes
	size_t GetMaxNodes();														// gets the maximum number of nodes that will be searched before it chokes

	// the main pathing function
	bool DoFindPath( GridPath &path, 
				     const Vec2i &start, const Vec2i &goal, AstarPathfindingResult& pathResult, const AstarBaseFindPathPred& pred );
	
	bool DoFindOpen( Vec2i& firstOpen, const Vec2i &centerCell, const Vec2i &refCell, const AstarBaseFindOpenPred& pred );

	bool DoFindValidWithinRange( Vec2i& firstOpen, const Vec2i &start, const Vec2i &goal, float AP, const AstarBaseFindValidPred& pred );

	bool DoFindBestUsingLine( const Vec2i& hither,
							  const Vec2i& yon,
							  Vec2i& result,
							  const AstarBaseFindBestPred& pred ) const;

	int	 DoFloodFill( const Vec2i& startCell );

	/////////////////////////////////////////////////////////////////////////
	//two phase pathfind search that keeps searching with no goal position

	void DoSearchStart( const Vec2i &start, const AstarBaseFindBestPred& pred );										// start the search given start position
	bool SearchNext( Vec2i& nextPos, int& searched, const AstarBaseFindBestPred& pred );							// continue search returning next best position and num searched nodes

	// stat functions
	virtual void ResetNumSearched();													// resets the num searched counter
	virtual int GetNumSearched();														// gets the number of nodes searched since last reset

	// debug functions
	const GridPath& GetPassibleVisited() const;									// returns the passible cells visited in the last search
	const GridPath& GetImpassibleVisited() const;								// returns the impassible cells visited in the last search


// Virtual Functions	
public:

	virtual size_t	GetMemUsage() const;

protected:	

	virtual float HeuristicCost( int x, int z, int goalx, int goalz );			// the heuristic cost to get from (x, z) to (goalx, goalz)

	virtual bool InFloodFillRange( int x, int y );
	virtual bool IsFloodFillMatch( int x, int y );
	virtual bool OnFloodFillMatch( int x, int y, int floodCount );
	virtual bool CanFloodGo( FloodDir dir, int x, int y );

// Types
private:
	
	typedef std::priority_queue<AstarGridNode*, std::vector<AstarGridNode*>, SortNodePredicate> NodeContainer;					// priority_queue of open nodes
	typedef std::deque<AstarGridNode*> FloodStack;

// Functions
private:
	inline void AstarBase::GetFirst( AstarGridNode*& node );

	void  ClearOpenList();

// Data
private:

	AstarPool<AstarGridNode>*		m_pool;			// the pool of nodes

	NodeContainer	m_open;			// Open list.		FIX: this would be more efficient if it stored pointers to nodes.  Would mean less copying each push_back.
	MarkingBitfield	m_marking;		// A bitfield map to mark already visited nodes (closed list).

	FloodStack		m_floodStack;

	DirectionMap	m_dirMap;		// the DirectionMap

	size_t			m_maxNodes;		// the maximum number of nodes that will be search.  0 means no limit

	int				m_width;		// the width of the grid to pathfind on
	int				m_height;		// the height of the grid to pathfind on

	int							m_statNumSearched;	// accumulates the number of nodes searched
	int				m_localSearchCount;

	GridPath					m_dbPassibleList;	// will store the passible grid cells visited during a search
	GridPath					m_dbImpassibleList;	// will store the impassible grid cells visited during a search 
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: pops the top node off of v and returns it as n
//	Result	: 
//	Param.	: container - the container we are popping
//			  node - the top node
//	Author	: dswinerd
//
inline void AstarBase::GetFirst( AstarGridNode*& node )
{
	dbAssert( !m_open.empty() );			// can only do top if the priority queue isn't empty
	
    node = m_open.top();
	m_open.pop();
}
