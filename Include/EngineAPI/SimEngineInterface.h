/////////////////////////////////////////////////////////////////////
//	File	: SimEngineInterface.h
//	Desc.	: 
//		17.Nov.00 (c) Relic Entertainment Inc.
//
#pragma once

class EntityFactory;
class TriggerFactory;
class DialogTriggerFactory;
class CharacterMap;

///////////////////////////////////////////////////////////////////// 
// SimEngineInterface

class SimEngineInterface
{
public:
	virtual ~SimEngineInterface() {};

	virtual EntityFactory *GetEntityFactory( ) = 0;
	virtual TriggerFactory *GetTriggerFactory( ) = 0;
	virtual DialogTriggerFactory *GetDialogTriggerFactory( ) = 0;
	virtual CharacterMap* GetCharacterMap( ) = 0;
};
