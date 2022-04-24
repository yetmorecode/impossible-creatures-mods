// EntityAnimator.h

// This is the Base EntityAnimator class. The EntityAnimator class cannot be instantiated
//	itself, but it provides a base framework for Animators to fit into. All Entity Animators
//	will be derived from this class.
#pragma once

#include "SimExports.h"
#include "EntityGroup.h"

#include <Math/Matrix43.h>


/////////////////////////////////////////////////////////////////////
// Forward Declarations
class Entity;
class EntityGroup;

typedef unsigned long AnimStyle;			//	four byte string, eg 'ATTK','MOVE','IDLE'

class SIM_API EntityAnimator
{
// types
public:
	enum DecoratorFlags
	{
		DF_DrawSelected		= 0x00000001,		// Draw this decorator only when unit is selected
		DF_Visible			= 0x00000002,		// When set to true it's visible, when false it is invisible
	};

	enum DecoratorTypes
	{
		DT_PowerBar = 0,
		DT_Flag,
	};

	enum SelIntersectTypes
	{
		SIT_OverSizeBV		= 0x1,	// animated bv 
		SIT_ChildBVs		= 0x2,	// animated bone bvs 
		SIT_GroundSelect	= 0x4,	// the GroundSelect footprint
	};

	enum DrawStyle
	{
		DS_OrientSim,
		DS_OrientGround,
		DS_OrientVertical,
	};

	typedef unsigned long EventHandle;
	typedef unsigned long EffectHandle;
	typedef unsigned long DecalHandle;

// fields
protected:

	// i use an entitygroup so it cleans up properly
//	EntityGroup				m_target;
//	float					m_radius;

	bool					m_bFullbright;
	bool					m_bInFOW;
	float					m_opacity;

	// blow stuff up (straight up)
	float					m_damageTime;
	float					m_damageVel;
	float					m_damageOfs;

	// Selection related fields
	unsigned long			m_selectionIntersectionFlags;
	unsigned long			m_footprintWidth,
							m_footprintHeight;

	DrawStyle				m_DrawStyle;

// fields
private:
	const Entity			*m_Entity;
	
// construction
public:
	EntityAnimator( const Entity *e );
	virtual ~EntityAnimator();

// interface, ALL functions declared in the public section should be write only
// in other words the simulation can write whatever it want's to the animation system
// but it should *NEVER* read from the animation system
public:
	const Entity*			GetEntity() const		{	return m_Entity;	}

	// Function implementations to be written by animators dervied from this base class.
	virtual void			SimUpdate() {;}

	// This function is called once before any simulations are done.
	// It gives the animator a chance to initialize itself after the Entity has been fully
	// loaded and initialized.
	virtual void			PreFirstSimulate() {;}

	//	Animation control system
	virtual void			SetStyle( AnimStyle )	
		{;}																//	PROTOTYPE : Sets the animation style
	virtual void			ResetStyle( AnimStyle )	
		{;}																//	PROTOTYPE : Sets the animation style and makes it start again
	virtual void			SetMotionTreeNode( const char *)	
		{;}																//	PROTOTYPE : Sets motion tree node
	virtual void			ResetMotionTreeNode( const char *)	
		{;}																//	PROTOTYPE : Sets motion tree node and makes it start again
	virtual void			SetMotionVelocity( float velocity)	
		{ UNREF_P( velocity );}											//	PROTOTYPE : Sets the animation velocity
	virtual void			SetPlaybackRateForPlayingAnims( float rate )
		{ UNREF_P( rate ); }											//	PROTOTYPE : Sets playback rate
	virtual void			ClearPlayingAnims( ) {;}					//	PROTOTYPE : Clears any playing animations
	virtual void			SetTargetLook( const Entity* ) {;}	//	PROTOTYPE : Sets the target of the animation
	virtual void			ChooseTargetLook( const EntityGroup &) {;}		//	PROTOTYPE : Chooses a target from group of entities
	virtual void			SetSpineBendAngle( float ) {;}	//	PROTOTYPE : Sets angle for spine bending when creature turns corners
	virtual void			SetMotionVariable( const char *, float)	{;}		//	PROTOTYPE : Sets animation variable
	virtual void			SetDefaultMotion() 	{;}

	//
	//	Event system : triggers all the AnimEvents in 'filename' (see RenderAnim/AnimEvent.h)
	//
	virtual EventHandle		FireEvent( const char* filename ) = 0;
	virtual void			DeleteEvent( EventHandle ) = 0;
	virtual void			SetEventVisibility( EventHandle, bool visible ) = 0;
	//	Allows "%variable" event parameters to be modified by the mod
	virtual void			SetEventVariable( const char* var, const char* value ) = 0;

	//
	//	Entity FX system
	//
	virtual EffectHandle	AttachEffect( const char* name, 
									const char* location = NULL, 
									unsigned long count = 1, 
									float scale = 1 ) = 0;
	virtual void			DeleteEffect( EffectHandle ) = 0;
	virtual void			SetEffectVisibility( EffectHandle, bool visible ) = 0;
	//	This specifies a target for this animator's effect to work with
	virtual void			SetEffectsTarget( Entity* ) = 0;
	virtual void			SetEffectsTarget( const Vec3f& ) = 0;
	//	Sets the radius of an effect bound using "fx_scale=radius"
	virtual void			SetEffectsRadius( float r ) = 0;
	
	//
	//  Entity decals
	//
	virtual DecalHandle		AttachDecal( const char* shaderName, float scale, bool bVisible, bool bInheritAnimVis ) = 0;
	virtual void			DeleteDecal( DecalHandle ) = 0;
	virtual void			SetupDynamicDecal( DecalHandle handle, 
									float fadeInTime, 
									float fadeOutTime, 
									float lifeTime,
									bool indefiniteLife ) = 0;
	virtual void			UpdateAllDecals() = 0;

	//	HACK : sets the radius the culling system uses (for stink and electric domes that vary in size)
	virtual void			SetMinimumBVRadius( float r ) = 0;

	//	Animator model effects
	void					SetFullbright( bool b )		{	m_bFullbright = b;	}
	void					SetInFOW( bool b )			{	m_bInFOW = b;		}
	void					SetOpacity( float o )		{	m_opacity = o;		}
	virtual void			SetOccludee( bool b ) = 0;		//	Will this entity turn colour when blocked
	void					SetDamageVelocity( float v) {	m_damageVel = v; m_damageTime = 0;	}
	float					GetDamageOffset() const		{	return m_damageOfs;	}

	// DrawStyle
	void					SetDrawStyle( DrawStyle ds ) { m_DrawStyle = ds; }
	DrawStyle				GetDrawStyle() const	{ return m_DrawStyle; }

	// EntityAnimator Decorator creation and update System
	// This is used by the simulation to enable things like health bars, flags etc...
	// Create a decorator based on these parametrs, and hold onto the ID to update it's status
	virtual unsigned long	CreateDecorator( unsigned long type, const char *resource, float value, unsigned long flags ) = 0;
	virtual void			UpdateDecorator( unsigned long id, float value, unsigned long flags ) = 0;
	virtual void			UpdateDecorator( unsigned long id, float value ) = 0;

	virtual void			SetSelectionIntersection( unsigned long );
	virtual unsigned long	GetSelectionIntersection() const;

	// Set ground selection size, the width and height specified are in meters
	virtual void			SetGroundSelectInfo( unsigned long footprintWidth, unsigned long footprintHeight );

	// called whenever the entities position changes
	virtual void			OnEntityPosChanged( ) = 0;
};
