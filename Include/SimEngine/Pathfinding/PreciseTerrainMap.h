/////////////////////////////////////////////////////////////////////
// File    : PreciseTerrainMap.h
// Desc    : 
// Created : Wednesday, December 12, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "../SimExports.h"

#include <SimEngine/Pathfinding/PathTypes.h>
#include "PathfindingPool.h"

#include <Assist/Array2D.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class TerrainCellMap;
class IFF;
class BiFF;


/////////////////////////////////////////////////////////////////////
//	Class	: PreciseTerrainMap
//
class SIM_API PreciseTerrainMap
{
// construction
public:
	 PreciseTerrainMap();																	// ctor
	~PreciseTerrainMap();																	// dtor

	static void* operator new   ( size_t );													// operator new
	static void  operator delete( void* );													// operator delete

// interface 
public:
	// methods that change the map
	void			Generate( const TerrainCellMap *tcMap, int numDivisions, int maxEntitySize );	// generate the whole precise map from the TerrainCellMap.  MUST be called before any other methods.
	void			MarkRect( const TerrainCellMap *tcMap, 
							  long cellX, long cellZ, long width, long height );					// add an impassible rectangle to the map

	// query the map
	int				GetWidth() const;																// returns the width of the map
	int				GetHeight() const;																// returns the height of the map
	int				GetNumDivisions() const;														// returns the height of the map
	int				GetValue( int x, int y ) const;													// returns the value at (x, y)
	float			GetCellSize( ) const;															// returns the size of a cell
					
	inline int		WorldXToCellX( float worldX ) const;											// convert a world.x coord to cell.x
	inline int		WorldZToCellZ( float worldZ ) const;											// convert a world.z coord to cell.z
					
	float			CellXToWorldX( int cellX ) const;												// convert a cell.x to a world.x
	float			CellZToWorldZ( int cellZ ) const;												// convert a cell.z to a world.z
					
	void			CellToWorld( const Vec2i& cell,  Vec2f& world ) const;							// convert a cell coord to world coord
	inline void		WorldToCell( const Vec2f& world, Vec2i& cell ) const;							// convert a world coord 2D to cell coord
	inline void		WorldToCell( const Vec3f& world, Vec2i& cell ) const;							// convert a world coord 3D to cell coord

	void			PreciseToImpass( const Vec2i& precise, Vec2i& impass ) const;					// given a precise coord, returns the impass map coord
	inline void		PreciseToImpass( int prX, int prY, int& tcX, int& tcY) const;
					
	bool			TestAllCells( const Vec2f& start, const Vec2f& goal, const GroundAttributes& attributes, unsigned long ignoreFlags = 0 ) const;	// test all cells between start & end for passibility

	// debug
	void			Dump( const char *filename ) const;												// dumps the map to a file as a .raw file

	size_t			GetMemUsage() const;

	// save & load
	void			Save(IFF &iff) const;
	void			AddLoadHandler( IFF& iff );
	unsigned long	Load(IFF &iff);		// will be called then the IFF handler added by AddLoadHandler() is parsed


// Types
private:

	template< class T >
		class PreciseGridAllocator
	{
	public:
		void Free( T* p, unsigned long w, unsigned long h )
		{
			UNREF_P( w );
			UNREF_P( h );

			PathMemFree( p );
		}

		T* Alloc( unsigned long w, unsigned long h )
		{
			return reinterpret_cast< T* >( PathMemAlloc( sizeof(T) * w * h ) );
		}
	};


	typedef Array2D<unsigned char, PreciseGridAllocator<unsigned char> > PreciseGrid;

// fields
private:
	PreciseGrid m_grid;

	int			m_numDivisions;
	int			m_maxEntitySize;
	float		m_cellSize;
	float		m_invCellSize;	//	1/m_cellSize
				
	float		m_halfGridWidth;
	float		m_halfGridHeight;

// implementation
private:

