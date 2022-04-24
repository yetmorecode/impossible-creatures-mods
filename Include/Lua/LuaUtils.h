/////////////////////////////////////////////////////////////////////
// File    : LuaUtils.h
// Desc    : 
// Created : Monday, January 07, 2002
// Author  : 
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include "LuaConfig.h" 

///////////////////////////////////////////////////////////////////// 
// LuaConfig overloaded field accessor

class __LCMsg
{
public:
	virtual void Write( const char* msg ) const = 0;
};

bool LUA_API __LCGetVal( LuaConfig& L, const char* field, double& v );
bool LUA_API __LCGetVal( LuaConfig& L, const char* field, float& v );
bool LUA_API __LCGetVal( LuaConfig& L, const char* field, bool& v );
bool LUA_API __LCGetVal( LuaConfig& L, const char* field, long& v );
bool LUA_API __LCGetVal( LuaConfig& L, const char* field, unsigned long& v );

void LUA_API __LCSetVal( LuaConfig& L, const char* field, double v );
void LUA_API __LCSetVal( LuaConfig& L, const char* field, float v );
void LUA_API __LCSetVal( LuaConfig& L, const char* field, bool v );
void LUA_API __LCSetVal( LuaConfig& L, const char* field, long v );
void LUA_API __LCSetVal( LuaConfig& L, const char* field, unsigned long v );

void LUA_API __LCGetString
	( 
	LuaConfig&			lua,
	const char*			header,
	const char*			field,
	const char*			defstring,
	char*				outstring,
	unsigned long		maxn,
	const __LCMsg&		output
	);

class LUA_API __LCMsgTrace : public __LCMsg
{
public:
	virtual void Write( const char* msg ) const;
};

class LUA_API __LCMsgWarning : public __LCMsg
{
private:
	const unsigned long m_id;

public:
	__LCMsgWarning( unsigned long id );
	
public:
	virtual void Write( const char* msg ) const;
};

template< class T > 
	void __LCGetVal
		( 
		LuaConfig&			lua,
		const char*			header,
		const char*			field,
		const T&			min,
		const T&			max,
		T&					value,
		const __LCMsg&		output
		)
{
	// validate parms
	dbAssert( min < max );

	// retrieve value
	T v;

	if( !__LCGetVal( lua, field, v ) )
	{
		char msg[ 512 ];
		_snprintf( msg, LENGTHOF(msg), "%s field '%s' is missing, setting to min", 
			header, 
			field 
			);

		output.Write( msg );

		//
		value = min;
		return;
	}

	// check limits
	value = T( v );

	if( value < min || value > max )
	{
		char msg[ 512 ];
		_snprintf( msg, LENGTHOF(msg), "%s field '%s' is out of bounds, clipping to valid range", 
			header, 
			field 
			);

		output.Write( msg );

		//
		if( value < min )
			value = min;

		if( value > max )
			value = max;
	}

	return;
}

template< class T > 
	void __LCGetVal
		( 
		LuaConfig&			lua,
		const char*			header,
		const char*			field,
		const T&			defval,
		T&					value,
		const __LCMsg&		output
		)
{
	// retrieve value
	T v;

	if( !__LCGetVal( lua, field, v ) )
	{
		char msg[ 512 ];
		_snprintf( msg, LENGTHOF(msg), "%s field '%s' is missing, setting to default", 
			header, 
			field 
			);

		output.Write( msg );

		//
		value = defval;
		return;
	}
	else
	{
		value = T( v );
	}

	return;
}

template< class T > 
	void __LCGetSetVal
		( 
		LuaConfig&			lua,
		const char*			header,
		const char*			field,
		const T&			defval,
		T&					value,
		const __LCMsg&		output
		)
{
	// retrieve value
	T v;

	if( !__LCGetVal( lua, field, v ) )
	{
		//
		char msg[ 512 ];
		_snprintf( msg, LENGTHOF(msg), "%s field '%s' is missing, setting to default", 
			header, 
			field 
			);

		output.Write( msg );

		//
		value = defval;

		// add the fields to lua
		__LCSetVal( lua, field, value );

		return;
	}
	else
	{
		value = T( v );
	}

	return;
}

///////////////////////////////////////////////////////////////////// 
// 

void LUA_API LCGetSetString
	( 
	LuaConfig&			lua,
	const char*			header,
	const char*			field,
	const char*			defstring,
	char*				outstring,
	unsigned long		maxn
	);

void LUA_API LCGetSetWString
	( 
	LuaConfig&			lua,
	const char*			header,
	const char*			field,
	const wchar_t*		defstring,
	wchar_t*			outstring,
	unsigned long		maxn
	);

template< class T > 
	inline void LCGetVal
		( 
		LuaConfig&			lua,
		const char*			header,
		const char*			field,
		const T&			min,
		const T&			max,
		T&					value
		)
{
	__LCGetVal( lua, header, field, min, max, value, __LCMsgTrace() );
}

template< class T > 
	inline void LCGetVal
		( 
		LuaConfig&			lua,
		const char*			header,
		const char*			field,
		const T&			defval,
		T&					value
		)
{
	__LCGetVal( lua, header, field, defval, value, __LCMsgTrace() );
}

template< class T > 
	inline void LCGetSetVal
		( 
		LuaConfig&			lua,
		const char*			header,
		const char*			field,
		const T&			defval,
		T&					value
		)
{
	__LCGetSetVal( lua, header, field, defval, value, __LCMsgTrace() );
}

template< class T > 
	inline void LCGetValW
		( 
		LuaConfig&			lua,
		const char*			header,
		const unsigned long id,
		const char*			field,
		const T&			min,
		const T&			max,
		T&					value
		)
{
	__LCGetVal( lua, header, field, min, max, value, __LCMsgWarning(id) );
}

template< class T > 
	inline void LCGetValW
		( 
		LuaConfig&			lua,
		const char*			header,
		const unsigned long id,
		const char*			field,
		const T&			defval,
		T&					value
		)
{
	__LCGetVal( lua, header, field, defval, value, __LCMsgWarning(id) );
}

template< class T > 
	inline void LCGetSetValW
		( 
		LuaConfig&			lua,
		const char*			header,
		const unsigned long id,
		const char*			field,
		const T&			defval,
		T&					value
		)
{
	__LCGetSetVal( lua, header, field, defval, value, __LCMsgWarning(id) );
}

inline void LCGetString
	( 
	LuaConfig&			lua,
	const char*			header,
	const char*			field,
	const char*			defstring,
	char*				outstring,
	unsigned long		maxn
	)
{
	__LCGetString( lua, header, field, defstring, outstring, maxn, __LCMsgTrace() );
}

inline void LCGetStringW
	( 
	LuaConfig&			lua,
	const char*			header,
	const unsigned long id,
	const char*			field,
	const char*			defstring,
	char*				outstring,
	unsigned long		maxn
	)
{
	__LCGetString( lua, header, field, defstring, outstring, maxn, __LCMsgWarning(id) );
}
