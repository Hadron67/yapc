#include "./action.h"
static const int yytokenCount = 4;
static const int yyntCount = 8;
#ifndef YYMALLOC
    #define YYMALLOC malloc
#endif
#ifndef YYREALLOC
    #define YYREALLOC realloc
#endif
#ifndef YYFREE
    #define YYFREE free
#endif
#ifndef YYDESTRUCTOR
    #define YYDESTRUCTOR(a)
#endif
#define YYPUSH_STATE(s) \
    if(yyparser->sLen >= yyparser->sSize){ \
        yyparser->sSize *= 2; \
        yyparser->state = (int *)YYREALLOC(yyparser->state,sizeof(int) * yyparser->sSize); \
    } \
    yyparser->state[yyparser->sLen++] = (s);

#define YYSTATE() (yyparser->state[yyparser->sLen - 1])
#define YYCHECK_PUSH_TOKEN() \
    if(yyparser->sp - yyparser->pstack >= yyparser->pSize){\
        size_t offset = yyparser->sp - yyparser->pstack;\
        yyparser->pSize *= 2;\
        yyparser->pstack = (int *)YYREALLOC(yyparser->pstack,sizeof(int) * yyparser->pSize);\
        yyparser->sp = yyparser->pstack + offset;\
    }
/** shift action table
 * positive numbers indicate the states shift to,
 * negative numbers indicate the rules reduce with.
 * the state should be the number in the table minus one,since zero marks
 * for error.*/
static const int yyshift[] = {
    /* state 0 */
         0,    -7,     0,    -9,
    /* state 1 */
        -1,     0,     0,     0,
    /* state 2 */
        -2,     0,     4,     0,
    /* state 3 */
        -3,    -3,    -3,    -3,
    /* state 4 */
         0,    -7,     0,    -9,
    /* state 5 */
        -4,     0,    -4,     0,
    /* state 6 */
        -5,     0,    -5,     0,
    /* state 7 */
         0,    -6,     0,     0,
    /* state 8 */
         0,    10,     0,     0,
    /* state 9 */
        -8,     0,    -8,     0,
    /* state 10 */
         0,     0,     0,    12,
    /* state 11 */
       -10,     0,   -10,     0,
};
/** goto table,
 * zero iff there is an error*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     3,     0,     7,     0,     8,    11,
    /* state 1 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 3 */
         0,     0,     0,     5,     0,     0,     0,     0,
    /* state 4 */
         0,     0,     0,     0,     6,     0,     8,    11,
    /* state 5 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 6 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 7 */
         0,     0,     0,     0,     0,     9,     0,     0,
    /* state 8 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 9 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 10 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 11 */
         0,     0,     0,     0,     0,     0,     0,     0,
};
/* the lhs of each rule. */
static const int yylhs[] = {
         0,     1,     3,     2,     2,     5,     6,     4,     7,     4,
};
static const char *yytokenNames[] = {
    "EOF","id","+","num",
};
static const char *yytokenAlias[] = {
    "EOF","ID","PLUS","NUM",
};
static const char *yynonTerminals[] = {
    "(accept)","start","expr","@1","atom","@2",
    "@3","@4",
};
static int yyParser_reduce(yyParser *yyparser,int yyrule){
    YYCHECK_PUSH_TOKEN();
    int yyval;
    void *yydata = (void *)yyparser->userData;
    switch(yyrule){
        case 0:
            /* (accept) -> start  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 1:
            /* start -> expr  */
            #line 25 "./action.y"
            { printf("accept");yyval = (yyparser->sp[-1]); }
            #line 122 "./action.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* @1 ->  */
            #line 27 "./action.y"
            { printf("i saw a plus");yyval = 0; }
            #line 131 "./action.c"
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* expr -> expr <+> @1 atom  */
            #line 27 "./action.y"
            { yyval = (yyparser->sp[-4]) + (yyparser->sp[-2]); }
            #line 138 "./action.c"
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* expr -> atom  */
            #line 27 "./action.y"
            { yyval = (yyparser->sp[-1]); }
            #line 147 "./action.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* @2 ->  */
            #line 29 "./action.y"
            { printf("a new node"); }
            #line 156 "./action.c"
            break;
        case 6:
            /* @3 ->  */
            #line 29 "./action.y"
            { printf("i'm expecting an id"); }
            #line 162 "./action.c"
            break;
        case 7:
            /* atom -> @3 @2 <id>  */
            #line 29 "./action.y"
            { yyval = (yyparser->sp[-1]); }
            #line 168 "./action.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 8:
            /* @4 ->  */
            #line 29 "./action.y"
            { printf("i'm expecting a num"); }
            #line 177 "./action.c"
            break;
        case 9:
            /* atom -> @4 <num>  */
            #line 29 "./action.y"
            { yyval = (yyparser->sp[-1]); }
            #line 183 "./action.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
    }
    int yyindex = YYSTATE() * yyntCount + yylhs[yyrule];
    YYPUSH_STATE(yygoto[yyindex] - 1);
    return 0;
}
int yyParser_init(yyParser *yyparser){
    yyparser->sLen = 1;
    yyparser->done = 0;
    yyparser->sSize = yyparser->pSize = 16;
    yyparser->state = (int *)YYMALLOC(sizeof(int) * yyparser->sSize);
    yyparser->state[0] = 0;
    yyparser->sp = yyparser->pstack = (int *)YYMALLOC(sizeof(int) * yyparser->pSize);
    return 0;
}
int yyParser_reInit(yyParser *yyparser){
    yyparser->sLen = 0;
    yyparser->done = 0;
    yyparser->state[0] = 0;
    yyparser->sp = yyparser->pstack;
    return 0;
}
int yyParser_free(yyParser *yyparser){
    YYFREE(yyparser->state);
    YYFREE(yyparser->pstack);
    return 0;
}
int yyParser_acceptToken(yyParser *yyparser,int yytokenid){
    int shifted = 0;
    while(!shifted){
        int yyaction = yyshift[YYSTATE() * yytokenCount + yytokenid];
        if(yyaction > 0){
            YYCHECK_PUSH_TOKEN();
            *yyparser->sp++ = yyparser->token;
            YYPUSH_STATE(yyaction - 1);
            shifted = 1;
        }
        else if(yyaction < 0){
            if(yyaction == -1){
                yyparser->done = 1;
                return 0;
            }
            yyParser_reduce(yyparser,-1 - yyaction);
        }
        else {
            yyparser->error = 1;
            yyparser->errToken = yytokenid;
            return -1;
        }
    }
    return 0;
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
    return 0;
}
int yyParser_clearStack(yyParser *yyparser){
    while(yyparser->sp != yyparser->pstack){
        YYDESTRUCTOR(--yyparser->sp);
    }
    return 0;
}
