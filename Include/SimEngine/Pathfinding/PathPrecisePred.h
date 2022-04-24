/////////////////////////////////////////////////////////////////////
// File    : PathPrecisePred.cpp
// Desc    : 
// Created : July 15, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include "AstarBasePred.h"

#include "PathTypes.h"

#include <SimEngine/EntityUtil.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class PreciseTerrainMap;
class PathSectorMap;
class SparseGrid;
class GroundAttributes;
class Target;
class SimEntity;


/////////////////////////////////////////////////////////////////////
//	Class	: FindPathNormalPred
//
class FindPathNormalPred : public AstarBaseFindPathPred
{
public:
	FindPathNormalPred(	const PreciseTerrainMap* preciseMap, const PathSectorMap* psm, 
						const int nodeLimit,
						const GroundAttributes& attributes, 
						const Target* T,
						const float AP,
						const Vec2i& goalCell,
						const int targetMinX, const int targetMinY,
						const int targetWidth, const int targetHeight,
						const PathSubCellSet* allowedSectors, const bool bGetOffBuilding ) : 
		m_preciseMap( preciseMap ),
		m_pathSectorMap( psm ),
		m_nodeLimit( nodeLimit ),
		m_attributes( attributes ),
		m_T( T ),
		m_AP( AP ),
		m_goalCell( goalCell ),
		m_targetMinX( targetMinX ), m_targetMinY( targetMinY ),
		m_targetWidth( targetWidth ),m_targetHeight( targetHeight ),
		m_allowedSectors( allowedSectors ),
		m_bGetOffBuilding( bGetOffBuilding )
	{
		dbAssert( m_preciseMap );
		dbAssert( m_pathSectorMap );
	}

	virtual bool PassiblePosition( int x, int y, DirectionMap::Dir dir ) const;

	virtual bool TestExitCondition( int x, int y, int count, int numSearched ) const;

	virtual bool HitNodeLimit( int numNodes ) const;

private:
	const PreciseTerrainMap*	m_preciseMap;
	const PathSectorMap*		m_pathSectorMap;
	const int					m_nodeLimit;
	const Target*				m_T;
	const float					m_AP;
	const Vec2i&				m_goalCell;
	const int					m_targetMinX, m_targetMinY;
	const int					m_targetWidth, m_targetHeight;
	const GroundAttributes		m_attributes;
	const PathSubCellSet*		m_allowedSectors;
	const bool					m_bGetOffBuilding;
};

/////////////////////////////////////////////////////////////////////
//	Class	: FindPathSectorPred
//
class FindPathSectorPred : public AstarBaseFindPathPred
{
public:
	FindPathSectorPred(	const PreciseTerrainMap* preciseMap, const PathSectorMap* psm,
						const int nodeLimit,
						const GroundAttributes& attributes, 
						const PathSubCell goalSector,
						const PathSubCellSet* allowedSectors, const bool bGetOffBuilding ) : 
		m_preciseMap( preciseMap ),
		m_pathSectorMap( psm ),
		m_nodeLimit( nodeLimit ),
		m_attributes( attributes ),
		m_goalSector( goalSector ),
		m_allowedSectors( allowedSectors ),
		m_bGetOffBuilding( bGetOffBuilding )
	{
		dbAssert( m_preciseMap );
		dbAssert( m_pathSectorMap );
	}

	virtual bool PassiblePosition( int x, int y, DirectionMap::Dir dir ) const;

	virtual bool TestExitCondition( int x, int y, int nodeCount, int numSearched ) const;

	virtual bool HitNodeLimit( int numNodes ) const;

private:
	const PreciseTerrainMap*	m_preciseMap;
	const PathSectorMap*		m_pathSectorMap;
	const int					m_nodeLimit;
	const GroundAttributes		m_attributes;
	const PathSubCell			m_goalSector;
	const PathSubCellSet*		m_allowedSectors;
	const bool					m_bGetOffBuilding;
};

/////////////////////////////////////////////////////////////////////
//	Class	: FindPathSector2Pred
//
class FindPathSector2Pred : public AstarBaseFindPathPred
{
public:
	FindPathSector2Pred(const PreciseTerrainMap* preciseMap, const PathSectorMap* psm,
						const int nodeLimit,
						const GroundAttributes& attributes, 
						const PathSubCell oneGoalSector, const PathSubCell twoGoalSector,
						const PathSubCellSet* allowedSectors, const bool bGetOffBuilding ) : 
		m_preciseMap( preciseMap ),
		m_pathSectorMap( psm ),
		m_nodeLimit( nodeLimit ),
		m_attributes( attributes ),
		m_oneGoalSector( oneGoalSector ),
		m_twoGoalSector( twoGoalSector ),
		m_allowedSectors( allowedSectors ),
		m_bGetOffBuilding( bGetOffBuilding ),
		m_oneGoalCell( -1, -1 ),
		m_oneGoalCount( 0 )
	{
		dbAssert( m_preciseMap );
		dbAssert( m_pathSectorMap );
	}

