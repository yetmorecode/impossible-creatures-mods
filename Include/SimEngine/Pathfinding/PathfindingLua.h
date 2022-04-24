/////////////////////////////////////////////////////////////////////
// File    : PathfindingLua.h
// Desc    : 
// Created : Thursday, June 20, 2002
// Author  : 
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h" 

#include <Lua/LuaLibMan.h>

///////////////////////////////////////////////////////////////////// 
// 

extern SIM_API const char* LUALIB_PATHFINDING;

///////////////////////////////////////////////////////////////////// 
// 

// set following define to 1 to allow GetVerboseLevel() calls to be compiled into the code
#define VERBOSE_PATHFINDING 1

// set the following define to 1 to allow GetAnalLevel() calls to be compiled into the code
#define ANAL_PATHFINDING 1

// set the following define to 1 to allow GetDebugLevel() calls to be compiled into the code
#define DEBUG_PATHFINDING 1

///////////////////////////////////////////////////////////////////// 
// 

class LuaPathfindingLib : public LuaLibrary
{
// 
private:

	static void pf_verbose( int level )
	{
		s_verboseLevel = level;
	}

	static void pf_anal( int level )
	{
		s_analLevel = level;
	}

	static void pf_debug( int level )
	{
		s_debugLevel = level;
	}

	static void pf_old( bool onOff )
	{
		s_bOld = onOff;
	}

private:
	LuaPathfindingLib( )
	{
		LuaLibMan::AddLibrary( LUALIB_PATHFINDING, this );

		s_verboseLevel = 0;
		s_analLevel = 0;
	}

public:
	~LuaPathfindingLib( )
	{
		LuaLibMan::RemLibrary( LUALIB_PATHFINDING, this );
	}

	static LuaPathfindingLib *i()
	{
		return(&s_instance);
	}

public:
	int GetVerboseLevel()
	{
		return( s_verboseLevel );
	}

	int GetAnalLevel()
	{
		return( s_analLevel );
	}

	int GetDebugLevel()
	{
		return( s_debugLevel );
	}

	bool IsOldPathfinding()
	{
		return( s_bOld );
	}

// 
private:
	static LuaPathfindingLib s_instance;

	static int  s_verboseLevel;
	static int  s_analLevel;
	static int  s_debugLevel;
	static bool s_bOld;

// Inherited from LuaLibrary
private:
	virtual bool Register  ( LuaConfig& lc );
	virtual bool DeRegister( LuaConfig& lc );
};
