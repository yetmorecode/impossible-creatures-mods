/////////////////////////////////////////////////////////////////////
// File    : AstarGridNode.cpp
// Desc    : 
// Created : 
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

/////////////////////////////////////////////////////////////////////
//	Class	: AstarGridNode
//	Desc.	: A node in the A* grid path.
//
class AstarGridNode
{
public:

	inline void Set( const int x, const int y, const int count, const float gval, const float hval );

	// unsigned?
	int x, y;				// grid coords
	int			count;		// the number of nodes taken to reach this point
	float		gval;       // g in A* represents how far we've already gone
	float		hval;       // h in A* represents an estimate of how far is left
	float		fval;       // f in A* represents the sum of the gval and the hval
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: sets all necessary parameters of the node
//
inline void AstarGridNode::Set( const int _x, const int _y, const int _count, const float _gval, const float _hval )
{
	x		= _x;
	y		= _y;
	count	= _count;
	gval	= _gval;
	hval	= _hval;
	fval	= gval + hval;
}