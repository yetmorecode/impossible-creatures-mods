/////////////////////////////////////////////////////////////////////
// File    : World.h
// Desc    : 
// Created : Thursday, February 22, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// A Simulation of our game world

#pragma once

#include "SimExports.h"
#include "SimEngineTypes.h"
#include "EntityGroup.h"

#include "SyncDefines.h"

// forward declarations
class EntityCommand;
class EntityCommandQueue;
class EntityFactory;
class TerrainHMBase;
class SpatialBucketSystem;
class SimEngineInterface;
class Pathfinder;
class ControllerBlueprint;
class Vec3f;
class FindClosestFilter;
class BiFF;
class IFF;
class LogFile;

class GroupController;


/////////////////////////////////////////////////////////////////////////////////////////
// World
/////////////////////////////////////////////////////////////////////////////////////////

class SIM_API World
{
	friend class RenderWindow;

// types
public:
	
	enum
	{
		PLAYERMAX = 6,
	};

	enum PlayerRelationship
	{
		PR_Enemy,
		PR_Ally,
		PR_Neutral
	};

	// NOTE: we use a vector to remove the unnecessary overhead of a list or set
	typedef std::vector< Entity* > EntityList;

	// a storage structure for controllers.
	typedef std::vector<EntityController *>	EntityControllerList;

	// a storage structure for group controllers.
	typedef std::vector<GroupController *>	GroupControllerList;

// construction
public:
	World( SimEngineInterface *pSimInterface );
	virtual ~World();

// interface
public:

	/////////////////////////////////////////////////////////////////////////////////////
	// These functions can be over riden to enhance their function
	// BUT a inherited World *MUST* call the base class functions
	/////////////////////////////////////////////////////////////////////////////////////

	virtual void Simulate();

	// called before the first frame of simulation, after everything is loaded
	virtual void SimulatePre ();

	// called after the last frame of simulation, before destroying the world
	virtual void SimulatePost();

	// called when the terrain is loaded.  The pathfinding info can be created in this call
	virtual void SetTerrain( TerrainHMBase* Terrain );

	// calculate a crc value for the world and all entities
	// does NOT clear array
	virtual void GetSyncToken( std::vector< std::pair< unsigned long, const char* > >& crcArray ) const;

	// This function will give control of the entity to the world.
	// It will add the entities controller to a newborn list
	// that will then be added to the simulation in the next simulate call.
	void SpawnEntity(Entity *e);

	// first step of the two step spawn
	virtual void DoSpawnEntity( Entity *e );

	// second step of the two step spawn
	virtual void OnSpawnEntity( Entity *e );

	// This function will remove the entity from the world.
	// This would be used if one entity needed to be removed from the world and
	// placed inside of another entity, ala docking in Homeworld or the
	// fighters docking inside of the carrier in Starcraft
	virtual void DeSpawnEntity(Entity *e);

	// This function will remove the entity from the world and then delete it.
	// It calls DeSpawnEntity to do so
	virtual void DeleteEntity(Entity *e);

	// this function is called when entities are loaded
	void RestoreEntity( Entity *e );

	// first step of the two step restore
	virtual void DoRestoreEntity( Entity *e );

	// second step of the two step restore
	virtual void OnRestoreEntity( Entity *e );

	// This is called directly after the game is loaded, the MOD should load all of the EBPs
	// that it needs at this point, so that their network ID's are assigned deterministically
	virtual void PreLoadEBPS( );

	// This will return true if the CBP is referenced by the world
	virtual bool CBPHasReference( long CBPNetID ) const;

	// default behavior let all players see everything
	virtual bool IsEntityVisible( const Player* localPlayer, const Entity* entity ) const;

	virtual bool IsEntitySpawned( const Entity* pEntity ) const;

	// This function will add the entity to pathfinding in the next simulate call.
	// This function is used to re-introduce an entity to the simulation after 
	// a call to SimDeSpawnEntity.
	virtual void AddEntityPathfinding(Entity *, bool) {}

	// This function will remove the entity from pathfinding in the next simulate call.
	// It is useful for removing an entity from the impass maps while leaving it visible.
	virtual void RemEntityPathfinding(Entity *) {}

