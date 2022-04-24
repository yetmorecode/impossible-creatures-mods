/////////////////////////////////////////////////////////////////////
// File    : Player.h
// Desc    : 
// Created : Monday, May 28, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "SimExports.h"

// forward declaratrion
class Entity;
class EntityGroup;
class ControllerBlueprint;
class IFF;
class ChunkNode;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS: Player
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SIM_API Player
{
// types
public:
	typedef unsigned long PlayerID;

	static const PlayerID InvalidID;

// fields
private:
	const PlayerID	m_id;	

	EntityGroup*	m_entities;

	bool			m_isDead;

	std::wstring	m_name;

// construction
public:
	Player( );
	virtual ~Player();

// interface
public:
	PlayerID				GetID() const;
	void					SetID(PlayerID id );
							
	const wchar_t*			GetName() const;
	virtual void			SetName( const wchar_t* name );
							
	bool					IsPlayerDead() const;

	// entity access functions.
	const EntityGroup&		GetEntities() const;
							
	virtual void			AddEntity   (Entity* e);
	virtual void			RemoveEntity(Entity* e);

	virtual bool			CanControlEntity( const Entity* pEntity ) const;

	virtual size_t			GetNumCBPEntities( const ControllerBlueprint* ) const;

	virtual void			PreFirstSimulate();

	virtual void			KillPlayer( int reason );

	virtual void			SetArmy( const std::vector< long >& soldiers );
	virtual const std::vector< long >&
							GetArmy( ) const;

	// commands
		// NOTE: these have empty implementation, they should be redefined
		// in the mod player
	virtual void			CommandDoProcess
								(
								const unsigned int		cmd, 
								const unsigned long		param,
								const unsigned int		flags,
								Player*					sender
								);

	virtual void			CommandDoProcess
								(
								const unsigned int		cmd, 
								const unsigned long		param,
								const unsigned int		flags,
								Player*					sender,
								const EntityGroup&		entities,
								const Vec3f*			pos,
								const size_t			posCount
								);

// Save and Load functions.  You MUST call the base class version of this function first
public:

	virtual void Save( IFF& iff ) const;
	virtual void Load( IFF& iff );

// Chunk Handlers
private:

	static unsigned long HandleSIMP( IFF&, ChunkNode*, void*, void* );

// copy -- do not define
private:
	Player( const Player& );
	Player& operator= ( const Player& );
};
