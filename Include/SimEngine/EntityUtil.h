/////////////////////////////////////////////////////////////////////
// File    : EntityUtil.h
// Desc    : 
// Created : Saturday, September 15, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "Entity.h"

#include <Math/Vec2.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class Entity;
class Target;

///////////////////////////////////////////////////////////////////// 
// EntityUtil

namespace EntityUtil
{
	// These are used by the dynamics when specifying a new entity position.
	float SIM_API DistSqrDirCalcPos   ( const Vec2f &pos, const Entity *e, const Vec2f &target2DPos, const float AP , Vec2f* heading );
	float SIM_API DistSqrDirCalcEntity( const Vec2f &pos, const Entity *e, const Entity *targetEntity, const float AP , Vec2f* heading );

	// These are used by the states when querying the current position.
	float SIM_API DistSqrDirCalcPos   ( const Entity *e, const Vec2f &target2DPos, const float AP , Vec2f* heading );
	float SIM_API DistSqrDirCalcEntity( const Entity *e, const Entity *targetEntity, const float AP , Vec2f* heading );
	float SIM_API DistSqrDirCalcTarget( const Entity *e, const Target* target, const float AP, Vec2f* heading );

	void SIM_API  CalcBoxSeparation( const long eCellX, const long eCellZ, const long eWidth, const long eHeight,
									 const long tCellX, const long tCellZ, const long tWidth, const long tHeight,
									 long& sepX, long& sepZ );

	bool SIM_API  GetEntityBox( const Vec2f& pos,		const Entity *targetEntity, long& cellX, long& cellZ, long& width, long& height );
	bool SIM_API  GetEntityBox( const Vec2i& posCell,	const Entity *targetEntity, long& cellX, long& cellZ, long& width, long& height );

	float SIM_API DistSqrGivenCellSeparation( long sepX, long sepY );

	bool SIM_API GetRandomPointNearEntity( const Entity *e, float dist, Vec2f& resultPos );

	bool SIM_API AreEntitiesAdjacent( const Vec2f &pos1, const Entity *e1, const Vec2f &pos2, const Entity *e2 );
	bool SIM_API AreEntitiesAdjacent( const Vec2i &pos1, const Entity *e1, const Vec2i &pos2, const Entity *e2 );

	bool SIM_API AreEntitiesAdjacent( const Vec2f &pos1, const Entity *e1, const Entity *e2 );
	bool SIM_API AreEntitiesAdjacent( const Vec2i &pos1, const Entity *e1, const Entity *e2 );

	bool SIM_API AreEntitiesOverlapping( const Entity *e, const Entity *t );


	void SIM_API GetWorldEntityBox( const Vec2f& pos, const Entity *targetEntity, Vec2f& minCoord, Vec2f& maxCoord  );
	void SIM_API GetWorldEntityBox( const long cellX, const long cellZ, const long width, const long height, Vec2f& minCoord, Vec2f& maxCoord );
	float SIM_API DistSqrPointRectangle( const Vec2f& point, const Vec2f& minCoord, const Vec2f& maxCoord );

	bool SIM_API GetRectangleUnpenetration( const Vec2f& pos, const Vec2f& minCoord, const Vec2f& maxCoord, Vec2f& pushVector, float& pushDistance );
}
