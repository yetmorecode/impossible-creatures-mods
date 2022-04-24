/////////////////////////////////////////////////////////////////////
// File    : ModSimVis.h
// Desc    : 
// Created : Thursday, September 13, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

class Entity;

///////////////////////////////////////////////////////////////////// 
// forward declarations

template< class T >	class Array2DNewAllocator;
template< class T, class Allocator = Array2DNewAllocator<T> > class Array2D;

class ModSimVis
{
// interface
public:
	// this is called by the game just before an entity is rendered each frame
	virtual void EntityVisUpdate( const Entity* , const Vec3f& interpPos, bool bSelected ) = 0;

	virtual bool EntityVisible( const Entity* e ) const = 0;

	virtual void Draw( ) = 0;

	virtual const Array2D< unsigned long >* GetFOWInfo( unsigned long& visiblemask, unsigned long& exploredmask ) = 0;

};
