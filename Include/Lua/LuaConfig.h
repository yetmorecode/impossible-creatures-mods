/////////////////////////////////////////////////////////////////////
// File    : LuaConfig.h
// Desc    : 
// Created : Friday, February 02, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

//----------------------------------------------------------------------
// LuaConfig
//
//   Uses Lua to read in config files. Which is just a list of strings
//   and numbers. These values can also be in tables.
//
//   Example script:
//		physics = {
//			velocity = {x=10,y=20,z=30}
//		}
//		math = {}
//      math["poohead"] = "string goes here"; <-optional semi-colon
//		math.poohead = "string here";		<- same as above line
//
//   Variables are accessed like so:
//
//	   vec.x = GetNumber("physics.velocity.x");
//     vec.y = GetNumber("physics.velocity.y");
//     // this string should be copied
//     name = GetString("math.poohead");
//
//----------------------------------------------------------------------

#pragma once

#include "LuaExports.h"

#include <Assist/Callback.h>

// add this so all files that include this file can write binded functions
// without having to go search for this include
extern "C" 
{ 
	#include <Lua/core/inc/lua.h>
};

// forward declaration
struct lua_State;

class ByteStream;

///////////////////////////////////////////////////////////////////// 
// LuaConfig

class LUA_API LuaConfig
{
// types
public:
	// all the different types of lua data
	enum LuaType 
	{
		LT_USERDATA, 
		LT_NIL, 
		LT_NUMBER, 
		LT_STRING, 
		LT_TABLE, 
		LT_FUNCTION
	};

	enum LuaTag
	{
		LT_NOTAG,
	};

	// All the different default libs available to add to this state
	enum	
	{
		DL_BASELIB	= 1 << 0,
		DL_IOLIB	= 1 << 1,
		DL_STRLIB	= 1 << 2,
		DL_MATHLIB	= 1 << 3,
		DL_DEBUGLIB = 1 << 4,
	};

	// the type that should be used when adding new tags
	typedef int(*TagMethod)(lua_State*);
	// type to be used for registering functions into lua
	typedef int(__stdcall *LuaFunc)(lua_State*);
	// type to be used for the alert callback
	typedef void(*CBAlert)(const char*);

	typedef Callback::Obj1nRet<const char*>	CBRegister;
	typedef Callback::Obj1nRet<const char*>	CBClear;
	
// construction
public:
	 LuaConfig(const char* name = "");
	~LuaConfig();
	
// interface
public:
	// 
	const char*		GetName() const;

	//------------------------------------------------------------
	// access variables within this lua state

	// get the type of the variable -- looks in current table only
		// returns LT_NIL if the variable is not defined
	LuaType			GetType		  ( const char* name );

	// get the value of the variable specified -- looks in current table only
		// NOTE: if the variable is not found, the out parm is not modified
		// NOTE: no type checking
	bool			GetFloat	  ( const char* name, float& out );
	bool 			GetInt		  ( const char* name, int& out );
	bool			GetBool		  ( const char* name, bool& out );
	bool			GetDouble	  ( const char* name, double& out );
	bool			GetString	  ( const char* name, char* out, size_t maxn );
	bool			GetStringArray( const char* name, std::vector< std::string >& v );
	unsigned long	GetStringArray( const char* name, char** out, size_t maxn, size_t strsize);
	bool			GetWString	  ( const char* name, wchar_t* out, size_t maxn );
	bool			GetIntArray	  ( const char* name, std::vector< int >& out );
	unsigned long	GetIntArray	  ( const char* name, int* out, size_t maxn );
	bool			GetFloatArray ( const char* name, std::vector< float >& out );
	unsigned long	GetFloatArray ( const char* name, float* out, size_t maxn );

	//------------------------------------------------------------
	// add variables (constants)
		
	// set variable to value 
		// NOTE: if variable already exists, the value will be changed
	void	SetNumber	  ( const char* name, const double  d );
	void	SetNumberArray( const char* name, const double* d, size_t n );
	void	SetString	  ( const char* name, const char* s );
	void	SetWString	  ( const char* name, const wchar_t* s );

	void	ClearVariable ( const char* name );

	void	ClearTag	  ( const char* name );

	// adds a new table and then pushes it be the local scoped table
	void	AddTable      ( const char* name );
		
	//------------------------------------------------------------
	// add variables (binded)

	// add a binded type to the global space
	void	AddBool		  ( bool* ptr,	const char* name );
	void	AddInt		  ( int* ptr,	const char* name );
	void	AddFloat	  ( float* ptr,	const char* name );

	// this adds a new type to this lua state (for proper type checking) set and get methods are optional
	int		AddTag		  ( const char* name, TagMethod set = NULL, TagMethod get = NULL );
	int		GetTag		  ( const char* name );
	static int	
			GetTag		  ( lua_State* state, const char* name );

