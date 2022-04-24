/////////////////////////////////////////////////////////////////////
// File    : GhostInterface.h
// Desc    : 
// Created : Tuesday, Feb 19, 2002
// Author  : 
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

// forward declaration
class Entity;
class Player;
class ControllerBlueprint;
class Matrix43f;
class Ghost;
class IFF;

///////////////////////////////////////////////////////////////////// 
// Types

//
typedef std::vector<Ghost*> GhostCont;

///////////////////////////////////////////////////////////////////// 
// Ghost

// NOTE: this class MUST remain an abstract base class, without any 
// non-virtual member function or member variables

class Ghost
{
// interface
public:
	virtual const Entity*				GetEntity() const = 0;
	virtual const Player*				GetOwner() const = 0;
	virtual const ControllerBlueprint*	GetControllerBP() const = 0;
	virtual const Matrix43f&			GetTransform() const = 0;

	virtual void						Update( bool ghosting ) = 0;
	virtual void						Reset() = 0;					//	Resets the ghost to whatever player it now is

	virtual bool						IsVisible() const = 0;	//
	virtual bool						IsVisited() const = 0;	//

protected:
	//	Force the use of the GhostInterface for allocation/deallocation
	Ghost()				{}
	virtual ~Ghost()	{}
};

///////////////////////////////////////////////////////////////////// 
// GhostInterface

// NOTE: this class MUST remain an abstract base class, without any 
// non-virtual member function or member variables

class GhostInterface
{
// interface
public:
	typedef bool IsGhostedFunc( Ghost* );

	virtual Ghost*	GhostNew	( const Entity* ) = 0;
	virtual void	GhostDelete	( Ghost* ) = 0;

	virtual void	Update		( IsGhostedFunc* ) = 0;

	virtual void	Save		( IFF& ) const = 0;
	virtual void	Load		( IFF& ) = 0;

};
