/////////////////////////////////////////////////////////////////////
// File    : PathHeuristic.cpp
// Desc    : 
// Created : July 15, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once


inline void CalcSep( int x, int y, 
				     int minX, int minY, 
				     int maxX, int maxY,
					 int& sepX, int& sepY )
{
	if (x < minX)
	{
		sepX = minX - x;
	}
	else if (x > maxX)
	{
		sepX = x - maxX;
	}
	else
	{
		sepX = 0;
	}

	if (y < minY)
	{
		sepY = minY - y;
	}
	else if (y > maxY)
	{
		sepY = y - maxY;
	}
	else
	{
		sepY = 0;
	}
}


inline float FindOpenHeuristicCost( int x, int y, const Vec2i& minCell, const Vec2i& maxCell, const Vec2i& refCell )
{
	// The (under-)estimated cost from this cell to the goal.

	float cellVal = 0.0f;

	int sepX, sepY;
	CalcSep( x, y, minCell.x, minCell.y, maxCell.x, maxCell.y, sepX, sepY );

	cellVal = (float)sqrt( sepX*sepX + sepY*sepY );

	// the refVal is used to bias (x, y) towards cells that are nearer to (refX, refY)
	//	FIX: adding the refVal might break the under-estimation.  Is this bad?

	int refxdir = abs( refCell.x - x );
	int refydir = abs( refCell.y - y );

	float refVal = 0.0f;

	if ( refxdir < refydir )
	{
		refVal = refydir + refxdir*0.41421f;
	}
	else
	{
		refVal = refxdir + refydir*0.41421f;
	}

	const float REF_WEIGHT = 0.01f;

	return( cellVal + (refVal * REF_WEIGHT) );
}