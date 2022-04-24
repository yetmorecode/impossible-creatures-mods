/////////////////////////////////////////////////////////////////////
// File    : EntityFilter.h
// Desc    : 
// Created : Friday, June 29, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

class Entity;
class EntityGroup;

class EntityFilter
{
public:
	enum FilterMode
	{
		FILTER_Select,
		FILTER_SingleSelect,
		FILTER_Bandbox,
		FILTER_Context,
		FILTER_LocalPlayer,
		
		FILTER_Invalid,
		FILTER_Count = FILTER_Invalid,
	};
	virtual void		SetMode( FilterMode ) = 0;
	virtual void		ClearMode() = 0;

	virtual bool		QueryFilter( const Entity* ) const = 0;
		//	QueryFilter() checks if the given entity is allowed in the current FilterMode.

	virtual int			QueryPriority( const Entity* ) const = 0;
		//	QueryPriority() returns a positive value that can be used for selection priority
		//	for overlapping queries.

	virtual void		FilterGroup( EntityGroup& ) const = 0;
		//	FilterGroup() allows the EntityFilter to remove any entities that pass
		//	QueryFilter() but have conflicts with other Entities in the group.  An
		//	example would be for single select entities, in which case FilterGroup()
		//	would remove all the single select entities if there are multi-select
		//	entities in the group.

	virtual void		SetContextGroup( const EntityGroup& ) = 0;
	virtual void		ClearContextGroup() = 0;
		//	SetContextGroup() allows the EntityFilter to build a reference to filter
		//	entities against.  For example, if the EntityGroup contains only entities
		//	that have a default action that can be performed on allies (healing, repair, 
		//	transport, etc), we then allow the QueryFilter() to pass only allies.
		//	ClearContextGroup() clears the list.

	virtual void		SetExclusionGroup( const EntityGroup& ) = 0;
	virtual void		ClearExclusionGroup() = 0;
		//	SetExclusionGroup() enables a list of entities that will always fail QueryFilter().
		//	ClearExclusionGroup() clears that list.
};