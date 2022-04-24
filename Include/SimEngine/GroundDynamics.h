/////////////////////////////////////////////////////////////////////
// File    : GroundDynamics.h
// Desc    : 
// Created : Tuesday, February 27, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "SimExports.h"

#include "GroundDynamicsInternal.h"
#include "GroundDynamicsPath.h"

#include "Pathfinding/SparseGridEntry.h"

#include "EntityDynamics.h"

#include <Math/Vec2.h>
#include <SurfVol/Hull2d.h>
#include "EntityGroup.h"

#include "SimPath.h"
#include "SimHighPath.h"

#include "Pathfinding/Impassmap.h"


/////////////////////////////////////////////////////////////////////
//	Forward Declarations
//
class Pathfinder;
class TerrainCellMap;
class OBB2f;
class EntityGroup;

///////////////////////////////////////////////////////////////////// 
// GroundDynamics 
class SIM_API GroundDynamics : public EntityDynamics
{
	friend class GroundDynamicsPath;
public:	

	enum
	{
		DynamicsID = DYNAMICSID_Ground,
	};

	enum MovementFlags
	{
		MF_Moving		= 1 << 0
	};

	// variable to check for when determining the height of the creature based on land/water levels.

// Static Functions
public:
	static int		CalculatePreciseSize( float xscale, float zscale );
	static bool		GetCellPassible( float x, float z, const TCMask pfCellMask );
	static bool		GetCellPassibleGrid(int x, int y, const TCMask pfCellMask );

	static bool		GetCellPassiblePrecise		 ( float x, float z, const GroundAttributes& attributes, unsigned long ignoreFlags = 0 );
	static bool		GetCellPassiblePreciseGrid	 ( int x, int y, const GroundAttributes& attributes, unsigned long ignoreFlags = 0 );
	static bool		GetCellPassiblePreciseTerrain( int x, int y, int entitySize, const TCMask pfCellMask );
	static bool		GetCellPassiblePreciseTerrain( int x, int y, int entitySize, const TCMask pfCellMask, const PathMajorCell majorCell, bool bAmphibian );
	static bool		GetCellPassiblePreciseEntity ( int x, int y, const GroundAttributes& attributes, unsigned long ignoreFlags = 0 );
	static bool		GetCellPassiblePreciseEntity ( float x, float z, const GroundAttributes& attributes, unsigned long ignoreFlags = 0 );
	static int		GetEntitySize(const SimEntity* entity);
	static TCMask	GetPassibleTypes( const SimEntity* pe );
	static int		GetGroundLevel( const SimEntity* pe );
	
// Functions.
public:
	// Constructor.
	GroundDynamics(SimEntity *e);															// ctor
	~GroundDynamics( );																		// dtor

	EntityDynamics::eEDState	QueryStatus() const;										// query the state of the GD

	bool						IsMoving() const;											// returns true if this entity is moving FIX: should be in EntityDynamics?

	void						SetMovementMask( const TCMask &pfCellMask );				// sets the movement passibility mask

	inline int					GetPreciseSize() const;										// returns the precise size that the pathfinding used
	inline SparseGridEntry&		GetSparseGridEntry();
	const Vec2f*				GetPath(int *NumWaypoints) const;							// returns the low-level path (used for debug output)
	void						GetHighPath( std::vector<Vec2f>& points ) const;			// returns the high-levle path (used for debug output)

	void						DoStep(float speed);										// called by the controller to perform a step of movement

	void						SnapPosition( );

	TCMask						GetPassibleTypes() const;

	inline const Target*		GetCurrentTarget() const;
	inline float				GetCurrentAP() const;

	inline bool					GetMovementFlag( unsigned long flag ) const;
	inline int					GetGroundLevel() const;
	inline void					SetGroundLevel( int groundLevel );

// Inherited Functions
public:

	// retrieve the ID of this dynamics
	virtual EntityDynamics::DynamicsIDType	
								GetDynamicsID( ) const;													// returns the DynamicsID

	// Functions to Set Movement
	virtual void				RequestMove( Target &T, float AP );
	virtual void				RequestJump( Target &T, 
											 float jumpSpeed, float jumpHeight, 
											 float pathDeviation, 
											 float amplitudeMultiplier, 
											 float periodTicks );
	// give a move order.
	virtual void				RequestStop();															// request the GroundDynamics stop
	virtual bool				RequestTeleport( const Vec2f& destination );							// request the GroundDynamics to teleport
	virtual bool				RequestEntityFacing( const Vec2f& newFacingDir, float degreesPerTick = 0.0f );						// request that the Entity face in a new direction
	virtual bool				RequestMoveToLock( long attackTicks );
	virtual bool				RequestDig( bool bDigDown );
	
	virtual	void				OnAddWorld();															// called by the controller when added to the world
	virtual void				OnRemWorld();															// called by the controller when removed from the world

