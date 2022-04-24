/////////////////////////////////////////////////////////////////////
// File    : SimPath.cpp
// Desc    : 
// Created : 
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <Math/Vec2.h>

#include <Memory/MemoryAlloc.h>

/////////////////////////////////////////////////////////////////////
//	Class	: SimPath
//

class SimPath
{
// types
public:
	typedef std::vector< Vec2f, mempool_alloc<Vec2f> > Path;

// interface
public:
	SimPath();

	// Get the current path.
	inline const Path&	GetPoints() const;
	inline Path&		GetPoints();
	
	// Get the current waypoint.
	inline size_t		GetCurrentWPidx() const;

	// Set the current points in the path.
	//	Just copy over the 
	inline void			SetPoints( const Path &p );
	inline void			SetCurrentWPidx( const size_t wp );

	// Useful Member functions.
	inline const Vec2f&	GetCurrentWP() const;

	inline void			Clear();

	inline size_t		Size() const;

	bool				IncWP();

	bool				IsAtEnd() const;

// data
private:
	Path m_Path;	// Stores the points in the path.
								//	A straight line between these points is the defined path.
	size_t m_CurrentWaypoint;	// The current waypoint.
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the points in the path (const)
//
inline const SimPath::Path& SimPath::GetPoints() const 
{
	return m_Path; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the points in the path
//
inline SimPath::Path& SimPath::GetPoints() 
{ 
	return m_Path; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline size_t SimPath::GetCurrentWPidx() const 
{ 
	return m_CurrentWaypoint; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void SimPath::SetPoints( const Path &p ) 
{ 
	m_CurrentWaypoint = 0;
	m_Path = p; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void SimPath::SetCurrentWPidx( const size_t wp )
{
	dbAssert( wp < m_Path.size() );
	m_CurrentWaypoint = wp; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Vec2f& SimPath::GetCurrentWP() const
{
	dbAssert( m_CurrentWaypoint < m_Path.size() );
	return m_Path[ m_CurrentWaypoint ]; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void SimPath::Clear()
{
	m_Path.clear(); 
	m_CurrentWaypoint = 0; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline size_t SimPath::Size() const
{
	return m_Path.size();
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline bool SimPath::IsAtEnd() const
{
	return m_CurrentWaypoint >= m_Path.size();
}