/////////////////////////////////////////////////////////////////////
// File    : EntityDynamics.h
// Desc    : This is the base EntityDynamics class. The EntityDynamics class cannot be instantiated itself,
//			 but it provides a base framework for all Entity dynamics to fit into. All Entity dynamics will
//			 be derived from this class.
// Created : Wednesday, October 03, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "SimExports.h"

#include "GroupController.h"
#include "SimGroupController.h"
#include "SimController.h"

#include "SimEntity.h"

///////////////////////////////////////////////////////////////////// 
// Forward Declarations

class SimEntity;
class SimController;
class SimGroupController;
class Target;
class SimWorld;

///////////////////////////////////////////////////////////////////// 
// EntityDynamics class

class SIM_API EntityDynamics
{
// types.
public:

	enum eEDState
	{ 
		eStateMove = 0, 
		eStateStop, 
		eStateDie, 
		eStateCantGetThere 
	};

	enum EDmovementType
	{ 
		eEDLand, 
		eEDWater, 
		eEDAmphibian, 
		eEDAir 
	};

	enum DynamicsIDType
	{	
		DYNAMICSID_Ground,
		DYNAMICSID_Flying,
		DYNAMICSID_Gyro,
		DYNAMICSID_Building,
		DYNAMICSID_NullBuilding,
		DYNAMICSID_Flocking,
		
		DYNAMICSID_Count,
	};

	enum StopType
	{
		ST_Invalid,
		ST_AtGoal,
		ST_NotAtGoal,
	};

	enum CantGetThereType
	{
		CGTT_Invalid,
		CGTT_Terrain,
		CGTT_Building,
		CGTT_Entity
	};

// Data.
protected:

	eEDState			m_state;				// Status, Moving or Idle.
	StopType			m_stopType;				// only valid if m_state == eStateStop
	
	Target				m_T;					// the current target
	float				m_AP;					// Acceptible Proximity to Target.
//	Vec3f				m_TargetPos;			// Overall target for movement, may be different than the position of m_T (if we have an AP, can't reach m_T, etc.)
	
	SimEntity*			m_Entity;				// Pointer to the entity that this EntityDynamics belongs to. The entity holds the data that the dynamics "gets" and "sets" to adjust the position.
	EDmovementType		m_movementType;			// the current movement type
	EDmovementType		m_prevMovementType;		// the previous movement type

	int					m_groupID;

// Construction
protected:

	EntityDynamics(SimEntity *e);										// ctor

// Virtual functions to control movement.
public:

	virtual ~EntityDynamics();											// dtor

	// retrieve the ID of this dynamics
	virtual DynamicsIDType	GetDynamicsID( ) const = 0;

	//	AP stands for Acceptible Proximity.
	virtual void			RequestMove			( Target &T, float AP ) = 0;
	virtual void			RequestJump			( Target &T, 
												  float jumpSpeed, float jumpHeight, 
												  float pathDeviation, float amplitudeMultiplier, 
												  float periodTicks );
	virtual void			RequestStop			() = 0;
	virtual void			RequestImmobilize	();
	virtual void			RequestDie			();
	virtual bool			RequestTeleport		( const Vec2f& destination );	
	virtual bool			RequestPark			( Target &T, long numTicks );
	virtual bool			RequestEntityFacing	( const Vec2f& newFacingDir, float degreesPerTick = 0.0f );	
	virtual bool			RequestMoveToLock	( long attackTicks );
	virtual bool			RequestDig			( bool bDigDown );
	

	virtual void			StartAttack			( long attackTicks, long coolTicks, bool bRange );
	virtual void			StartCoolDown		( long coolTicks );
	virtual void			StartReaquire		( long attackTicks, long coolTicks );

	// Called when the Entity that this dynamics object operates on is added or removed from the world
	virtual void			OnAddWorld();
	virtual void			OnRemWorld();

	virtual void			OnConstructed();

	// Pure Virtual function to perform a step of the dynamics.
	//	A return of True means we are in a "stopped" state.
	virtual eEDState		QueryStatus() const = 0;

	// Pure Virtual function to perform a step of the dynamics.
	virtual void			DoStep( float speed ) = 0;

	// snap this entity's position
	virtual void			SnapPosition( );

	// set a hint that this is the last waypoint in a series move.
	virtual void			HintLastWaypoint( bool ) {;}

	// determines if the given position is valid (within the world)
	virtual bool			IsInsideWorld( const Matrix43f* ) const = 0;

	// virtual functions to add and remove this dynamics from the pathfinding map
	virtual void			AddPathfinding( );
	virtual void			RemPathfinding( );

	// explicitly set the direction an Entity should look.  This function should be avoided in favour of RequestFacing() as only the dynamics should be setting the facing
	// NOTE: newFacingDir is a direction, not a position
	virtual void			SetEntityFacing( const Vec2f& newFacingDir );
	
	virtual bool			HasPathfindingSector() const;											// returns true if this Entity is written into and has a Pathfinding sector

	virtual long			GetTurnAroundTicks( long, long ) const;									// 

