/////////////////////////////////////////////////////////////////////
// File    : SpatialBucket.h
// Desc    : 
// Created : Thursday, November 08, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

// Forward declarations.
class Entity;

// return value when the bucket index could not be located
const long k_InvalidBucketIndex = -1;

class SpatialBucket
{

// Data.
public:
	Entity*			m_pEntity;

	SpatialBucket*	m_Next;
	SpatialBucket*	m_Prev;

	long			m_BucketIndex;
};


