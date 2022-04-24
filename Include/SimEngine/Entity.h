/////////////////////////////////////////////////////////////////////
// File    : Entity.h
// Desc    : 
// Created : Monday, May 28, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// * a basic Entity class for Sigma

#pragma once

#include <Assist/StlExSmallVector.h>

#include "SimExports.h"
#include "SimEngineTypes.h"

#include <EngineAPI/ControllerBlueprint.h>

#include <Math/Matrix43.h>
#include <Math/Vec2.h>

#include <SurfVol/Obb3.h>

#include <Memory/MemoryFSAlloc.h>
#include <Memory/MemoryAlloc.h>

// forward declarations.
class Player;
class Entity;
class EntityGroup;
class EntityController;
class ControllerBlueprint;
class EntityAnimator;
class Group;
class BiFF;
class IFF;
class ByteStream;
class ChunkNode;

///////////////////////////////////////////////////////////////////// 
// 

enum ENTITY_FLAGS
{
	EF_Selectable = 0,
	EF_SingleSelectOnly,
	EF_CanCollide,
	EF_IsVisible,
	EF_IsSpawned,
	EF_IsSaint,
};


enum ENTITYLOAD_FLAGS
{
	ELF_SkipSpawn = 0,
};

///////////////////////////////////////////////////////////////////// 
// Entity

class SIM_API Entity
{
	friend class EntityFactory;
	friend class World;
	friend class Player;

// types
private:
	typedef std::vector< EntityGroup*, mempool_alloc<EntityGroup*> >
		ObserverList;

// construction
public:
	Entity(EntityIDNumber EID, const ControllerBlueprint* pCBP);
	virtual ~Entity();

// interface
public:

//**********************************
// THIS WAS PUT BACK IN FOR THE AMBIENT SOUND EMMITERS EXPORTED
// FROM THE MISSION EDITOR, NEED A BETTER WAY TO DO THIS ***  SA 12/21/00
	inline void SetSoundID(long id)	{m_SoundID = id;}
	inline long GetSoundID(void) const {return m_SoundID;}
//**********************************

	// Get a particular flag from the flags bitfield
	void	ClearEntityFlag( ENTITY_FLAGS flag );
	bool	GetEntityFlag  ( ENTITY_FLAGS flag ) const;
	void	SetEntityFlag  ( ENTITY_FLAGS flag );

	// Get the entity's unique ID.
	EntityIDNumber		GetID() const;

	//
	const ControllerBlueprint* 
						GetControllerBP() const;

	// Get the player owner for this entity
	const Player*		GetOwner() const;
	Player*				GetOwner();

	// Get transform and position
	const Matrix43f&	GetTransform() const;
	const Matrix43f&	GetPrevTransform() const;
	const Matrix43f&	GetPrevPrevTransform() const;

	const Vec3f&		GetPosition() const;
	const Vec3f&		GetPrevPosition() const;
	const Vec3f&		GetPrevPrevPosition() const;

	const OBB3f&		GetOBB() const;
	const OBB3f&		GetShrunkenOBB() const;

	// Set transform and position data
	void				SetPosition( const Vec3f& v );
	void				SetTransform( const Matrix43f &mat );
	void				SetPrevTransform( const Matrix43f &mat );
	void				SetPrevPrevTransform( const Matrix43f &mat );

	void				RotateTransforms( );

	const EntityController*	
						GetController() const;
	EntityController*	GetController();

	// if something has a const Entity *They* are allowed to edit the animator
	EntityAnimator*		GetAnimator() const;

	// Observer functionality.
	void				RegisterObserver	(EntityGroup* EGroup);
	void				RemoveObserver		(EntityGroup* EGroup);
	size_t				ObserverCount		( ) const;

	bool				GetEntityLoadFlag( ENTITYLOAD_FLAGS flag );

// io
public:
	// NOTE: you MUST call the base class version of this function first	
	virtual void Save( BiFF& biff ) const;
	virtual void Load( IFF& iff );

	// NOTE: you MUST call the base class version of this function first	
	virtual void SaveBasicEmbedded( IFF& iff ) const;
	virtual void LoadBasicEmbedded( IFF& iff );

	// This function will save out the EntityID to a chunk
	void SaveEIDChunk( BiFF& biff ) const;

