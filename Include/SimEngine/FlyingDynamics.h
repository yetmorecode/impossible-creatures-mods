/////////////////////////////////////////////////////////////////////
// File    : FlyingDynamics.h
// Desc    : 
// Created : Sunday, September 16, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// * 

#pragma once

#include "EntityDynamics.h"
#include "SimExports.h"
#include "Target.h"

#include <Math/Vec2.h>


///////////////////////////////////////////////////////////////////// 
// FlyingDynamics 

class SIM_API FlyingDynamics : public EntityDynamics
{
// types
public:	
	enum
	{
		DynamicsID = DYNAMICSID_Flying,
	};

// construction
public:
	FlyingDynamics(SimEntity *e);

// statics
public:

	static void		SetFlyingPlaneHeight( float height );							// sets the height that flyers fly at
	static float	GetFlyingPlaneHeight();											// returns the height fliers fly at
	static void		SetStoppedSpeed( float speed );									// sets the speed fliers move at when stopped
	static float	GetStoppedSpeed();												// returns the speed fliers move at when stopped
	static void		SetAttackRate( long ticksPerAttack );							// sets the rate of attack

// inherited
public:

	// retrieve the ID of this dynamics
	virtual EntityDynamics::DynamicsIDType	GetDynamicsID( ) const;

	virtual void RequestMove(Target &T, float AP );
	virtual void RequestStop();
	virtual void RequestImmobilize();
	virtual void RequestDie();
	virtual bool RequestTeleport( const Vec2f& destination );

	virtual void StartAttack( long attackTicks, long coolTicks, bool bRange );
	virtual void StartCoolDown( long coolTicks );
	virtual void StartReaquire( long attackTicks, long coolTicks );

	virtual long GetTurnAroundTicks( long attackTicks, long coolTicks ) const;

	virtual void OnAddWorld();
	virtual void OnRemWorld();

	virtual EntityDynamics::eEDState QueryStatus() const;

	virtual bool IsInsideWorld( const Matrix43f* m ) const;								// is the given orientation inside the world

	virtual void DoStep(float speed);

	virtual bool				HasSoftRadius() const;
	virtual float				GetSoftRadius() const;

	virtual bool GetCellTopLeft( long& cellX, long& cellZ ) const;						// returns the top-left impass cell taken up by this Entity at it's current position
	virtual bool GetCellTopLeft( const Vec2f& pos, long& cellX, long& cellZ ) const;	// returns the top-left impass cell taken up by this Entity if it were are pos
	virtual bool GetCellTopLeft( const Vec2i& pos, long& cellX, long& cellZ ) const;
	virtual bool GetCellWidthHeight( long& width, long& height ) const;					// returns the width and height of the Entity in cell coords

	virtual float GetDistanceSqrToPoint( const Vec2f& pos, const Vec2f& targetPos2D, Vec2f* heading ) const;
	virtual float GetDistanceSqrToPoint(				   const Vec2f& targetPos2D, Vec2f* heading ) const;
	virtual float GetDistanceSqrToEntity( const Vec2f& pos, const SimEntity* targetEntity, Vec2f* heading ) const;
	virtual float GetDistanceSqrToEntity(					const SimEntity* targetEntity, Vec2f* heading ) const;

// types
private:

	enum MoveSubState
	{
		SS_FlyToRange, 
		SS_LineUpTarget,
		SS_Attack,
		SS_FollowThru,
		SS_CircleTarget,
		SS_TurnAround,
		SS_Immobilize,
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
	long			m_attackTicks;
	long			m_coolTicks;
	long			m_ticks;
	long			m_tiredTicks;

	MoveSubState	m_subState;
	float			m_speedLeft;

	bool			m_bNewRequest;			// will be true if there is new request to be processed
	Request			m_request;				// the actual request

	Vec2f			m_startDive;
	bool			m_bRangeAttack;
	float			m_turnAmount;

	// new
	float			m_turnRadius;
	Vec2f			m_turnForward;
	Vec2f			m_turnRight;
	Vec2f			m_turnCenter;
	// new

	bool			m_bImmobilize;

	static float	s_stoppedSpeed;			// the speed a flier should move at when stopped
	static float	s_flyingPlaneHeight;	// the height fliers fly at
	static long		s_totalAttackRate;		// should attack every this number of ticks

// implementation
private:
	// Private utility function that does the bulk of the movement work.
	//	Returns true if Entity is idle, returns false if entity is moving.
	float			GetHeight(float x, float z) const;
	float			GetTargetHeight() const;
	const Vec3f&	GetTargetPosition() const;
	void			DoStepInternal(float speed);

	float			GetSpeed( ) const;
	void			SetSpeed( float speed );

	void			SetTargetValidating( Target *T );
	void			SetCurrentPositionAsTarget();

	void			MoveEntity( const Vec3f& newPos );
	void			MoveEntity( const Matrix43f& newTransform );

	void			HandleNewRequests();
	void			ToDynamicsStateMove();
	void			ToDynamicsStateStop();
	void			ToDynamicsStateImmobilize();
	void			ToDynamicsStateAttack();
	void			ToDynamicsStateDie();

	void			ClampToWorld();

	MoveSubState	WithinRange();
	void			DoFlyWithTurn( const Vec2f& dirToTarget, float distToTarget, float turnDegrees );

	void			TurnTowardsTarget( const Vec2f& dirToTarget, float distToTargetSqr, float turnDegrees,
									   Vec2f& newDirection );

	bool			IsTargetFlyer() const;
	bool			UseDiveSpeed() const;
	float			GetTiredFactor() const;
	float			GetHeight_DiveBombApproach( const Vec2f& position2D );
	float			GetHeight_DiveBombFollowThru( const Vec2f& position2D );

	void			Accelerate( float accel, float decel, float speedLimit, bool bClamp );

	void			FlyToPoint( const Vec2f& entityPos2D, const Vec2f& currentDirection,
								const Vec2f& targetPos2D,
								float param, float minTurnDegrees, 
								Vec2f& newPos, Vec2f& newDirection );

	const char*		GetSubStateName() const;

	MoveSubState	ToStateAttack		();
	MoveSubState	ToStateFollowThru	();
	MoveSubState	ToStateFlyToRange	();
	MoveSubState	ToStateLineUpTarget	();
	MoveSubState	ToStateCircleTarget	();
	MoveSubState	ToStateTurnAround	();
	MoveSubState	ToStateImmobilize	();

	MoveSubState	HandleFlyToRange	( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleCircleTarget	( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleLineUpTarget	( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleAttack		( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleFollowThru	( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleFallDead		( );
	MoveSubState	HandleTurnAround	( const Vec2f& dirToTarget, float distToTargetSqr );
	MoveSubState	HandleImmobilize	( );
};
