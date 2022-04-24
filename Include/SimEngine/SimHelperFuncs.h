// SimHelperFuncs.h

#pragma once

/*
#include "EntityGroup.h"
#include <Math/Vec3.h>
#include "SimExports.h"

// Forward declarations
class Entity;


// FindClosest: Find the closest entity passing in,
//	eg - an entitygroup to store the results of the search.
//	cb - a callback compare function.
//	MaxEntities - the maximum number of entities to return. 0 means return all entities
//					found (that match compare func).
//	Pos - Position in the world to do the search from.
//	Radius - The radius of the search.

//typedef bool (*FindClosestFilter)(const Entity*);

class FindClosestFilter
{
public:
	virtual bool Check( Entity* ) = 0;
};

class NullFilter : public FindClosestFilter
{
public:
	bool Check( Entity* ) {return true;}
};


void SIM_API FindClosest
	(
	EntityGroup&			 out, 
	FindClosestFilter&		 predicate,
	const size_t			 maxEntities, 
	const Vec3f&			 center, 
	const float				 maxRadius, 
	const Entity*			 ignore
	);
*/