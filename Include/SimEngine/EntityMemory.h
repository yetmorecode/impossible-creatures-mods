/////////////////////////////////////////////////////////////////////
// File    : EntityMemory.h
// Desc    : 
// Created : Wednesday, July 17, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//


#pragma once

#include "SimExports.h"

#include "EntityGroup.h"

/////////////////////////////////////////////////////////////////////
//	Class	: EntityMemory
//	Desc.	: Responsible for remembering an Entity with a given time
//
class SIM_API EntityMemory : private EntityGroup::Observer
{
// types
public:
	enum { MAXMEMORIES = 8 };

// construction
public:
	 EntityMemory();
	~EntityMemory();
	
// interface
public:

	// adds or updates the memory of an entity
	void	AddMemory( Entity *pEntity, long time );

	// determines if we remember the entity since time
	bool	HaveMemory( Entity *pEntity, long time ) const;

	// determines if we remember any entities controlled by the given player since time
	bool	HaveMemory( Player *pPlayer, long time ) const;

	// determines if we remember any entities since a given time
	bool	HaveMemoriesSince( long time ) const;

	// returns the entities we remember since the given time
	void	GetMemories( EntityGroup& attackers, long time ) const;

// EntityGroup::Observer
private:
	virtual void Notify_Insertion( Entity *e );
	virtual void Notify_Removal( Entity *e );

// fields
private:
	// to save memory, we only keep a single entity group
	struct Memory
	{
		// we need to keep the id around to sync when an entity is removed
		// from the group
		unsigned long	entityId;
		long			time;
	};

	typedef std::smallvector<Memory, MAXMEMORIES> MemoryVector;

	// IMPORTANT NOTE: these 2 structures must be KEPT IN SYNC
	EntityGroup		m_entities;
	MemoryVector	m_memories;

// implementation

	// GH: Added to compile with .NET
	static bool Older ( const Memory& lhs, const Memory& rhs );

// copy 
private:
	EntityMemory( const EntityMemory& );
	EntityMemory& operator= ( const EntityMemory& );
};