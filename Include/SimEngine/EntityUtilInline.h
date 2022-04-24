#pragma once


#include "EntityDynamics.h"
#include "SimController.h"
#include "WorldGlobal.h"

#include <SimEngine/World.h>
#include <SimEngine/SimWorld.h>
#include <SimEngine/Pathfinding/Pathfinding.h>
#include <SimEngine/Pathfinding/PreciseTerrainMap.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations
//
class Entity;
class EntityController;
class SimController;
class EntityDynamics;


namespace EntityUtilInline
{

	inline SimWorld* GetWorld( )
	{
		return static_cast< SimWorld* >( g_World );
	}

	inline void CalcBoxSeparation( const long eCellX, const long eCellZ, const long eWidth, const long eHeight,
								   const long tCellX, const long tCellZ, const long tWidth, const long tHeight,
								   long& sepX, long& sepZ )
	{
		// do X
		if (eCellX < tCellX)
		{
			sepX = tCellX - (eCellX+eWidth);
		}
		else
		{
			sepX = eCellX - (tCellX+tWidth);
		}

		// do X
		if (eCellZ < tCellZ)
		{
			sepZ = tCellZ - (eCellZ+eHeight);
		}
		else
		{
			sepZ = eCellZ - (tCellZ+tHeight);
		}

		if (sepX < 0)
		{
			sepX = 0;
		}
		if (sepZ < 0)
		{
			sepZ = 0;
		}		
	}
	

	inline float DistSqrGivenCellSeparation( long sepX, long sepY )
	{
		dbAssert( sepX >= 0 && sepY >= 0 );

		// this algorithm needs to change if cellsize != 1.0f
		dbAssert( GetWorld()->GetPathfinder()->GetPreciseTerrainMap()->GetCellSize() == 1.0f );
			
		return (float)( sepX*sepX + sepY*sepY );
	}

	inline float DistSqrDirCalcPos( const Vec2f &pos, const Entity *e, const Vec2f &target2Dpos, const float AP , Vec2f* heading )
	{
		dbAssert(e);
		UNREF_P(AP);

		const SimController *sc = static_cast< const SimController* >( e->GetController() );
		dbAssert(sc);

		const EntityDynamics *pDynamics = sc->GetEntityDynamics();
		dbAssert(pDynamics);

		return pDynamics->GetDistanceSqrToPoint( pos, target2Dpos, heading );
	}

	inline float DistSqrDirCalcPos( const EntityDynamics* pDynamics, const Vec2f &target2DPos, const float AP, Vec2f* heading )
	{
		// validate parms
		dbAssert( pDynamics );
		UNREF_P( AP );

		return pDynamics->GetDistanceSqrToPoint( target2DPos, heading );
	}


	inline float DistSqrDirCalcPos( const Entity *e, const Vec2f &target2DPos, const float AP , Vec2f* heading )
	{
		// validate parms
		dbAssert( e );
		UNREF_P( AP );

		const SimController *sc = static_cast<const SimController*>( e->GetController() );
		dbAssert(sc);

		const EntityDynamics *pDynamics = sc->GetEntityDynamics();
		dbAssert(pDynamics);

		return DistSqrDirCalcPos( pDynamics, target2DPos, AP, heading );
	}

}