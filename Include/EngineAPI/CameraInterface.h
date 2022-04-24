/////////////////////////////////////////////////////////////////////
// File    : CameraInterface.h
// Desc    : 
// Created : Thursday, February 15, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

// forward declaration
class EntityGroup;
class Camera;
class Vec3f;
class IFF;

///////////////////////////////////////////////////////////////////// 
// CameraInterface

// NOTE: this class MUST remain an abstract base class, without any 
// non-virtual member function or member variables

class CameraInterface
{
// interface
public:

	virtual void	OrbitCamera				( float ) = 0;
	virtual void	DeclinateCamera			( float ) = 0;
	virtual void	ZoomCameraMouse			( float ) = 0;

	virtual void	FocusOnEntityGroup		( const EntityGroup& g ) = 0;
	virtual void	FocusOnTerrain			( const Vec3f& pos ) = 0;
	virtual void	NextFocus				() = 0;
	virtual void	PrevFocus				() = 0;
	virtual void	CancelFocus				() = 0;
	virtual void	RemoveFocusFromEntity	( const Entity* e ) = 0;

	virtual void	ForceCamera				() = 0;

	virtual float	GetDeclination			( ) = 0;
	virtual void	SetDeclination			( float decl ) = 0;

	virtual float	GetRotation				( ) = 0;
	virtual void	SetRotation				( float rotate ) = 0;

	virtual float	GetZoom					( ) = 0;
	virtual void	SetZoom					( float zoom ) = 0;

	virtual void	GetTarget				( Vec3f& target ) = 0;
	virtual void	GetEyePos				( Vec3f& eye ) = 0;
	virtual void	GetLookAt				( Vec3f& lookAt ) = 0;
	
	virtual void	SetCanRotate			( bool bRotate ) = 0;
	virtual bool	GetCanRotate			() = 0;

	virtual void	SetCanDeclinate			( bool bDeclinate ) = 0;
	virtual bool	GetCanDeclinate			() = 0;

	virtual void	SetInvertDec			( bool bInvert ) = 0;
	virtual bool	GetInvertDec			() = 0;

	virtual void	SetInvertPan			( bool bInvert ) = 0;
	virtual bool	GetInvertPan			() = 0;

	// The following parameters are multipliers on the scrolling speeds, i.e. 0.0 makes no panning
	// 1.0 is default, and 5.0 is 5 times faster.
	virtual void	SetMouseScrollMult		( float ) = 0;
	virtual float	GetMouseScrollMult		( ) = 0;

	virtual void	SetKeyScrollMult		( float ) = 0;
	virtual float	GetKeyScrollMult		( ) = 0;
};
