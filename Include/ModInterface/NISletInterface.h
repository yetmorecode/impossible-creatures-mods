/////////////////////////////////////////////////////////////////////
// File    : NISletInterface.h
// Desc    : 
// Created : Thursday, Jan 3, 2002
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

class EntityGroup;

class NISletInterface
{
public:
	// teleport entity group to new location
	virtual void Teleport( const EntityGroup& eg, const Vec2f& destination ) = 0;
};