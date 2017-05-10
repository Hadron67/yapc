#ifndef __YARRAY_H__
#define __YARRAY_H__

#include "ydef.h"

typedef struct _YArray{
    yheap_t *heap;    
    void *elem;
    size_t elemSize;
    unsigned int len,size;
}YArray;

int YArray_init(YArray *a,size_t elemsize,unsigned int initSize,yheap_t *heap);
int YArray_free(YArray *a,void **data);
void *YArray_get(YArray *a,unsigned int index);
void *YArray_push(YArray *a);
void *YArray_pop(YArray *a);


#endif