	// This function will process a previously saved EntityID chunk and set the *ppEntity pointer to
	// the correct Entity
	static void LoadEIDChunk( IFF& iff, const EntityFactory*, Entity** ppEntity );

// fields
private:

//**********************************
// THIS WAS PUT BACK IN FOR THE AMBIENT SOUND EMMITERS EXPORTED
// FROM THE MISSION EDITOR, NEED A BETTER WAY TO DO THIS ***  SA 12/21/00
	long						m_SoundID;
//**********************************

	//
	const EntityIDNumber		m_id;
	const ControllerBlueprint* const	
								m_cbp;

	//
	Player*						m_Owner;
								
	Matrix43f*					m_TransPtr;
	Matrix43f*					m_PrevTransPtr;
	Matrix43f*					m_PrevPrevTransPtr;

	Matrix43f					m_Transforms[3];
//	Matrix43f					m_PrevTrans;
//	Matrix43f					m_PrevPrevTrans;
								
	// strong pointers, but the entity must NOT delete these
	EntityController			*m_Controller;
	EntityAnimator				*m_Animator;
								
	std::bitset< 32 >			m_EntityFlags;

	//
	ObserverList				m_ObservingGroups;

	std::bitset< 32 >			m_EntityLoadFlags;

// io
private:
	static unsigned long HandleBSEI( IFF&, ChunkNode*, void*, void* );

// implementation
private:
	// only the player can set this, that way you *must* call AddEntity / RemoveEntity to change the owner
	void SetOwner			( Player *pPlayer );

	void SetController		( EntityController *pCont );
	void SetAnimator		( EntityAnimator *pAnim );

	void OnPositionChange	( );

// copy -- do not define
private:
	Entity( const Entity& );
	Entity& operator= ( const Entity& );
};

///////////////////////////////////////////////////////////////////// 
// inlines

inline bool Entity::GetEntityFlag( ENTITY_FLAGS flag ) const 
{ 
	return m_EntityFlags.test( flag ); 
}

inline EntityIDNumber Entity::GetID() const 
{
	return m_id;
}

inline const ControllerBlueprint* Entity::GetControllerBP() const 
{
	return m_cbp;
}

inline const Player* Entity::GetOwner() const 
{
	return m_Owner;
}

inline Player* Entity::GetOwner() 
{
	return m_Owner;
}

inline const Matrix43f& Entity::GetTransform() const 
{
	return *m_TransPtr;
}

inline const Matrix43f& Entity::GetPrevTransform() const 
{
	return *m_PrevTransPtr;
}

inline const Matrix43f& Entity::GetPrevPrevTransform() const 
{
	return *m_PrevPrevTransPtr;
}

inline const Vec3f& Entity::GetPosition() const 
{
	return m_TransPtr->T;
}

inline const Vec3f& Entity::GetPrevPosition() const 
{
	return m_PrevTransPtr->T;
}

inline const Vec3f& Entity::GetPrevPrevPosition() const 
{
	return m_PrevPrevTransPtr->T;
}

inline const OBB3f& Entity::GetOBB() const 
{ 
	return m_cbp->GetOBB(); 
}

inline const OBB3f& Entity::GetShrunkenOBB() const 
{ 
	return m_cbp->GetShrunkenOBB(); 
}

inline void Entity::SetPosition( const Vec3f& v ) 
{ 
	m_TransPtr->T = v;
	OnPositionChange();
}

inline void Entity::SetTransform( const Matrix43f &mat ) 
{ 
	*m_TransPtr = mat; 
	OnPositionChange();
}

inline void Entity::SetPrevTransform( const Matrix43f &mat ) 
{ 
	*m_PrevTransPtr = mat; 
}

inline void Entity::SetPrevPrevTransform( const Matrix43f &mat ) 
{	
	*m_PrevPrevTransPtr = mat; 
}

inline const EntityController*	Entity::GetController() const
{
	return m_Controller;
}

inline EntityController* Entity::GetController() 
{
	return m_Controller;
}

inline EntityAnimator* Entity::GetAnimator() const 
{
	return m_Animator;
}

inline void Entity::RegisterObserver(EntityGroup *EGroup)
{
	// validate parm
	dbAssert( EGroup != 0 );

#if defined( RELIC_DEBUG )
	ObserverList::iterator found = 
		std::find( m_ObservingGroups.begin(), m_ObservingGroups.end(), EGroup );

	dbAssert( found == m_ObservingGroups.end() );
#endif

	m_ObservingGroups.push_back( EGroup );
	
	return;
}

inline size_t Entity::ObserverCount( ) const
{
	return m_ObservingGroups.size();
}
