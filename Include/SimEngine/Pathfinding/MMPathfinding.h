// Pathfinding.h

// This file is the gateway to access the pathfinding.
//	All communication with the pathfinding should go through this interface.

#include "../SimExports.h"

#pragma once

#include "MipMapPath.h"	// for MipMapTerrainCellMap and MMAstar

// Forward Declarations.
class Entity;
class Target;

// The pathfinder.
class SIM_API MMPathfinder
{
public:
	MMPathfinder(MipMapTerrainCellMap *tcMap);
	~MMPathfinder();
	
	bool FindPath(std::vector<Vec2f> &path, SimEntity *e, Target *T,float AP);

	MipMapTerrainCellMap *GetMMTerrainCellMap() { return m_terrainmap; }
	const MipMapTerrainCellMap *GetMMTerrainCellMap() const { return m_terrainmap; }
	

private:

	MMPathfinder();
	MipMapTerrainCellMap*	m_terrainmap;
	MMAstar m_Astar;

};

