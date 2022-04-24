#pragma once

#include "PathTypes.h"

/////////////////////////////////////////////////////////////////////
//	Struct	: AstarSectorNode
//	Desc.	: a node in the sector path
//
class AstarSectorNode
{
public:
	
	PathSubCell			sector;
	AstarSectorNode*	prev;
	int					count;
	float				gval;       // g in A* represents how far we've already gone
	float				hval;       // h in A* represents an estimate of how far is left
};


