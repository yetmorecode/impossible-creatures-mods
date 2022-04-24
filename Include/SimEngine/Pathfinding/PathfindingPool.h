/////////////////////////////////////////////////////////////////////
// File    : PathfindingPool.h
// Desc    : memory pool to be used by pathfinding objects
// Created : Wednesday, May 29, 2002
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once



/////////////////////////////////////////////////////////////////////
//	Desc.	: allocate memory in the 'pathfind' memory pool
void *PathMemAlloc( size_t size );

/////////////////////////////////////////////////////////////////////
//	Desc.	: free memory allocated memory in the 'pathfinding' pool
void PathMemFree( void* pointer );

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the handle of the 'pathfinding' memory pool
HMEMPOOL PathMemHandle( );