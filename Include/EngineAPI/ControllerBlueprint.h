/////////////////////////////////////////////////////////////////////
// File    : ControllerBlueprint.h
// Desc    : 
// Created : Friday, February 23, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <Localizer/Localizer.h>

// forward declaration
class Vec3f;
class OBB3f;

///////////////////////////////////////////////////////////////////// 
// Attribute Data

struct AttributeData
{
	// this bodypart chart should match the one in excel exactly
	enum BodyPart 
	{
		part_creature = 0,	// on anylimb, this attribute/ability is always copied across ORed in
		part_stock,			// only in stock creature, not used in combiner
		
		// -- limbs where attributes and abilities can be attached
		part_startlimb,
		part_front_legs = part_startlimb,	
		part_back_legs,
		part_head,
		part_tail,
		part_torso,
		part_wings,
		part_pincers,
		part_endlimb,
		// --
		
		part_unknown,		// should only exist on return of bad data
	};

	float		value;
	BodyPart	bodypart;
};

///////////////////////////////////////////////////////////////////// 
//

typedef long EBPNetworkID;

enum { EBPInvalidNetID = 0 };

typedef std::vector< EBPNetworkID >	EBPNetworkIDList;

/////////////////////////////////////////////////////////////////////
// ControllerBlueprint

// this class gives access to the entity blueprint for the controllers
// on the MOD DLL.

class ControllerBlueprint
{
// types
public:
	struct Ability
	{
		char		attribute[ 128 ];	// attribute name

		wchar_t		name[ 128 ];
		wchar_t		desc[ 512 ];
	};

// fields
protected:
	// for debugging purposes only, this is a pointer to the BPManagerID
	void*	m_pv;

// interface
public:
	// network id -- unique blueprint id that can be sent over the network
	virtual long			GetEBPNetworkID	 () const = 0;

	// 
	virtual unsigned long	GetControllerType() const = 0;

	// variable sized attribute lists, where attributes are key / value pairs
		// return true if attribute exists
	virtual bool			GameAttributeCheck   ( const char* ) const = 0;

		// return the value of the requested attribute, or -666.0f on error
	virtual float			GameAttributeRetrieve( const char* ) const = 0;

		// return number of game attributes
	virtual size_t			GameAttributeCount   () const = 0;

		// return name of requested attribute
	virtual const char*		GameAttributeAt      ( size_t index ) const = 0;

	// 
	virtual const Vec3f&	GetBuildingOffset() const = 0;
	virtual unsigned long	GetBuildingWidth() const = 0;
	virtual unsigned long	GetBuildingHeight() const = 0;

	virtual const OBB3f&	GetOBB(void) const = 0;
	virtual const OBB3f&	GetShrunkenOBB(void) const = 0;

	// ui stuff
		// name to be used in display 
	virtual const wchar_t*	GetScreenName() const = 0;

		// name of icon to display in the taskbar
	virtual const char*		GetIconName() const = 0;
	virtual bool			IsIconValid() const = 0;

		// abilities
	virtual size_t			GetAbilityCount() const = 0;
	virtual void			GetAbilityAt   ( size_t index, Ability& out ) const = 0;

	virtual size_t			GetRangeAttackCount() const = 0;

	// name of the File this ControllerBlueprint comes from
	virtual const char*		GetFileName() const = 0;
};

///////////////////////////////////////////////////////////////////// 
// 

typedef std::vector< const ControllerBlueprint* >	ControllerBPList;
