/////////////////////////////////////////////////////////////////////
// File    : SoundInterface.h
// Desc    : 
// Created : Thursday, May 03, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once


class Vec3f;

///////////////////////////////////////////////////////////////////// 
// SoundInterface

// NOTE: this class MUST remain an abstract base class, without any 
// non-virtual member function or member variables

class SoundInterface
{
// interface
public:

	virtual void	PlaySound  ( const char* filename ) = 0;
	
	virtual void	PlaySoundAt( const char* filename, const Vec3f& pos, long group = -1 ) = 0;

	// set 'timeout' to -1.0f to ignore timeout
	virtual void	PlaySpeech ( const char* filename, float timeout ) = 0;
	
	virtual void	PlayStinger( const char* filename ) = 0;

	virtual void	PlayBattleTrack( bool bPlay ) = 0;
};
