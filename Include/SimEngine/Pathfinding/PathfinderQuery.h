/////////////////////////////////////////////////////////////////////
// File    : PathfinderQuery.h
// Desc    : This file contains the implementation of methods that query but do not modify
//				any of the pathfinding data
// Created : Friday, January 25, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h"

#include "PathTypes.h"
#include "PathPlan.h"

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class PathfinderImpl;
struct PathfinderMaps;
class Entity;
class EntityGroup;
class SimEntity;
class Target;
class BrokenSectorConnections;
class OBB3f;
class ControllerBlueprint;
class SimHighPath;
class SimPath;

/////////////////////////////////////////////////////////////////////
//	Class	: PathfinderQuery
//
class SIM_API PathfinderQuery
{
// Types
public:

	enum ValidDestinationResult
	{
		VD_Perfect,
		VD_Close,
		VD_Screwed,
	};

// Methods
public:

	PathfinderQuery( const PathfinderImpl *pimpl, const PathfinderMaps* pMaps );	// ctor

	static void* operator new   ( size_t );											// operator new
	static void  operator delete( void* );											// operator delete	


	bool	IsClearPath( int x1, int y1, int x2, int y2, const TCMask pfCellMask ) const;
	bool	IsSectorValid( PathSubCell sector ) const;

	bool	PossiblePathCheck( const Vec2i& startCell, const Vec2i& goalCell, int entitySize, const TCMask &passTypes, Vec2i& bestReachable ) const;
	bool	PossiblePathCheck( const Vec2f& startPos, const Vec2f& goalPos, int entitySize, const TCMask &passTypes, Vec2i& bestReachable ) const;

	bool	StraightLineToTarget( const Vec2f& start, const SimEntity *pEntity, const SimEntity *pTarget, Vec2i& bestReachable ) const;
	bool	StraightLineFromTarget( const Vec2f& dest, const SimEntity *pEntity, const Target& target, Vec2i& bestReachable ) const;

	bool	GetValidDestination( const SimEntity *pe, const Target *T, 
								 float AP, const GroundAttributes& attributes, bool bUseEntities,
							     ValidDestinationResult& vdResult, PathPlan &pathPlan  ) const;

	bool	GetValidDestinationMelee( const SimEntity *pe, const Target *T, 
									  const GroundAttributes& attributes,
									  bool bUseEntities, bool bUseStraightLine,
									  ValidDestinationResult& vdResult, PathPlan &pathPlan ) const;

	bool	GetBestReachableMelee( const SimEntity *pe, const Target *T, 
							       const GroundAttributes& attributes, 
								   bool bUseEntities, bool bUseStraightLine,
								   Vec2i& bestReachable, ValidDestinationResult& vdResult ) const;

	bool	FindPathHigh( SimHighPath &path, AstarSectorPathResult& pathResult, PathPlan& pathPlan,
						  SimEntity *pe, const Target *T, float AP, const TCMask &pfCellMask, const BrokenSectorConnections* brokenConnections ) const;

	bool	FindPathGoal( SimPath &path, AstarPathfindingResult& pathResult, PathPlan& pathPlan,
						  SimEntity *pe, const Target *T, float AP, const TCMask &pfCellMask, bool bGetOffBuilding ) const;

	bool	FindPathSector( SimPath &path, AstarPathfindingResult& pathResult, 
						    const PathSubCellSet* allowedSectors, SimEntity *pe, PathSubCell goalSector, const TCMask &pfCellMask ) const;

	bool	FindFirstOpen( Vec2i& firstOpen, const Vec2i& start, const Vec2i& goal, const TCMask& pfCellMask ) const;
	bool	FindFirstOpenPrecise( const Vec2i& refCell, const Vec2i& targetCell, int maxSearch, 
								  const GroundAttributes& attributes, 
								  PathMajorCell majorCell, bool bAmphibian, bool bUseEntities,
								  Vec2i& bestReachable ) const;
	bool	FindFirstOpenPrecise( const Vec2i& refCell, const Vec2i& targetCell, 
	 							  const Vec2i& minCell, const Vec2i& maxCell,
								  int maxSearch, 
								  const GroundAttributes& attributes,
								  PathMajorCell majorCell, bool bAmphibian, bool bUseEntities,
								  Vec2i& bestReachable ) const;
	// new
	bool	FindPathSector2( SimPath &path, AstarPathfindingResult& pathResult, 
							 const PathSubCellSet* allowedSectors, SimEntity *pe, const SimHighPath& highPath, const TCMask &pfCellMask ) const;
	// new
	

	bool	FindClosestOpenAdjacent( Vec2i& firstOpen, const Entity* pTarget, const Vec2i& refCell, const GroundAttributes& attributes ) const;
	bool	FindClosestOpenAdjacent( Vec3f& firstOpen, const Entity* pMe, const Entity* pTarget ) const;

