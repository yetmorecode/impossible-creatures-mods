/////////////////////////////////////////////////////////////////////
// File    : PathHigh.h
// Desc    : 
//			 
// Created : Monday, January 28, 2002
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h"
#include "PathTypes.h"
#include "AstarSector.h"


/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class Player;
class BrokenSectorConnections;
class PathSectorMap;
class PreciseTerrainMap;
class SparseGrid;
class PathCacheHigh;
class SimHighPath;

/////////////////////////////////////////////////////////////////////
//	Class	: PathPrecise
//
class SIM_API PathHigh: public AstarSector
{
public:	
	PathHigh( AstarPool<AstarSectorNode> *pPool, const PathSectorMap* psm, const PreciseTerrainMap* ptm, const SparseGrid* sparseGrid );
	~PathHigh();

	void BlockSector( PathSubCell sector );
	void ClearBlockedSectors();

	bool FindPath( SimHighPath& path, AstarSectorPathResult& pathResult, Vec2i& bestRangeCell,
				   const Vec2i& startGrid, const Vec2i& goalGrid, 
				   float AP, const Player* player, int entitySize, bool bAmphibian, bool bToBuilding, bool bNextToBuilding, const BrokenSectorConnections* brokenConnections );

	bool FindBestCellWithinAP( const PathSubCell sector, const Vec2i& centerCell,  const Vec2i& refCell, float AP, int entitySize, bool bUseEntities, Vec2i& bestCell ) const;
	bool AreAnyCellsReachableWithinAP( const PathSubCell sector, const Vec2i& centerCell, float AP, int entitySize, bool bUseEntities ) const;

	void ResetNumReused();
	int  GetNumReused() const;

// Pure Virtual overrides from AstarSector
protected:

	virtual const PathSector *	GetPathSector( PathSubCell sector ) const;
	virtual float				HeuristicCost( PathSubCell sector ) const;
	virtual float				ActualCost( const PathSector* pCurrent, const PathSector* pNext, const unsigned short adjSize ) const;
	virtual bool				CanTraverse( int adjSize, PathSubCell hitherSector, PathSubCell yonSector ) const;

// Virtual overrides from AstarSector
public:

	virtual size_t				GetMemUsage() const;

protected:

	virtual void DoFindPathInitialize() ;
	virtual bool TestExitCondition( PathSubCell sector ) const;
	virtual bool PassibleSector( const PathSector* pSector ) const;


private:
	PathHigh();				// hide default ctor

// Data
private:
	
	const PathSectorMap*			m_psm;
	const PreciseTerrainMap*		m_preciseMap;
	const SparseGrid*				m_sparseGrid;

	std::vector<PathSubCell>		m_blockedSectors;					// a vector of blocked sectors that we won't consider in the Pathfind

	PathCacheHigh*					m_pathCache;
	int								m_numReused;

	// pathfinding parameters
	Vec2i							m_startCell;						// precise coord
	Vec2i							m_goalCell;							// precise coord
	Vec2i							m_tcStartCell;						// impass coord
	Vec2i							m_tcGoalCell;						// impass coord
	PathSubCell						m_startSector;
	PathSubCell						m_goalSector;
	float							m_AP;
	int								m_entitySize;
	bool							m_bAmphibian;
	bool							m_bToBuilding;
	bool							m_bNextToBuilding;
	const Player*					m_pPlayer;
	const BrokenSectorConnections*	m_brokenConnections;

	mutable Vec2i					m_bestRangeCell;
};