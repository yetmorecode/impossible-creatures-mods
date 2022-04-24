/////////////////////////////////////////////////////////////////////
// File  : TerrainHM.h
// Author: 
// Date  : 2000-11-9
//      (c) relic entertainment inc.2000
// 
// *
// 

#pragma once

#include <Assist/Array2d.h>

/////////////////////////////////////////////////////////////////////
// Forward Declarations
class TerrainCellMap;
class TerrainSectorMap;
class PathSectorMap;
class PreciseTerrainMap;

//---------------------------------------------------------------------------------------------
// TerrainHM
//
//  HM stands for heightmapped terrain. This is a base class for all terrain implementations
//  that use a HM. The internal implemenation can use this heightmap any way they choose.
//  This class should be all that the simulation needs.
//
//---------------------------------------------------------------------------------------------

class TerrainHMBase
{
public:

	//-------------------------------
	// enums and typedefs
	//-------------------------------
	
	enum TerrainType{
		eSky, eLand, eWater,
		eOutsideMap = eLand		// treat as land so swimming creatures don't pathfind off the map
	};

public:

	virtual ~TerrainHMBase() {};

	// get the height at the world coord point x and z (meters)
	virtual float						GetHeight(float x, float z) const = 0;
	virtual float						GetSurfaceHeight(float x, float z) const = 0;
	virtual float						GetWaterHeight(float x, float z) const = 0;

	// get the entire islands length in world coords (meters)
	virtual float						GetIslandWidth() const = 0;		// This is x.
	virtual float						GetIslandLength() const = 0;		// This is z.

	virtual void						GetNormal(const float x, const float z, Vec3f &Normal) const = 0;

	// cell info
	virtual TerrainType					GetTerrainCellType(float x, float z) const = 0;

	// get a list of points that connect 'a' to 'b'
	virtual const std::vector<Vec3f>&	GetTerrainLine( const Vec2f& a, const Vec2f& b, float resolution ) const = 0;
			//	This data is only good until the next call to GetTerrainLine()
			//	This should really only be used for rendering

	// terrain type interface
	virtual unsigned char				GetTerrainType( float x, float z ) = 0;
	virtual const char*					GetTerrainTypeName( unsigned char type ) = 0;
	virtual bool						FindTerrainType( const char* typeName, unsigned char& type ) = 0;

	///////////////////////////////////////////////////////////////////// 
	// FOW Functions
	virtual void						SetFogOfWarFullbright( bool fullbright ) = 0;

};
