////////////////////////////////////////////////////////////////////
// File  : GameAI.h
// Author: Shelby Hubick
// Date  : 2000-11-18
//      (c) relic entertainment inc.2000
// 
// * GameAI interface
// 

#pragma once

//	forward declaration
class IFF;

//	GameAI
class GameAI
{
// construction
public:
	virtual	~GameAI() {}
	
// interface
public:

	// Initialize the AI, the AI can't access the world inside of it's constructor at all.
	virtual void			AIInit( unsigned long PlayerID, const char* script ) = 0;

	// id of player 
	virtual unsigned long	GetPlayerID() const = 0;

	// do the thinking for this AI
	virtual void			Think( float currentTime ) = 0;

	// save load, should be used for SP only.  That way AI transfer works in MP.
	virtual void			Save( IFF& iff ) const = 0;

	virtual void			Load( IFF& iff ) = 0;
};
