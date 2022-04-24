/////////////////////////////////////////////////////////////////////
// File    : TerrainSectorMap.h
// Desc    : 
// Created : Friday, November 16, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "PathTypes.h"
#include "../SimExports.h"

#include <Util/BitPlane.h>

#include <Assist/Array2D.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class TerrainCellMap;
class BiFF;

/////////////////////////////////////////////////////////////////////
//	Name	: TerrainSectorMap
//	Desc	:
//
class SIM_API TerrainSectorMap
{
// types
public:

	typedef Array2D<PathMajorCell> CellGrid;


// Functions.
public:
	TerrainSectorMap();
	~TerrainSectorMap();

	void Generate( const TerrainCellMap* pMap );

#if 0
	bool Save(BiFF &iff) const;
	bool Load(IFF &iff);
#endif

	// Perhaps the conversion world->cell should be in here too.
	//	(aswell as the outside of map checking).
	//	just get this working first, then fix0r.

	inline const CellGrid* GetLandGrid() const;
	inline const CellGrid* GetAmphGrid() const;

	inline const long GetNumLandSectors() const;
	inline const long GetNumAmphSectors() const;

#if 0
	// Getting the Cell Properties.
	Cell GetCell( const int x, const int z ) const;
	void SetCell( const int x, const int z, Cell cell );

	// dimension accessor functions
	inline int		GetWidth() const	{return m_width;}
	inline int		GetHeight() const	{return m_height;}
#endif

#if 0
	bool FindClosestMatchingSector( const int hitherX, const int hitherZ, const int yonX, const int yonZ,
									int& resultX, int& resultZ ) const;

	bool FindClosestPassibleSector( const int hitherX, const int hitherZ, const int yonX, const int yonZ,
									int& resultX, int& resultZ ) const;

	bool TestConnectivity( const Cell hither, const Cell yon ) const;
#endif

// types:
private:

// functions
private:

	typedef bool (*FloodMatchCB)( const TerrainCellMap *tcMap, const CellGrid *grid, int x, int y, unsigned char/*FIX should be TCMask*/ seedMask, PathMajorCell replaceValue);
	int				FloodFill( const TerrainCellMap* pMap, CellGrid* grid, int x, int y, PathMajorCell cellValue, PathMajorCell replaceValue, FloodMatchCB floodMatchCB );

	typedef bool (*SectorCB)( const TerrainSectorMap*, const CellGrid*, int, int, PathMajorCell );

	long DoGenerate( const TerrainCellMap* pMap, CellGrid& grid, FloodMatchCB floodMatchCB );

#if 0
	bool DoFindBestUsingLine( const int hitherX, const int hitherZ, 
							  const int yonX, const int yonZ,
							  const CellGrid *grid,
							  const SectorCB callback,
							  int& resultX, int& resultZ ) const;

	void CalculateAdjacency( int numSectors );

	void AddConnection( int numSectors, int thisSector, int addSector );
	void FillConnections( int numSectors, int startSector );
	void CalculateAmphibianConnectivity( int numSectors );

	// backward compatibility bullshit
	bool LoadVersion1(IFF &iff);	
#endif

// data
private:

	CellGrid		m_landGrid;
	CellGrid		m_amphGrid;

//	Cell*			m_cell;

//	long			m_width;		// number of cells in map across
//	long			m_height;		// number of cells in map down

	long			m_numLandSectors;	// [] the number of land/water sectors in the map
	long			m_numAmphSectors;	// [] the number of amphibian sectors in the map

	// temp for now
//	BitPlane		m_adjacency;	// []
//	BitPlane		m_connectivity;	// []
};

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const TerrainSectorMap::CellGrid* TerrainSectorMap::GetLandGrid() const
{
	return( &m_landGrid );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const TerrainSectorMap::CellGrid* TerrainSectorMap::GetAmphGrid() const
{
	return( &m_amphGrid );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const long TerrainSectorMap::GetNumLandSectors() const
{
	return( m_numLandSectors );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const long TerrainSectorMap::GetNumAmphSectors() const
{
	return( m_numAmphSectors );
}