	virtual bool			HasSoftRadius() const = 0;
	virtual float			GetSoftRadius() const;

	virtual bool			GetCellTopLeft( long& cellX, long& cellZ ) const = 0;						// returns the top-left impass cell taken up by this Entity at it's current position
	virtual bool			GetCellTopLeft( const Vec2f& pos, long& cellX, long& cellZ ) const = 0;		// returns the top-left impass cell taken up by this Entity if it were are pos
	virtual bool			GetCellTopLeft( const Vec2i& pos, long& cellX, long& cellZ ) const = 0;
	virtual bool			GetCellWidthHeight( long& width, long& height ) const = 0;					// returns the width and height of the Entity in cell coords

	virtual float			GetDistanceSqrToPoint( const Vec2f& pos, const Vec2f& targetPos2D, Vec2f* heading ) const = 0;
	virtual float			GetDistanceSqrToPoint(					 const Vec2f& targetPos2D, Vec2f* heading ) const = 0;
	virtual float			GetDistanceSqrToEntity( const Vec2f& pos, const SimEntity* targetEntity, Vec2f* heading ) const = 0;
	virtual float			GetDistanceSqrToEntity(					  const SimEntity* targetEntity, Vec2f* heading ) const = 0;

	virtual CantGetThereType	GetCantGetThereType() const;
	virtual SimEntity*			GetBlockingBuilding();

	virtual void			GetSyncLogString( char *buffer, int buflen ) const;

// Functions.
public:

	void* operator new   ( size_t );																// operator new
	void  operator delete( void* );																	// operator delete

	inline SimEntity*		GetEntity( );															// returns the SimEntity that owns this Dynamics
	inline const SimEntity*	GetEntity( ) const;														// returns the SimEntity that owns this Dynamics (const)

	inline SimController*		GetController( );
	inline const SimController*	GetController( ) const;

	// gets the SimGroupController the Entity is currently in.  Can be NULL.
	inline SimGroupController*			GetGroupController( );
	inline const SimGroupController*	GetGroupController( ) const;

	inline void				SetTarget( const Target& target );										// returns the Dynamics current Target*
	inline const Target*	GetTarget( ) const;														// returns the Dynamics current Target*
	inline float			GetAP( ) const;															// returns the current AP

	// returns the GroupController ID of the group this dynamics is currently in.  Will be 0 if no group.
	inline int				GetGroupID() const;

	// sets the GroupController ID of the group this dynamics is currently in.  Can be 0 if no group.
	inline void				SetGroupID( int groupID );

	// Put this in to instantly stop the dynamics.
	// This is a hax0r thing to fix the animal teleport keep running bug.
	void					SetStateStop();

	inline void				SetVisualMovementType		( EDmovementType moveType );
	inline EDmovementType	GetVisualMovementType		( ) const;
	inline EDmovementType	GetPrevVisualMovementType	( ) const;

	bool					RequestEntityLookAtPoint( const Vec3f& pos );

	// explicitly set the direction an Entity should look.  This function should be avoided in favour of RequestEntityLookAtPoint() as only the dynamics should be setting the facing
	void					SetEntityLookAtPoint( const Vec3f& pos );

// Internal functions
private:

	EntityDynamics();	// Prevent use of the default no-argument constructor.

	void					SetEntity(SimEntity *e);
};

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//
inline SimEntity* EntityDynamics::GetEntity( ) 
{ 
	return m_Entity; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//
inline const SimEntity*	EntityDynamics::GetEntity( ) const 
{ 
	return m_Entity; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline SimController* EntityDynamics::GetController( )
{
	return static_cast< SimController* >( m_Entity->GetController() );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const SimController*	EntityDynamics::GetController( ) const
{
	return static_cast< const SimController* >( m_Entity->GetController() );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline SimGroupController* EntityDynamics::GetGroupController( )
{
	return static_cast< SimGroupController* >( GetController()->GetGroupController() );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const SimGroupController* EntityDynamics::GetGroupController( ) const
{
	return static_cast< const SimGroupController* >( GetController()->GetGroupController() );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void	EntityDynamics::SetTarget( const Target& target )
{
	m_T = target;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Target* EntityDynamics::GetTarget() const
{ 
	return &m_T; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline float EntityDynamics::GetAP() const
{ 
	return m_AP; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void EntityDynamics::SetStateStop()
{ 
	m_state = eStateStop; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//
inline void EntityDynamics::SetVisualMovementType( EDmovementType moveType ) 
{ 
	m_prevMovementType = m_movementType; 

	if ( m_movementType != moveType )
	{ 
		m_movementType = moveType; 
	} 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//
inline EntityDynamics::EDmovementType EntityDynamics::GetVisualMovementType() const 
{ 
	return m_movementType; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//
inline EntityDynamics::EDmovementType EntityDynamics::GetPrevVisualMovementType() const
{ 
	return m_prevMovementType; 
}
/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//
inline int EntityDynamics::GetGroupID() const
{
	return m_groupID;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//
inline void	EntityDynamics::SetGroupID( int groupID )
{
	m_groupID = groupID;
}