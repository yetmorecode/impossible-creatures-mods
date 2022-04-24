/////////////////////////////////////////////////////////////////////
// File    : TerrainOverlayInterface.h
// Desc    : 
// Created : Thursday, November 08, 2001
// Author  : Dominic Mathieu
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

///////////////////////////////////////////////////////////////////// 
// TerrainOverlayInterface

// NOTE: this class MUST remain an abstract base class, without any 
// non-virtual member function or member variables

class TerrainOverlayInterface
{
// interface
public:

	virtual bool DrawGrid
		(
		const unsigned char	grid[],
		const size_t		gridW,
		const size_t		gridH,
		const Colour		pal[], 
		const size_t		palCount
		) = 0;
};
