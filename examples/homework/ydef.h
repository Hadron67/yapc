#ifndef __YDEF_H__
#define __YDEF_H__

#include <stdlib.h>

#include "yalloc.h"

#if defined _WIN32 || defined __CYGWIN__
    #ifdef BUILDING_DLL
        #ifdef __GNUC__
            #define YAPC_API __attribute__ ((dllexport))
        #else
            #define YAPC_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
        #endif
    #else
        #ifdef __GNUC__
            #define YAPC_API __attribute__ ((dllimport))
        #else
            #define YAPC_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
        #endif
    #endif
#else
    #ifdef BUILDING_DLL
        #if __GNUC__ >= 4
            #define YAPC_API __attribute__ ((visibility ("default")))
        #else
            #define YAPC_API
        #endif
    #else
        #define YAPC_API
    #endif
#endif

#ifdef __GNUC__
    #define YA_NORETURN __attribute(("noreturn"))
#else
    #define YA_NORETURN 
#endif

#define YYTAB "    "

#define YERROR_COLORED "\033[31;1merror:\033[0m "
#define YFATAL_ERROR_COLORED "\033[31;1mfatal error:\033[0m "




#endif
