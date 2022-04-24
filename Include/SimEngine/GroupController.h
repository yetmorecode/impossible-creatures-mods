/////////////////////////////////////////////////////////////////////
// File    : GroupController.h
// Desc    : 
// Created : Thursday, April 25, 2002
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

// This is the Base GroupController class. The GroupController class cannot be instantiated
//	itself, but it provides a base framework for groups of controllers to fit into. All Group Controllers
//	will be derived from this class.

#pragma once

#include "SimExports.h"

#include "EntityGroup.h"

///////////////////////////////////////////////////////////////////// 
// Forward Declarations
class Entity;

///////////////////////////////////////////////////////////////////// 
// GroupController

class SIM_API GroupController
{
// construction
public:
	GroupController( );
	virtual ~GroupController();

// interface
public:

	// returns a long identifying the type of group
	virtual long				GetTypeID() const = 0;

	// gets the entities in the group
	inline const EntityGroup&	GetGroup() const;

	// sets the entities that belong in the group
	virtual void				SetGroup( const EntityGroup& eg );

	// gets the unique ID of the group
	inline	int					GetGroupID() const;

	// sets the unique ID of the group
	inline	void				SetGroupID( int groupID );

	// removes an entity from the group
	virtual void				RemoveEntity( Entity* pEntity );

	// update the group
	virtual bool				Update();

	// execute the group
	virtual void				Execute();

// virtual overrides
protected:

	// called before the entities in the group are executed
	virtual void				PreExecute( )  {;}

	// called after the entities in the group are executed
	virtual void				PostExecute( ) {;}

	// does the actual Execute for entities in the group
	virtual void				DoExecuteEntity( Entity* pEntity );


// Save and Load functions.  You MUST call the base class version of this function first
public:

	virtual void				Save( IFF& iff ) const;
	virtual void				Load( IFF& iff );


// implementation
protected:

	EntityGroup		m_entities;

	int				m_groupID;

private:
	static unsigned long		HandleGCFW( IFF&, ChunkNode*, void*, void* );
};

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the entities in the group
//
inline const EntityGroup& GroupController::GetGroup() const
{
	return m_entities;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the ID of the group
//
inline int GroupController::GetGroupID() const
{
	return m_groupID;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: sets the ID of the group
//
inline void	GroupController::SetGroupID( int groupID )
{
	m_groupID = groupID;
}