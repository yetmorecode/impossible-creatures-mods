/////////////////////////////////////////////////////////////////////
// File    : ModUIEvent.h
// Desc    : 
// Created : Friday, October 26, 2001
// Author  : Dominic Mathieu
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

///////////////////////////////////////////////////////////////////// 
// ModUIEvent

// * NOTE: these functions are for display purposes only

class ModUIEvent
{
// interface
public:
	virtual void OnHostMigrated ( unsigned long idplayer ) = 0;
	virtual void OnPlayerDrops  ( unsigned long idplayer ) = 0;
	virtual void OnCinematicMode( bool bCinematic ) = 0;
	virtual void OnShowTeamColour( bool bShow ) = 0;
	virtual void OnResetSM	    ( ) = 0;
	virtual void OnCharacterTalk( unsigned long entityID, bool bTalk ) = 0;
};