	// set and get pointers that have been added to a lua state with a registered tag name
	void	SetTagPtr	  ( const char* name, void* vptr, const char* tagName );
	void*	GetTagPtr	  ( const char* name, const char* tagName );
	static void*
			GetTagPtr	  ( lua_State* state , const char* name, const char* tagName );

	//------------------------------------------------------------
	// registration functions

	bool	RegisterLibrary( const char* libname );
	bool	DeRegisterLibrary( const char* libname );

	void	RegisterCFunc(const char* funcname, LuaFunc func);
	void	RegisterCFunc(const char* funcname, LuaFunc func, size_t closureCount, void** closureArray);
	void	ClearFunction(const char* name);
	
	// function that gets called everytime something is registered
	void	SetOnRegister( const CBRegister& );
	void	SetOnClear   ( const CBClear& );
	void	SetOnAlert	 ( const CBAlert& );

	//------------------------------------------------------------
	// navigation interface
	
	// number of entry in the current table
	size_t	CountEntry();

	// gets list of entries of the current table that is set (use Push/Pop Table calls)
		// set 'buffer' to empty to start an enumeration
	bool	NextEntry( char* buffer, size_t maxn, LuaType* type = 0 );
	
	// push table to set accessor scope within a table - so to get the var "foo.bar" you would need 
	// to first push "foo" than search for "bar".
	void	PushTable( const char *name );

	// similar to PushTable() but creates the table if it doesn't exist
	void	PushTableEx( const char *name );

	// must be called with every push
	void	PopTable( void );
	
	//------------------------------------------------------------

	// run a single string - smartComplete turns single word parameters into functions with 0 args
	bool	RunString(const char* string, bool smartComplete = false);

	bool	RunBuffer(const char* buffer, size_t buflen);

	//------------------------------------------------------------
	// save interface
	
	// load a file into this luaconfig state
	bool	LoadFile(const char* filename);

	bool	Load( ByteStream* bytestream, long size );

	// save global state of this luaconfig file
	void	SaveAll( bool indent = false );

	// save a var ( if saving a var within a table you must call PushTable to enter a table's scope)
	bool	SaveVar( const char* name, size_t level = 0, bool indent = false );

	// start the save mode
	bool	StartSave( const char* filename );
	bool	StartSave( ByteStream* bytestream );

	// end the save mode
	bool	EndSave( );

// extensions to lua_State
public:
	
	// This adds the ability to add/ bind variables to int,bool, and float
	void	RegisterDefaultTypes();

	// Register a default lib to this lua state
	void	RegisterDefaultLibs( int eTypes );

public:
	//------------------------------------------------------------
	// internal accessor

	lua_State*	GetState();

// fields
private:
	class Data;
	Data* m_pimpl;

// implementation
private:
	const char* GetStringInternal( const char* name );	// internal use only

// copy -- do not define
private:
	LuaConfig( const LuaConfig& );
	LuaConfig& operator= ( const LuaConfig& );
};

///////////////////////////////////////////////////////////////////// 
// Helper Get/Set Functions
//		- These functions will handle the casting to/from a double

inline bool LCGetFloat( LuaConfig& lc, const char* name, float& out )
{
	double ret;
	if ( !lc.GetDouble( name, ret ) )
		return false;

	out = (float)ret;
	return true;
}

inline void LCSetFloat( LuaConfig& lc, const char* name, const float in )
{
	lc.SetNumber( name, (double)in );
}

inline bool LCGetInt( LuaConfig& lc, const char* name, int& out )
{
	double ret;
	if ( !lc.GetDouble( name, ret ) )
		return false;

	out = (int)ret;
	return true;
}

inline void LCSetInt( LuaConfig& lc, const char* name, const int in )
{
	lc.SetNumber( name, (double)in );
}

inline bool LCGetULong( LuaConfig& lc, const char* name, unsigned long& out )
{
	double ret;
	if ( !lc.GetDouble( name, ret ) )
		return false;

	out = (unsigned long)ret;
	return true;
}

inline void LCSetULong( LuaConfig& lc, const char* name, const unsigned long in )
{
	lc.SetNumber( name, (double)in );
}

inline bool LCGetBool( LuaConfig& lc, const char* name, bool& out )
{
	double ret;
	if ( !lc.GetDouble( name, ret ) )
		return false;

	out = ( ret != 0.0 );
	return true;
}

inline void LCSetBool( LuaConfig& lc, const char* name, const bool in )
{
	lc.SetNumber( name, (double)in );
}

inline bool LCGetDouble( LuaConfig& lc, const char* name, double& out )
{
	double ret;
	if ( !lc.GetDouble( name, ret ) )
		return false;

	out = ret;
	return true;
}

inline void LCSetDouble( LuaConfig& lc, const char* name, const double in )
{
	lc.SetNumber( name, in );
}

// Set the output stream

void LUA_API LuaSetPrintFunc(void(*func)(const char*));

