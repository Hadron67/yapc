#ifndef __JSON_H__
#define __JSON_H__

#include <stdio.h>

typedef enum _jtype_t{
    JTYPE_UNUSED = 1,
    JTYPE_NUM,
    JTYPE_BOOL,
    JTYPE_STRING,
    JTYPE_ARRAY,
    JTYPE_OBJ,
    JTYPE_UNDEFINED,
    JTYPE_NULL
}jtype_t;

typedef struct _jsonval jsonval;
typedef struct _jsonArray jsonArray;
typedef struct _jsonObjEntry jsonObjEntry;
typedef struct _jsonObj jsonObj;

struct _jsonArray{
    int len,size;
    jsonval *val;
};

struct _jsonObj{
    int len,size;
    jsonObjEntry *val;
};

struct _jsonval{
    jtype_t type;
    void *reserved;
    union {
        double numv;
        int boolv;
        struct jstring{
            char *s;
            int length;
        }stringv;
        jsonArray arrayv;
        jsonObj objv;
    }u;
};

struct _jsonObjEntry{
    char *key;
    int length;
    jsonval val;
};

extern jsonval jsonnull;
extern jsonval jsonundefined;

int JSON_init(jsonval *val,jtype_t type);
int JSON_parse(FILE *input,jsonval *out,FILE *err);
int JSON_stringify(jsonval *val,int indent,FILE *out);
jsonval *JSON_match(jsonval *val,const char *input,FILE *err);
int JSON_free(jsonval *val);

int jsonArray_push(jsonval *array,const jsonval *val);
jsonval *jsonArray_get(jsonval *array,int index);
jsonval *jsonObj_get(jsonval *obj,const char *key,int keylen);
int jsonObj_add(jsonval *obj,char *key,int keylen,const jsonval *val);

#endif
