/////////////////////////////////////////////////////////////////////
// File    : EntityController.h
// Desc    : 
// Created : Thursday, February 22, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// This is the Base EntityController class. The EntityController class cannot be instantiated
//	itself, but it provides a base framework for controllers to fit into. All Entity Controllers
//	will be derived from this class.

#pragma once

#include "SimExports.h"

// forward declaration
class Entity;
class EntityGroup;
class EntityCommand;
class GroupController;
class Vec3f;
class BiFF;
class IFF;
class ChunkNode;

///////////////////////////////////////////////////////////////////// 
// EntityController

class SIM_API EntityController
{
// construction
protected:
	EntityController( Entity *e );
	virtual ~EntityController();

// interface
public:

	// Access function to get the Entity associated with this controller.
	inline const Entity*	GetEntity() const;
	inline Entity*			GetEntity();

	inline const GroupController*	GetGroupController() const;
	inline GroupController*			GetGroupController();

	// hack for slowing down entities
	inline bool				UseSpeedCeil() const;
	inline float			GetSpeedCeil() const;
	inline void				SetSpeedCeil( float c );
	inline void				UnsetSpeedCeil();

	inline float			GetMovingSpeed( float speed );

// Virtual interface, base class does minimal implementation
public:

	// Notification callbacks when the entity is spawned into the world
	virtual void			OnSpawnEntity();
	virtual void			OnDeSpawnEntity();
	
	virtual void			SetGroupController( GroupController *pGC );

	// Commands issued to the entity controllers.

		// return true if the command was processed and should not be stored
	virtual bool			CommandDoProcessNow( const EntityCommand* ) = 0;

		// return true if the command should clear the queue before adding itself
	virtual bool			CommandIsClearQueue( const EntityCommand* ) const = 0;

	// this function should update the statemachine for the controller
		// return true if the command was processed and should be discarded
		// NOTE: currentCommand may be NULL
		// This command can be called multiple times per simstep.
	virtual bool			Update( const EntityCommand* currentCommand ) = 0;

	// This command is called once per simstep.
	virtual void			Execute() = 0;

	// Called before just before the simulation starts
	virtual void			SimulatePre();

	// Save and Load functions.  You MUST call the base class version of this function first
	virtual void			Save( BiFF& ) const;
	virtual void			Load( IFF& );

// Data
private:
	Entity* m_Entity;	
		// pointer to the entity that this controller belongs to. 
		// the entity provides data that the controller needs

		// NOTE: a controller should only read (or have read access) to the
		// entities data. It should not modify the position, etc... 
		// (that is the job of the dynamics).

	GroupController* m_pGroupController;

		// hack: used for slowing down entities during simulation
	bool	m_bUseSpeedCeil;
	float	m_speedCeil;

//
private:
	EntityController();

	static unsigned long	HandleECON( IFF& iff, ChunkNode*, void* pContext1, void* );
};

///////////////////////////////////////////////////////////////////// 
// Inline Functions

// Access function to get the Entity associated with this controller.
inline const Entity* EntityController::GetEntity() const
{
	return m_Entity;
}

//
inline Entity* EntityController::GetEntity()
{
	return m_Entity;
}

inline const GroupController* EntityController::GetGroupController() const
{
	return m_pGroupController;
}

inline GroupController* EntityController::GetGroupController()
{
	return m_pGroupController;
}

// hack for slowing down entities
inline bool EntityController::UseSpeedCeil() const
{
	return m_bUseSpeedCeil;
}

//
inline float EntityController::GetSpeedCeil() const
{
	return m_speedCeil;
}

//
inline void EntityController::SetSpeedCeil(float c)
{
	m_bUseSpeedCeil = true;
	m_speedCeil = c;
}

//
inline void EntityController::UnsetSpeedCeil()
{
	m_bUseSpeedCeil = false;
}