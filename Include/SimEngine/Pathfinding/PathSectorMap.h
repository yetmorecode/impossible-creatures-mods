/////////////////////////////////////////////////////////////////////
// File    : PathSectorMap.h
// Desc    : 
//
// Created : November 2001
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h"
#include "PathTypes.h"


/////////////////////////////////////////////////////////////////////
//	Forward Declarations
//
class PathSectorMapImpl;
class PathSectorGen;
class PreciseTerrainMap;
class TerrainCellMap;
class PathSector;
class BiFF;
class IFF;
class BrokenSectorConnections;
class AstarSectorPathResult;
class SimEntity;
class EntityGroup;

/////////////////////////////////////////////////////////////////////
//	Class	: PathSectorMap
//
class SIM_API PathSectorMap
{
// Types
 public:
	typedef std::vector< PathSubCell > SectorPath;								// the type of path FindHighPath returns
	typedef std::vector< const PathSector* > SectorVector;

// Methods
public:
	PathSectorMap();															// ctor
	~PathSectorMap();															// dtor

	static void* operator new   ( size_t );										// operator new
	static void  operator delete( void* );										// operator delete

	// --- save/load ----
	bool					IsLoaded		();									// returns true if the map has been loaded
	bool					Save			(IFF &iff) const;					// save the PathSectorMap
	void					AddLoadHandler	( IFF& iff );						//

	// --- methods that modify the sector map ---
	void					SetMaps			( TerrainCellMap *tcMap, PreciseTerrainMap *preciseMap );	// sets the TerrainCellMap that will be used by the generation
	void					Generate		( TerrainCellMap* tcMap, PreciseTerrainMap* preciseMap);	// generates the sectors

	bool					AddBuilding		( SimEntity* pEntity, long cellX, long cellZ, long width, long height );	// add a building to the map
	bool					RemoveBuilding	( SimEntity* pEntity, long cellX, long cellZ, long width, long height );	// remove a building from the map

	bool					AddBatchedBuildings( const EntityGroup& batchedBuildings);

	// --- methods that query the sector map ---
	const int				GetWidth		() const;								// 
	const int				GetHeight		() const;								// 
	const size_t			GetNumSectors	() const;								//

	const PathSubCell		GetCell			( int x, int y ) const;					// given the grid coord, returns the PathSubCell
	const PathSubCell		GetCell			( const Vec3f& pos ) const;				// given the 3D world coord, returns the PathSubCell
	const PathSubCell		GetCell			( const Vec2f& pos ) const;				// given the 2D world coord, returns the PathSubCell

	const PathMajorCell		GetLandMajorCell( int x, int y ) const;					// given the grid coord, returns the PathMajorCell for land/water
	const PathMajorCell		GetAmphMajorCell( int x, int y ) const;					// given the grid coord, returns the PathMajorCell for amphibian
	const PathMajorCell		GetMajorCell	( int x, int y, bool bAmphibian ) const;//

	bool					GetSectorCenter	( const PathSubCell& cell, Vec2i& center ) const;	// given a cell, returns the center of the sector
	bool					GetAdjacentSectors( const PathSubCell& cell, std::vector<PathSubCell>& adjacent ) const;	// returns the sectors adjacent to the given cell
	
	bool					IsSectorValid	( PathSubCell sector ) const;						// detemines if the given sector is valid

	const	PathSector*		GetPathSector	( PathSubCell cell ) const;				// given a Cell return the associated PathSector* (const)
	const	PathSector*		GetPathSector	( int x, int z ) const;					// given the coords into the map, return the associated PathSector* (const)

	const std::vector<PathSector*>			GetAllPathSectors() const;				// returns the pathSectorArray

	// --- utility methods ---
	void	CellToWorld( const Vec2i& cell, Vec2f& world) const;				// given cell coords, convert to world coords
	void	WorldToCell( const Vec2f& world, Vec2i& cell) const;				// given world coords, convert to cell coords

	float	CellXToWorldX( int cellX ) const;									// convert cell X to world X
	float	CellZToWorldZ( int cellZ ) const;									// convert cell Z to world Z

	int		WorldXToCellX( float worldX ) const;								// convert world X to cell X
	int		WorldZToCellZ( float worldZ ) const;								// convert world Z to cell Z

	// --- Pathfinding methods ----
	bool	TestSubSectorConnectivity( PathSubCell hitherSubSector, PathSubCell yonSubSector, bool bAmphibian ) const;			// tests if the two sub sectors are connected

	bool	PossiblePathCheck(	const Vec2i& startCell, const Vec2i& goalCell, bool bAmphibian,
								Vec2i& bestReachable );

	bool	FindValidDestination( const int hitherX, const int hitherZ, 
								  const int yonX, const int yonZ,
								  bool bAmphibian,
								  int& resultX, int& resultZ ) const;

	bool	IsReachableFrom( const Vec2f& start, const Vec2f& goal, bool bAmphibian ) const;

	bool	IsSectorWithinAP( const Vec2f& center, float AP, const PathSector *pSector ) const;
	bool	IsSectorWithinAP( const Vec2f& center, float AP, PathSubCell sector ) const;

	void	FindSectorsWithinAP( const Vec2f& center, float AP, SectorVector& sectors ) const;

	// --- debugging methods ---
	void	DebugVisualize() const;													// debug drawing
	size_t	GetMemUsage() const;

// Data
private:

	PathSectorMapImpl* m_pimpl;
	PathSectorGen* m_gen;
};
