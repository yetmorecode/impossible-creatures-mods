/////////////////////////////////////////////////////////////////////
// File    : PathPrecise.h
// Desc    : 
// Created : Wednesday, December 12, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "AstarBase.h"

#include <SimEngine/Pathfinding/PathTypes.h>

#include <Assist/Array2D.h>

#include <Math/Vec2.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations
//
class SimEntity;
class Target;
class SparseGrid;
class GroundDynamics;
class PreciseTerrainMap;
class PathSectorMap;
class SparseGrid;
class SimPath;
class SimHighPath;

/////////////////////////////////////////////////////////////////////
//	Class	: PathPrecise
//
class PathPrecise : public AstarBase
{
public:
	 PathPrecise( AstarPool<AstarGridNode> *pPool, PathSectorMap *psm, int preciseWidth, int preciseHeight );
	~PathPrecise();

	void Initialize( const PreciseTerrainMap *preciseMap, const SparseGrid *sparseGrid );

	bool FindPath( SimPath &path, AstarPathfindingResult& pathResult, const Vec2i& startCell, const Vec2i& goalCell, SimEntity *pe, const Target *T, float AP, const TCMask &pfCellMask, bool bGetOffBuilding );
	bool FindPathToSector( SimPath &path, AstarPathfindingResult& pathResult, const Vec2i& startCell, const Vec2i& goalCell, PathSubCell goalSector, const PathSubCellSet *allowedSectors, SimEntity *pe, const TCMask &pfCellMask );
	bool FindFirstOpen(	Vec2i& firstOpen, 
						const Vec2i& refCell, const Vec2i& goalCell, 
						const GroundAttributes& attributes, PathMajorCell majorCell, bool bAmphibian, bool bUseEntities );
	bool FindFirstOpen(	Vec2i& firstOpen, 
						const Vec2i& refCell, const Vec2i& goalCell, const Vec2i& minCell, const Vec2i& maxCell,
						const GroundAttributes& attributes, PathMajorCell majorCell, bool bAmphibian, bool bUseEntities );

	bool FindFirstOpenPred( Vec2i& firstOpen, 
							const Vec2i& refCell, const Vec2i& targetCell, 
							const AstarBaseFindOpenPred& pred );

	bool FindPathSimple( SimPath &path, AstarPathfindingResult& pathResult, const Vec2f& startPos, const Vec2f& goalPos, int entitySize, const TCMask& pfCellMask, int maxNodes = 0 );

	// new
	bool FindPathToSector2( SimPath& path, AstarPathfindingResult& pathResult,
							 const Vec2i& startCell, /*const Vec2i& goalCell,*/
							 const SimHighPath& highPath,
							 const PathSubCellSet* allowedSectors,
							 SimEntity* pe, const TCMask& pfCellMask );
	// new

	bool FindValidWithinRange( Vec2i& firstOpen, const Vec2i& start, const Vec2i& goal, SimEntity *pe, float AP, const TCMask& pfCellMask );

	bool PossiblePathCheck( const Vec2i& startCell, const Vec2i& goalCell, int preciseSize, bool bAmphibian, bool bUseEntities, Vec2i& bestReachable );

	bool GetReachableConnections( const Vec2i& startCell, int preciseSize, bool bAmphibian, PathSubCellSet& reachables );

	// debugging
	void DumpSparse( const char *filename ) const;

// AstarBase overrides
public:

	virtual size_t	GetMemUsage() const;	

private:

	void ConvertGridPathToWorldPath( const AstarBase::GridPath& gridPath, SimPath& worldPath, const GroundAttributes& attributes, unsigned long ignoreFlags = 0 ) const;
	
private:

	PathSectorMap*		m_pathSectorMap;		// non-owning pointer

	const PreciseTerrainMap *m_preciseMap;
	const SparseGrid		*m_sparseGrid;

	AstarBase::GridPath		m_gridPath;

#if 0
	// temporary pathfinding variables
	SimEntity*				m_pe;				// only used during the scope of a FindPath call so it's ok to store
	const Target*			m_T;
	//TCMask					m_pfCellMask;
	Vec2i					m_startCell;
	Vec2i					m_goalCell;
	//int						m_entitySize;
	//int						m_groundLevel;
	bool					m_bGetOffBuilding;
	float					m_AP;
	const PathSubCellSet*	m_allowedSectors;
	PathMajorCell			m_majorCell;
	bool					m_bAmphibian;
	bool					m_bUseEntities;
	GroundAttributes		m_attributes;

	long m_targetMinX, m_targetMinY;
	long m_targetWidth, m_targetHeight;

	enum SearchType { eNormalSearch, eToSectorSearch };
	SearchType m_searchType;

	PathSubCell m_goalSector;
#endif
};