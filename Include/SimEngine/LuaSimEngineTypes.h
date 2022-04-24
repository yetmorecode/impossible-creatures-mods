/////////////////////////////////////////////////////////////////////
// File    : SimEngineLuaTypes.h
// Desc    : 
// Created : Wednesday, October 17, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <Lua/LuaBinding.h>

// forward declaration
class Entity;

///////////////////////////////////////////////////////////////////// 
// 

namespace LuaBinding
{
	///////////////////////////////////////////////////////////////////// 
	// Entity Pointer Custom Paramater

	template<> inline bool ParmTraits<Entity*>::Pop( type& t, lua_State* ls, int n )
	{
		const int entityTag = LuaConfig::GetTag( ls, "entity" );

		if( lua_tag( ls, n ) != entityTag )
			return false;

		t = static_cast<Entity*>( lua_touserdata( ls, n ) );

		return true;
	}

	template<> inline void ParmTraits<Entity*>::Push( const type t, lua_State* ls )
	{
		int entitytag = LuaConfig::GetTag( ls, "entity" );
		
		lua_pushusertag( ls, (void*)t, entitytag);

		return;
	}

}
