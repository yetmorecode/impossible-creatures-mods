/////////////////////////////////////////////////////////////////////
// File    : PathSector.h
// Desc    : 
// Created : Monday, January 21, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//


#pragma once

#include "../SimExports.h"
#include "PathTypes.h"

#include "PathSectorAdj.h"

#include <Memory/MemoryAlloc.h>

#include <SimEngine/EntityGroup.h>



/////////////////////////////////////////////////////////////////////
//	Forward Declarations:
//
class BiFF;
class IFF;
class SimEntity;

/////////////////////////////////////////////////////////////////////
//	Class	: PathSector
//
class SIM_API PathSector
{
public:

	PathSector( int minX, int minZ, 
				int width, int height,  
				int centerX, int centerZ, 
				const PathSubCell& subSector, const PathMajorCell& landSector, const PathMajorCell& amphSector);	// ctor
	PathSector( IFF &iff );

	~PathSector();																// dtor

	inline void SetInactive();													// marks the PathSector inactive
	void SetBuilding( Entity *pEntity );										// sets the building on this sector
	void ClearBuilding( );														// sets the building on this sector

	inline const bool	IsActive() const;										// returns whether or not the sector is active
	inline const bool	IsBuilding() const;										// returns whether or not the sector is on a building
	inline const Vec2i& GetSize() const;										// returns the width & height of the sector
	inline const Vec2i& GetCorner() const;										// returns the min corner of the sector
	inline const Vec2i& GetCenter() const;										// returns the rough center of the sector
	inline const PathSubCell&	GetSubSector() const;							// returns the subsector index
	inline const PathMajorCell&	GetLandSector() const;							// returns the land major sector value
	inline const PathMajorCell&	GetAmphSector() const;							// returns the amphibian major sector value
	inline const PathMajorCell& GetMajorSector( bool bAmphibian ) const;		// returns the land or amphibian major sector

	void				 GetMinMax( Vec2i& min, Vec2i& max ) const;				// returns the min and max coordinate
	inline const Entity* GetBuilding() const;									// returns the entity building

	inline const PathSectorAdj& GetAdjacency() const;
	inline		 PathSectorAdj& GetAdjacency();

	void ExpandExtents( int minX, int minZ, int width, int height );			// possible grows the extents of the sector

	size_t	GetMemUsage() const;												// returns the amount of memory used

	void* operator new   ( size_t );											// operator new
	void  operator delete( void* );												// operator delete

	bool	Save(IFF &iff) const;												// save the PathSector
	bool	Load(IFF &iff);														// load the PathSector

// Data
private:

	bool				m_bActive;					// is the sector active.  ie, hasn't been split
//	bool				m_bBuilding;				// is the sector a building
	PathSubCell			m_subSector;				// the subsector index
	PathMajorCell		m_landSector;				// the land major sector value
	PathMajorCell		m_amphSector;				// the amphibian major sector value
	Vec2i				m_extentsCorner;			// the top-left cell coord
	Vec2i				m_extentsSize;				// the number of cells in x and z
	Vec2i				m_center;					// roughly the center of the sector.  This cell must actually be in the sector
	EntityGroup			m_entityGroup;				// will contain a pointer back to the SimEntity that IS this sector (ie. building)

	PathSectorAdj		m_adj;
};


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline void PathSector::SetInactive()
{
	m_bActive = false;
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const bool PathSector::IsActive() const
{
	return( m_bActive );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const bool PathSector::IsBuilding() const
{
	return !m_entityGroup.empty();
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Vec2i& PathSector::GetSize() const
{
	return( m_extentsSize );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Vec2i& PathSector::GetCorner() const
{
	return( m_extentsCorner );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Vec2i& PathSector::GetCenter() const
{
	return( m_center );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathSubCell& PathSector::GetSubSector() const
{
	return( m_subSector );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathMajorCell& PathSector::GetLandSector() const
{
	return( m_landSector );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathMajorCell& PathSector::GetAmphSector() const
{
	return( m_amphSector );
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathMajorCell& PathSector::GetMajorSector( bool bAmphibian ) const
{
	if ( bAmphibian )
	{
		return GetAmphSector();
	}
	else
	{
		return GetLandSector();
	}
}

/*
/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathSector::AdjacencySet& PathSector::GetAdjacencySet() const
{
	return( m_adjacencySet );
}
*/

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const PathSectorAdj& PathSector::GetAdjacency() const
{
	return m_adj;
}

/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline PathSectorAdj& PathSector::GetAdjacency()
{
	return m_adj;
}


/////////////////////////////////////////////////////////////////////
//	Desc.	: 
//	Result	: 
//	Param.	: 
//	Author	: dswinerd
//
inline const Entity* PathSector::GetBuilding() const
{
	if (m_entityGroup.empty())
	{	// no entity
		return NULL;
	}

	return m_entityGroup.front();
}
