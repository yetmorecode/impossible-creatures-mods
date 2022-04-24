/////////////////////////////////////////////////////////////////////
// File    : LuaLibMan.h
// Desc    : 
// Created : Wednesday, June 13, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//
#pragma once

#include "LuaExports.h"
#include "LuaBinding.h"

#include <Assist/StlExSmallVector.h>

// forward declarations
class LuaConfig;

/////////////////////////////////////////////////////////////////////
// LuaLibrary 

// * inherit from this class to add your library to the library manager

class LuaLibrary
{
// construction
public:
	LUA_API virtual ~LuaLibrary();

// interface
public:
	LUA_API virtual bool Register  ( LuaConfig& ) = 0;
	LUA_API virtual bool DeRegister( LuaConfig& ) = 0;

// LuaBinding utilities
public:

	// this function will store the lua binding object in a bucket per LuaConfig
	// when DeRegister is called you can Clear the LuaBinding Objects for that LuaConfig state
	LUA_API void StoreLuaBindingObj( LuaConfig&, const LuaBinding::Obj& );
	LUA_API void ClearLuaBindingObj( LuaConfig& );

// fields
public:
	// this vector holds onto the LuaBinding Objects
	typedef std::vector< LuaBinding::Obj > BindObjList;
	
	struct BindInfo
	{
		const LuaConfig*	state;
		BindObjList			bindings;
	};
	
	typedef std::vector< BindInfo > BindInfoList;

private:	
	// this vector holds onto the list of BindInfo objects
	BindInfoList	m_LCBindMap;
};

/////////////////////////////////////////////////////////////////////
// LuaLibMan

class LUA_API LuaLibMan
{
public:

	static void AddLibrary( const char* name, LuaLibrary* );
	static void RemLibrary( const char* name, LuaLibrary* );

private:

	// Only LuaConfig should call these functions.  Everyone else must go through the
	// LuaConfig interface for registering and derigerting lua libraries
	friend LuaConfig;

	static LuaLibrary* GetLibrary( const char* name );

	static bool RegisterLib( LuaLibrary*, LuaConfig& );
	static bool DeRegisterLib( LuaLibrary*, LuaConfig& );
};
