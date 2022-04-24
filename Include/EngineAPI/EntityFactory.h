/////////////////////////////////////////////////////////////////////
// File    : EntityFactory.h
// Desc    : 
// Created : Friday, February 23, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//
// a pure virtual base class for the entity creation

#pragma once

#include <SimEngine/Entity.h>

// forward declarations
class EntityController;
class EntityAnimator;
class Entity;
class ECStaticInfo;
class ControllerBlueprint;

///////////////////////////////////////////////////////////////////////////////
// types

typedef ECStaticInfo*		(*StaticInfoAllocatorCB)( const ControllerBlueprint* );
typedef EntityController*	(*ControllerAllocatorCB)( Entity* entity, const ECStaticInfo* );

///////////////////////////////////////////////////////////////////////////////
//  EntityFactory

class EntityFactory
{
	friend class World;

// types
public:
	class ControllerCreator;

// interface
public:
	// this function bind a factory object for a controller type
		// NOTE: the function takes ownership of the creator object
	virtual void			RegisterController	
								( 
								const char* name, 
								const int controllerType, 
								ControllerCreator* creator
								) = 0;
												
	// override the sizeof(Entity) for allocation memory for entities
	virtual void			SetEntitySize       ( size_t size ) = 0;

	// create entity of specified type
		// the entity factory keeps a pointer to all existing entities
	virtual Entity*			CreateEntity		( const ControllerBlueprint*, float x, float z ) = 0;

	virtual Entity*			CreateEntity		( const ControllerBlueprint*, const Matrix43f& ) = 0;
													
	// return pointer to an existing entity, as specified by its id
	virtual Entity*			GetEntityFromEID	( const EntityIDNumber ) = 0;

	// return pointer to an existing entity, as specified by its id
	virtual const Entity*	GetEntityFromEID	( const EntityIDNumber ) const = 0;

	// return name unless it couldn't find it then it returns NULL
	virtual const char*		GetControllerName	( const int controllerType ) const = 0;
												
	virtual const ControllerBlueprint* 
							GetControllerBP		( const char* dirname, const char* ebpname ) const = 0;

	virtual const ControllerBlueprint* 
							GetControllerBP		( const EBPNetworkID ) const = 0;

	virtual bool			IsEBPNetworkIDValid	( const EBPNetworkID ) const = 0;

	virtual const ECStaticInfo*	
							GetECStaticInfo		( const EBPNetworkID ) const = 0;

	virtual const ECStaticInfo*	
							GetECStaticInfo		( const ControllerBlueprint* ) const = 0;

	// fill the out parm with the <dirname,eboname> of the specified cbp
		// returns false if this is not a combined creature
	virtual bool			GetCombinedSources  
								( 
								const ControllerBlueprint*, 
								std::pair<std::string, std::string>& source0, 
								std::pair<std::string, std::string>& source1, 
								std::string& limbs 
								) const = 0;

	// fill vector with values that can be used to sync between remote computers
		// does NOT clear array
	virtual void			GetSyncToken		( std::vector< std::pair< unsigned long, const char* > >& crcArray ) const = 0;

// construction
protected:
	virtual ~EntityFactory()
		{
		}

// implementation
private:
	virtual void			DeleteEntity		( Entity * ) = 0;

protected:

	inline void	EntityDestroy( Entity* p )
	{
		p->~Entity();
	}

	inline void	EntitySetController( Entity* p, EntityController* control )
	{
		p->SetController( control );
	}

	inline void	EntitySetAnimator( Entity* p, EntityAnimator* anim )
	{
		p->SetAnimator( anim );
	}

//	inline void	EntitySetCollisionRadius( Entity* p, float radius ) 
//	{ 
//		p->SetCollisionRadius( radius );
//	}
};

///////////////////////////////////////////////////////////////////// 
// EntityFactory::ControllerCreator

class EntityFactory::ControllerCreator
{
// construction
public:
	virtual ~ControllerCreator()
	{
	}

// interface
public:
	virtual size_t				StaticInfoSize   () const = 0;
	virtual ECStaticInfo*		StaticInfoCreate ( void* buffer, const ControllerBlueprint* ) const = 0;
	virtual void*				StaticInfoDestroy( ECStaticInfo* ) const = 0;
	
	virtual size_t				ControllerSize   () const = 0;
	virtual EntityController*	ControllerCreate ( void* buffer, Entity* entity, const ECStaticInfo* ) const = 0;
	virtual void* 				ControllerDestroy( EntityController* ) const = 0;
};

///////////////////////////////////////////////////////////////////// 
// EntityFactory_ControllerCreator_Templ

template< class I, class C >
	class EntityFactory_ControllerCreator_Templ : public EntityFactory::ControllerCreator
{
// interface
public:
	virtual size_t StaticInfoSize() const
	{
		return sizeof( I );
	}

	virtual ECStaticInfo* StaticInfoCreate( void* buffer, const ControllerBlueprint* cbp ) const
	{
		return new( buffer ) I( cbp );
	}
	
	virtual void* StaticInfoDestroy( ECStaticInfo* si ) const
	{
		I* p = static_cast< I* >( si );
			p->~I();

		return p;
	}

	virtual size_t ControllerSize() const
	{
		return sizeof( C );
	}

	virtual EntityController* ControllerCreate( void* buffer, Entity* entity, const ECStaticInfo* info ) const
	{
		return new( buffer ) C( entity, info );
	}

	virtual void* ControllerDestroy( EntityController* controller ) const
	{
		C* p = static_cast< C* >( controller );
			p->~C();

		return p;
	}
};
