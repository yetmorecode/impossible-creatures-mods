/////////////////////////////////////////////////////////////////////
// File    : Pathfinding.h
// Desc    : This file is the gateway to access the pathfinding.
//			 All communication with the pathfinding should go through this interface.
// Created : Thursday, December 06, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <Util/Statgraph.h>
#include "../SimExports.h"

#include "PathfindingInternal.h"
#include "PathTypes.h"
#include <SurfVol/Rec2.h>

#include <Assist/Array2D.h>


/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class PathfinderImpl;
class PathfinderQuery;
class IFF;
class Entity;
class EntityGroup;
class SimEntity;
class PathSectorMap;
class TerrainCellMap;
class PathPrecise;
class PathHigh;
class PathSector;
class SparseGrid;
class PreciseTerrainMap;
class TerrainHMBase;
class SimWorld;
class BrokenSectorConnections;
class ImpassEditArray;
class FindImpassGoallessPred;

/////////////////////////////////////////////////////////////////////
//	Class	: Pathfinder
//	Desc.	: The main interface through which all pathfinding calls are done.
//	Author	: dswinerd
//
class SIM_API Pathfinder
{
// Types
public:

	typedef std::vector<Entity*> BadEntityList;

public:

	Pathfinder();					// ctor
	~Pathfinder();					// dtor

	// returns the instance of the PathfinderQuery
	inline const PathfinderQuery*	Query() const;

	// called from the SimWorld::SetTerrain()
	void							SetTerrain( const TerrainHMBase* pTerrain );

	// save & load code
	bool							Save( IFF& iff, SimWorld *pSimWorld, const ImpassEditArray* impassEdit );
	bool							Load( IFF& iff, SimWorld* pWorld );
	void							AddLoadHandlers(IFF& iff);

	// has the pathfinding been loaded or generated
	bool							IsPathfindingGenerated() const;
	

	// accessor functions
	inline TerrainCellMap*			GetTerrainCellMap		();
	inline const TerrainCellMap*	GetTerrainCellMap		() const;

	inline const PreciseTerrainMap*	GetPreciseTerrainMap	() const;

	inline const PathSectorMap*		GetPathSectorMap		() const;

	inline SparseGrid*				GetSparseGrid			();
	inline const SparseGrid*		GetSparseGrid			() const;

	inline const PathPrecise*		GetPathPrecise			() const;
	inline const PathHigh*			GetPathHigh				() const;


	// sim frame functions
	void							SimulatePre( SimWorld* pWorld );	// call before the first sim frame
	void							FrameStart();						// call at the start of every sim frame
	void							FrameEnd();							// call at the end of every sim frame

	// add/remove a building in the TerrainCellMap
	void							AddBuildingImpass( SimEntity*, const TCMask pfCellMask );
	void							RemBuildingImpass( SimEntity*, const TCMask pfCellMask );

	// add/remove a building in the PreciseTerrainMap and the PathSectorMap
	void							AddBuilding( SimEntity* pSEntity );
	void							RemBuilding( SimEntity* pSEntity );

	// called to update info when the terrain is edited, the rect is the x,z extents of the edit operation
	void							OnTerrainModify( SimWorld* , const Rect2f&, const ImpassEditArray* impassEdit );

	// called to teleport a ground to a given location
	bool							TeleportGroup( const EntityGroup& eg, const Vec2f& destination );

	// goal-less pathfinder
	void							SearchImpassStart( const Vec2i& pos, const FindImpassGoallessPred& pred );
	bool							SearchImpassNext( Vec2i& nextPos, int& searched, const FindImpassGoallessPred& pred );

	// lock a position so no entity will be able to step there
	PathLockHandle					LockPosition( const Vec2f& position, const GroundAttributes& attributes );
	PathLockHandle					LockPosition( const Vec2i& posCell,  const GroundAttributes& attributes );
	bool							UnlockPosition( PathLockHandle& lockHandle );

	bool							UnapplyLock( PathLockHandle lockHandle );
	bool							ApplyLock( PathLockHandle lockHandle, Vec2i& position );

	//
	bool							GetPathfindingLoad() const;

	// Pathfinding validation functions
	const BadEntityList&			GetBadEntities() const;

	// debugging
	void							FillSparseData( std::vector<unsigned char>& data ) const;

	size_t							GetMemUsage() const;

	// used by the MissionEditor
	void							Regenerate( SimWorld *pSimWorld, const ImpassEditArray* impassEdit );

private:

	// call to regenerate pathfinding, after it has already been done
	void	RegeneratePathfinding( SimWorld* pSimWorld, const ImpassEditArray* impassEdit );

	// call to generate the pathfinding initially
	void	GeneratePathfinding( SimWorld*, const ImpassEditArray* );

	// introduces nature objects and buildings into the pathfinding
	void	AddNatureObjectsAndBuildings( SimWorld* );

	// called after pathfinding has been loaded or generated to set things up
	void	PostGenerateOrLoad( SimWorld* );

	// call to recalculate the impassmap for a rectangle
	void	MarkImpassability( const TerrainHMBase* , const Rect2f& );

	// marks nature objects into the impassmap
	void	MarkStaticEntities( SimWorld* );

//	int		HeuristicCost( int startx, int startz, int goalx, int goalz );
//	int		RealCost( int x, int z );

	// Pathfinding validation functions
	void	StartRecordingBadEntities();
	void	EndRecordingBadEntities();
	void	RecordBadEntity( SimEntity* pSEntity );

// Types
private:

	PathfinderMaps		m_maps;
	PathfinderImpl*		m_pimpl;				// the private implementation
	PathfinderQuery*	m_pQuery;				// the query functions
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the PathfinderQuery object
//
inline const PathfinderQuery* Pathfinder::Query() const
{
	return m_pQuery;
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the TerrainCellMap object
//
inline TerrainCellMap* Pathfinder::GetTerrainCellMap() 
{ 
	return m_maps.m_pTerrainMap; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the TerrainCellMap object (const)
//
inline const TerrainCellMap* Pathfinder::GetTerrainCellMap() const 
{ 
	return m_maps.m_pTerrainMap; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the PreciseTerrainMap
//
inline const PreciseTerrainMap* Pathfinder::GetPreciseTerrainMap() const
{ 
	return m_maps.m_pPreciseMap; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the PathSectorMap
//
inline const PathSectorMap* Pathfinder::GetPathSectorMap() const 
{
	return m_maps.m_pPathSectorMap; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the SparseGrid
//
inline SparseGrid* Pathfinder::GetSparseGrid()
{ 
	return m_maps.m_sparseGrid; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the SparseGrid (const)
//
inline const SparseGrid* Pathfinder::GetSparseGrid() const
{ 
	return m_maps.m_sparseGrid; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the PathPrecise object
//
inline const PathPrecise* Pathfinder::GetPathPrecise() const 
{
	return m_maps.m_pathPrecise; 
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the PathHigh object
//
inline const PathHigh* Pathfinder::GetPathHigh() const 
{
	return m_maps.m_pathHigh; 
}
