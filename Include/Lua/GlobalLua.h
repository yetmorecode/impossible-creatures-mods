/////////////////////////////////////////////////////////////////////
// File    : GlobalLua.h
// Desc    : 
// Created : Wednesday, June 13, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "LuaExports.h"

// Global lua is added so that all modules can access and register it self to the global
// lua state. If a module wants to register itself to more than one state, it will have to
// create a lib that will be registered by the state (see LuaBlueprintLib, LuaSimEngineLib,...)

// examples of use: if inside 


// i include this here because there is a one to one dependancy of this file and luaconfig.h
#include "LuaConfig.h"

class LUA_API GlobalLua
{
public:

	static void			Initialize();
	static void			Shutdown();
	static LuaConfig*	GetState();

private:
	GlobalLua();
	static LuaConfig* s_luaState;
};