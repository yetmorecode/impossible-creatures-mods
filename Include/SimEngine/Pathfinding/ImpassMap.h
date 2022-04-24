/////////////////////////////////////////////////////////////////////
// File    : ImpassMap.h
// Desc    : 
// Created : Friday, November 16, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h"

#include "../SimEntity.h"
#include "../SimWorld.h"
#include "../Target.h"

#include "PathTypes.h"

#include <Assist/GBitSet.h>

#include <Math/Vec2.h>

#include <SurfVol/Rec2.h>
#include <Assist/Array2D.h>

// Here we are not using TypedBitSet, for a reason of saving memory. TypedBitSet uses a
//	minimum of 4 bytes per entry (unsigned long).
//	With maps of 256x256,	mem-used = 262144 bytes.
//	using char's,			mem-used = 65536 bytes, a saving of 196608 bytes.

// A BitPlane wasn't used because all of the bits of a cell are accessed per cell access,
//	hence to prevent thrashing the cache, we keep all data for a particular cell together.


/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class BiFF;
class TerrainHMBase;


enum PFCellFields
{
	eLandImpassible			= 0x01,
	eWaterImpassible		= 0x02,
	eAmphibianImpassible	= 0x04,
	eStructure				= 0x08,
	eEntity					= 0x10,
	eCantBuild				= 0x20,
};

inline bool		TCTest	 ( const TerrainCell &a, const TCMask &b ) { return !!(a & b); }
inline TCMask	TCCombine( const TerrainCell &a, const TCMask &b ) { return (TCMask)(a | b); }
inline TCMask	TCClr	 ( const TerrainCell &a, const TCMask &b ) { return (TCMask)(a & ~b); }

inline TCMask	TCBitNegate( const TCMask a ) { return (TCMask)( ~a ); }

inline bool		TCIsLand ( const TerrainCell &a ) { return TCTest( a, eLandImpassible ); }
inline bool		TCIsWater( const TerrainCell &a ) { return TCTest( a, eWaterImpassible ); }
inline bool		TCIsAmph ( const TerrainCell &a ) { return TCTest( a, eAmphibianImpassible ); }


class SIM_API ImpassEditArray
{
public:
	ImpassEditArray();

	void SetSize( int width, int height );
	void SetValue( int x, int y, unsigned char val );

	unsigned char GetValue( int x, int y ) const;

private:
	Array2D<unsigned char> m_map;
};


// A whole bunch of TerrainCells all in a grid makes a lovely map.
class SIM_API TerrainCellMap
{
// Types
public:

	enum MaskOperation
	{
		OP_OR,
		OP_AND,
		OP_SET,
	};

	enum EditedChannels
	{
		EC_Generated	= 0,
		EC_Impassible,
		EC_Passible,		
	};

// Functions.
public:
	TerrainCellMap();
	~TerrainCellMap();

	static void* operator new   ( size_t );										// operator new
	static void  operator delete( void* );										// operator delete

	void			SetSize( const int width, const int height );

	long			GetUpdateStamp( ) const;

	// Perhaps the conversion world->cell should be in here too.
	//	(aswell as the outside of map checking).
	//	just get this working first, then fix0r.

	// Setting Cell Properties.
	void SetCell( const int x, const int z, const TCMask mask, MaskOperation );

	// Getting the Cell Properties.
	inline TCMask GetCell( const int x, const int z ) const;


	// dimension accessor functions
	inline int		GetWidth() const {return m_width;}
	inline int		GetHeight() const {return m_height;}
	inline float	GetCellSize() const {return m_cellsize;}

	void			LockPosToCell( Vec2f& pos ) const;

	int				WorldToCellDim( float Xpos ) const;

	int				WorldXToCellX( float Xpos ) const;
	int				WorldZToCellZ( float Zpos ) const;

	float			CellXToWorldX( int Xpos ) const;
	float			CellZToWorldZ( int Zpos ) const;

	void			WorldToCell( const Vec2f& world, Vec2i& cell ) const;
	void			WorldToCell( const Vec3f& world, Vec2i& cell ) const;

	void			CellToWorld( const Vec2i& cell,  Vec2f& world ) const;

	void			Dump( const char* name, const TCMask &pfCell );

	// This function will blit a rectangle with the upper left corner at (StartX, StartZ)  of width and height
	// It will combine the current cell contents with the mask using the BlitType operand
	void			BlitCells( int StartX, int StartZ, int width, int height, const TCMask mask, MaskOperation );

	// This function will return true if the cells covered by the area [startx,startz]->[width,height], are all clear
	// after masking of the cells using the provided mask
	bool			TestCells( int StartX, int StartZ, int width, int height, const TCMask mask ) const;

	// This funciton will OR all of the cells inside the area [startx,startz]->[width,height]
	TCMask			CumulateCells( int StartX, int StartZ, int width, int height ) const;

	void			Save(IFF &iff) const;
	void			AddLoadHandler( IFF& iff );
	unsigned long	Load(IFF &iff);				// will be called then the IFF handler added by AddLoadHandler() is parsed

	// this function needs fix0ring.
	bool			IsAllSameType(const SimEntity *pe, Target *T, float APc, const TCMask &pfCellMask ); //TerrainCellMap * terrainmap );

	bool			TestAllCells( Vec2f start, Vec2f goal, const TCMask &pfCellMask );

	//
	void			Generate( const TerrainHMBase* pTerrain, const ImpassEditArray* impassEdit );
	void			MarkImpassability( const TerrainHMBase* pTerrain, const Rect2f& rect, const ImpassEditArray* impassEdit );

	// 
	size_t			GetMemUsage() const;

private:	

	void AllocateCells( int width, int height );
	void DeallocateCells();

	inline TerrainCell *LookupCell( const int x, const int z );
	inline const TerrainCell *LookupCell( const int x, const int z ) const;


// Data.
public:
	
	TerrainCell *	m_cell;

	int				m_width;		// number of cells in map across
	int				m_height;		// number of cells in map down

	float			m_cellsize;		// size of one cell side in world units.
									//	cells are square.

	long			m_UpdateStamp;

private:

};


/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the address of the cell at the given x, z location.  Will be NULL if out-of-bounds
//	Result	: return sconst TerrainCell*
//	Param.	: x, z - 2D grid coords
//
inline const TerrainCell *TerrainCellMap::LookupCell( const int x, const int z ) const
{
	// Private function to find the cell given an x,z.
	if( x < 0 || z < 0 || x >= m_width || z >= m_height )
	{
		return NULL;
	}
	return &(m_cell[z*m_height+x]);
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the address of the cell at the given x, z location.  Will be NULL if out-of-bounds
//	Result	: return sconst TerrainCell*
//	Param.	: x, z - 2D grid coords
//
inline TerrainCell *TerrainCellMap::LookupCell( const int x, const int z )
{
	return const_cast<TerrainCell *>( static_cast<const TerrainCellMap *>(this)->LookupCell(x, z) );
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: returns the TCMask of the cell at the (x, z) coord
//	Result	: returns TCMask.  Will return 0xff if coords areout-of-bounds
//	Param.	: x, z, - 2D grid coords
//	Author	: dswinerd
//
inline TCMask TerrainCellMap::GetCell( const int x, const int z ) const
{
#if 0
	// this code is too slow, replaced by the below
	// Get the mask from the specified cell.
	const TerrainCell *tc = LookupCell( x, z );
	if(tc)
		return *tc;
	else
		return 0xff;//0;
#else

	dbAssert( x >= 0 && x < m_width && z >= 0 && z < m_height );
	return m_cell[ z * m_height + x ];
#endif
}
