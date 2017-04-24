#include "./xml.h"
static const int yytokenCount = 11;
static const int yyntCount = 9;
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
         0,     0,    -4,     0,     0,    21,     0,     0,     0,     0,     0,
    
    /* state 1 */
        -1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 2 */
         0,     0,     8,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 3 */
        -2,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 4 */
        -5,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 5 */
         0,     0,    -8,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 6 */
         0,     0,    -9,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 7 */
         0,     0,     0,     0,     0,     0,     0,     0,     9,     0,     0,
    
    /* state 8 */
         0,   -12,     0,     0,     0,     0,     0,   -12,   -12,     0,     0,
    
    /* state 9 */
         0,    11,     0,     0,     0,     0,     0,    17,    27,     0,     0,
    
    /* state 10 */
         0,     0,     8,     0,     0,     0,     0,     0,     0,     0,     7,
    
    /* state 11 */
         0,     0,    13,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 12 */
         0,     0,     0,     0,     0,     0,     0,    14,     0,     0,     0,
    
    /* state 13 */
         0,     0,     0,     0,     0,     0,     0,     0,    15,     0,     0,
    
    /* state 14 */
         0,    16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 15 */
        -6,     0,    -6,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 16 */
         0,    18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 17 */
        -7,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 18 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,    20,     0,
    
    /* state 19 */
         0,   -14,     0,     0,     0,     0,     0,   -14,   -14,     0,     0,
    
    /* state 20 */
         0,     0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
    
    /* state 21 */
         0,     0,     0,     0,     0,     0,   -12,     0,   -12,     0,     0,
    
    /* state 22 */
         0,     0,     0,     0,     0,     0,    24,     0,    27,     0,     0,
    
    /* state 23 */
         0,     0,    -3,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 24 */
         0,   -11,     0,     0,     0,     0,   -11,   -11,   -11,     0,     0,
    
    /* state 25 */
         0,   -13,     0,    19,     0,     0,   -13,   -13,   -13,     0,     0,
    
    /* state 26 */
         0,   -16,     0,   -16,    28,     0,     0,   -16,   -16,     0,     0,
    
    /* state 27 */
         0,     0,     0,     0,     0,     0,     0,     0,    29,     0,     0,
    
    /* state 28 */
         0,   -15,     0,   -15,     0,     0,   -15,   -15,   -15,     0,     0,
    
};
/** goto table,
 * zero iff there is an error*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     3,     0,     0,     0,     0,     0,     0,
    /* state 1 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     4,     5,     0,     0,     0,     0,
    /* state 3 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 4 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 5 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 6 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 7 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 8 */
         0,     0,     0,     0,     0,     0,    10,     0,     0,
    /* state 9 */
         0,     0,     0,     0,     0,     0,     0,    25,    26,
    /* state 10 */
         0,     0,     0,     0,     6,    12,     0,     0,     0,
    /* state 11 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 12 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 13 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 14 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 15 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 16 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 17 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 18 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 19 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 20 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 21 */
         0,     0,     0,     0,     0,     0,    23,     0,     0,
    /* state 22 */
         0,     0,     0,     0,     0,     0,     0,    25,    26,
    /* state 23 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 24 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 25 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 26 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 27 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 28 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
};
/* the lhs of each rule. */
static const int yylhs[] = {
         0,     1,     2,     2,     3,     4,     4,     5,     5,     5,     6,
         6,     7,     7,     8,     8,
};
static const char *yytokenNames[] = {
    "EOF","gt","lt","=",":","lt?",
    "gt?","/","id","string","content",
};
static const char *yytokenAlias[] = {
    "EOF","GT","LT","EQU","COLLON","LT_ASK",
    "GL_ASK","SLASH","ID","STRING","CONTENT",
};
static const char *yynonTerminals[] = {
    "(accept)","xml","head","body","tag","innerTag",
    "properties","property","propertyName",
};
static int yyParser_reduce(yyParser *yyparser,int yyrule){
    YYCHECK_PUSH_TOKEN();
    int yyval;
    void *yydata = (void *)yyparser->userData;
    switch(yyrule){
        case 0:
            /* (accept) -> xml  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 1:
            /* xml -> head body  */
            /* no action. */
            yyval = yyparser->sp[-2];
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* head -> <lt?> <id> properties <gt?>  */
            /* no action. */
            yyval = yyparser->sp[-4];
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* head ->  */
            /* no action. */
            yyval = yyparser->sp[0];
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* body -> tag  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* tag -> <lt> <id> properties <gt> innerTag <lt> </> <id> <gt>  */
            /* no action. */
            yyval = yyparser->sp[-9];
            yyparser->sp -= 9;
            yyparser->sLen -= 9;
            *yyparser->sp++ = yyval;
            break;
        case 6:
            /* tag -> <lt> <id> properties </> <gt>  */
            /* no action. */
            yyval = yyparser->sp[-5];
            yyparser->sp -= 5;
            yyparser->sLen -= 5;
            *yyparser->sp++ = yyval;
            break;
        case 7:
            /* innerTag -> tag  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 8:
            /* innerTag -> <content>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 9:
            /* innerTag ->  */
            /* no action. */
            yyval = yyparser->sp[0];
            *yyparser->sp++ = yyval;
            break;
        case 10:
            /* properties -> properties property  */
            /* no action. */
            yyval = yyparser->sp[-2];
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 11:
            /* properties ->  */
            /* no action. */
            yyval = yyparser->sp[0];
            *yyparser->sp++ = yyval;
            break;
        case 12:
            /* property -> propertyName  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 13:
            /* property -> propertyName <=> <string>  */
            /* no action. */
            yyval = yyparser->sp[-3];
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 14:
            /* propertyName -> <id> <:> <id>  */
            /* no action. */
            yyval = yyparser->sp[-3];
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 15:
            /* propertyName -> <id>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
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
