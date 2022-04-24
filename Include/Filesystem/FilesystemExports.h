/////////////////////////////////////////////////////////////////////
// 
// George Hornmoen (September 17th / 2002)
//

#ifndef RELIC_LIB
    #ifdef FILESYSTEM_EXPORTS
	    #define FILESYSTEM_API __declspec(dllexport)
    #else
	    #define FILESYSTEM_API __declspec(dllimport)
    #endif
#else
    #define FILESYSTEM_API /**/
#endif // RELIC_LIB