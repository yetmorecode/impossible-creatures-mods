/////////////////////////////////////////////////////////////////////
// File    : GroundDynamicsPath.h
// Desc    : Responsible for GroundDynamics pathing logic
// Created : Monday, January 21, 2002
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "GroundDynamicsInternal.h"

#include "SimPath.h"
#include "SimHighPath.h"
#include "Pathfinding/BrokenSectorConnections.h"
#include "Pathfinding/PathPlan.h"

/////////////////////////////////////////////////////////////////////
//	Forward Declarations
//
class GroundDynamics;

/////////////////////////////////////////////////////////////////////
//	Class	: GroundDynamicsPath
//
class GroundDynamicsPath
{
// Types
public:
	
	enum PathfindingResult		// type of pathfinding results
	{ 
		PR_GoodPath,			// found a good path to the goal
//		PR_PartialPath,			// didn't find a path to the target, but have a path the closest point we could reach
		PR_Unreachable,			// couldn't find a path, but later we might be able to (like if entities move)
		PR_TotallyUnreachable,	// couldn't find a path, we won't be able to later, so don't even try
	};	

	enum NextSectorResult
	{
		NSR_NotFollowingHighPath,
		NSR_SectorInvalid,
		NSR_AtNext,
		NSR_NotAtNext,
	};

	struct GrandPathResult
	{
		PathfindingResult		pathResult;
		AstarSectorPathResult	highlevelResult;
		AstarPathfindingResult	lowlevelResult;
	};


// Methods
public:
	GroundDynamicsPath( );																	// ctor

	inline void		SetGroundDynamics( GroundDynamics *pDG );								// sets the pointer back to the owner

	const Vec2f*	GetPath(int *NumWaypoints) const;										// returns the low-level path (used for debug output)
	void			GetHighPath( std::vector<Vec2f>& points ) const;						// returns the high-levle path (used for debug output)
	const SimPath&	GetLowPath() const;

	void			Reset();																// resets path information
	void			Initialize( const Target* T, float AP );
	void			HandleNewRequest();

	void			SetNextPathfindDelay( unsigned long delay );

	const Vec2f&	GetCurrentWP() const;
	MoveSubState	IncrementWaypoint( MoveSubState oldSubState, MovementInfo& mi );

	bool			TimeToPathfind() const;													// returns true if it is time to pathfind
	bool			CheckPathfindingLoad();

	bool			EvaluateMovingTarget();

	bool			NextWaypointTarget( const Vec2f& initialPos2D, 
										Vec2f& outputHeading2D, float& outputDist );

	// pathfinding routines
	PathfindingResult	NewPath			( bool bGetOffBuilding, TCMask movementMask );					// find a totally new path
	PathfindingResult	NewPath_Old		( bool bGetOffBuilding, TCMask movementMask );
	PathfindingResult	FindHighPath	( SimHighPath &path,	AstarSectorPathResult& pathResult );				// try to find a high-level sector path
	PathfindingResult	FindPathToSector( SimPath &path,		AstarPathfindingResult& pathResult );				// try to find a path to the next sector
	PathfindingResult	FindPathToGoal	( SimPath &path,		AstarPathfindingResult& pathResult, TCMask movementMask, bool bGetOffBuilding );	// try to find a path to the goal
	bool				FindPath		( SimPath &path );			// OLD PATHFINDING

	bool				FindPathStraightLine( SimPath& lowPath, AstarPathfindingResult& pathResult, bool bGetOffBuilding );

	MoveSubState		SectorSwitch( const MoveSubState oldSubState, MovementInfo& mi );			// determine if we need to switch sectors in the high-level path

	NextSectorResult	ToNextSector( MovementInfo& mi ) const;
	MoveSubState		HandleNextSector( const MoveSubState oldSubState );

	void				AddBlockedConnection( PathSubCell source, PathSubCell destination );
	void				ClearBlockedConnections();

	inline const PathPlan&		GetPathPlan() const;
	inline PathPlan&			GetPathPlan();

	MoveSubState		DealWithPathfindingResult( MoveSubState oldSubState, const PathfindingResult& result );

// Types
private:

	enum PathfindingState		// what kind of path are we following
	{ 
		PS_NoPath,				// don't have any path
		PS_HighLevelPath,		// have a high-level path
		PS_ToGoal				// just following a low-level path to the goal
	};

// Data
private:

	GroundDynamics*		m_pGD;					// pointer back to the owning GroundDynamics
	
	SimPath				m_lowPath;				// the low-level Path.
	SimHighPath			m_highPath;				// the high-level path
	PathfindingState	m_pathState;			// the current path state

	PathPlan			m_pathPlan;

	GrandPathResult		m_grandPathResult;	

	// pathfinding retry information
	int					m_retryCount;			
	long				m_lastPathfind;
	long				m_nextPathfind;
	long				m_loadDelayTicks;

	long				m_lastNewPath;			// records the time of the last NewPath

	bool				m_bLockedToSectors;		
	BrokenSectorConnections	m_blockedSectors;	
	
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void GroundDynamicsPath::SetGroundDynamics( GroundDynamics *pGD )
{
	dbAssert(pGD);
	m_pGD = pGD;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathPlan& GroundDynamicsPath::GetPathPlan() const
{
	return m_pathPlan;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline PathPlan& GroundDynamicsPath::GetPathPlan()
{
	return m_pathPlan;
}


