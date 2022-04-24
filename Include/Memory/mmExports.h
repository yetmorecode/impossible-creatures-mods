/////////////////////////////////////////////////////////////////////
// File  : mmExports.h
// Author: Drew Dunlop
// Date  : 2002-10-28
//      (c) relic entertainment inc.2000
// 
// *
// 

#pragma once

/////////////////////////////////////////////////////////////////////
// 

#ifndef RELIC_LIB
    #if defined (MEMORY_DLL)
	    #define MMAPI __declspec(dllexport)
    #else
	    #define MMAPI __declspec(dllimport)
    #endif
#else
    #define MMAPI /**/
#endif // RELIC_LIB
