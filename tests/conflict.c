/*
    Generated by yapc -- An LR(1) parser generator made by CFY
*/

#include "./conflict.h"
static const int yytokenCount = 4;
static const int yyntCount = 4;
#ifndef YYMALLOC
    #define YYMALLOC(parser,size) malloc(size)
#endif
#ifndef YYREALLOC
    #define YYREALLOC(parser,ptr,size) realloc((ptr),(size))
#endif
#ifndef YYFREE
    #define YYFREE(parser,ptr) free((ptr))
#endif
#ifndef YYDESTRUCTOR
    #define YYDESTRUCTOR(a)
#endif
#define YYPUSH_STATE(s) \
    if(yyparser->sLen >= yyparser->sSize){ \
        yyparser->sSize *= 2; \
        yyparser->state = (int *)YYREALLOC(yyparser,yyparser->state,sizeof(int) * yyparser->sSize); \
    } \
    yyparser->state[yyparser->sLen++] = (s);

#define YYSTATE() (yyparser->state[yyparser->sLen - 1])
#define YYCHECK_PUSH_TOKEN() \
    if(yyparser->sp - yyparser->pstack >= yyparser->pSize){\
        size_t offset = yyparser->sp - yyparser->pstack;\
        yyparser->pSize *= 2;\
        yyparser->pstack = (int *)YYREALLOC(yyparser,yyparser->pstack,sizeof(int) * yyparser->pSize);\
        yyparser->sp = yyparser->pstack + offset;\
    }
/*
    shift action table
    positive numbers indicate the states shift to,
    negative numbers indicate the rules reduce with.
    the state should be the number in the table minus one,since zero marks
    for error.
*/
static const int yyshift[] = {
    /* state 0 */
         0,     0,     0,     9,
    /* state 1 */
        -1,     0,     0,     0,
    /* state 2 */
        -2,     6,     4,     0,
    /* state 3 */
         0,     0,     0,     9,
    /* state 4 */
        -4,     6,     4,     0,
    /* state 5 */
         0,     0,     0,     9,
    /* state 6 */
        -3,     6,     4,     0,
    /* state 7 */
        -5,    -5,    -5,     0,
    /* state 8 */
        -6,    -6,    -6,     0,
};
/*
    goto table
*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     3,     8,
    /* state 1 */
         0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     0,
    /* state 3 */
         0,     0,     5,     8,
    /* state 4 */
         0,     0,     0,     0,
    /* state 5 */
         0,     0,     7,     8,
    /* state 6 */
         0,     0,     0,     0,
    /* state 7 */
         0,     0,     0,     0,
    /* state 8 */
         0,     0,     0,     0,
};
/*
    the left hand side of each rule,used to determine goto action.
*/
static const int yylhs[] = {
         0,     1,     2,     2,     2,     3,
};
/*
    the length of the symbols on the rhs of each rule
    used to pop states from the state stack when doing
    an reduction.
*/
static const int yyruleLen[] = {
         1,     1,     3,     3,     1,     1,
};
const char *yytokenNames[] = {
    "EOF","+","*","id",
};
const char *yytokenAlias[] = {
    "EOF","PLUS","MULTIPLY","ID",
};
const char *yynonTerminals[] = {
    "(accept)","start","expr","atom",
};
static int yyParser_reduce(yyParser *yyparser,int yyrule){
    YYCHECK_PUSH_TOKEN();
    int yyval;
    void *yydata = (void *)yyparser->userData;
    switch(yyrule){
        case 0:
            /* (accept) -> start  */
            yyval = yyparser->sp[-1];
            /* no action. */
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 1:
            /* start -> expr  */
            yyval = yyparser->sp[-1];
            /* no action. */
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* expr -> expr <+> expr  */
            yyval = yyparser->sp[-3];
            /* no action. */
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* expr -> expr <*> expr  */
            yyval = yyparser->sp[-3];
            /* no action. */
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* expr -> atom  */
            yyval = yyparser->sp[-1];
            /* no action. */
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* atom -> <id>  */
            yyval = yyparser->sp[-1];
            /* no action. */
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
    }
    yyparser->sLen -= yyruleLen[yyrule];
    int yyindex = YYSTATE() * yyntCount + yylhs[yyrule];
    YYPUSH_STATE(yygoto[yyindex] - 1);
    return 0;
}
int yyParser_init(yyParser *yyparser){
    yyparser->sLen = 1;
    yyparser->done = 0;
    yyparser->sSize = yyparser->pSize = 16;
    yyparser->state = (int *)YYMALLOC(yyparser,sizeof(int) * yyparser->sSize);
    yyparser->state[0] = 0;
    yyparser->sp = yyparser->pstack = (int *)YYMALLOC(yyparser,sizeof(int) * yyparser->pSize);
    return 0;
}
static int yyParser_clearStack(yyParser *yyparser){
    while(yyparser->sp > yyparser->pstack){
        yyparser->sp--;
        YYDESTRUCTOR(yyparser->sp);
    }
    return 0;
}
int yyParser_reInit(yyParser *yyparser){
    yyParser_clearStack(yyparser);
    yyparser->sLen = 0;
    yyparser->done = 0;
    yyparser->state[0] = 0;
    yyparser->sp = yyparser->pstack;
    return 0;
}
int yyParser_free(yyParser *yyparser){
    yyParser_clearStack(yyparser);
    YYFREE(yyparser,yyparser->state);
    YYFREE(yyparser,yyparser->pstack);
    return 0;
}
int yyParser_acceptToken(yyParser *yyparser,int yytokenid){
    int yyaction = yyshift[YYSTATE() * yytokenCount + yytokenid];
    if(yyaction > 0){
        YYCHECK_PUSH_TOKEN();
        *yyparser->sp++ = yyparser->token;
        YYPUSH_STATE(yyaction - 1);
        return YY_SHIFT;
    }
    else if(yyaction < 0){
        if(yyaction == -1){
            yyparser->done = 1;
            return YY_ACCEPT;
        }
        yyParser_reduce(yyparser,-1 - yyaction);
        return YY_REDUCE;
    }
    else {
        yyparser->error = 1;
        yyparser->errToken = yytokenid;
        return YY_ERR;
    }
}
int yyParser_printError(yyParser *yyparser,FILE *out){
    if(yyparser->error){
        int index = YYSTATE() * yytokenCount;
        fprintf(out,"unexpected token '%s' (%s),was expecting one of:\n",yytokenNames[yyparser->errToken],yytokenAlias[yyparser->errToken]);
        int i;
        for(i = 0;i < yytokenCount;i++){
            if(yyshift[index + i] != 0){
                fprintf(out,"    '%s' (%s) ...\n",yytokenNames[i],yytokenAlias[i]);
            }
        }
    }
    return YY_OK;
}
