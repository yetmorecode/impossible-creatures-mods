/////////////////////////////////////////////////////////////////////
// File  : MiniMap.h
// Author: Shelby
// Date  : 2001-04-06
//      (c) relic entertainment inc.2001
// 
// *
// 

#pragma once

#include <Assist/Array2d.h>

// forward declaration
class Player;
class Colour;
class BitPlane;

///////////////////////////////////////////////////////////////////// 
// MiniMap

class MiniMap
{
// interface
public:

	//	Minimap marking
	virtual void	BeginMarkingPoints() = 0;
	virtual void	BeginMarkingCircles() = 0;
	virtual void	EndMarkingPoints() = 0;
	virtual void	EndMarkingCircles() = 0;
	virtual void	AddPoint( float x, float y, const Colour& c ) = 0;
	virtual void	AddPoint( float x, float y, const Player* ) = 0;
	virtual void	AddCircle( float x, float y, float radius, const Colour& c ) = 0;

	virtual void	UpdateFOW( const Array2D< unsigned long >& fowvision, unsigned long visiblemask, unsigned long exploredmask ) = 0;

	virtual void	SetMapSize( unsigned long width, unsigned long height ) = 0;
	virtual void	RevealAll( ) = 0;

	// Set the UI mode for the MiniMap
	virtual void	SetModalClickCapture( bool bCapture ) = 0;

};
