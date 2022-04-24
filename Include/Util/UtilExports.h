/////////////////////////////////////////////////////////////////////
// 

#ifndef RELIC_LIB
    #ifdef UTIL_EXPORTS
	    #define UTIL_API __declspec(dllexport)
    #else
	    #define UTIL_API __declspec(dllimport)
    #endif
#else
    #define UTIL_API /**/
#endif // RELIC_LIB
