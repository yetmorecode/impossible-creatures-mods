/////////////////////////////////////////////////////////////////////
// File    : PathLowLevel.h
// Desc    : 
// Created : Wednesday, December 12, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "AstarBase.h"

#include <SimEngine/Pathfinding/PathTypes.h>

#include <Math/Vec2.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations
//
class TerrainCellMap;

class SimWorld;
class SimEntity;
class Target;

class AstarPool;

/////////////////////////////////////////////////////////////////////
//	Class	: PathLowLevel
//
class PathLowLevel : public AstarBase
{
public:
	PathLowLevel( AstarPool<AstarGridNode> *pPool, const TerrainCellMap *tcMap, const SimWorld *pSimWorld, int windowWidth, int windowHeight, int numDivisions );
	~PathLowLevel();


	bool FindPath( std::vector<Vec2f> &path, SimEntity *pe, Target *T, float AP, const TCMask &pfCellMask, bool rePathing );

	bool WillMoveCollide( const Vec2f& position, int size, const TCMask& mask ) const;

// AstarBase overrides
private:

	bool PassiblePosition( int x, int y, DirectionMap::Dir dir );
	bool TestExitCondition( int x, int y );

// Methods	
private:

	inline bool GetWindowCell(int x, int y);
	inline void SetWindowCell(int x, int y, bool value);

	int WorldXToWindowX( float worldX );
	int WorldZToWindowZ( float worldZ );						//

	float WindowXToWorldX( int windowX );
	float WindowZToWorldZ( int windowZ );						//

	void TakeSnapshot(int centerX, int centerY, TCMask mask);
	void BurnTerrain( const TCMask& mask );
	void BurnEntities();

	void Dump(const char* name);

	void ConvertWindowPathToWorldPath( const AstarBase::GridPath& gridPath, std::vector<Vec2f>& worldPath );


// Data
private:

	const SimWorld *m_pSimWorld;

	const TerrainCellMap *m_tcMap;

	SimEntity *m_pe;				// only used during the scope of a FindPath call so it's ok to store
	int m_entitySize;

	bool *m_window;

	int m_tcWidth;				// the width of the window, in TerrainCellMap cells
	int m_tcHeight;				// the height of the window, in TerrainCellMap cells

	int m_windowWidth;			// the width of the window
	int m_windowHeight;			// the height of the window

	int m_numDivisions;


	Vec2i m_windowRectCorner;		// in tcCells
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline bool PathLowLevel::GetWindowCell(int x, int y)
{
	int index = y * m_windowWidth + x;
	dbAssert( index < m_windowWidth * m_windowHeight );

	return( m_window[index] );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void PathLowLevel::SetWindowCell(int x, int y, bool value)
{
	int index = y * m_windowWidth + x;
	dbAssert( index < m_windowWidth * m_windowHeight );

	m_window[index] = value;
}
