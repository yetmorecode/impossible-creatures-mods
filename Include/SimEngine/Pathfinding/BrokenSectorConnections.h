/////////////////////////////////////////////////////////////////////
// File    : BrokenSectorConnections.h
// Desc    : 
// Created : Monday, January 7, 2002
// Author  : dswinerd
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "PathTypes.h"

/////////////////////////////////////////////////////////////////////
//	BrokenSectorConnections
//
class BrokenSectorConnections
{
// Methods
public:
	
	void Break( PathSubCell sector1, PathSubCell sector2 );								// breaks a connection
	void Clear();																		// clears a breaks

	bool IsBroken( PathSubCell sector1, PathSubCell sector2 ) const;					// is sector connection broken?

	bool AnyBroken() const;

// Methods
private:
	unsigned long BuildConnection( PathSubCell sector1, PathSubCell sector2 ) const;

// Types
private:

	typedef std::set<unsigned long> BlockedConnectionSet;

// Data
private:

	BlockedConnectionSet m_connections;	
};

