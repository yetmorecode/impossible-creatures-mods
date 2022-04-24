/////////////////////////////////////////////////////////////////////
// File    : AstarImpassPred.h
// Desc    : 
// Created : July 15, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h"

#include "AstarBasePred.h"

#include "PathTypes.h"


/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class TerrainCellMap;
class SimEntity;
class Target;


/////////////////////////////////////////////////////////////////////
//	Class	: FindPathNormalPred
//
class FindImpassPathPred : public AstarBaseFindPathPred
{
public:
	FindImpassPathPred(	const TerrainCellMap* tcMap,
						int nodeLimit,
						SimEntity *pe, const Target *T, float APc, const TCMask passTypes,
						const Vec2i& goalCell ) :
		m_tcMap( tcMap ),
		m_nodeLimit( nodeLimit ),
		m_pe( pe ),
		m_T( T ),
		m_APc( APc ),
		m_passTypes( passTypes ),
		m_goalCell( goalCell )
	{
		dbAssert( m_tcMap );
	}

	virtual bool PassiblePosition( int x, int y, DirectionMap::Dir dir ) const;

	virtual bool TestExitCondition( int x, int y, int count, int numSearched ) const;

	virtual bool HitNodeLimit( int numNodes ) const;

private:

	bool WithinDistanceOfGoal( int x, int y ) const;


private:

	const TerrainCellMap*	m_tcMap;
	const int				m_nodeLimit;
	const Vec2i				m_goalCell;
	const SimEntity*		m_pe;
	const Target*			m_T;
	const float				m_APc;
	const TCMask			m_passTypes;
};



/////////////////////////////////////////////////////////////////////
//	Class	: FindImpassOpenPred
//
class FindImpassOpenPred : public AstarBaseFindOpenPred
{
public:

	FindImpassOpenPred( const TCMask passTypes, const Vec2i& minCell, const Vec2i& maxCell, const Vec2i& refCell ) : 
		m_passTypes( passTypes ),
		m_minCell( minCell ),
		m_maxCell( maxCell ),
		m_refCell( refCell )
	{;}

	virtual bool TestExitCondition( int x, int y ) const;

	virtual float HeuristicCost( int x, int y ) const;

private:

	const TCMask	m_passTypes;
	const Vec2i		m_minCell;
	const Vec2i		m_maxCell;
	const Vec2i		m_refCell;
};

/////////////////////////////////////////////////////////////////////
//	Class	: FindImpassOpenPred
//
class SIM_API FindImpassGoallessPred : public AstarBaseFindBestPred
{
public:

	FindImpassGoallessPred( const TCMask passTypes ) : 
		m_passTypes( passTypes )
	{;}

	virtual bool TestExitCondition( int x, int y ) const;

private:

	const TCMask	m_passTypes;
};