	void			SetGridSize( int width, int height );
	void			MergeCells( int minX, int minY, int maxX, int maxY, int mergeSize );
	void			SetImpassibles( const TerrainCellMap *tcMap, int minX, int minY, int maxX, int maxY );
};

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int PreciseTerrainMap::GetWidth() const
{
	return m_grid.GetWidth();
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int PreciseTerrainMap::GetHeight() const
{
	return m_grid.GetHeight();
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int PreciseTerrainMap::GetNumDivisions() const
{
	return m_numDivisions;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int PreciseTerrainMap::GetValue( int x, int y ) const
{
	return m_grid.GetValue(x, y);
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline float PreciseTerrainMap::GetCellSize( ) const
{
	return m_cellSize;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int PreciseTerrainMap::WorldXToCellX( float worldX ) const
{
	int temp;

	//	0.500000001 or so
	const float half = 0.50001f;

	__asm
	{
		mov		ecx, this

		fld		worldX
		fmul	[ecx]this.m_invCellSize;

		fadd	[ecx]this.m_halfGridWidth;

		fadd	half

		fistp	temp;

		dec		temp
	}

/*
	float xsc = (worldX * m_invCellSize) + m_halfGridWidth;
	int temp = int(xsc);
	dbAssert( temp == floorf(xsc) );
*/
	return temp;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int PreciseTerrainMap::WorldZToCellZ( float worldZ ) const
{
	int temp;

	//	0.500000001 or so
	const float half = 0.50001f;

	__asm
	{
		mov		ecx, this

		fld		worldZ
		fmul	[ecx]this.m_invCellSize;

		fadd	[ecx]this.m_halfGridHeight;

		fadd	half

		fistp	temp;

		dec		temp
	}

/*
	float zsc = (worldZ * m_invCellSize) + (m_grid.GetHeight() / 2);
	int temp = int(zsc);
	dbAssert( temp == floorf(zsc) );
*/
	return temp;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//	
inline void PreciseTerrainMap::WorldToCell( const Vec2f& world, Vec2i& cell ) const
{
	cell.x = WorldXToCellX(world.x);
	cell.y = WorldZToCellZ(world.y);
#if 0
	//	0.500000001 or so
	const float half = 0.50001f;

	// todo : interleaving integer and float will speed this up
	__asm
	{

		mov		ecx, this

		// setup constants
		fld		half								// half

		mov		eax, world

		fld		[ecx]this.m_invCellSize				// m_invCellSize | half

		fld		[ecx]this.m_halfGridHeight			// m_halfGridHeight | m_invCellSize | half

		fld		[eax]world.y						// world.y | m_halfGridHeight | m_invCellSize | half

		fld		[eax]world.x						// world.x | world.y | m_halfGridHeight | m_invCellSize | half

		// x

		fmul	ST(0), ST(3)						// world.x*m_invCellSize | world.y | m_halfGridHeight | m_invCellSize | half

		mov		ebx, cell

		fadd	ST(0), ST(2)						// world.x*m_invCellSize + m_halfGridHeight | world.y | m_halfGridHeight | m_invCellSize | half

		fadd	ST(0), ST(4)						// world.x*m_invCellSize + m_halfGridHeight + half | world.y | m_halfGridHeight | m_invCellSize | half


		// store cell.x
		fistp	dword ptr [ebx]						// world.y | m_halfGridHeight | m_invCellSize | half

		// y

		fmul	ST(0), ST(2)						// world.y*m_invCellSize | m_halfGridHeight | m_invCellSize | half

		dec		[ebx]

		fadd	ST(0), ST(1)						// world.y*m_invCellSize + m_halfGridHeight | m_halfGridHeight | m_invCellSize | half

		fadd	ST(0), ST(3)						// world.y*m_invCellSize + m_halfGridHeight + half | m_halfGridHeight | m_invCellSize | half

		// store cell.y		
		fistp	dword ptr [ebx+4]					// m_halfGridHeight | m_invCellSize | half


		// clear the fp stack
		fstp	ST(0)								// m_invCellSize | half
		fstp	ST(0)								// half
		fstp	ST(0)								// 

		dec		[ebx+4]
	}
#endif
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//	
inline void PreciseTerrainMap::WorldToCell( const Vec3f& world, Vec2i& cell ) const
{
	cell.x = WorldXToCellX(world.x);
	cell.y = WorldZToCellZ(world.z);
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void	PreciseTerrainMap::PreciseToImpass( int prX, int prY, int& tcX, int& tcY ) const
{
	tcX = prX >> 1;
	tcY = prY >> 1;
}