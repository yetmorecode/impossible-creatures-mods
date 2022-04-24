/////////////////////////////////////////////////////////////////////
//	File	: ECStaticInfo.h
//	Desc.	: Pure virtual base class for creating and holding onto
//			  MOD specific controller static info.
//		22.Nov.00 (c) Relic Entertainment Inc.
//
#pragma once

#include <EngineAPI/ControllerBlueprint.h>

///////////////////////////////////////////////////////////////////////////////
//  ECStaticInfo

class ECStaticInfo
{
// fields
private:
	const ControllerBlueprint *m_pCBP;

// interface
public:
	ECStaticInfo( const ControllerBlueprint *pCBP )
	  : m_pCBP( pCBP ) 
	{
	}
	
	virtual ~ECStaticInfo() 
	{
	}

// interface
public:
	const ControllerBlueprint* GetControllerBlueprint() const
	{
		return m_pCBP;
	}
};