	// asks if the two players have the given relationship
	virtual bool QueryRelationship( const Player* p1, const Player* p2, PlayerRelationship rel ) const;

	///////////////////////////////////////////////////////////////////// 
	// Command Querry functions
	///////////////////////////////////////////////////////////////////// 
	
	bool					EntityHasCommand( Entity* );

	/////////////////////////////////////////////////////////////////////////////////////
	// commands
	/////////////////////////////////////////////////////////////////////////////////////

	void					DoCommandEntity
								(
								const unsigned int		cmd,
								const unsigned long		cmdparm,
								const unsigned int		cmdflags,
								const unsigned long		sender,
								const unsigned long*	entities,
								const unsigned long		entitiesCount
								);

	void					DoCommandEntityPoint
								(
								const unsigned int		cmd,
								const unsigned long		cmdparm,
								const unsigned int		cmdflags,
								const unsigned long		sender,
								const unsigned long*	entities,
								const unsigned long		entitiesCount,
								const Vec3f*			pos,
								const unsigned long		posCount
								);

	void					DoCommandEntityEntity
								(
								const unsigned int		cmd,
								const unsigned long		cmdparm,
								const unsigned int		cmdflags,
								const unsigned long		sender,
								const unsigned long*	entities,
								const unsigned long		entitiesCount,
								const unsigned long*	targets,
								const unsigned long		targetsCount
								);

	void					DoCommandPlayerPlayer
								(
								const unsigned int		cmd,
								const unsigned long		cmdparm,
								const unsigned int		cmdflags,
								const unsigned long		sender,
								const unsigned long		dest
								);

	void					DoCommandPlayerEntity
								(
								const unsigned int		cmd,
								const unsigned long		cmdparm,
								const unsigned int		cmdflags,
								const unsigned long		sender,
								const unsigned long		dest,
								const unsigned long*	entities,
								const unsigned long		entitiesCount,
								const Vec3f*			pos,
								const unsigned long		posCount
								);

// ME Interface
public:

	// Caled when you want dbWarning messages generated on invalid world state (used for the MissionEditor warnings Dialog)
	virtual void				GenerateWarnings( ) const;

protected:
	virtual void				OnProcessEntityCommand( const EntityCommand* ec );

	const EntityCommand*		GetTopEntityCommand( Entity* );

	// adds a GroupController to the world
	virtual void				AddGroupController( GroupController * );

	// call to create a GroupController of the given type
	virtual GroupController*	CreateGroupController( long typeID );

	// called when a GroupController is no longer needed and should be deleted
	virtual void				DeleteGroupController( GroupController * );

	// called to create groups from commands
	virtual void				OnGroupCommandProcess( const EntityCommand* ec );


	// This function should only be called by the mission editor.
	// It contains the entity spawning and deletion code and that's it.
	// The MissionEditor needs this so that it can work properly.
	virtual void				MESimulate( );

	// This is called to log deterministic Entity information used to debug sync errors
	virtual void				SyncLogEntity( LogFile& logfile, const Entity *pEntity ) const;

	/////////////////////////////////////////////////////////////////////////////////////
	// timing
	/////////////////////////////////////////////////////////////////////////////////////
public:
	// amount of time to advance per "simulate".
	float					GetTimeDelta() const;
	float					GetNumSimsPerSecond() const;

	// Get the total elapsed game time
	float					GetGameTime() const;
	long					GetGameTicks() const;

protected:
	void					SetSimsPerSecond( float simsPerSecond );

	/////////////////////////////////////////////////////////////////////////////////////
	// status
	/////////////////////////////////////////////////////////////////////////////////////
public:
	// Is game over call
	bool					IsGameOver () const;
	virtual void			SetGameOver();

	void					SetSyncLogging();
	bool					IsSyncLogging() const;

#if EXTREME_SYNC_LOGGING
protected:
	LogFile&				GetSyncLog() const;
#endif

	/////////////////////////////////////////////////////////////////////////////////////
	// entities
	/////////////////////////////////////////////////////////////////////////////////////
public:
	//
	const EntityFactory*	GetEntityFactory() const;
	EntityFactory*			GetEntityFactory();

	int						GetNumEntities() const;

	const EntityList&		GetEntities() const;

