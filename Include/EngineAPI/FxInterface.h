/////////////////////////////////////////////////////////////////////
// File    : FXInterface.h
// Desc    : 
// Created : Thursday, May 03, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

// forward declaration
class Vec3f;

///////////////////////////////////////////////////////////////////// 
// FXInterface

// NOTE: this class MUST remain an abstract base class, without any 
// non-virtual member function or member variables

class FXInterface
{
// types
public:
	class Handle
	{
	private:
		unsigned char reserved[8];

	public:
		Handle();
	};

// interface
public:
	virtual Handle	FXCreate      ( const char* fxName ) = 0;
	virtual void	FXDestroy     ( Handle& fx ) = 0;

	virtual void	FXSetTransform( const Handle& fx, const Matrix43f& ) = 0;
	virtual void	FXSetScale    ( const Handle& fx, float ) = 0;
	virtual void	FXSetLength   ( const Handle& fx, float ) = 0;

	virtual void	FXSetVisible  ( const Handle& fx, bool  ) = 0;

	virtual void	FXPreload     ( const char* fxName ) = 0;
};

///////////////////////////////////////////////////////////////////// 
// implementation;

inline FXInterface::Handle::Handle()
{
	memset( reserved, 0, sizeof(reserved) );
}
