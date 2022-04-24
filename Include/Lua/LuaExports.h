/////////////////////////////////////////////////////////////////////
// File    : LuaExports.h
// Desc    : 
// Created : Wednesday, June 13, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#ifndef RELIC_LIB
    #ifdef LUA_EXPORTS
        #define LUA_API __declspec(dllexport)
        #define LUALIB_API __declspec(dllexport)
    #else
        #define LUA_API __declspec(dllimport)
        #define LUALIB_API __declspec(dllimport)
    #endif
#else
        #define LUA_API /**/
        #define LUALIB_API /**/
#endif // RELIC_LIB
