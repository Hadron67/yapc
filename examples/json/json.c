#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "parser.h"

#define jmalloc malloc
#define jrealloc realloc
#define jfree free

jsonval jsonnull = { JTYPE_NULL };
jsonval jsonundefined = { JTYPE_UNDEFINED };

#define JBLOCKSIZE 128

typedef struct _jStringBlock{
    struct _jStringBlock *next,*prev;
    int len,size;
    char data[1];
}jStringBlock;

static jStringBlock *jStringBlock_new(jStringBlock *prev,int size){
    jStringBlock *b = (jStringBlock *)jmalloc(sizeof(jStringBlock) + size * sizeof(char));
    b->len = 0;
    b->size = size;
    b->next = NULL;
    b->prev = prev;
    if(prev != NULL){
        prev->next = b;
    }
    return b;
}

static char *jStringBlock_addString(jStringBlock *b,const char *s,int length){
    if(length <= b->size - b->len){
        char *ret = b->data + b->len;
        strncpy(ret,s,length);
        b->len += length;
        return ret;
    }
    return NULL;
}

static int jStringBlock_free(jStringBlock *b){
    while(b != NULL){
        jStringBlock *temp = b->next;
        jfree(b);
        b = temp;
    }
    return 0;
}

typedef struct _jParser{
    yyParser parser;
    FILE *input,*err;
    char c;
    int tokenid;
    jStringBlock *head,*cBlock;
    int len,size;
    char *buf;
}jParser;

static int jParser_init(jParser *p,FILE *err){
    yyParser_init(&p->parser);
    p->err = err;
    p->len = 0;
    p->size = 16;
    p->buf = (char *)jmalloc(sizeof(char) * p->size);
    
    
    return 0;
}

static int jParser_reInit(jParser *p,FILE *err){
    yyParser_reInit(&p->parser);
    p->err = err;
    p->len = 0;
    
    return 0;
}

static int jParser_pushChar(jParser *p,char c){
    if(p->len >= p->size){
        p->size *= 2;
        p->buf = (char *)jrealloc(p->buf,sizeof(char) * p->size);
    }
    p->buf[p->len++] = c;
    return 0;
}

static char *jParser_commitS(jParser *p){
    char *ret = jStringBlock_addString(p->cBlock,p->buf,p->len);
    if(ret == NULL){
        p->cBlock = jStringBlock_new(p->cBlock,p->len > JBLOCKSIZE ? p->len : JBLOCKSIZE);
        ret = jStringBlock_addString(p->cBlock,p->buf,p->len);
    }
    p->len = 0;
    return ret;
}

static int jParser_isS(jParser *p,const char *s){
    while(*s){
        if(p->c != *s){
            return 0;
        }
        s++;
        p->c = fgetc(p->input);
    }
    return 1;
}

static int jParser_free(jParser *p){
    yyParser_free(&p->parser);
    jfree(p->buf);
    return 0;
}

