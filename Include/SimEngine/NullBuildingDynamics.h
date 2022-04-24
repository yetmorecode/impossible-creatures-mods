/////////////////////////////////////////////////////////////////////
// File    : NullBuildingDynamics.h
// Desc    : 
// Created : Monday, November 26, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "EntityDynamics.h"
#include "SimExports.h"

class ControllerBlueprint;

///////////////////////////////////////////////////////////////////// 
// NullBuildingDynamics 

class SIM_API NullBuildingDynamics : public EntityDynamics
{
// types
public:
	enum
	{
		DynamicsID = DYNAMICSID_NullBuilding,
	};

// construction
public:
	NullBuildingDynamics( SimEntity *e );

// inherited
public:

	// retrieve the ID of this dynamics
	virtual EntityDynamics::DynamicsIDType	GetDynamicsID( ) const;

	virtual void			RequestMove( Target &T, float AP );
	virtual void			RequestStop();

	// Called when the Entity that this dynamics object operates on is added or removed from the world
	virtual void			OnAddWorld();
	virtual void			OnRemWorld();

	virtual eEDState		QueryStatus() const;

	// Pure Virtual function to perform a step of the dynamics.
	virtual void			DoStep( float speed );

	virtual void			AddPathfinding( );
	virtual void			RemPathfinding( );

	virtual bool			HasSoftRadius() const;

	virtual bool			GetCellTopLeft( long& cellX, long& cellZ ) const;					// returns the top-left impass cell taken up by this Entity at it's current position
	virtual bool			GetCellTopLeft( const Vec2f& pos, long& cellX, long& cellZ ) const;	// returns the top-left impass cell taken up by this Entity if it were are pos
	virtual bool			GetCellWidthHeight( long& width, long& height ) const;				// returns the width and height of the Entity in cell coords

	virtual float			GetDistanceSqrToPoint( const Vec2f& pos, const Vec2f& targetPos2D, Vec2f* heading ) const;
	virtual float			GetDistanceSqrToEntity( const Vec2f& pos, const SimEntity* targetEntity, Vec2f* heading ) const;

	virtual bool			HasPathfindingSector() const;										// returns true if this Entity is written into and has a Pathfinding sector

	virtual void			SetEntityFacing( const Vec2f &newFacingDir );

	// determines if the given position is valid (within the world)
	virtual bool			IsInsideWorld( const Matrix43f* m ) const;								// is the given orientation inside the world

// Static functions for building specific operations
public:

// fields
private:

// implementation
private:
	
};
