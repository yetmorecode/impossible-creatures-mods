/////////////////////////////////////////////////////////////////////
// File    : luaentity.h
// Desc    : 
// Created : Monday, February 18, 2002
// Author  : 
// 
// (c) 2002 Relic Entertainment Inc.
//


#pragma once

class LuaConfig;

///////////////////////////////////////////////////////////////////// 
// LuaEntityLib

class LuaEntityLib
{
public:
	static void Initialize( LuaConfig* );
	static void Shutdown( LuaConfig* );
};



