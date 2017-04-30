#ifndef __YDEF_H__
#define __YDEF_H__
#define ya_malloc malloc
#define ya_realloc realloc
#define ya_free free

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

#define YYTAB "    "

#endif
