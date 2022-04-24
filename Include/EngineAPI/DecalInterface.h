/////////////////////////////////////////////////////////////////////
// File    : DecalInterface.h
// Desc    : 
// Created : Tuesday, Oct 23, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

class OBB3f;

///////////////////////////////////////////////////////////////////// 
// DecalInterface

// NOTE: this class MUST remain an abstract base class, without any 
// non-virtual member function or member variables

class DecalInterface
{
// types
public:
	class Handle
	{
		friend class DecalInterface;

	private:
		long reserved;

	public:
		Handle();
		inline bool IsValid();
	};

// interface
public:
	// add static decal to the terrain
	virtual Handle			AddDecal(			const char*		shaderName, 
												float			x, 
												float			z, 
												float			scale, 
												float			rotRad) = 0;

	// add dynamic decal to the terrain
	virtual Handle			AddDynamicDecal(	const char*		shaderName, 
												float			x, 
												float			z, 
												float			scale, 
												float			rotRad) = 0;

	// add dynamic decal to the terrain
	// - determine the position, scale and rotation of the decal from a given OBB
	virtual Handle			AddDynamicDecal(	const char*		shaderName, 
												const OBB3f&	obb ) = 0;
												
	// remove a static / dynamic decal immediately
	virtual void			RemoveDecal(		Handle&			handle) = 0;

	// setup dynamic decal parameters
	virtual void			SetupDynamicDecal(	Handle&			handle, 
												float			fadeInTime, 
												float			fadeOutTime, 
												float			lifeTime, 
												bool			bEternalLife) = 0;
	
	// tell a dynamic decal to 
	virtual void			KillDynamicDecal(	Handle&			handle) = 0;

// implementation
protected:
	static long				GetReserved( const Handle& handle );
	static void				SetReserved( Handle& handle, long value );
};

///////////////////////////////////////////////////////////////////// 
// implementation;

inline DecalInterface::Handle::Handle()
	: reserved( -1 )
{
}

inline bool DecalInterface::Handle::IsValid()
{
	return (reserved != -1);
}

inline long DecalInterface::GetReserved( const Handle& handle )
{
	return handle.reserved;
}

inline void DecalInterface::SetReserved( Handle& handle, long value )
{
	handle.reserved = value;
}
