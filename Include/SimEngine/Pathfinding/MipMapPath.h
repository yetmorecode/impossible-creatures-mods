/////////////////////////////////////////////////////////////////////
// File    : MipMapPath.h
// Desc    : 
// Created : Friday, November 16, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h"

#include "ImpassMap.h"	// for terraincellmap.
#include "BitArray.h"	// for Markingbitfield.

#include <Assist/Array2d.h>

//---------------------------------------------------------------------------------------------------
// MipMapTerrainCellMap:
//
//   Mipmapped version of the terrain cell map. This allows for a hierarchial check of this map for
//	 a quicker way to check if an area is occupied or is open. I don't think this works well if the
//	 world is sparsely occupied vs having lots of chunks of impassible objects. A dynamic quad tree
//   that uses memory pools of nodes 'could' be more efficient and take up less memory if the world 
//	 isn't completely occupied.
//
//---------------------------------------------------------------------------------------------------

class SIM_API MipMapTerrainCellMap
{
// constructor/ destructor
public:
	MipMapTerrainCellMap( const int width, const int height );
	~MipMapTerrainCellMap();

public:

	typedef unsigned char MMCellType;

	// only used to set the top level mipmap
	enum 
	{
		MM_passible,
		MM_impassible,
		MM_mixed,
	};

	// sets the cell type of a specified x,z location on the map
	void			SetCellType( const int x, const int z, MMCellType type );
	// gets the cell type at the givel mipmap level (default is the top level)
	MMCellType		GetCellType( const int x, const int z, int level = 0) const;
	// 
	inline int		GetWidth() const {return m_width;}
	//
	inline int		GetHeight() const {return m_height;}
	//
	inline float	GetCellSize() const {return m_cellsize;}
	inline void		SetCellSize( float size ) {m_cellsize = size;}

	int				WorldXToCellX( float Xpos ) const;
	int				WorldZToCellZ( float Zpos ) const;

	float			CellXToWorldX( int Xpos ) const;
	float			CellZToWorldZ( int Zpos ) const;
	
	void			FindHighPassibleNode( const Vec2i& pt, Vec2i& newPt, int &newlevel );

	void			Dump();
private:
	
	int		m_width;
	int		m_height;
	float	m_cellsize;
	int		m_numlevels;

	// definition of a mipmap
	typedef Array2D<unsigned char> MipMap;
	
	// list of mipmaps
	std::vector<MipMap>	m_mmarray;
};

#if 1

// A node in the path.
struct MMPathNode
{
	int			x, y;		// position of this node - could be two bytes each
	int			level;		// the level of this node - could be one byte
	float		gval;       // g in A* represents how far we've already gone
	float		hval;       // h in A* represents an estimate of how far is left
	MMPathNode* FromNode;		// where this node came from (if its in the open list)
};


class NodeTotalGreater 
{
public:
   //This is required for STL to sort the priority queue
   //(its entered as an argument in the STL heap functions)
   bool operator()( MMPathNode * first, MMPathNode * second ) const 
   {
      return (first->gval+first->hval) > (second->gval+second->hval);
   }
};

typedef std::priority_queue<MMPathNode*, std::vector<MMPathNode*>, NodeTotalGreater >	MMContainer;
typedef std::list<MMPathNode*>					PathSearchList;

// eventually this can be made to allocate and deallocate from a memory pool

class MMPathNodeAllocator
{
public:
	static MMPathNode*	add() {return new MMPathNode;}
	static void			remove(MMPathNode* n) {delete n;}
};


// FORWARD DECLARATIONS
class TerrainHMBase;
class Entity;
class Target;


// Astar pathfinding.

// Inputs:
//	start, end, 

// The pathfinder.
class MMAstar
{
// Functions.
public:
	MMAstar(){}
	~MMAstar(){}
	
	bool FindPath(std::vector<Vec2i> &path, SimEntity *pe, Target *T, float APc, MipMapTerrainCellMap * terrainmap );

private:

	MMPathNode* newpathnode( int x, int y, int level, const Vec2i& goal, MMPathNode* from );

	float	HeuristicCost( int startx, int startz, int goalx, int goalz );
//	float RealCost( int x, int z );

	void	AddOneSide( int x, int y, int level, int side, MMContainer& openlist, 
						MipMapTerrainCellMap *terrainmap, MMPathNode* FromNode,
						const Vec2i& goal);

	void	AddEastNeighbours( int x, int y, int level, MMContainer& openlist, 
								MipMapTerrainCellMap *terrainmap, MMPathNode* FromNode,
								const Vec2i& goal);
	void	AddWestNeighbours( int x, int y, int level, MMContainer& openlist, 
								MipMapTerrainCellMap *terrainmap, MMPathNode* FromNode,
								const Vec2i& goal);
	void	AddSouthNeighbours( int x, int y, int level, MMContainer& openlist, 
								MipMapTerrainCellMap *terrainmap, MMPathNode* FromNode,
								const Vec2i& goal);
	void	AddNorthNeighbours( int x, int y, int level, MMContainer& openlist, 
								MipMapTerrainCellMap *terrainmap, MMPathNode* FromNode,
								const Vec2i& goal);

	void	AddNeighbours( int x, int y, int level, MMContainer& openlist, MipMapTerrainCellMap *terrainmap,
							MMPathNode* FromNode, const Vec2i& goal );

// Data.
public:

private:


	// Grid to A* pathfind over. (also stores path traversed inside each node)
	//MarkingBitfield*		m_marking;	// A bitfield map to mark already visited nodes (closed list).
	//(NodeTotalGreater);		// Open list.
//	std::vector<Vec2i> m_FoundPath;


//	std::vector<Vec2f> m_FoundPath;

};





#endif // if 0
