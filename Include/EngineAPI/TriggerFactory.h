/////////////////////////////////////////////////////////////////////
// File    : TriggerFactory.h
// Desc    : 
// Created : Monday, September 17, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <SimEngine/entitygroup.h>

// forward declarations
class TExpression;
class World;
class LuaConfig;

///////////////////////////////////////////////////////////////////// 
// 

struct ESimGroup
{
	int			m_id;
	EntityGroup	m_egroup;
	std::string m_name;
};

typedef TExpression	*(*ExpressionCB)	( );

///////////////////////////////////////////////////////////////////// 
// TriggerFactory

class TriggerFactory
{
// interface
public:
	// this binds a class to an expression def by name
	virtual void			RegisterExpressionCB( const char* name, ExpressionCB ) = 0;
	
	// this loads all the definitions for each condition/ action in
	virtual bool			LoadExpressionDefs( const char* filename ) = 0;
			
	virtual void			Evaluate( World* ) = 0;

	virtual void			ResetKeypressState() = 0;

	// recovering trigger states from saved games
	virtual void			AddStateRecoverer( TExpression* action ) = 0;


	//---------------------------------------------------------
	// entity trigger group access interface
	
	// this lets the triggers get access to groups
	virtual ESimGroup*		GetEGroup( int id ) = 0;

	//---------------------------------------------------------
	// text interface
	virtual const char*		GetText( int id ) const = 0;

	//---------------------------------------------------------
	// flag interface

	// get and set event flags via event type ID
	virtual void			SetEventFlag(int id, int val) = 0;
	virtual int				GetEventFlag(int id) = 0;

	//---------------------------------------------------------
	// export functions to Lua
	virtual void			Register( LuaConfig* lc ) = 0;
	virtual void			Unregister( LuaConfig* lc ) = 0;
};