static int jParser_next(jParser *p){
    #define C (p->c)
    #define NC() (p->c = fgetc(p->input))
    #define IEOF (feof(p->input))
    #define T p->tokenid
    jsonval *token = &p->parser.token;
    while(C == ' ' || C == '\n' || C == '\t'){
        NC();
    }
    if(IEOF){
        T = T_EOF;
        return 0;
    }
    JSON_init(token,JTYPE_UNUSED);
    switch(C){
        case '(':
            NC();
            T = T_BRA;
            return 0;
        case ')':
            NC();
            T = T_KET;
            return 0;
        case '[':
            NC();
            T = T_CBRA;
            return 0;
        case ']':
            NC();
            T = T_CKET;
            return 0;
        case '{':
            NC();
            T = T_BBRA;
            return 0;
        case '}':
            NC();
            T = T_BKET;
            return 0;
        case ':':
            NC();
            T = T_COLLON;
            return 0;
        case ',':
            NC();
            T = T_COMMA;
            return 0;
        case 't':
            NC();
            if(C == 'r'){
                NC();
                if(C == 'u'){
                    NC();
                    if(C == 'e'){
                        NC();
                        JSON_init(token,JTYPE_BOOL);
                        token->u.boolv = 1;
                        T = T_TRUE;
                        return 0;
                    }
                }
            }
            goto yyunexpected;
        case 'f':
            NC();
            if(jParser_isS(p,"alse")){
                JSON_init(token,JTYPE_BOOL);
                token->u.boolv = 0;
                T = T_FALSE;
                return 0;
            }
            goto yyunexpected;
        case 'n':
            NC();
            if(jParser_isS(p,"ull")){
                JSON_init(token,JTYPE_NULL);
                T = T_NULL;
                return 0;
            }
            goto yyunexpected;
        case 'u':
            NC();
            if(jParser_isS(p,"ndefined")){
                JSON_init(token,JTYPE_UNDEFINED);
                T = T_UNDEFINED;
                return 0;
            }
            goto yyunexpected;
        default:
            if(C >= '0' && C <= '9'){
                double num = 0;
                while(C >= '0' && C <= '9'){
                    num *= 10;
                    num += C - '0';
                    NC();
                }
                JSON_init(token,JTYPE_NUM);
                token->u.numv = num;
                T = T_NUM;
                return 0;
            }
            else if(C == '"' || C == '\''){
                char ter = C;
                NC();
                while(C != ter){
                    if(IEOF){
                        fprintf(p->err,"unexpected character end of file: unclosed string literal\n");
                        return -1;
                    }
                    if(C == '\\'){
                        NC();
                        if(IEOF){
                            fprintf(p->err,"unexpected character end of file: expecting excape character.\n");
                            return -1;
                        }
                        jParser_pushChar(p,C);
                        NC();
                    }
                    else {
                        jParser_pushChar(p,C);
                        NC();
                    }
                }
                NC();
                JSON_init(token,JTYPE_STRING);
                token->u.stringv.length = p->len;
                token->u.stringv.s = jParser_commitS(p);
                T = T_STRING;
                return 0;
            }
            goto yyunexpected;
    }
yyunexpected:
    fprintf(p->err,"unexpected character '%c'\n",C);
    return -1;
    #undef C
    #undef NC
    #undef T
}

static int jParser_parse(jParser *p,FILE *input,jsonval *val){
    p->input = input;
    p->c = fgetc(input);
    p->head = p->cBlock = jStringBlock_new(NULL,JBLOCKSIZE);
    JSON_init(val,JTYPE_UNUSED);
    while(!p->parser.done){
        if(jParser_next(p)){
            goto jjerr;
        }
        if(yyParser_acceptToken(&p->parser,p->tokenid)){
            yyParser_printError(&p->parser,p->err);
            goto jjerr;
        }
    }
    *val = *p->parser.pstack;
    val->reserved = p->head;
    p->head = p->cBlock = NULL;
    return 0;
jjerr:
    //yyParser_clearStack(&p->parser);
    jStringBlock_free(p->head);
    p->head = p->cBlock = NULL;
    return -1;
}

int JSON_parse(FILE *input,jsonval *out,FILE *err){
    jParser p;
    jParser_init(&p,err);
    int ret = jParser_parse(&p,input,out);
    jParser_free(&p);
    return ret;
}

