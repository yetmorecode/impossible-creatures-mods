/////////////////////////////////////////////////////////////////////
// File    : PathfinderImpl.h
// Desc    : This file contains the private implementation of the Pathfinder
// Created : Friday, January 25, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include <Util/Statgraph.h>

#include "AstarPool.h"
#include "AstarBase.h"
#include "AstarGridNode.h"
#include "AstarSectorNode.h"

#include "PathfindingInternal.h"

#include "ImpassMap.h"
#include "PreciseTerrainMap.h"
#include "PathSectorMap.h"

#include "PathPrecise.h"
#include "PathHigh.h"
#include "SparseGrid.h"

#include "AstarImpass.h"
//#include "AstarLowLevel.h"


/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class Entity;
class Target;
class Hull2d;
class PathSectorMap;
class AstarPool;
class AstarImpass;
//class AstarLowLevel;
class PathPrecise;
class PathSector;
class SparseGrid;
class PreciseTerrainMap;
class TerrainHMBase;
class SimWorld;
class BrokenSectorConnections;
class TerrainCellMap;
class MarkingBitfield;
class QuadNode;

/////////////////////////////////////////////////////////////////////
//	Class	: PathfinderImpl
//	Desc.	: the private implementation of the Pathfinder
//	Author	: dswinerd
//
class PathfinderImpl
{
	friend class Pathfinder;
	friend class PathfinderQuery;

// Methods
private:
	PathfinderImpl( PathfinderMaps* pMaps );
	~PathfinderImpl();

	static void* operator new   ( size_t );										// operator new
	static void  operator delete( void* );										// operator delete	

// Data
private:

	bool						m_bPathfindingValid;
	bool						m_bPreFirstSimulate;

	PathfinderMaps*				m_maps;									// weak pointer

/*
	// now in PathfinderMaps
	TerrainCellMap*				m_pTerrainMap;
	PreciseTerrainMap*			m_pPreciseMap;
	PathSectorMap*				m_pPathSectorMap;

	AstarImpass*				m_AstarImpass;
	SparseGrid*					m_sparseGrid;
	PathPrecise*				m_pathPrecise;
	PathHigh*					m_pathHigh;								// the AstarSector class handles the pa
*/

	AstarPool<AstarGridNode>*	m_AstarGridPool;
	AstarPool<AstarSectorNode>* m_AstarSectorPool;

	// lock variables
	std::set<PathLockHandle>	m_pathLocks;

	// Pathfinding validation variables
	bool						m_bRecordBadEntities;
	std::vector<Entity*>		m_badEntities;	// FIX: should be an EntityGroup?
	EntityGroup					m_batchedBuildings;

	// statgraph
	StatGraph::Handle	m_sgGridSearch;
	StatGraph::Handle	m_sgSectorSearch;
	StatGraph::Handle	m_sgMemUsage;
	StatGraph::Handle	m_sgHighReused;
};