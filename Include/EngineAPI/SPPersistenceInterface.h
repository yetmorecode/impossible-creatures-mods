/////////////////////////////////////////////////////////////////////
// File    : SPPersistenceInterface.h
// Desc    : 
// Created : Tuesday, November 20, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include <EngineAPI/ControllerBlueprint.h>

// forward declarations
class ByteStream;

/////////////////////////////////////////////////////////////////////
// SPPersistenceInterface Interface

class SPPersistenceInterface
{
// interface
public:

	virtual void SaveEBPID( ByteStream&, const EBPNetworkID& ) = 0;

	virtual void LoadEBPID( ByteStream&, EBPNetworkID& ) = 0;
};
