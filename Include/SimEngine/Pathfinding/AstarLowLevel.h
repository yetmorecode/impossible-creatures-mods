#pragma once


#include "AstarBase.h"


/////////////////////////////////////////////////////////////////////
//	Forward Declarations
class TerrainCellMap;
class PathSectorMap;
class Target;
class SimEntity;
class AstarPool;

/////////////////////////////////////////////////////////////////////
//	Class.	: AstarImpass
//
class AstarLowLevel : public AstarBase
{
public:
	AstarLowLevel( AstarPool<AstarGridNode> *pPool, const TerrainCellMap* tcMap, const PathSectorMap* psMap );
	~AstarLowLevel();

	bool FindPath( GridPath &path, 
				   const Vec2i& startCell, const Vec2i& goalCell, int goalSubSector,
				   SimEntity *pe, Target *T, float APc, SimWorld *simWorld, const TCMask &passTypes, size_t maxNodes );

// AstarBase overrides
public:

	virtual size_t	GetMemUsage() const;	

private:

	virtual bool PassiblePosition_DoFindPath( int x, int y, DirectionMap::Dir dir ) const;
	virtual bool TestExitCondition_DoFindPath( int x, int y ) const;

private:
	void SetPassTypes(const TCMask& passTypes);

	const TerrainCellMap *m_tcMap;
	const PathSectorMap *m_psMap;

	TCMask m_passTypes;

	SimEntity *m_pe;				// only used within the duration of the FindPath call, so it's alright to store
	Target *m_T;

	Vec2f m_CF;
	float m_ooCellSize;

	int m_goalSubSector;

	int m_goalCellCount;
};