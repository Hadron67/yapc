#include <string.h>
#include <stdlib.h>
#include "json_match.h"
#include "matcher.h"

#define jmalloc malloc
#define jrealloc realloc
#define jfree free

#define ISNUM(a) ((a) >= '0' && (a) <= '9')
#define ISNAMEHEAD(a) (((a) >= 'a' && (a) <= 'z') || ((a) >= 'A' && (a) <= 'Z') || (a) == '_' || (a) == '$')
#define ISNAME(a) (ISNAMEHEAD(a) || ISNUM(a))

static char *sEntry_new(sEntry **prev,char *s,int length){
    sEntry *ret = (sEntry *)jmalloc(sizeof(sEntry) + sizeof(char) * length);
    strncpy(ret->data,s,length);
    ret->next = *prev;
    *prev = ret;
    return ret->data;
}

int jMatch_init(jMatch *m,FILE *err){
    jmParser_init(&m->parser);
    m->sHead = NULL;
    m->parser.userData = m;
    m->len = 0;
    m->size = 16;
    m->err = 0;
    m->errout = err;
    m->buf = (char *)jmalloc(sizeof(char) * m->size);
    
    return 0;
}
int jMatch_free(jMatch *m){
    jmParser_free(&m->parser);
    jfree(m->buf);
    while(m->sHead != NULL){
        sEntry *temp = m->sHead->next;
        jfree(m->sHead);
        m->sHead = temp;
    }
    return 0;
}
static int jMatch_pushChar(jMatch *m,char c){
    if(m->len >= m->size){
        m->size *= 2;
        m->buf = (char *)jrealloc(m->buf,m->size * sizeof(char));
    }
    m->buf[m->len++] = c;
    return 0;
}
int jMatch_match(jMatch *m,jsonval *val,const char *s){
    jnode *node;
    int id;
    m->val = val;
jjlex:
    node = &m->parser.token;
    m->len = 0;
    node->sv = NULL;
    while(*s == ' ' || *s == '\t' || *s == '\n'){
        s++;
    }
    if(!*s){
        id = MT_EOF;
        goto jjparse;
    }
    switch(*s){
        case '[':
            s++;
            id = MT_CBRA;
            goto jjparse;
        case ']':
            s++;
            id = MT_CKET;
            goto jjparse;
        case '.':
            s++;
            if(ISNAMEHEAD(*s)){
                jMatch_pushChar(m,*s);
                s++;
                while(ISNAME(*s)){
                    jMatch_pushChar(m,*s);
                    s++;
                }
                id = MT_MEMBER;
                node->nv = m->len;
                node->sv = sEntry_new(&m->sHead,m->buf,m->len);
                goto jjparse;
            }
            goto jjunexpected;
        case '\'':
        case '"':{
            char t = *s;
            s++;
            while(*s != t){
                jMatch_pushChar(m,*s);
                s++;
            }
            s++;
            id = MT_STRING;
            node->nv = m->len;
            node->sv = sEntry_new(&m->sHead,m->buf,m->len);
            goto jjparse;
        }
        default:
            if(ISNUM(*s)){
                int num = 0;
                while(ISNUM(*s)){
                    num *= 10;
                    num += *s - '0';
                    s++;
                }
                id = MT_NUM;
                node->nv = num;
                goto jjparse;
            }
            goto jjunexpected;
    }
jjparse:
    if(jmParser_acceptToken(&m->parser,id)){
        fprintf(m->errout,"error in pattern string:\n");
        jmParser_printError(&m->parser,m->errout);
        
        goto jjerr;
    }
    if(m->err){
        goto jjerr;
    }
    if(!m->parser.done){
        goto jjlex;
    }
    return 0;
jjunexpected:
    fprintf(m->errout,"error in pattern string:\n");
    fprintf(m->errout,"unexpected character '%c'\n",*s);
jjerr:
    
    return -1;
}
int jMatch_doNum(jMatch *m,const jnode *n){
    if(m->val->type == JTYPE_ARRAY){
        m->val = jsonArray_get(m->val,n->nv);
    }
    else if(m->val->type == JTYPE_UNDEFINED){
        fprintf(m->errout,"Cannot read property %d of undefined.\n",n->nv);
        m->err = 1;
    }
    else if(m->val->type == JTYPE_NULL){
        fprintf(m->errout,"Cannot read property %d of null.\n",n->nv);
        m->err = 1;
    }
    else {
        m->val = &jsonundefined;
    }
    return 0;
}
int printS(FILE *out,const jnode *n){
    int i;
    for(i = 0;i < n->nv;i++){
        fputc(n->sv[i],out);
    }
    return 0;
}
int jMatch_doMember(jMatch *m,const jnode *n){
    if(m->val->type == JTYPE_OBJ){
        m->val = jsonObj_get(m->val,n->sv,n->nv);
    }
    else if(m->val->type == JTYPE_ARRAY && !strncmp(n->sv,"length",n->nv)){
        JSON_init(&m->num,JTYPE_NUM);
        m->num.u.numv = jsonArray_length(m->val);
        m->val = &m->num;
    }
    else if(m->val->type == JTYPE_UNDEFINED){
        fprintf(m->errout,"Cannot read property ");
        printS(m->errout,n);
        fprintf(m->errout," of undefined.\n");
        m->err = 1;
    }
    else if(m->val->type == JTYPE_NULL){
        fprintf(m->errout,"Cannot read property \n");
        printS(m->errout,n);
        fprintf(m->errout," of null.\n");
        m->err = 1;
    }
    else {
        m->val = &jsonundefined;
    }
    return 0;
}
//-----------------------------
jsonval *JSON_match(jsonval *val,const char *input,FILE *err){
    jMatch m;
    jMatch_init(&m,err);
    if(!jMatch_match(&m,val,input)){
        val = m.val;
    }
    else{
        val = NULL;
    }
    jMatch_free(&m);
    return val;
}
