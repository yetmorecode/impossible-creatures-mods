/////////////////////////////////////////////////////////////////////
// File    : PathSectorGen.h
// Desc    : This is the private implementation class of PathSectorMap.  
//				It is responsible for initial generation of the PathSectorMap and regeneration of it
//				when buildings are added and removed.
// Created : November, 2001
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "PathTypes.h"
#include "TerrainSectorMap.h"

#include "FloodFill.h"

#include <SimEngine/SyncDefines.h>

#if defined( EXTREME_SYNC_LOGGING )
	#include <Util/LogFile.h>
#endif

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class PathSector;
class PathSectorAdj;
class PathSectorMapImpl;
class TerrainSectorMap;
class TerrainCellMap;
class PreciseTerrainMap;
class RebuildSubSectorFloodFill;
class SimEntity;
class EntityGroup;

/////////////////////////////////////////////////////////////////////
//	Class	: PathSectorGen
//	Desc	: This is class is used internally by PathSectorMap to perform all 
//				Sector generation and regeneration.  
//				This code was moved here out of PathSectorMap.cpp because it was getting ornery.
//	Author	: dswinerd
//
class PathSectorGen
{
// Methods that will be called from PathSectorMap
public:

	PathSectorGen( PathSectorMapImpl *pimpl );
	~PathSectorGen();

	void		SetMaps				( TerrainCellMap *tcMap, PreciseTerrainMap *preciseMap );

	void		Generate			( /*const TerrainCellMap *tcMap*/ );
	bool		RemoveBuilding		( SimEntity* pSimEntity, long cellX, long cellZ, long width, long height );
	bool		AddBuilding			( SimEntity* pSimEntity, long cellX, long cellZ, long width, long height );

	bool		AddBatchedBuildings	( const EntityGroup& buildingList );

	static void* operator new   ( size_t );										// operator new
	static void  operator delete( void* );										// operator delete

// Types
public:

	class PathSubCellPredicate
	{
	public:
		virtual bool Test( PathSubCell cell ) const = 0;
	};

	class RebuildPred
	{
	public:
		RebuildPred() {;}
		virtual ~RebuildPred() {;}

		virtual void Do( PathSector* pPathSector, PathSubCell thisCell, int x, int y ) = 0;
	};


// Types
private:

	typedef std::vector<PathSubCell> PathSubCellList;


// Methods used internally
private:

	void		GenerateSubSectors( TerrainSectorMap *tsMap, std::vector<PathSector*>& newSectors );
	void		ComputeWholeMapAdjacency();

	unsigned char ConnectivityFloodFill( const Vec2i& tcMin, const Vec2i& tcMax, 
										 int preciseSize, unsigned char sectorToReplace, unsigned char writeValue,
										 Array2D<unsigned char>& sectorBuffer,
										 Array2D<unsigned char>& preciseBuffer/*fillBuffer*/ );

	int			TestInternalConnectivity(  const Vec2i& min, const Vec2i& max, /*int preciseSize, unsigned char sectorToReplace, unsigned char writeValue,*/
										   Array2D<unsigned char>& sectorBuffer,
										   Array2D<unsigned char>& preciseBuffer,
										   std::set<unsigned char>& calculatedSectors
										   /*int& newSectorMin, int &newSectorMax*/ );

	void		MergeForConnectivity( unsigned char sectorToReplace, unsigned char exMin, unsigned char exMax, Array2D<unsigned char>& tcBuffer ) const;
	void		SplitSectorForConnectivity( const Vec2i& tcMin, const Vec2i& tcMax, PathMajorCell landSector, PathMajorCell amphSector, PathSubCell replacementCell, int numRegions, std::set<unsigned char>& calculatedSectors, const Array2D<unsigned char>& tcBuffer, std::vector<PathSector*>& newSectors );
	void		DumpConnectivityBuffer( const Vec2i& tcMin, const Vec2i& tcMax, Array2D<unsigned char>& preciseBuffer, Array2D<unsigned char>& tcBuffer );

	void		FindNextCell( int& startX, int& startZ, int width, int height, const TerrainSectorMap::CellGrid* sectorGrid );

	void		CopyImpassibles( int width, int height, const TerrainSectorMap::CellGrid *sectorGrid );

	void		AddToCellSet( PathSubCellSet& cellSet, long x, long y ) const;

	void		GetCellsBordering( const Vec2i& min, const Vec2i& max, PathSubCellSet& cellSet ) const;
	void		RemoveAdjacency( const PathSubCellList& cells );

//	void		RebuildAdjacencyForCell( PathSector *pPathSector, PathSubCell thisCell, int x, int y, int deltaX, int deltaY );
//	void		RebuildAdjacency( PathSector *pPathSector );
	void		RebuildAdjacency( PathSector *pPathSector, RebuildPred& pred );
	void		RebuildAdjacencyGivenList( PathSubCellList& cells );

//	void		RebuildAdjacencyForBuildingCell( PathSectorAdj& adj, PathSubCell thisCell, int x, int y );
	void		RebuildBuildingAdjacency( PathSector* pPathSector );

	PathSubCell GetNextFreePathSubCell() const;

	bool		CalcSectorCenter( int minX, int minY, int width, int height, PathSubCell subSector, int& centerX, int& centerY ) const;

	PathSector*	AddPathSector( const Vec2i& min, const Vec2i& max, PathSubCell subSector, PathMajorCell landSector, PathMajorCell amphSector );
	void		AddPathSectorThatMightNeedToBeSplitForConnectivity( const Vec2i& min, const Vec2i& max, PathMajorCell landSector, PathMajorCell amphSector, std::vector<PathSector*>& newSectors );

	void		BlitCells( const Vec2i& min, const Vec2i& max, PathSubCell blitValue, const PathSubCellPredicate& pred );

	size_t		FindOverlapCells( const Vec2i& min, const Vec2i& max, std::vector<bool>& cellSet, std::vector<PathSubCell>& cells );

	void		RebuildSector( RebuildSubSectorFloodFill* floodfill, PathSector *pSector, std::vector<PathSector*>& newSectors );

	int			PruneCells( std::vector<PathSubCell>& cells, const PathSubCellPredicate& pred );

	void		CellsToMajors(const std::vector<PathSubCell>& cells, 
							  std::vector<PathMajorCell>& landMajors, 
							  std::vector<PathMajorCell>& amphMajors );

	bool		FindMajorsForNewBuilding( const PathSubCellList& overlapSubSectors, PathMajorCell& buildingLandSector, PathMajorCell& buildingAmphSector );

	void		ModifiedSectorSplitForConnectivity( PathSector *pSector, std::vector<PathSector*>& newSectors );

// Data used internally
private:

	PathSectorMapImpl*		m_pimpl;

	TerrainCellMap*			m_tcMap;
	PreciseTerrainMap*		m_preciseMap;

	FloodFillDeque			m_floodStack;


	#if defined( EXTREME_SYNC_LOGGING )
	// this should be in a pimpl
	LogFile					m_extremeLog;
	#endif

};
