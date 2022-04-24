/////////////////////////////////////////////////////////////////////
// File    : LuaSimEngine.h
// Desc    : 
// Created : Monday, February 18, 2002
// Author  : 
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include "SimExports.h"

class LuaConfig;

///////////////////////////////////////////////////////////////////// 
// LuaSimEngineLib

class SIM_API LuaSimEngineLib
{
public:
	static void Initialize( LuaConfig * );
	static void Shutdown( LuaConfig * );
};