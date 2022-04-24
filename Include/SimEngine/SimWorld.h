/////////////////////////////////////////////////////////////////////
// File    : SimWorld.h
// Desc    : A Simulation of our game world, World specific to 
//			 Pathfinding extensions to our Framework World
// Created : Thursday, February 22, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "SimExports.h"
#include "SimEngineTypes.h"

#include <SimEngine/World.h>
#include <SimEngine/SimEntity.h>

///////////////////////////////////////////////////////////////////// 
// Forward Declarations
class LogFile;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS: SimWorld

class SIM_API SimWorld : public World
{
private:

	SpatialBucketSystem*		m_SBSystem;

	// Pathfinding crap.
	Pathfinder*				 	m_pathfinder;

	// The size of the buckets to use when creating the spatial buckets
	long						m_BucketSize;
	
// Inherited from World
public:

	SimWorld( SimEngineInterface *pSimInterface, long BucketSize );
	virtual ~SimWorld();

	virtual void			Simulate();

	virtual void			SimulatePre();
	
	virtual void			SimulatePost();

	virtual void			SetTerrain( TerrainHMBase* Terrain );

	// Adding and removing entities from the world.
	virtual void			DoSpawnEntity( Entity *e );

	virtual void			DeSpawnEntity( Entity *e );

	virtual void			DoRestoreEntity( Entity *e );

public:

	void					AddEntityPathfinding(Entity *e, bool bConstructed);

	void					RemEntityPathfinding(Entity *e);

	virtual int				CalculatePathfindingSize( float xscale, float zscale ) const;

	void			FindClosest
		(
		std::vector<Entity*>&	 eg,
		FindClosestFilter&		 predicate,
		const size_t			 MaxEntities, 
		const Vec3f&			 Pos, 
		const float				 MaxRadius, 
		const Entity*			 EIgnore
		);

	void			FindClosest
		(
		EntityGroup&			 eg,
		FindClosestFilter&		 predicate,
		const size_t			 MaxEntities, 
		const Vec3f&			 Pos, 
		const float				 MaxRadius, 
		const Entity*			 EIgnore
		);

	void			FindClosest
		(
		EntityGroup&			 eg,
		FindClosestFilter&		 predicate,
		const size_t			 MaxEntities, 
		const Entity*            pEntity,
		const float				 MaxRadius, 
		const Entity*			 EIgnore
		);

	Entity*			FindClosestEntity
		(
		FindClosestFilter&		 predicate,
		const Vec3f&			 Pos,
		const float				 MaxRadius,
		const Entity*			 EIgnore
		) const;

	void			FindAll
		(
		EntityGroup&			 eg,
		FindClosestFilter&		 predicate,
		const Vec3f&			 Pos,
		const float				 MaxRadius
		);

	void			FindAll
		(
		std::vector<Entity*>&	 eg,
		FindClosestFilter&		 predicate,
		const Vec3f&			 Pos,
		const float				 MaxRadius
		);

	// given a group and a position this will return the closest entity from that group
	// use the predicate to filter out dead, unbuilt, or whatever type of entities
	const Entity*	FindClosestOne
		(
		EntityGroup&			 searchGroup,
		FindClosestFilter&		 predicate,
		const Vec3f&			 Pos
		);

	// this is such a shit way to do this....
	//	pathfinding should be held by the dynamics, perhaps a global accessed through a particular dynamics
	//	is what we need instead.
	const Pathfinder *GetPathfinder() const;
	Pathfinder *GetPathfinder();

public:
	const SpatialBucketSystem*	GetSBSystem() const	{return m_SBSystem;}
	SpatialBucketSystem*		GetSBSystem() {return m_SBSystem;}

// Save and Load functions.
public:

	virtual void Save( BiFF& biff ) const;
	virtual void Load( IFF& iff );

// Chunk Handlers
private:

	static unsigned long HandleSWLD( IFF&, ChunkNode*, void*, void* );

protected:

	virtual void			MESimulate( );

	virtual void			SyncLogEntity( LogFile& logfile, const Entity *pEntity ) const;

	virtual void			RemAllPathfinding( );

private:
	void CreatePathfinding( );

	void SetPathfinder( Pathfinder *pathfinder );

	void SpawnEntityInternal( Entity *e );
};

///////////////////////////////////////////////////////////////////// 
// inlines

inline Pathfinder* SimWorld::GetPathfinder()
{
	return m_pathfinder;
}

inline const Pathfinder* SimWorld::GetPathfinder() const
{
	return m_pathfinder;
}