//------------------------------------------------------------------------
int JSON_init(jsonval *val,jtype_t type){
    memset(val,0,sizeof(jsonval));
    val->type = type;
    if(type == JTYPE_ARRAY){
        jsonArray *array = &val->u.arrayv;
        array->size = 16;
        array->val = (jsonval *)jmalloc(sizeof(jsonval) * array->size);
    }
    else if(type == JTYPE_OBJ){
        jsonObj *obj = &val->u.objv;
        obj->size = 16;
        obj->val = (jsonObjEntry *)jmalloc(sizeof(jsonObjEntry) * obj->size);
    }
    return 0;
}
static int printS(FILE *out,const char *s,int len){
    int i;
    for(i = 0;i < len;i++){
        if(s[i] == '"'){
            fprintf(out,"\\\"");
        }
        else if(s[i] == '\n'){
            fprintf(out,"\\n");
        }
        else {
            fprintf(out,"%c",s[i]);
        }
    }
    return 0;
}
static int printIndent(FILE *out,int i){
    while(i --> 0){
        fprintf(out,"    ");
    }
}
int JSON_stringify(jsonval *val,int indent,FILE *out){
    if(val->type == JTYPE_NUM){
        fprintf(out,"%lf",val->u.numv);
    }
    else if(val->type == JTYPE_BOOL){
        fprintf(out,val->u.boolv ? "true" : "false");
    }
    else if(val->type == JTYPE_STRING){
        fprintf(out,"\"");
        char *s = val->u.stringv.s;
        int len = val->u.stringv.length;
        printS(out,s,len);
        fprintf(out,"\"");
    }
    else if(val->type == JTYPE_NULL){
        fprintf(out,"null");
    }
    else if(val->type == JTYPE_UNDEFINED){
        fprintf(out,"undefined");
    }
    else if(val->type == JTYPE_ARRAY){
        jsonArray *array = &val->u.arrayv;
        int i;
        fprintf(out,"[ ");
        for(i = 0;i < array->len;i++){
            JSON_stringify(array->val + i,indent == -1 ? -1 : indent,out);
            if(i < array->len - 1){
                fprintf(out,",");
            }
        }
        fprintf(out," ]");
    }
    else if(val->type == JTYPE_OBJ){
        if(val == &jsonnull){
            fprintf(out,"null");
        }
        else {
            jsonObj *obj = &val->u.objv;
            if(obj->len == 0){
                fprintf(out,"{}");
            }
            else{
                int i;
                fprintf(out,"{ \n");
                for(i = 0;i < obj->len;i++){
                    jsonObjEntry *entry = obj->val + i;
                    printIndent(out,indent + 1);
                    fprintf(out,"\"");
                    printS(out,entry->key,entry->length);
                    fprintf(out,"\"");
                    fprintf(out," : ");
                    JSON_stringify(&entry->val,indent == -1 ? -1 : indent + 1,out);
                    if(i < obj->len - 1){
                        fprintf(out,",\n");
                    }
                }
                fprintf(out,"\n");
                printIndent(out,indent);
                fprintf(out,"}");
            }
        }        
    }
    return 0;
}
int JSON_free(jsonval *val){
    if(val->reserved != NULL){
        jStringBlock_free((jStringBlock *)val->reserved);
    }
    if(val->type == JTYPE_ARRAY){
        int i;
        jsonArray *array = &val->u.arrayv;
        for(i = 0;i < array->len;i++){
            JSON_free(array->val + i);
        }
        jfree(array->val);
    }
    else if(val->type == JTYPE_OBJ){
        int i;
        jsonObj *obj = &val->u.objv;
        for(i = 0;i < obj->len;i++){
            JSON_free(&obj->val[i].val);
        }
        jfree(obj->val);
    }
    return 0;
}

int jsonArray_push(jsonval *val,const jsonval *val1){
    if(val->type == JTYPE_ARRAY){
        jsonArray *array = &val->u.arrayv;
        if(array->len >= array->size){
            array->size *= 2;
            array->val = (jsonval *)jrealloc(array->val,sizeof(jsonval) * array->size);
        }
        array->val[array->len++] = *val1;
        return 0;
    }
    else{
        return -1;
    }
}
jsonval *jsonArray_get(jsonval *array,int index){
    if(array->type == JTYPE_ARRAY){
        jsonArray *val = &array->u.arrayv;
        if(index >= 0 && index < val->len){
            return val->val + index;
        }
        else{
            return &jsonundefined;
        }
    }
    else {
        return &jsonundefined;
    }
}
int jsonArray_length(jsonval *array){
    if(array->type == JTYPE_ARRAY){
        jsonArray *val = &array->u.arrayv;
        return val->len;
    }
    else{
        return 0;
    }
}

jsonval *jsonObj_get(jsonval *val,const char *key,int keylen){
    if(val->type == JTYPE_OBJ){
        jsonObj *obj = &val->u.objv;
        int i;
        for(i = 0;i < obj->len;i++){
            jsonObjEntry *entry = obj->val + i;
            if(keylen == entry->length){
                if(!strncmp(key,entry->key,keylen)){
                    return &entry->val;
                }
            }
        }
        return &jsonundefined;
    }
    else {
        return &jsonundefined;
    }
}
int jsonObj_add(jsonval *obj,char *key,int keylen,const jsonval *val){
    if(obj->type == JTYPE_OBJ){
        jsonObj *obj1 = &obj->u.objv;
        if(jsonObj_get(obj,key,keylen) != &jsonundefined){
            return -2;
        }
        if(obj1->len >= obj1->size){
            obj1->size *= 2;
            obj1->val = (jsonObjEntry *)jrealloc(obj1->val,sizeof(jsonObjEntry) * obj1->size);
        }
        jsonObjEntry *entry = obj1->val + obj1->len++;
        entry->key = key;
        entry->length = keylen;
        entry->val = *val;
        return 0;
    }    
    else{
        return -1;
    }
}



