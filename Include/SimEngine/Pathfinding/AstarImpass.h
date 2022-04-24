/////////////////////////////////////////////////////////////////////
// File    : AstarSector.cpp
// Desc    : 
// Created : 
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once


#include "AstarBase.h"
#include "AstarPool.h"

/////////////////////////////////////////////////////////////////////
//	Forward Declarations
class TerrainCellMap;
class Target;
class SimEntity;
class AstarPool;
class FindImpassGoallessPred;

/////////////////////////////////////////////////////////////////////
//	Class.	: AstarImpass
//
class AstarImpass : public AstarBase
{
public:
	AstarImpass( AstarPool<AstarGridNode> *pPool, const TerrainCellMap* tcMap );
	~AstarImpass();

	bool FindPath( GridPath &path, const Vec2i& startCell, const Vec2i& goalCell, SimEntity *pe, const Target *T, float APc, SimWorld *simWorld, const TCMask &passTypes, size_t maxNodes );

	bool FindFirstOpen( Vec2i& firstOpen, const Vec2i& startCell, const Vec2i& goalCell, const TCMask& passTypes );

	void SearchStart( const Vec2i& startCell, const FindImpassGoallessPred& pred );

// AstarBase overrides
public:

	virtual size_t	GetMemUsage() const;	


private:
	
	const TerrainCellMap *m_tcMap;

#if 0
	TCMask m_passTypes;

	SimEntity *m_pe;				// only used within the duration of the FindPath call, so it's alright to store
	const Target *m_T;
	float m_APc;
	Vec2i m_startCell;
	Vec2i m_goalCell;

	Vec2f m_CF;
	float m_ooCellSize;

	enum SearchType { eNormalSearch, eFirstOpenSearch };
	SearchType m_searchType;
#endif
};