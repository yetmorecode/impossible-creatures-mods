/////////////////////////////////////////////////////////////////////
// File    : PathfindingInternal.h
// Desc    : 
// Created : Wednesday June 26, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once


/////////////////////////////////////////////////////////////////////
//	Forward Declarations
//
class TerrainCellMap;
class PreciseTerrainMap;
class PathSectorMap;
class AstarImpass;
class SparseGrid;
class PathPrecise;
class PathHigh;


/////////////////////////////////////////////////////////////////////
//	PathfinderMaps
//
struct PathfinderMaps
{
	TerrainCellMap*				m_pTerrainMap;
	PreciseTerrainMap*			m_pPreciseMap;
	PathSectorMap*				m_pPathSectorMap;

	AstarImpass*				m_AstarImpass;
	SparseGrid*					m_sparseGrid;
	PathPrecise*				m_pathPrecise;

	PathHigh*					m_pathHigh;								// the AstarSector class handles the pa
};