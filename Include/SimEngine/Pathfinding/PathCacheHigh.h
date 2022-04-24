/////////////////////////////////////////////////////////////////////
// File    : PathCacheHigh.h
// Desc    : 
//			 
// Created : Monday, May 27, 2002
// Author  : dswinerd
// 
// (c) 2002 Relic Entertainment Inc.
//

#pragma once

#include "PathTypes.h"

#include <SimEngine/SimHighPath.h>

/////////////////////////////////////////////////////////////////////
//	Class	: PathCacheHigh
//
class PathCacheHigh
{
// Types
public:

	struct CacheEntry
	{
		// keys
		PathSubCell				m_startSector;
		PathSubCell				m_goalSector;
		int						m_entitySize;
		bool					m_bAmphibian;
		bool					m_bToBuilding;

		// results
		AstarSectorPathResult	m_pathResult;
		SimHighPath::Path		m_path;
	};


public:
	
	PathCacheHigh();
	~PathCacheHigh();

	void	SetCacheSize( size_t num );

	void	Cache( PathSubCell startSector, PathSubCell goalSector, int entitySize, bool bAmphibian, bool bToBuilding, 
				   const AstarSectorPathResult& pathResult, const SimHighPath::Path& path );

	bool	CheckCache( PathSubCell startSector, PathSubCell goalSector, int entitySize, bool bAmphibian, bool bToBuilding,
						AstarSectorPathResult& pathResult, SimHighPath::Path& path );

	static void* operator new   ( size_t );
	static void  operator delete( void* );


// Types
private:

	typedef std::list<CacheEntry*>	CacheEntries;

// Functions
private:
	

// Data
private:

	size_t				m_cacheSize;
	CacheEntries		m_cacheEntries;
	
};