	bool	FindBestPositionInSector( PathSubCell sector, const Vec2f& goal, int entitySize, bool bUseEntities, Vec2f& bestPos ) const;

	void	FindSectorsWithinAP( const Vec2f& center, float AP, int entitySize, bool bAmphibian, PathMajorCell majorCell, PathSubCellSet& sectors ) const;

	bool	GiveClosestFreePosition( const Vec2f& wantedPosition, const TCMask& movementMask, int groundLevel, const OBB3f& obb, Vec3f &bestPosition ) const;
	bool	GiveClosestFreePosition( const Vec2f& wantedPosition, const SimEntity* pEntity, int groundLevel, Vec3f &bestPosition ) const;
	bool	GiveClosestFreePosition( const Vec2f& wantedPosition,const Vec2f& refPosition, const TCMask& movementMask, int groundLevel, const OBB3f& obb, Vec3f &bestPosition ) const;
	bool	GiveClosestFreePosition( const Vec2f& wantedPosition, const Vec2f& refPosition, const Vec2i& minCell, const Vec2i& maxCell, const TCMask& movementMask, int groundLevel, const OBB3f& obb, Vec3f &bestPosition ) const;

	bool	GiveClosestFreePositionWithStraightLine(const Vec2f& wantedPosition, const Vec2f& refPosition,  
												   const SimEntity* pOriginEntity,
												   const TCMask& movementMask, int groundLevel, const OBB3f& obb, 
												   Vec3f &bestPosition ) const;

	// determines if an entity can stand on a given location
	bool	IsPositionFree( const Vec2f& wantedPosition, const SimEntity* pEntity, int groundLevel ) const;

	void	GetEntitiesOnBuilding( const Entity *pBuilding, EntityGroup& overlappers ) const;

	int		EstimateSectorPathLength( const SimHighPath& highpath, size_t startIndex ) const;

	bool	FindValidWithinRange( Vec2i& firstOpen, const Vec2i& start, const Vec2i& goal, SimEntity *pe, float AP, const TCMask& pfCellMask ) const;

	// fast check that only uses terrain to determine if it's possible to reach a location
	bool	IsReachableFrom( const Vec2f& start, const Vec2f& goal, bool bAmphibian ) const;

	// slow check that only uses terrain to determine if it's possible to reach a location within a radius
	bool	IsReachableFrom( const Vec2f& start, const Vec2f& goal, float AP, bool bAmphibian ) const;

	bool	IsInRange( const Vec2f& pos2D, const SimEntity *pe, const Target* T, float AP ) const;
	bool	IsInRange( const Vec2i& grid2D, const SimEntity *pe, const Target* T, float AP ) const;

	bool	CanPlaceBuildingImpass( const ControllerBlueprint*, const Matrix43f* pTransform, const Vec3f& pos, const TCMask pfCellMask ) const;

	void	FindAdjacentBuildings( const Vec2f& position, EntityGroup& eg ) const;

	bool	GetPathEstimate( WorldPath& path, const Vec3f& startpos, const Vec3f& endpos, const Player* player, int entitySize, bool bAmphibian, bool bToBuilding, bool& resultThruBuilding) const;
	bool	GetPathEstimateDist( float& dist, const Vec3f& startpos, const Vec3f& endpos, const Player* player, int entitySize, bool bAmphibian, bool bToBuilding ) const;
	float	GetDistFromPath( const WorldPath& path ) const;

	bool	GetPathExact( SimPath& path, const Vec3f& startpos, const Vec3f& endpos, int entitySize, const TCMask& pfCellMask ) const;
	bool	GetPathExactDist( float& dist, const Vec3f& startpos, const Vec3f& endpos, int entitySize, const TCMask& pfCellMask ) const;

	bool	CanEntityPathToLocation( const SimEntity* pe, const Target* T, bool bToBuilding ) const;

	// determines if an entity if preciseSize, pfCellMask can stand next to pTarget by checking terrain
	bool	CanStandAdjacent( const GroundAttributes& attributes, const SimEntity* pTarget, const bool bUseEntities ) const;

	bool	GetBuildingsBlockingAdjacency( const int preciseSize, const SimEntity* pTarget, EntityGroup& eg ) const;


	void	GetPrecisePassibleVisited( GridPath& path ) const;
	void	GetPreciseImpassibleVisited( GridPath& path ) const;

	void				BlockSector( PathSubCell sector );			// blocks a sector so that it is considered impassible by the FindPathHigh
	void				ClearBlockedSectors();						// clears all sectors previously blocked


// Methods
private:
	PathfinderQuery();										// hide the default ctor
	PathfinderQuery( const PathfinderQuery& );				// hide the copy constructor
	PathfinderQuery& operator= ( const PathfinderQuery& );

// Data
private:

	const PathfinderMaps *m_maps;
	const PathfinderImpl *m_pimpl;

	mutable GridPath	m_gridPath;
};