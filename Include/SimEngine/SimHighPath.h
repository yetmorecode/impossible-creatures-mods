/////////////////////////////////////////////////////////////////////
// File    : SimHighPath.cpp
// Desc    : 
// Created : 
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "Pathfinding\PathTypes.h"

#include <Memory/MemoryAlloc.h>

/////////////////////////////////////////////////////////////////////
//	Class	: 
//	Result	: 
//	Param.	: 
//	Author	: 
//
class SimHighPath
{
// Types
public:
	typedef std::vector<PathSubCell, mempool_alloc<PathSubCell> > Path;

// Methods
public:
	SimHighPath();												// ctor

	inline void					SetAt( size_t index );
	inline bool					IsDone() const;

	// Get the current path.
	inline const Path&			GetPath() const;				// returns the path points (const)
	inline Path&				GetPath();						// returns the path points

	inline const size_t			GetNumPoints() const;
	inline const PathSubCell	GetPoint(size_t index) const;

	// Get the current waypoint.
	inline size_t				GetCurrentWPIndex() const;		// returns the current waypoint index


	// Useful Member functions.
	inline void					Clear();						// clears the path
	inline const PathSubCell	GetCurrentWP() const;			// returns the current waypoint
	inline const PathSubCell	GetLastWP() const;				// returns the last waypoint
	bool						IncrementWP();					// increment to the next waypoint
	size_t						GetNumFromEnd() const;			// calculates the number of sectors from the end


// Data
private:

	Path m_path;												// stores the sectors in the path.
								
	size_t m_currentWaypoint;									// the current waypoint
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void SimHighPath::SetAt( size_t index )
{
	dbAssert( index < m_path.size() );
	m_currentWaypoint = index;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns true if we are done following the path
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline bool SimHighPath::IsDone() const
{
	return( m_currentWaypoint >= m_path.size() );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const SimHighPath::Path& SimHighPath::GetPath() const 
{ 
	return m_path; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline SimHighPath::Path& SimHighPath::GetPath() 
{
	return m_path; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const size_t SimHighPath::GetNumPoints() const
{
	return( m_path.size() );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathSubCell SimHighPath::GetPoint(size_t index) const
{
	dbAssert( index < m_path.size() );

	return( m_path[index] );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline size_t SimHighPath::GetCurrentWPIndex() const 
{ 
	return m_currentWaypoint; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathSubCell SimHighPath::GetCurrentWP() const 
{ 
	return m_path[m_currentWaypoint]; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathSubCell SimHighPath::GetLastWP() const
{
	dbAssert( !m_path.empty() );
	return m_path.back();
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void SimHighPath::Clear() 
{ 
	m_path.clear(); 
	m_currentWaypoint = 0; 
}