	virtual bool PassiblePosition( int x, int y, DirectionMap::Dir dir ) const;

	virtual bool TestExitCondition( int x, int y, int nodeCount, int numSearched ) const;

	virtual bool ModifyEndCell( int& x, int& y, int& count ) const;

	virtual bool HitNodeLimit( int numNodes ) const;

private:
	const PreciseTerrainMap*	m_preciseMap;
	const PathSectorMap*		m_pathSectorMap;
		  int					m_nodeLimit;
	const GroundAttributes		m_attributes;
	const PathSubCell			m_oneGoalSector;
	const PathSubCell			m_twoGoalSector;
	const PathSubCellSet*		m_allowedSectors;
	const bool					m_bGetOffBuilding;
	mutable Vec2i				m_oneGoalCell;
	mutable	int					m_oneGoalCount;
};


/////////////////////////////////////////////////////////////////////
//	Class	: FindOpenPred
//
class FindOpenPred : public AstarBaseFindOpenPred
{
public:
	FindOpenPred( const PreciseTerrainMap* preciseMap, const PathSectorMap* psm, 
				  const GroundAttributes& attributes,
				  const Vec2i& minCell, const Vec2i& maxCell,
				  const Vec2i& refCell,
				  const PathMajorCell majorCell,
				  const bool bAmphibian ) :
		m_preciseMap( preciseMap ),
		m_pathSectorMap( psm ),
		m_attributes( attributes ),
		m_minCell( minCell ), m_maxCell( maxCell ),
		m_refCell( refCell ),
		m_majorCell( majorCell ),
		m_bAmphibian( bAmphibian )
	{
		dbAssert( m_preciseMap );
		dbAssert( m_pathSectorMap );
	}

	virtual bool TestExitCondition( int x, int y ) const;

	virtual float HeuristicCost( int x, int y ) const;

private:
	const PreciseTerrainMap*	m_preciseMap;
	const PathSectorMap*		m_pathSectorMap;
	const GroundAttributes		m_attributes;
	const Vec2i					m_minCell;
	const Vec2i					m_maxCell;
	const Vec2i					m_refCell;
	const PathMajorCell			m_majorCell;
	const bool					m_bAmphibian;
};



/////////////////////////////////////////////////////////////////////
//	Class	: FindValidPred
//
class FindValidPred : public AstarBaseFindValidPred
{
public:
	FindValidPred( const PreciseTerrainMap* preciseMap, const PathSectorMap* psm,
				   const SimEntity* pe,
				   const float AP,
				   const GroundAttributes& attributes, 
				   const Vec2i& startCell ) :
		m_preciseMap( preciseMap ),
		m_pathSectorMap( psm ),
		m_pe( pe ),
		m_AP( AP ),
		m_attributes( attributes ),
		m_startCell( startCell )
	{
		dbAssert( m_preciseMap );
		dbAssert( m_pathSectorMap );
	}

	virtual bool  IsMatch( int x, int y ) const;

	virtual float Dist( int x, int y ) const;

private:
	const PreciseTerrainMap*	m_preciseMap;
	const PathSectorMap*		m_pathSectorMap;
	const SimEntity*			m_pe;
	const float					m_AP;
	const GroundAttributes		m_attributes;
	const Vec2i					m_startCell;

};


/////////////////////////////////////////////////////////////////////
//	Class	: FindBestPred
//
class FindBestPred : public AstarBaseFindBestPred
{
public:
	FindBestPred( const PreciseTerrainMap* preciseMap, const PathSectorMap* psm, const SparseGrid* sparseGrid,
				  const int preciseSize, 
				  const PathMajorCell majorCell,
				  const bool bAmphibian,
				  const bool bUseEntities ) : 
		m_preciseMap	( preciseMap ),
		m_pathSectorMap	( psm ),
		m_sparseGrid	( sparseGrid ),
		m_preciseSize	( preciseSize ),
		m_majorCell		( majorCell ),
		m_bAmphibian	( bAmphibian ),
		m_bUseEntities	( bUseEntities )
	{
		dbAssert( m_preciseMap );
		dbAssert( m_pathSectorMap );
	}

	virtual bool TestExitCondition( int cellX, int cellY ) const;

private:
	const PreciseTerrainMap*	m_preciseMap;
	const PathSectorMap*		m_pathSectorMap;
	const SparseGrid*			m_sparseGrid;
	const int					m_preciseSize;
	const PathMajorCell			m_majorCell;
	const bool					m_bAmphibian;
	const bool					m_bUseEntities;
};
