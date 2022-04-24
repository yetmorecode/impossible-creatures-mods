/////////////////////////////////////////////////////////////////////
//	File	: SimEngineExport declaration
//	Desc.	: This files contains useful typedefs for use in the
//			  Simulation engine.  No file should EVER be included
//			  In this header.
//
//		19.Nov.00 (c) Relic Entertainment Inc.
//
#pragma once


#ifdef SIMENGINE_EXPORTS

#	define SIM_API __declspec(dllexport)

#else

#	define SIM_API __declspec(dllimport)

#endif