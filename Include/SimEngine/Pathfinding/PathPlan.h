/////////////////////////////////////////////////////////////////////
// File    : PathPlan.h
// Desc    : 
// Created : Tuesday, February 5, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include "PathTypes.h"
#include "SimEngine/Target.h"

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class PathfinderQuery;
class PreciseTerrainMap;
class SimEntity;
class Target;


/////////////////////////////////////////////////////////////////////
//	Class	: PathPlan
//	Desc.	: 
//
class PathPlan
{
// Types
public:
	enum EndAction
	{
		EA_Invalid,
		EA_Recheck,
		EA_Stop,
		EA_CantReachTerrain,
		EA_CantReachBuilding,
		EA_CantReachEntity
	};

// Methods
public:

	PathPlan();				

	void					Reset();
	void					Initialize( const SimEntity* pe, const Target *T, float AP );

	inline EndAction		GetEndAction() const;
	inline const Vec2i&		GetGoalCell() const;

	inline float			GetOriginalAP() const;
	inline float			GetAdjustedAP() const;

	inline void				SetAdjustedAP( float AP );

	inline const Target*	GetCurrentTarget() const;
	inline const float		GetCurrentAP() const;

	void					AdjustGoal( const Vec2i& prCell,	EndAction ea );
	void					AdjustGoal( const Entity* pEntity, EndAction ea );
	void					SetEndAction( EndAction ea, bool bOnlyIfInvalid = true );

	void					GetOriginalTargetCell( Vec2i& goalCell ) const;
	void					GetAdjustedTargetCell( Vec2i& goalCell ) const;
	void					GetAdjustedTargetWorld( Vec2f& goalWorld ) const;

// Methods
private:

// Data
private:
	// values that don't change throughout a pathfind
	Vec2i			m_startCell;								// the start precise coord
	Vec2i			m_goalCell;									// the position of target at the time of initialization

	// values that may change during the course of a pathfind
//	Vec2i			m_goalCell;
	const Target*	m_originalTarget;
	const Target*	m_adjustedTarget;

	float			m_originalAP;	
	float			m_adjustedAP;

	EndAction		m_endAction;

	Target			m_dummyTarget;
};



/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline PathPlan::EndAction PathPlan::GetEndAction() const
{
	return m_endAction;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Vec2i& PathPlan::GetGoalCell() const
{
	return m_goalCell;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline float PathPlan::GetOriginalAP() const
{
	return m_adjustedAP;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline float PathPlan::GetAdjustedAP() const
{
	return m_originalAP;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Target* PathPlan::GetCurrentTarget() const
{
	return m_adjustedTarget;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const float PathPlan::GetCurrentAP() const
{
	return m_adjustedAP;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void	PathPlan::SetAdjustedAP( float AP )
{
	m_adjustedAP = AP;
}
