/////////////////////////////////////////////////////////////////////
// File    : DialogTriggerFactory.h
// Desc    : 
// Created : Monday, September 17, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

// forward declaration
class TExpression;
class World;

///////////////////////////////////////////////////////////////////// 
// DialogTriggerFactory

class DialogTriggerFactory
{
public:
	// modinterface
	virtual ~DialogTriggerFactory(){;}

	virtual void			Evaluate( World* ) = 0;
};
