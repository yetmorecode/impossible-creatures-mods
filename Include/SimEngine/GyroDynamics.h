/////////////////////////////////////////////////////////////////////
// File    : GyroDynamics.h
// Desc    : 
// Created : Thursday, December 13, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once


#include "EntityDynamics.h"
#include "SimExports.h"
#include "Target.h"

#include <Math/Vec2.h>


///////////////////////////////////////////////////////////////////// 
// GyroDynamics 

class SIM_API GyroDynamics : public EntityDynamics
{
// types
public:	
	enum
	{
		DynamicsID = DYNAMICSID_Gyro,
	};

// construction
public:
	GyroDynamics(SimEntity *e);

	void		LiftOff();																// call when the gyro is finished constructed and needs to take off
	void		SetFlying();
	bool		IsLanded() const;

// inherited
public:

	// retrieve the ID of this dynamics
	virtual EntityDynamics::DynamicsIDType	GetDynamicsID( ) const;

	virtual void RequestMove(Target &T, float AP );
	virtual void RequestStop();
	virtual void RequestDie();
	virtual bool RequestTeleport( const Vec2f& destination );
	virtual bool RequestPark( Target &T, long numTicks );

	virtual void OnAddWorld();
	virtual void OnRemWorld();

	// set a hint that this is the last waypoint in a series move.
	virtual void HintLastWaypoint( bool bLast );

	virtual void AddPathfinding( );
	virtual void RemPathfinding( );

	virtual bool GetCellTopLeft( long& cellX, long& cellZ ) const;						// returns the top-left impass cell taken up by this Entity at it's current position
	virtual bool GetCellTopLeft( const Vec2f& pos, long& cellX, long& cellZ ) const;	// returns the top-left impass cell taken up by this Entity if it were are pos
	virtual bool GetCellTopLeft( const Vec2i& pos, long& cellX, long& cellZ ) const;
	virtual bool GetCellWidthHeight( long& width, long& height ) const;					// returns the width and height of the Entity in cell coords
	virtual	bool HasPathfindingSector() const;

	virtual float			GetDistanceSqrToPoint( const Vec2f& pos, const Vec2f& targetPos2D, Vec2f* heading ) const;
	virtual float			GetDistanceSqrToPoint(					 const Vec2f& targetPos2D, Vec2f* heading ) const;
	virtual float			GetDistanceSqrToEntity( const Vec2f& pos, const SimEntity* targetEntity, Vec2f* heading ) const;
	virtual float			GetDistanceSqrToEntity(					  const SimEntity* targetEntity, Vec2f* heading ) const;

	virtual bool				HasSoftRadius() const;
	virtual float				GetSoftRadius() const;

	virtual bool IsInsideWorld( const Matrix43f* m ) const;								// is the given orientation inside the world

	virtual EntityDynamics::eEDState QueryStatus() const;

	virtual void DoStep(float speed);

// types
private:

	enum MoveSubState
	{
		SS_Landed,
		SS_LiftOff,
		SS_Landing,
		SS_FlyToRange, 
		SS_LineUpTarget,
		SS_Attack,
		SS_FollowThru,
		SS_HoverTarget,
		SS_FallDead
	};
	
	struct Request
	{
		eEDState		state;			// wanted state
		Target*			T;				// Target
		float			AP;				// Acceptible Proximity to Target.
		long			attackTicks;
		long			coolTicks;
	};


// fields
private:

	float			m_speed;
	bool			m_bNearTarget;
	bool			m_bInLineTarget;
	
	float			m_maxSpeed;
	bool			m_SetSpeed;

	MoveSubState	m_subState;
	float			m_speedLeft;

	bool			m_bNewRequest;			// will be true if there is new request to be processed
	Request			m_request;				// the actual request
	long			m_originalTicks;
	long			m_ticks;

	bool			m_bLastWaypoint;

	float			m_turnAmount;
	float			m_altitude;

	Vec2f			m_velocity;
	Vec2f			m_acceleration;

// implementation
private:
	// Private utility function that does the bulk of the movement work.
	//	Returns true if Entity is idle, returns false if entity is moving.
	float			GetHeight(float x, float z) const;
	float			GetTerrainHeight( float x, float z ) const;
	float			GetTargetHeight() const;
	const Vec3f&	GetTargetPosition() const;
	void			DoStepInternal(float speed);

	void			SetTargetValidating( Target *T );
	void			SetCurrentPositionAsTarget();

	void			MoveEntity( const Vec3f& newPos );
	void			MoveEntity( const Matrix43f& newTransform );

	void			HandleNewRequests();
	void			ToDynamicsStateMove();
	void			ToDynamicsStateStop();
	void			ToDynamicsStateDie();

	void			ClampToWorld( const Vec3f& position, Vec3f& newPos ) const;

	// steering behaviour functions
	Vec2f			ArrivalCalculate( const Vec2f& pos, const Vec2f &targetPos );
	Vec2f			SeparationCalculate( );
	Vec2f			AvoidanceCalculate( const Vec2f& forward );
	bool			IsPositionPenetrating( const Vec3f& position );

	float			AvoidanceHeightCalc( );

	float			DistToTargetSqr( const Vec2f &pos, Vec2f &heading ) const;

	MoveSubState	WithinRange();
	void			DoFlyWithTurn( const Vec2f& dirToTarget, float distToTarget, float turnDegrees );

	void			FlyDirect( const Vec2f& dirToTarget, float distToTargetSqr, float turnDegrees );

	void			TurnTowardsTarget( const Vec2f& dirToTarget, float distToTargetSqr, float turnDegrees,
									   Vec2f& newDirection );

	bool			IsTargetFlyer() const;

	void			HandleStopped();

	void			Accelerate( float accel, float decel, float speedLimit, bool bClamp );

	MoveSubState	ToStateLanded();
	MoveSubState	ToStateLiftOff();
	MoveSubState	ToStateFlyToRange();
	MoveSubState	ToStateLineUpTarget();
	MoveSubState	ToStateHoverTarget();
	MoveSubState	ToStateFallDead();
	MoveSubState	ToStateLanding();

	MoveSubState	HandleLanded( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleLiftOff( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleFlyToRange( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleHoverTarget( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleLineUpTarget( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleLanding( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleFallDead( const Vec2f& dirToTarget, float distToTargetSqr );
};
