// Astar.h

#pragma once

#include "../SimExports.h"

#include "../SimEntity.h"
#include "ImpassMap.h"	// for terraincellmap.
#include "DirectionMap.h"

//Do we need this anymore?
#include "BitArray.h"	// for Markingbitfield.


// FORWARD DECLARATIONS
class TerrainHMBase;
class Entity;
class Target;


// A node in the path.
struct PathNode
{
	// unsigned?
	int x, y;

//	int			x, z;		// position of this node.
//	int			gval;       // g in A* represents how far we've already gone
//	int			hval;       // h in A* represents an estimate of how far is left
	float		gval;       // g in A* represents how far we've already gone
	float		hval;       // h in A* represents an estimate of how far is left
};

typedef std::priority_queue<PathNode> Container;

// Astar pathfinding.

// Inputs:
//	start, end, 



// The pathfinder.
class Astar
{
// Functions.
public:
	Astar( TerrainCellMap *tcMap );
	~Astar();
	
//	bool Astar::FindPath(std::vector<Vec2i> &path, const Vec2i &start, const Vec2i &goal, float APc);
	bool Astar::FindPath(std::vector<Vec2i> &path, SimEntity *pe, Target *T, float APc, SimWorld *simWorld, const TCMask &passTypes, size_t maxNodes ); //, TerrainCellMap * terrainmap );

	// This is a crap way to do this, but oh well.
//	void SetMap(TerrainCellMap *tcMap) { m_tcMap = tcMap; }
//	void SetBitfield(MarkingBitfield *marking) { m_marking = marking; }

private:

	float HeuristicCost( int startx, int startz, int goalx, int goalz );
//	float RealCost( int x, int z );


// Data.
public:

private:


	TerrainCellMap	*m_tcMap;		// Grid to A* pathfind over. (also stores path traversed inside each node)
	Container		m_open;			// Open list.
	MarkingBitfield	m_marking;	// A bitfield map to mark already visited nodes (closed list).

	DirectionMap	m_dirMap;

//	std::vector<Vec2i> m_FoundPath;


//	std::vector<Vec2f> m_FoundPath;

};


