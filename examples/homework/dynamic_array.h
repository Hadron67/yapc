#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

#include <stdlib.h>

typedef struct _YArray{
    void *elem;
    size_t elemSize;
    unsigned int len,size;
}YArray;

int YArray_init(YArray *a,size_t elemsize,unsigned int initSize);
int YArray_free(YArray *a,void **data);
void *YArray_get(YArray *a,unsigned int index);
void *YArray_push(YArray *a);
void *YArray_pop(YArray *a);

#endif