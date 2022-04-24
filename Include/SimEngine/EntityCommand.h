/////////////////////////////////////////////////////////////////////
// File    : SimEngineCmd.h
// Desc    : 
// Created : Thursday, February 22, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "EntityGroup.h" 

// forward declarations
class World;
class Player;
class Entity;

class Vec3f;

///////////////////////////////////////////////////////////////////// 
// EntityCommand

class EntityCommand
{
// types
public:
	enum CommandType
	{
		CT_Entity,
		CT_EntityPoint,
		CT_EntityEntity,
	};

// construction
protected:
	EntityCommand();

public:
	virtual ~EntityCommand();

	static EntityCommand* CreateCommandEntity
		( 
		const World*			world,
		const unsigned int		cmd, 
		const unsigned long		param,
		const unsigned int		cmdflags, 
		const unsigned long		player,
		const unsigned long*	entities,
		const unsigned long		entitiesCount
		);

	static EntityCommand* CreateCommandEntityPoint
		( 
		const World*			world,
		const unsigned int		cmd, 
		const unsigned long		param,
		const unsigned int		cmdflags, 
		const unsigned long		player,
		const unsigned long*	entities,
		const unsigned long		entitiesCount,
		const Vec3f*			pos,
		const unsigned long		posCount
		);

	static EntityCommand* CreateCommandEntityEntity
		(
		const World*			world,
		const unsigned int		cmd, 
		const unsigned long		param,
		const unsigned int		cmdflags, 
		const unsigned long		player,
		const unsigned long*	entities,
		const unsigned long		entitiesCount,
		const unsigned long*	targets,
		const unsigned long		targetsCount
		);

	static void				SaveEntityCommand( ByteStream& stream, const EntityCommand* pEC );

	static EntityCommand*	LoadEntityCommand( ByteStream& stream, const World* world );

// interface
public:
	CommandType			GetCommandType() const;

	// command 
	unsigned char		GetCommand () const;

	unsigned long		GetParam   () const;

	unsigned char		GetFlags   () const;

	const Player*		GetPlayer  () const;

	const EntityGroup&	GetEntities() const;
	const EntityGroup&	GetOriginalEntities() const;
	const EntityGroup&	GetProcessedEntities() const;

	// call this function after an entity has processed a command
		// the command should be destroyed once all entities have been removed
	virtual void	RemoveEntityFromCommand( const Entity* e );

	// call this function to determine if it should be sent to the specified entity
	virtual bool	IsAddressedToEntity( const Entity* e ) const;

	// call this function to test whether this command should be tossed
	virtual bool	IsCommandDead() const;

	void			AddProcessedEntity( Entity* );
	void			ClearProcessedEntities( );

protected:

	virtual void	SaveEmbedded( ByteStream& stream ) const;
	virtual void	LoadEmbedded( ByteStream& stream, const World* world );

// fields
protected:
	CommandType		m_type;
	
	unsigned char	m_cmd;
	unsigned char	m_cmdflags;
	unsigned long	m_cmdparam;

	const Player*	m_player;

	EntityGroup		m_entities;
	EntityGroup		m_processedEntities;				// stores the group of entities that have process this command this frame.
};

///////////////////////////////////////////////////////////////////// 
// EntityCommand_Entity 

class EntityCommand_Entity : public EntityCommand
{
	friend class EntityCommand;

// construction
private:
	EntityCommand_Entity();

// Load/Save
private:
	virtual void	SaveEmbedded( ByteStream& stream ) const;
	virtual void	LoadEmbedded( ByteStream& stream, const World* world );
};

///////////////////////////////////////////////////////////////////// 
// EntityCommand_EntityPoint

class EntityCommand_EntityPoint : public EntityCommand
{
	friend EntityCommand;

// construction
private:
	EntityCommand_EntityPoint();

// interface
public:
	size_t			GetPosCount() const;
	const Vec3f&	GetPosAt( size_t index ) const;
	const Vec3f*	GetPos() const;

// Load/Save
private:
	virtual void	SaveEmbedded( ByteStream& stream ) const;
	virtual void	LoadEmbedded( ByteStream& stream, const World* world );

// fields
private:
	std::vector< Vec3f >	m_positions;
};

///////////////////////////////////////////////////////////////////// 
// EntityCommand_EntityEntity

class EntityCommand_EntityEntity : public EntityCommand
{
	friend EntityCommand;

// construction
private:
	EntityCommand_EntityEntity();

// interface
public:
	const EntityGroup& GetTargets() const;

	// call this function to test whether this command should be tossed
	virtual bool	IsCommandDead() const;

// Load/Save
private:
	virtual void	SaveEmbedded( ByteStream& stream ) const;
	virtual void	LoadEmbedded( ByteStream& stream, const World* world );

// fields
private:
	EntityGroup m_targets;
};

///////////////////////////////////////////////////////////////////// 
// 

inline EntityCommand::CommandType EntityCommand::GetCommandType() const
{
	return m_type;
}

inline unsigned char EntityCommand::GetCommand() const
{
	return m_cmd;
}

inline unsigned long EntityCommand::GetParam() const
{
	return m_cmdparam;
}

inline unsigned char EntityCommand::GetFlags() const
{
	return m_cmdflags;
}

inline const Player* EntityCommand::GetPlayer() const
{
	return m_player;
}

inline const EntityGroup& EntityCommand::GetEntities() const
{
	return m_entities;
}

inline const EntityGroup& EntityCommand::GetProcessedEntities() const
{
	return m_processedEntities;
}

inline const Vec3f* EntityCommand_EntityPoint::GetPos() const
{
	return &m_positions[0];
}

inline size_t EntityCommand_EntityPoint::GetPosCount() const
{
	return m_positions.size();
}

inline const Vec3f& EntityCommand_EntityPoint::GetPosAt( size_t index ) const
{
	dbAssert( index < m_positions.size() );
	
	return m_positions[index];
}

inline const EntityGroup& EntityCommand_EntityEntity::GetTargets() const
{
	return m_targets;
}
