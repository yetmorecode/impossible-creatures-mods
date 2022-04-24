/////////////////////////////////////////////////////////////////////
// File    : SimEntity.h
// Desc    : 
// Created : Thursday, August 02, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "SimExports.h"
#include "Entity.h"
#include "SpatialBucket.h"

// forward declarations

///////////////////////////////////////////////////////////////////// 
// SimEntity

class SIM_API SimEntity : public Entity
{
// construction
public:
	SimEntity(EntityIDNumber EID, const ControllerBlueprint* pCBP);
	virtual ~SimEntity();

// interface
public:
	// SimDynamics Entity Specific Data, e.g. spatial bucket stuff

	inline SpatialBucket*		GetSB()
									{ return &m_Bucket; }

	inline bool					GetInPathfinding() const
									{ return m_bInPathfinding; }

	inline void					SetInPathfinding( bool bInPathfinding)
									{ m_bInPathfinding = bInPathfinding; }

	inline bool					ShouldNotAddToPathfinding() const
									{ return m_bShouldNotAddToPathfinding; }

	inline void					ClearShouldNotAddToPathfinding()
									{ m_bShouldNotAddToPathfinding = false; }

	inline bool					IsBuilding() const
									{ return m_bIsBuilding; }

	Matrix3f					CalcSnappedOrient() const;

private:

	virtual void SaveBasicEmbedded( IFF& iff ) const;
	virtual void LoadBasicEmbedded( IFF& iff );

// Data
private:
	SpatialBucket	m_Bucket;
	bool			m_bInPathfinding;
	bool			m_bShouldNotAddToPathfinding;
	bool			m_bIsBuilding;

// implementation
private:

// copy -- do not define
private:
	SimEntity( const SimEntity& );
	SimEntity& operator= ( const SimEntity& );
};