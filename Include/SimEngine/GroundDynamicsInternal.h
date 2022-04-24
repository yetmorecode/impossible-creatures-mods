/////////////////////////////////////////////////////////////////////
// File    : GroundDynamicsInternal.h
// Desc    : 
// Created : Monday, January 21, 2002
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once


enum MoveSubState 
{ 
	SS_WaitingForPath,		// Can't follow my current intended heading, need to calculate a new heading/path.			
	SS_WaitBlocked,			// 
	SS_FollowingPath,		// Going toward next waypoint, the PathfindingState will hold what kind of path we are following
	SS_GetOffImpassible,	// Damn, Entity is on an impassible, allow it to get off.  This shouldn't happen but does if guys are spawn on something.

	SS_MoveToLock,			// Leaping to a locked position
	SS_JumpToLock,			// Jumping to a locked position
	SS_ReachedGoal,			// may not be necessary, but a default, do "nothing during DoGroundMovement" state.
	SS_HeadingTowardGoal,	// Getting the heading direction from the Target - may not be required.
	SS_NextSector,			//

	SS_CantReach,			// Trying to reach an impossible goal 
	SS_Stop,				// Stopping at the goal
	SS_PushFromTarget,		// entity is getting push away from its target
	SS_StraightToTarget,
};


struct MovementInfo
{
	bool	bStartedOnImpassible;	// did we start on an impassible
	bool	bStartedOnBuilding;		// did we start on a building
	bool	bStartedOnEntity;		// did we start on an Entity?
	Vec2f	position2D;				// current position
	Vec2f	heading2D;				// heading to the target (waypoint or entity)
	float	distToTarget;			// distance to target (waypoint or entity)
	float	speedRemaining;			// speed unaccounted for this move
	bool	bWaiting;				// waiting for a path
};
