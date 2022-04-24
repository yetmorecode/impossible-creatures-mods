/////////////////////////////////////////////////////////////////////
//	File	: SimEngineTypes
//	Desc.	: This files contains useful typedefs for use in the
//			  Simulation engine.  No file should EVER be included
//			  In this header.
//
//		19.Nov.00 (c) Relic Entertainment Inc.
//
#pragma once


/////////////////////////////////////////////////////////////////////
//	Forward Declarations


class EntityController;
class Player;
class World;


// A storage structure for players.
typedef std::vector<Player *>			PlayerList;
typedef PlayerList::iterator			PlayerListIter;


// the unique number for an entity
typedef unsigned long EntityIDNumber;
const unsigned long Invalid_Entity = 0;
