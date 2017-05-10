#include "dynamic_array.h"
#include "ydef.h"

int YArray_init(YArray *a,size_t elemsize,unsigned int initSize){
    a->elemSize = elemsize;
    a->len = 0;
    a->size = initSize;
    a->elem = ya_malloc(NULL,elemsize * a->size);

    return 0;
}
int YArray_free(YArray *a,void **data){
    if(data != NULL){
        *data = a->elem;
    }
    else{
        ya_free(NULL,a->elem);
    }
}
void *YArray_get(YArray *a,unsigned int index){
    return (char *)a->elem + index * a->elemSize;
}
void *YArray_push(YArray *a){
    if(a->len >= a->size){
        a->size *= 2;
        a->elem = ya_realloc(NULL,a->elem,a->elemSize * a->size);
    }
    return (char *)a->elem + a->elemSize * a->len++;
}
void *YArray_pop(YArray *a){
    return (char *)a->elem + a->elemSize * --a->len;
}