	const EntityControllerList&	
							GetEntityControllerList() const;

	/////////////////////////////////////////////////////////////////////////////////////
	// players
	/////////////////////////////////////////////////////////////////////////////////////

	// takes ownership of pointer
	void					AddPlayer( Player* );
	void					RmvPlayer( unsigned long playerId );
	
	size_t					GetPlayerCount() const;
	const Player*			GetPlayerAt   ( size_t index ) const;
	Player*					GetPlayerAt   ( size_t index );

	const Player*			GetPlayerFromID( unsigned long playernum ) const;
	Player*					GetPlayerFromID( unsigned long playernum );

	/////////////////////////////////////////////////////////////////////////////////////
	// other
	/////////////////////////////////////////////////////////////////////////////////////

	const TerrainHMBase*	GetTerrain() const;
	TerrainHMBase*			GetTerrain();
	
	void					SetRandomSeed( unsigned long seed );
	float					GetRand();
	unsigned long			GetRandMax( unsigned long max );
	
	void					LockRandom();
	void					UnLockRandom();

	void					CumulateDynamicsTimeBegin();
	void					CumulateDynamicsTimeEnd();

	GroupController*		GetGroupControllerFromID( int id );

	/////////////////////////////////////////////////////////////////////////////////////
	// save/load
	/////////////////////////////////////////////////////////////////////////////////////

	virtual void			Save( BiFF& biff ) const;
	virtual void			Load( IFF& iff );

// fields	
private:
	class Data;
	Data* m_pimpl;

private:

	// remove an Entity from the world, when the world is ready to release it.
		// return an iterator to the next entity
	EntityList::iterator	RemoveEntity( EntityList::iterator );

	EntityControllerList::iterator 
							RemoveEntityController( EntityControllerList::iterator );

	void					SpawnNewbornControllers();

	void					RemoveSaintControllers();

	void					DeleteDeadEntities();

	void					UpdateControllers();

	void					ProcessEntityCommand( EntityCommand* );

	void					CreateGroupsFromProcessedCommands();
	void					GroupControllersUpdate();
	void					GroupControllersExecute();
	void					IndividualControllersExecute();

private:
	static unsigned long HandleBWLD( IFF&, ChunkNode*, void*, void* );
	static unsigned long HandleGLST( IFF&, ChunkNode*, void*, void* );
	static unsigned long HandleGCRC( IFF&, ChunkNode*, void*, void* );
	static unsigned long HandleGCTI( IFF&, ChunkNode*, void*, void* );
	static unsigned long HandleGCIC( IFF&, ChunkNode*, void*, void* );
	static unsigned long HandleGCDI( IFF&, ChunkNode*, void*, void* );

};


/////////////////////////////////////////////////////////////////////
//	Desc.	: Utility function that issues a World::DoCommandEntityEntity but takes EntityGroups as parameters
//	Param.	: pWorld - the world instance
//			  The same params as World::DoCommandEntityEntity
//
extern void SIM_API WorldDoCommandEntity
	(
	World* world,
	const unsigned int		cmd,
	const unsigned long		cmdparm,
	const unsigned int		cmdflags,
	const unsigned long		sender,
	const EntityGroup&		entities
	);

extern void SIM_API WorldDoCommandEntityEntity
	(	
	World* world,
	const unsigned int		cmd,
	const unsigned long		cmdparm,
	const unsigned int		cmdflags,
	const unsigned long		sender,
	const EntityGroup&		entities,
	const EntityGroup&		targets
	);

extern void SIM_API WorldDoCommandEntityPoint
	(	
	World* world,
	const unsigned int		cmd,
	const unsigned long		cmdparm,
	const unsigned int		cmdflags,
	const unsigned long		sender,
	const EntityGroup&		entities,
	const Vec3f*			pos,
	const unsigned long		posCount
	);

extern void SIM_API WorldDoCommandPlayerEntity
	(
	World* world,
	const unsigned int		cmd,
	const unsigned long		cmdparm,
	const unsigned int		cmdflags,
	const unsigned long		sender,
	const unsigned long		dest,
	const EntityGroup&		entities,
	const Vec3f*			pos,
	const unsigned long		posCount
	);