	virtual void				AddPathfinding( );														// adds the dynamics into the pathfinding system
	virtual void				RemPathfinding( );														// remove the dynamics into the pathfinding system

	virtual bool				IsInsideWorld( const Matrix43f* m ) const;								// is the given orientation inside the world

	virtual EntityDynamics::CantGetThereType 
								GetCantGetThereType() const;											//
	virtual SimEntity*			GetBlockingBuilding();													// if CantGetThereType is CGTT_Building, this function will return the building

	virtual void				SetEntityFacing( const Vec2f &newFacingDir );			// sets the direction the entity faces

	virtual bool				HasSoftRadius() const;
	virtual float				GetSoftRadius() const;

	virtual bool				GetCellTopLeft( long& cellX, long& cellZ ) const;						// returns the top-left precise cell taken up by this Entity at it's current position
	virtual bool				GetCellTopLeft( const Vec2f& pos, long& cellX, long& cellZ ) const;		// returns the top-left precise cell taken up by this Entity if it were are pos
	virtual bool				GetCellTopLeft( const Vec2i& pos, long& cellX, long& cellZ ) const;
	virtual bool				GetCellWidthHeight( long& width, long& height ) const;					// returns the width and height of the Entity in cell coords

	virtual float				GetDistanceSqrToPoint( const Vec2f& pos, const Vec2f& targetPos2D, Vec2f* heading ) const;
	virtual float				GetDistanceSqrToPoint(					 const Vec2f& targetPos2D, Vec2f* heading ) const;
	virtual float				GetDistanceSqrToEntity( const Vec2f& pos, const SimEntity* targetEntity, Vec2f* heading ) const;
	virtual float				GetDistanceSqrToEntity(					  const SimEntity* targetEntity, Vec2f* heading ) const;

	virtual void				GetSyncLogString( char *buffer, int buflen ) const;

// types
private:

	struct Request
	{
		eEDState	state;		// wanted state
		Target*		T;			// Target
		float		AP;			// Acceptible Proximity to Target.
	};

	enum AtTargetAction
	{
		ReturnAtGoal,
		ReturnStopNotGoal,
		ReturnCantGetThereTerrain,
		ReturnCantGetThereBuilding,
		ReturnCantGetThereEntity		
	};

	enum StepResult
	{
		SR_Invalid,
		SR_ReachedGoal,
		SR_Stepped,
		SR_Blocked
	};

// Functions
private:

	// Prevent use of the default contructor.
	GroundDynamics();

	inline GroundDynamicsPath*			Pather();
	inline const GroundDynamicsPath*	Pather() const;

	float				GetHeight	(float x, float z);					// returns the height the entity should be at if standing at (x, z)
	Vec3f				GetUpVector	(float x, float z);					// returns the up vector the entity should have if standing at (x, z)
	const Vec3f&		GetTargetPos() const;							// returns the current target position

	void				SetCachedTerrainType();							// sets the terrain type we are currently on

	inline void			SetMovementFlag	 ( MovementFlags flag );
	inline void			ClearMovementFlag( MovementFlags flag );

	bool				DoGroundMovement(float speed);					// the main ground movement function


	void				HandleNewRequests();							// processes new requests
	void				ToDynamicsStateMove();
	void				ToDynamicsStateJump();
	void				ToDynamicsStateStop();

	void				EvaluateMovingTarget();							// moving target logic handler
	
	void				SetCurrentTarget( Target *target, float AP );
	void				SetCurrentTarget( Entity *pEntity, float AP );
	void				SetCurrentTarget( const Vec3f& pos, float AP );

	MoveSubState		DealWithPathfindingResult( const GroundDynamicsPath::PathfindingResult& result );
//	MoveSubState		DealWithBlockAction( const BlockAction& ba );

	MoveSubState		NextWaypointTarget( const Vec2f& initialPos2D, 
											Vec2f& outputHeading2D, float& outputDist );


	void				MoveEntity( MovementInfo& mi );					// changes the Entity's position/facing
	void				EndMovement( eEDState endState );

	float				DistToTarget( const Target* target, float AP, const Vec2f &pos, Vec2f &heading ) const;
	bool				OverlappingTarget( const Vec2f& pos ) const;

	bool				GetEntitySeparationVector( const Vec2f& pos, const SimEntity* targetEntity, Vec2f& pushVector, float& pushDistance );

	void				CalcNewPos( const Vec3f &ePosRef, const Vec2f& newMoveDir, const float newSpeed,
									Vec2f &newPos, float &horizontalSpeed );

	void				DoStepInternal(float speed);
	StepResult			DoMoveStep( MovementInfo& mi, const Vec2f& newEntityPos2D, const Vec2f& newMoveDir2D, float usedSpeed );
	MoveSubState		DoDirectMove( MovementInfo& mi );

	bool				WaypointStep( MovementInfo& mi, bool& bIncWaypoint, StepResult& stepResult );

