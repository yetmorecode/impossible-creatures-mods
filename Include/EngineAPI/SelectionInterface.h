/////////////////////////////////////////////////////////////////////
// File    : SelectionInterface.h
// Desc    : 
// Created : Thursday, February 15, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <SimEngine/EntityGroup.h>

class EntityFilter;

///////////////////////////////////////////////////////////////////// 
// SelectionInterface

// NOTE: this class MUST remain an abstract base class, without any 
// non-virtual member function or member variables

class SelectionInterface
{
// types
public:	
	enum { NUM_HOTKEYGROUPS = 10 };

// interface
public:
	virtual void				SetSelection( const EntityGroup& g ) = 0;
	virtual void				SetSelectionOnScreen( const EntityGroup& g ) = 0;
	virtual const EntityGroup&	GetSelection() const = 0;
	virtual bool				NextSubSelection( ) = 0;

	// hotkey group
	virtual void				SetSelectionToHotkeyGroup( int groupNb ) = 0;
	virtual void				AssignHotkeyGroupFromSelection( int groupNb, EntityFilter* ) = 0;
	virtual void				ClearSelectionFromAllHotkeyGroups( ) = 0;

	virtual const EntityGroup&	GetHotkeyGroup( int groupNb ) = 0;

	// observer stuff
	virtual void Register_Observer( EntityGroup::Observer* observer ) = 0;
	virtual void Remove_Observer  ( EntityGroup::Observer* observer ) = 0;
};
