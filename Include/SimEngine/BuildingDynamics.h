/////////////////////////////////////////////////////////////////////
// File    : BuildingDynamics.h
// Desc    : 
// Created : Wednesday, October 03, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "EntityDynamics.h"
#include "SimExports.h"

class ControllerBlueprint;

///////////////////////////////////////////////////////////////////// 
// BuildingDynamics 

class SIM_API BuildingDynamics : public EntityDynamics
{
// types
public:
	enum
	{
		DynamicsID = DYNAMICSID_Building,
	};

	enum HeightSnapType
	{
		HST_Surface,
		HST_HeightMap
	};
		
// construction
public:
	BuildingDynamics( SimEntity *e, HeightSnapType heightSnapType );

// inherited
public:

	// retrieve the ID of this dynamics
	virtual EntityDynamics::DynamicsIDType	GetDynamicsID( ) const;

	virtual void			RequestMove(Target &T, float AP);
	virtual void			RequestStop();

	// Called when the Entity that this dynamics object operates on is added or removed from the world
	virtual void			OnAddWorld();
	virtual void			OnRemWorld();

	virtual void			OnConstructed();

	virtual eEDState		QueryStatus() const;

	// determines if the given position is valid (within the world)
	virtual bool			IsInsideWorld( const Matrix43f* m ) const;								// is the given orientation inside the world

	// Pure Virtual function to perform a step of the dynamics.
	virtual void			DoStep( float speed );

	virtual void			AddPathfinding( );
	virtual void			RemPathfinding( );

	virtual bool			GetCellTopLeft( long& cellX, long& cellZ ) const;					// returns the top-left impass cell taken up by this Entity at it's current position
	virtual bool			GetCellTopLeft( const Vec2f& pos, long& cellX, long& cellZ ) const;	// returns the top-left impass cell taken up by this Entity if it were are pos
	virtual bool			GetCellTopLeft( const Vec2i& pos, long& cellX, long& cellZ ) const;
	virtual bool			GetCellWidthHeight( long& width, long& height ) const;				// returns the width and height of the Entity in cell coords

	virtual float			GetDistanceSqrToPoint( const Vec2f& pos, const Vec2f& targetPos2D, Vec2f* heading ) const;
	virtual float			GetDistanceSqrToPoint(					 const Vec2f& targetPos2D, Vec2f* heading ) const;
	virtual float			GetDistanceSqrToEntity( const Vec2f& pos, const SimEntity* targetEntity, Vec2f* heading ) const;
	virtual float			GetDistanceSqrToEntity(					  const SimEntity* targetEntity, Vec2f* heading ) const;

	virtual bool			HasSoftRadius() const;

	virtual bool			HasPathfindingSector() const;										// returns true if this Entity is written into and has a Pathfinding sector

	virtual void 			SetEntityFacing( const Vec2f &newFacingDir );

// Static functions for building specific operations
public:

	// Locks the lockPos to the correct location based on the ControllerBlueprint building info
	static void				CalcEntityLockedPos( const ControllerBlueprint*, const Matrix43f*, Vec3f& lockPos );
	static void				CalcEntityTopLeftCell( const ControllerBlueprint*, const Matrix43f*, const Vec3f& Pos, long& CellX, long& CellZ );
	static void				CalcEntityTopLeftCell( const ControllerBlueprint*, const Matrix43f*, const Vec2f& Pos, long& CellX, long& CellZ );

	static void				CalcEntityWorldFromTopLeftCell( const ControllerBlueprint*, const Matrix43f*, long CellX, long CellZ, Vec3f& retPos );

	static void				GetEntityWidthHeight( const ControllerBlueprint*, const Matrix43f*, long& width, long& height );
	
	
// fields
private:

// implementation
private:

	HeightSnapType	m_heightSnapType;
	bool			m_bConstructed;

	bool			m_bDirtyPos;
	Vec2i			m_cellTopLeft;

	bool			m_bDirtySize;
	Vec2i			m_cellSize;
};
