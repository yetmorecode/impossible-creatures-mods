/////////////////////////////////////////////////////////////////////
// File    : ModCommandVI.h
// Desc    : 
// Created : Thursday, February 15, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

// forward declaration
class Vec3f;

class Player;
class EntityGroup;

///////////////////////////////////////////////////////////////////// 
// CommandInterface

// NOTE: this class MUST remain an abstract base class, without any 
// non-virtual member function or member variables

// all these functions take the same set of parameters
//
// cmd      : identifies the type of action to be performed
//            the types are unique value, defined by the current mod
// 			  examples: gather, build, attack, move, patrol...
// 			  NOTE: VALUE MUST BE <= 255
//
// player   : identifies the player that issued the command
// 			  NOTE: ID MUST BE <= 65535
//
// parm     : command specific value
//
// flags    : mod specific value
// 			  NOTE: VALUE MUST BE <= 255
//
// entities : the list of entities that should execute this command
//
// positions: the list of positions needed for this command. ie: waypoints
//
// targets  : the list of targets for this command. ie: units to attack

class CommandInterface
{
// interface
public:
		// example: gather
	virtual void DoEntity
		( 
		const unsigned int				cmd,
		const unsigned long				cmdparm,
		const unsigned int				cmdflags,
		const Player*					sender,
		const EntityGroup&				entities
		) = 0;

		// example: move to
	virtual void DoEntityPoint
		( 
		const unsigned int				cmd,
		const unsigned long				cmdparm,
		const unsigned int				cmdflags,
		const Player*					sender,
		const EntityGroup&				entities,
		const Vec3f*					positionsArray,
        const size_t					positionsCount
		) = 0;

		// example: attack
	virtual void DoEntityEntity
		( 
		const unsigned int				cmd,
		const unsigned long				cmdparm,
		const unsigned int				cmdflags,
		const Player*					sender,
		const EntityGroup&				entities,
		const EntityGroup&				targets
		) = 0;

		// example: research
	virtual void DoPlayerPlayer
		( 
		const unsigned int				cmd,
		const unsigned long				cmdparm,
		const unsigned int				cmdflags,
		const Player*					sender,
		const Player*					dest
		) = 0;

		// example: build structure
	virtual void DoPlayerEntity
		( 
		const unsigned int				cmd,
		const unsigned long				cmdparm,
		const unsigned int				cmdflags,
		const Player*					sender,
		const Player*					dest,
		const EntityGroup&				entities,
		const Vec3f*					positionsArray,
        const size_t					positionsCount
		) = 0;

	enum CommandType
	{
		CT_DefaultCommand = 0
	};
};
