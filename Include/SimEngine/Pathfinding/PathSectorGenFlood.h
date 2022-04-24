#pragma once

#include "FloodFill.h"
#include "TerrainSectorMap.h"

/////////////////////////////////////////////////////////////////////
//	Switches
//
#define PATHSECTOR_SQUARES 0


/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class PathSectorMapImpl;
class PreciseTerrainMap;
class TerrainCellMap;


/////////////////////////////////////////////////////////////////////
//	Class	: GenerateSubSectorFloodFill
//
class GenerateSubSectorFloodFill : public FloodFillPackage
{
public:

	void SetConstant( PathSectorMapImpl *pimpl, const TerrainSectorMap::CellGrid *grid );
	void SetWriteValue( PathSubCell writeValue );
	void SetGridValue( PathMajorCell gridValue );
	void SetSeed( int seedX, int seedY );

	const Vec2i& GetMin() const { return m_extentsMin; }
	const Vec2i& GetMax() const { return m_extentsMax; }

// Virtual Overrides
public:

	bool							IsMatch( int x, int y ) const;
	FloodFillPackage::OnMatchAction	OnMatch( int x, int y, int fillCount );
	bool							IsFindNextMatch( int x, int y ) const;

// Data
private:

	PathSubCell							m_writeValue;
	PathMajorCell						m_gridValue;
	const TerrainSectorMap::CellGrid*	m_grid;
	Vec2i								m_extentsMin;
	Vec2i								m_extentsMax;

#if PATHSECTOR_SQUARES
	Vec2i								m_stopMin;
	Vec2i								m_stopMax;
#endif

	PathSectorMapImpl*					m_pimpl;
};


/////////////////////////////////////////////////////////////////////
//	Class	: RebuildSubSectorFloodFill
//
class RebuildSubSectorFloodFill : public FloodFillPackage
{
public:

	void SetConstant( PathSectorMapImpl *pimpl, const TerrainCellMap *tcMap );
	void SetReplaceValue( PathSubCell replaceValue );
	void SetWriteValue( PathSubCell writeValue );
	void SetSeed( int seedX, int seedY );

	const Vec2i& GetMin() { return m_extentsMin; }
	const Vec2i& GetMax() { return m_extentsMax; }


// Virtual Overrides
public:

	bool							IsMatch( int x, int y ) const;
	FloodFillPackage::OnMatchAction	OnMatch( int x, int y, int fillCount );
	bool							IsFindNextMatch( int x, int y ) const;

private:

	inline bool						DoIsMatch( int x, int y ) const;

// Data
private:

	PathSubCell							m_writeValue;
	PathSubCell							m_replaceValue;
	const TerrainCellMap*				m_tcMap;
	Vec2i								m_extentsMin;
	Vec2i								m_extentsMax;
	PathSectorMapImpl*					m_pimpl;
};

/////////////////////////////////////////////////////////////////////
//	Class	: RebuildSubSectorFloodFill
//
class ConnectivitySubSectorFloodFill : public FloodFillPackage
{
public:

	void SetConstant( PathSectorMapImpl *pimpl, const PreciseTerrainMap *preciseMap, 
					  Array2D<unsigned char>* preciseBuffer, unsigned char preciseSize,
					  Array2D<unsigned char>* sectorBuffer, unsigned char sectorToReplace );
	
	void SetSubSectorValue( unsigned char subCell );
	void SetWriteValue( unsigned char writeValue );

// Virtual Overrides
public:

	bool							IsMatch( int x, int y ) const;
	FloodFillPackage::OnMatchAction	OnMatch( int x, int y, int fillCount );
	bool							IsFindNextMatch( int x, int y ) const;

private:

	inline bool						DoIsMatch( int x, int y ) const;

// Data
private:

	unsigned char						m_preciseSize;
	unsigned char						m_writeValue;
	unsigned char						m_sectorToReplace;
	unsigned char						m_subCell;

	PathSectorMapImpl*					m_pimpl;
	const PreciseTerrainMap*			m_preciseMap;
	Array2D<unsigned char>*				m_preciseBuffer;
	Array2D<unsigned char>*				m_sectorBuffer;
};


