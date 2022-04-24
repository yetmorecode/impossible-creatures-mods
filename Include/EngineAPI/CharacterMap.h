/////////////////////////////////////////////////////////////////////
// File  : CharacterMap.h
// Author: Cedric
// Date  : 2001-10-05
//      (c) relic entertainment inc.2001
// 
// *
// 

#pragma once

///////////////////////////////////////////////////////////////////// 
// CharacterMap

class CharacterMap
{
public:
	// interface
	virtual void			SetName				( int id, const std::string& name ) = 0;	// EBP file name
	virtual void			SetSpeechIconFname	( int id, const std::string& fname ) = 0;	// image file name
	virtual void			SetScreenNameID		( int id, long textID ) = 0;				// localizer text id

	virtual bool			GetName				( int id, std::string& name ) = 0;			// EBP file name
	virtual bool			GetSpeechIconFname	( int id, std::string& fname ) = 0;			// image file name
	virtual bool			GetScreenNameID		( int id, long& textID ) = 0;				// localizer text id

	virtual void			Clear() = 0;

public:
	// construction
	virtual	~CharacterMap()
		{
		}
};
