/////////////////////////////////////////////////////////////////////
// File    : SpatialBucketSystem.h
// Desc    : 
// Created : Thursday, November 08, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "SimExports.h"

#include <Assist/StlExSmallVector.h>

#include <Math/Vec3.h>

class SpatialBucket;
class Entity;
class SimEntity;
class TerrainHMBase;
class FindClosestFilter;

class SpatialBucketSystem
{
// types
public:
	typedef std::smallvector< Entity*, 128 > SBEntityList;

// construction
public:
	// This class holds a list of buckets.
	SIM_API  SpatialBucketSystem( const long GridSize, TerrainHMBase *THM );
	SIM_API ~SpatialBucketSystem( );

// interface
public:
	SIM_API void	AddEntity	( SimEntity *e );
	SIM_API void	RemEntity	( SimEntity *e );
	SIM_API void	MoveEntity	( SimEntity *e );

	// This function returns a sorted list of entites, from closest to furthest
	SIM_API void	FindClosest	
						(
						SBEntityList&			searchlist,
						FindClosestFilter&		predicate,
						const unsigned long		MaxEntities,
						const Vec3f&			pos,
						const float				maxRadius,
						const Entity*			EIgnore
						) const;

	// This function returns a sorted list of entites, from closest to furthest
	SIM_API void	FindClosest	
						(
						SBEntityList&			searchlist,
						FindClosestFilter&		predicate,
						const unsigned long		MaxEntities,
						const Entity*			pEntity,
						const float				maxRadius,
						const Entity*			EIgnore
						) const;

	// This function returns a non sorted list of all entities inside the search radius that satisfy
	// the prdicate
	SIM_API void	FindAll		
						(
						SBEntityList&			searchlist,
						FindClosestFilter&		predicate,
						const Vec3f&			pos,
						const float				maxRadius
						) const;

	SIM_API bool	GetBucketEntities
						(
						SBEntityList&			searchlist,
						const Vec3f&			pos
						);

// fields
private:
	typedef std::vector< SpatialBucket > BucketList;

private:
	long			m_NumBuckets_x;		// Number of Buckets in the x direction.
	long			m_NumBuckets_z;		// Number of Buckets in the z direction.
	long			m_kNumGridpoints;	// A total of the gridpoint (m_x * m_z).

	long			m_WorldSize_x;		// The width of the island.
	long			m_WorldSize_z;		// The length of the island.

	long			m_BucketSize;		// The perimeter edge of a grid in meteres, each grid cell is square

	BucketList		m_Buckets;			// A Pointer to the list of spatial buckets.

// implementation
private:

	void			CalcBucketXZ	( const Vec3f &pos, long &x, long &z ) const;
	long			CalcBucketIndex	( const Vec3f& pos ) const;

	long			GetNumBuckets	( )	const	{ return m_kNumGridpoints;	}
	long			GetNumBuckets_x	( )	const	{ return m_NumBuckets_x;	}
	long			GetNumBuckets_z	( )	const	{ return m_NumBuckets_z;	}

	long			GetBucketSize	( )	const	{ return m_BucketSize;		}

	// get a pointer to the bucket at x, z.  Returns NULL if you ask for an invalid bucket
	const SpatialBucket*	
					GetBucket( long x, long z ) const;
	SpatialBucket*	GetBucket( long x, long z );

// copy -- do not define
private:
	SpatialBucketSystem( const SpatialBucketSystem& );
	SpatialBucketSystem& operator= ( const SpatialBucketSystem& );
};
