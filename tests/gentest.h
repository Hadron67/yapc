#ifndef __YY_H__
#define __YY_H__

#define T_EOF 0
#define T_ID 1
#define T_PLUS 2
#define T_NUM 3


typedef int size_t;
typedef void *(*yyalloc)(size_t size);
typedef void *(*yyrealloc)(void *p,size_t size);
typedef int (*yyfree)(void *p);


typedef struct _yyParser{
   yyalloc altor;
   yyfree dtor;
   yyrealloc rtor;
   //state stack
   int *state;
   int sLen,sSize;
   //sematic stack
   int *pstack,*sp;
   int pSize;
}yyParser;


int yyParser_init(yyParser *yyparser,yyalloc altor,yyrealloc rtor,yyfree dtor);
int yyParser_free(yyParser *yyparser);
int yyParser_acceptToken(yyParser *yyparser,int yytokenid,int yytoken);


#endif