	EntityDynamics*		GetTargetDynamics() const;						// gets the target's dynamics if it is an entity

	void				SignalRepath();

	void				DoTurning( );

	MoveSubState		DoRepath( MovementInfo& mi, unsigned long& pathfindCount );

	MoveSubState		HandleGetOffImpassible	( MovementInfo& mi );
	MoveSubState		HandleWaitingForPath	( MovementInfo& mi, unsigned long& pathfindCount );
	MoveSubState		HandleWaitBlocked		( MovementInfo& mi, unsigned long& pathfindCount );
	MoveSubState		HandleFollowingPath		( MovementInfo& mi );
	MoveSubState		HandleHeadingTowardGoal	( MovementInfo& mi );
	MoveSubState		HandleReachedGoal		( MovementInfo& mi );
	MoveSubState		HandleMoveToLock		( MovementInfo& mi );
	MoveSubState		HandleJumpToLock		( MovementInfo& mi );
	MoveSubState		HandlePushFromTarget	( MovementInfo& mi );
	MoveSubState		HandleNextSector		( MovementInfo& mi );
	MoveSubState		HandleStraightToTarget	( MovementInfo& mi );

// fields
private:

	// GD specific data
	int					m_preciseSize;			// stores the NxN meter size of the Entity
	TCMask				m_passibleTypes;		// stores the TCMask we need to determine passibility

	// requests
	bool				m_bNewRequest;			// will be true if a new request was given
	Request				m_request;				// the actual request

	// state stuff
	Target*				m_currentTarget;		// 
	Target				m_tempTarget;			// if we are moving with an AP or to an unreachable target m_tempTarget will be used
	MoveSubState		m_subState;
	float				m_currentAP;			// the AP we are using to get to the current target

	// flags
	unsigned long		m_movementFlags;

	// generic counter
	long				m_ticks;				

	long				m_straightLineTime;

	// movetolock stuff
	bool				m_bMoveToLock;			// state
	Vec2i				m_lockPos;				// locked position in cell coords
	long				m_moveToLockTicks;		// total time for the movement
    Vec2f				m_moveToLockStart;		// start position in world coords
	Vec2f				m_moveToLockDir;		// direction vector in world coords
	
	// jumptolock stuff
	// note: it sucks that there are so many member variables for this.
	// we are also using some of the movetolock stuff
	bool				m_bJump;				// are we jumping?
	float				m_jumpHeight;			// current height of jump
	float				m_jumpParabA;			// parabolic function parameter
	float				m_jumpParabB;			// parabolic function parameter
	float				m_jumpParabC;			// parabolic function parameter
	float				m_jumpErratic;			// erratic factor
	float				m_jumpSinA;				// sine wave function parameter
	float				m_jumpSinB;				// sine wave function parameter
		
	// turning stuff
	bool				m_bTurning;
	long				m_turnTicks;
	float				m_turnRadians;

	CantGetThereType	m_cantGetThereType;		// only valid if m_state == eStateCantGetThere
	EntityGroup			m_blockingEntity;		// only valid if m_cantGetThereType == CGTT_Building

	// component classes
	GroundDynamicsPath	m_gdPath;				// GroundDynamicsPath component class
	SparseGridEntry		m_sparseGridEntry;		// the dynamics entry in the SparseGrid
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the GroundDynamicsPath
//
inline GroundDynamicsPath* GroundDynamics::Pather()
{
	return &m_gdPath;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the GroundDynamicsPath (const)
//
inline const GroundDynamicsPath* GroundDynamics::Pather() const
{
	return &m_gdPath;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the precise size of the Entity. ie the NxN size of the entity in meter cells
//	Result	: int
//
inline int GroundDynamics::GetPreciseSize() const
{
	return m_preciseSize;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//
inline SparseGridEntry& GroundDynamics::GetSparseGridEntry()
{
	return m_sparseGridEntry;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline TCMask GroundDynamics::GetPassibleTypes() const
{
	return m_passibleTypes;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Target* GroundDynamics::GetCurrentTarget() const
{ 
	//return m_currentTarget; 
	return Pather()->GetPathPlan().GetCurrentTarget();
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline float GroundDynamics::GetCurrentAP() const 
{ 
//	return m_currentAP; 
	return Pather()->GetPathPlan().GetCurrentAP();
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline bool GroundDynamics::GetMovementFlag( unsigned long flag ) const
{
	return (m_movementFlags & flag) != 0;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void GroundDynamics::SetMovementFlag( MovementFlags flag )
{
	m_movementFlags |= flag;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void GroundDynamics::ClearMovementFlag( MovementFlags flag )
{
	m_movementFlags &= ~flag;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int GroundDynamics::GetGroundLevel( ) const
{
	return m_sparseGridEntry.GetGroundLevel();
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void GroundDynamics::SetGroundLevel( int groundLevel )
{
	m_sparseGridEntry.SetGroundLevel( groundLevel );
}