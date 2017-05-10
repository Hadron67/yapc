#ifndef __YALLOC_H__
#define __YALLOC_H__

#include <stdlib.h>

#define ya_malloc(heap,size) ((heap) == NULL ? malloc((size)) : ((yheap_t *)heap)->malloc((size),((yheap_t *)heap)->arg))
#define ya_realloc(heap,ptr,size) ((heap) == NULL ? realloc((ptr),(size)) : ((yheap_t *)heap)->realloc((ptr),(size),((yheap_t *)heap)->arg))
#define ya_free(heap,ptr) ((heap) == NULL ? free(ptr) : ((yheap_t *)heap)->free((ptr),((yheap_t *)heap)->arg))

typedef void *(*yallocator)(size_t size,void *arg);
typedef void *(*yreallocator)(void *ptr,size_t size,void *arg);
typedef void (*yfree)(void *ptr,void *arg);


typedef struct _yheap_t{
    yallocator malloc;
    yreallocator realloc;
    yfree free;
    void *arg;
}yheap_t;

#endif