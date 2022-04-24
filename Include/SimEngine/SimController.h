/////////////////////////////////////////////////////////////////////
// File    : SimController.h
// Desc    : 
// Created : Wednesday, October 03, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "EntityController.h"


class EntityDynamics;
class ChunkNode;
class GroupController;

class SIM_API SimController : public EntityController
{
// Data
protected:
	SimController (	Entity *e, EntityDynamics *d );

private:
	// dynamics
	EntityDynamics*			m_dynamics;

// Functions
public:
	virtual					~SimController();

	virtual void			OnAddPathfinding();
	virtual void			OnRemPathfinding();

	// Notification callbacks when the entity is spawned into the world
	virtual void			OnSpawnEntity();
	virtual void			OnDeSpawnEntity();

	virtual void			SetGroupController( GroupController *pGC );

// Functions
public:

 	inline EntityDynamics*			GetEntityDynamics()			{ return m_dynamics;}
	inline const EntityDynamics*	GetEntityDynamics() const	{ return m_dynamics;}

// Save and Load functions.  You MUST call the base class version of this function first
protected:

	virtual void Save( BiFF& biff ) const;
	virtual void Load( IFF& iff );

private:	
	SimController();
};