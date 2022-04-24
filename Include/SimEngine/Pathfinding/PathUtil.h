/////////////////////////////////////////////////////////////////////
// File    : PathUtil.h
// Desc    : various pathfinding utility functions
// Created : 
// Author  : 
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class PreciseTerrainMap;
class SimPath;
class GroundAttributes;

namespace PathUtil
{
	enum StraightLineResult
	{
		RESULT_ExitFail,
		RESULT_ExitSuccess,
		RESULT_Continue,
	};

	class StraightLinePred
	{
	public:
		virtual StraightLineResult Test( int x, int y ) const = 0;	// returns true if passible
	};


	bool StraightLineTest( const Vec2f& start, const Vec2f& goal, const StraightLinePred& pred, Vec2i& bestReachable );


	bool StraightenPath( const PreciseTerrainMap* psm, SimPath& path, const GroundAttributes& attributes, unsigned long ignoreFlags );
};