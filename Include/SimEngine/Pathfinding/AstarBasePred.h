/////////////////////////////////////////////////////////////////////
// File    : AstarBasePred.h
// Desc    : 
// Created : July 15, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once 

#include "../SimExports.h"

#include "DirectionMap.h"

/////////////////////////////////////////////////////////////////////
//	Class	: AstarBaseFindPathPred
//
class AstarBaseFindPathPred
{
public:
	virtual bool PassiblePosition( int x, int y, DirectionMap::Dir dir ) const = 0;
	virtual bool TestExitCondition( int x, int y, int count, int numSearched ) const = 0;

	virtual bool HitNodeLimit( int numNodes ) const = 0;

	virtual bool ModifyEndCell( int&, int&, int& ) const
	{
		return false;
	}
};


/////////////////////////////////////////////////////////////////////
//	Class	: AstarBaseFindOpenPred
//
class AstarBaseFindOpenPred
{
public:
	virtual bool  TestExitCondition( int x, int y ) const = 0;
	virtual float HeuristicCost( int x, int y ) const = 0;
};


/////////////////////////////////////////////////////////////////////
//	Class	: AstarBaseFindValidPred
//
class AstarBaseFindValidPred
{
public:
	virtual bool  IsMatch( int x, int y ) const = 0;
	virtual float Dist( int x, int y ) const = 0;
};

/////////////////////////////////////////////////////////////////////
//	Class	: AstarBaseFindBestPred
//
class SIM_API AstarBaseFindBestPred
{
public:
	virtual bool TestExitCondition( int x, int y ) const = 0;
};
