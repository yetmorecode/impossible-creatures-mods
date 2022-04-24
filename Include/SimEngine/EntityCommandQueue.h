/////////////////////////////////////////////////////////////////////
// File    : EntityCommandQueue.h
// Desc    : 
// Created : Thursday, February 22, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <Memory/MemoryFSAlloc.h>

// forward declarations
class EntityCommand;
class Entity;
class ByteStream;
class World;

///////////////////////////////////////////////////////////////////// 
// EntityCommandQueue

class EntityCommandQueue
{
// construction
public:
	 EntityCommandQueue();
	~EntityCommandQueue();

// types
public:
	typedef std::list< EntityCommand*, mempool_fs_alloc< EntityCommand* > >
		CommandList;

// interface
public:
	// takes ownership of command
	void					Push ( EntityCommand* );

	// call this function to retrieve the next command for this entity
		// return NULL if none
	const EntityCommand*	Top  ( const Entity* who ) const;
	
	// call this function when the command has been processed by the entity
	void					Pop  ( const Entity* who );

	// call this function to remove all commands for this entity
	void					Clear( const Entity* who );

	// call this function to remove all dead commands
	void					RemoveDeadCommands( );

	//
	inline CommandList&		GetCommandList();

	void					SaveEmbedded( ByteStream& stream ) const;
	void					LoadEmbedded( ByteStream& stream, const World* world );

// fields
private:

	CommandList	m_commands;

// implementation
private:
	CommandList::iterator FindTopCommand( const Entity* e );
	void PopCommand( const Entity* e, CommandList::iterator it );

// copy -- do not define
private:
	EntityCommandQueue( const EntityCommandQueue& );
	EntityCommandQueue& operator = ( const EntityCommandQueue& );
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: 
//
inline EntityCommandQueue::CommandList& EntityCommandQueue::GetCommandList()
{
	return m_commands;
}