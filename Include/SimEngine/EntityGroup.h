/////////////////////////////////////////////////////////////////////
// File    : EntityGroup.h
// Desc    : 
// Created : Thursday, February 22, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "SimExports.h"

#include "Entity.h"

#include <Assist/StlExSmallVector.h>

#include <Memory/MemoryFSAlloc.h>

///////////////////////////////////////////////////////////////////// 
// Forward Declarations

class BiFF;
class IFF;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS EntityGroup
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SIM_API EntityGroup
{
// types
public:
	class SIM_API Observer
	{
	public:
		virtual void Notify_Insertion( Entity *e ) = 0;
		virtual void Notify_Removal( Entity *e ) = 0;
	};

private:
	class SIM_API EntityCompare
	{
	public:
		inline bool operator()(const Entity* const l, const Entity* const r) const
			{
			return ( l->GetID() < r->GetID() );
			}
	};

	typedef std::smallvector<Entity*, 4>
		EntityList;

	// since most groups don't have observer, we'll use a vector for storing the set of observers
	// as vectors are the cheapest in memory
	typedef std::vector< Observer* > 
		ObserverSet;

public:
	typedef EntityList::iterator			iterator;
	typedef EntityList::const_iterator		const_iterator;

// construction
public:
	 EntityGroup();
	 EntityGroup(const EntityGroup&);

	~EntityGroup();

	 EntityGroup& operator=(const EntityGroup &g);

// interface
public:
	Entity*			front()			{ return( ( !empty() )? *( m_list.begin() ) : NULL ); }
	const Entity*	front() const	{ return const_cast< EntityGroup* >( this )->front(); }

	iterator		begin()			{ return m_list.begin(); };
	const_iterator	begin() const	{ return m_list.begin(); };
	iterator		end()			{ return m_list.end();	};
	const_iterator	end() const		{ return m_list.end();	};

	// 
	void			push_back( Entity* e );

	iterator		erase ( iterator pos );

	iterator		erase ( iterator f, iterator l )	{ while(l != f) l = erase(--l); return l; };
	void			clear ()							{ erase( begin(), end() ); };

	void			remove( const Entity* e );

	size_t			size() const						{ return m_list.size();	}
	bool			empty() const						{ return m_list.empty(); };

	const_iterator	find( const Entity* e ) const		{ return std::find( m_list.begin(), m_list.end(), const_cast< Entity* >( e ) ); }
	
	bool			issubset( const EntityGroup& ) const;


	// set flags that must be on for the entity to remain in the selection
	bool GetFlag( ENTITY_FLAGS flag ) const;
	void SetFlag( ENTITY_FLAGS flag );
	void ClearFlag( ENTITY_FLAGS flag );

	//
	bool operator== ( const EntityGroup& g ) const { return m_list == g.m_list; };

	// observer interface
	void Register_Observer( Observer* observer );
	void Remove_Observer  ( Observer* observer );

public:

	void Save( BiFF& biff ) const;
	void Load( IFF&  iff, const EntityFactory* pEFac );

	// prefer these two methods
	void SaveEmbedded( ByteStream& stream ) const;
	void LoadEmbedded( ByteStream& stream, const EntityFactory* pEFac );

	// depracted, use above instead
	void SaveEmbedded( IFF& iff ) const;
	void LoadEmbedded( IFF& iff, const EntityFactory* pEFac );

// fields
private:
	// the set itself.
	EntityList			m_list;

	// list of observers
	ObserverSet			m_observers;

	std::bitset< 32 >	m_flags;

// implementation
private:

	// handle registrations and notification whenever an Entity* is added or removed from the list.
	void HandleInsertion(Entity *e);
	void HandleRemoval(Entity *e);

	// notify observers of additions and removals from the group
	void NotifyInsertion(Entity *e);
	void NotifyRemoval(Entity *e);
};

///////////////////////////////////////////////////////////////////// 
// helper functions

void SIM_API EntityGroupToArray( std::vector< EntityIDNumber >& a, const EntityGroup& g );
bool SIM_API EntityGroupHasIntersection( const EntityGroup& groupA, const EntityGroup& groupB );
void SIM_API EntityGroupFilterIntersect( EntityGroup& groupA, const EntityGroup& groupB );