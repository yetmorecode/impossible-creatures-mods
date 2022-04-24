/////////////////////////////////////////////////////////////////////
// File    : ArmyBuilderInterface.h
// Desc    : 
// Created : Wednesday, December 12, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <EngineAPI/ControllerBlueprint.h>

class ArmyBuilderInterface
{
public:

	virtual bool	IsAnimalAllowed		( const char* ) const = 0;

	virtual void	PermitAnimalAccess	( const char* ) = 0;

	virtual void	ForbidAnimalAccess	( const char* ) = 0;

	// Zoo storage implementation
	virtual void	AddCBPToZoo			( const EBPNetworkID& ) = 0;
	virtual void	RemoveCBPFromZoo	( const EBPNetworkID& ) = 0;
	virtual bool	IsZooFull			( ) = 0;

	virtual size_t	GetZooCreatureCount	( ) = 0;
	virtual void	GetCreatureAt		( size_t index, EBPNetworkID& ) = 0;
};
