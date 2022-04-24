/////////////////////////////////////////////////////////////////////
// File    : PathSectorMapImpl.h
// Desc    : 
// Created : Friday, November 16, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "PathTypes.h"
#include "AstarSector.h"
#include "AstarPool.h"

#include <Memory/MemoryAlloc.h>
#include <Assist/Array2D.h>

/////////////////////////////////////////////////////////////////////
//	Forward Declarations : 
//
class PathSector;
class TerrainCellMap;
class BiFF;
class IFF;

/////////////////////////////////////////////////////////////////////
//	Class	: PathSectorMapImpl
//	Desc	: the private data of a PathSectorMap
//
class PathSectorMapImpl
{
// Types
public:

	typedef std::vector< PathMajorCell > SubSectorToMajorMap;

	typedef std::vector< PathSector* > PathSectorList;
	typedef PathSectorList::iterator		PathSectorListI;						// PathSectorList iterator
	typedef PathSectorList::const_iterator	PathSectorListCI;						// PathSectorList const_iterator

	template< class T >
		class CellGridAllocator
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

	typedef Array2D<PathSubCell, CellGridAllocator<PathSubCell> > CellGrid;

// Data
public:

	bool						m_bLoaded;									// was the PathSectorMap loaded from disk
	float						m_cellSize;									// the size of a cell.  MUST match the TerrainCellMap!!
	CellGrid					m_cellGrid;									// the divided sectors

	PathSectorList				m_pathSectors;								// the list of PathSectors.  Cell is the index to get the corresponding PathSector*

	SubSectorToMajorMap			m_subSectorToLandSector;					// the structor that maps subSectors to land majorSectors.  Cell is the index to get the corresponding major sector
	SubSectorToMajorMap			m_subSectorToAmphSector;					// the structor that maps subSectors to amphibian majorSectors.  Cell is the index to get the corresponding major sector

	mutable PathSectorListCI	m_iterator;									// used by IteratorBegin and IteratorNext

// Methods
public:

	PathSectorMapImpl();

	bool						Save(IFF &iff) const;										// save the PathSectorMap
	unsigned long				Load(IFF &iff);												// load the PathSectorMap
	void						AddLoadHandler(IFF &iff);


	void						DeleteSectors();											// deletes the PathSectors

	//PathSubCell*		LookupCell( int x, int z ) const;

	const PathSector*			IteratorBegin() const;
	const PathSector*			IteratorNext() const;

	const	PathSector*			GetPathSector( PathSubCell cell ) const;					// given a Cell return the associated PathSector*
			PathSector*			GetPathSector( PathSubCell cell );
	const	PathSector*			GetPathSector( int x, int z ) const;						// given the coords into the map, return the associated PathSector*
			PathSector*			GetPathSector( int x, int z );

	inline void					SetCell( int x, int z, const PathSubCell value );
	inline const PathSubCell	GetCell( int x, int z ) const;


	inline unsigned long		GetWidth() const;
	inline unsigned long		GetHeight() const;
	inline float				GetCellSize() const;

	inline float				CellXToWorldX( int cellX ) const;
	inline float				CellZToWorldZ( int cellZ ) const;
	inline int					WorldXToCellX( float worldX ) const;
	inline int					WorldZToCellZ( float worldZ ) const;

	inline void					CellToWorld( const Vec2i& cell, Vec2f& world) const;
	inline void					WorldToCell( const Vec2f& world, Vec2i& cell) const;

	inline PathMajorCell		GetLandMajorSector( PathSubCell cell ) const;
	inline PathMajorCell		GetLandMajorSector( int x, int y ) const;

	inline PathMajorCell		GetAmphMajorSector( PathSubCell cell ) const;
	inline PathMajorCell		GetAmphMajorSector( int x, int y ) const;

	bool						DoFindBestUsingLine( const int hitherX, const int hitherZ, 
													 const int yonX, const int yonZ,
													 bool bAmphibian,
													 int& resultX, int& resultZ ) const;

	size_t						GetMemUsage() const;

	static void* operator new   ( size_t );										// operator new
	static void  operator delete( void* );										// operator delete
};

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline PathMajorCell PathSectorMapImpl::GetLandMajorSector( PathSubCell cell ) const
{
	return m_subSectorToLandSector[cell];
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline PathMajorCell PathSectorMapImpl::GetLandMajorSector( int x, int z ) const
{
	return GetLandMajorSector( GetCell( x, z ) );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline PathMajorCell PathSectorMapImpl::GetAmphMajorSector( PathSubCell cell ) const
{
	return m_subSectorToAmphSector[cell];
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline PathMajorCell PathSectorMapImpl::GetAmphMajorSector( int x, int z ) const
{
	return GetAmphMajorSector( GetCell( x, z ) );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline unsigned long PathSectorMapImpl::GetWidth() const
{
	return m_cellGrid.GetWidth();
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline unsigned long PathSectorMapImpl::GetHeight() const
{
	return m_cellGrid.GetHeight();
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline float PathSectorMapImpl::GetCellSize() const
{
	return m_cellSize;
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathSubCell PathSectorMapImpl::GetCell( int x, int z ) const
{
	return m_cellGrid.GetValue(x, z);
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void PathSectorMapImpl::SetCell( int x, int z, const PathSubCell value )
{
	m_cellGrid.SetValue( x, z, value );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline float PathSectorMapImpl::CellXToWorldX( int cellX ) const
{
	return ( cellX - ( (int)GetWidth() / 2 ) ) * GetCellSize() + GetCellSize() * 0.5f;
	
//	return( m_pimpl->m_tcMap->CellXToWorldX( cellX ));
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline float PathSectorMapImpl::CellZToWorldZ( int cellZ ) const
{
	return ( cellZ - ( (int)GetWidth() / 2 ) ) * GetCellSize() + GetCellSize() * 0.5f;

//	return( m_pimpl->m_tcMap->CellZToWorldZ( cellZ ));
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int PathSectorMapImpl::WorldXToCellX( float worldX ) const
{
	float xsc = worldX / GetCellSize();
	return (int)(xsc + ((int)GetWidth() / 2) );
	
//	return( m_pimpl->m_tcMap->WorldXToCellX( worldX ) );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline int PathSectorMapImpl::WorldZToCellZ( float worldZ ) const
{
	float xsc = worldZ / GetCellSize();
	return (int)(xsc + ((int)GetHeight() / 2) );
	
//	return( m_pimpl->m_tcMap->WorldZToCellZ( worldZ ) );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void PathSectorMapImpl::CellToWorld( const Vec2i& cell, Vec2f& world) const
{
	world.x = CellXToWorldX(cell.x);
	world.y = CellZToWorldZ(cell.y);
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void PathSectorMapImpl::WorldToCell( const Vec2f& world, Vec2i& cell) const
{
	cell.x = WorldXToCellX(world.x);
	cell.y = WorldZToCellZ(world.y);
}
