/*
    Generated by yapc -- An LR(1) parser generator made by CFY
*/



#include <stdio.h>
#include "grammar_builder.h"
#include "grammar.h"
#include "spool.h"

#include "./yapc.h"
static const int yytokenCount = 15;
static const int yyntCount = 13;
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
        yyparser->pstack = (ysptr *)YYREALLOC(yyparser,yyparser->pstack,sizeof(ysptr) * yyparser->pSize);\
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
         0,    -9,    -9,    -9,     0,     0,    -9,     0,     0,     0,     0,
         0,     0,     0,    35,
    /* state 1 */
        -1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 2 */
         0,    -4,    -4,    -4,     0,     0,    -4,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 3 */
         0,    32,    30,    28,     0,     0,     5,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 4 */
         0,     0,     0,     0,    14,     0,     0,     0,     0,     0,    17,
         0,     0,     0,     0,
    /* state 5 */
         0,     0,     0,     0,    14,     0,     7,     0,     0,     0,    17,
         0,     0,     0,     0,
    /* state 6 */
        -2,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 7 */
         0,     0,     0,     0,   -10,     0,   -10,     0,     0,     0,   -10,
         0,     0,     0,     0,
    /* state 8 */
         0,     0,     0,     0,   -11,     0,   -11,     0,     0,     0,   -11,
         0,     0,     0,     0,
    /* state 9 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,    11,     0,
         0,     0,     0,     0,
    /* state 10 */
         0,     0,     0,     0,   -13,     0,   -13,     0,     0,     0,   -13,
         0,     0,     0,     0,
    /* state 11 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,    13,     0,
         0,     0,     0,     0,
    /* state 12 */
         0,     0,     0,     0,   -12,     0,   -12,     0,     0,     0,   -12,
         0,     0,     0,     0,
    /* state 13 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,   -16,     0,
       -16,     0,     0,     0,
    /* state 14 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,   -14,     0,
        16,     0,     0,     0,
    /* state 15 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,   -15,     0,
       -15,     0,     0,     0,
    /* state 16 */
         0,     0,     0,     0,     0,     0,     0,    18,     0,     0,     0,
         0,     0,     0,     0,
    /* state 17 */
         0,     0,     0,     0,     0,     0,     0,     0,   -21,   -21,   -21,
       -21,     0,   -21,     0,
    /* state 18 */
         0,     0,     0,     0,     0,     0,     0,     0,    20,   -17,     0,
         0,     0,     0,     0,
    /* state 19 */
         0,     0,     0,     0,     0,     0,     0,     0,   -21,   -21,   -21,
       -21,     0,   -21,     0,
    /* state 20 */
         0,     0,     0,     0,     0,     0,     0,     0,   -19,   -19,    24,
        25,     0,    23,     0,
    /* state 21 */
         0,     0,     0,     0,     0,     0,     0,     0,   -20,   -20,   -20,
       -20,     0,   -20,     0,
    /* state 22 */
         0,     0,     0,     0,     0,     0,     0,     0,   -24,   -24,   -24,
       -24,     0,   -24,     0,
    /* state 23 */
         0,     0,     0,     0,     0,     0,     0,     0,   -23,   -23,   -23,
       -23,     0,   -23,     0,
    /* state 24 */
         0,     0,     0,     0,     0,     0,     0,     0,   -22,   -22,   -22,
       -22,     0,   -22,     0,
    /* state 25 */
         0,     0,     0,     0,     0,     0,     0,     0,   -18,   -18,    24,
        25,     0,    23,     0,
    /* state 26 */
         0,    -3,    -3,    -3,     0,     0,    -3,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 27 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    29,     0,     0,
    /* state 28 */
         0,    -7,    -7,    -7,     0,     0,    -7,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 29 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    31,     0,     0,
    /* state 30 */
         0,    -6,    -6,    -6,     0,     0,    -6,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 31 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        33,     0,     0,     0,
    /* state 32 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    34,     0,     0,
    /* state 33 */
         0,    -5,    -5,    -5,     0,     0,    -5,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 34 */
         0,    -8,    -8,    -8,     0,     0,    -8,     0,     0,     0,     0,
         0,     0,     0,     0,
};
/*
    goto table
*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     0,     0,     3,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 1 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 2 */
         0,     0,     4,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 3 */
         0,     0,     0,    27,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 4 */
         0,     0,     0,     0,     0,     6,     9,    10,     0,    12,     0,
         0,     0,
    /* state 5 */
         0,     0,     0,     0,     0,     0,     8,    10,     0,    12,     0,
         0,     0,
    /* state 6 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 7 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 8 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 9 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 10 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 11 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 12 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 13 */
         0,     0,     0,     0,     0,     0,     0,     0,    15,     0,     0,
         0,     0,
    /* state 14 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 15 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 16 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 17 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
        26,     0,
    /* state 18 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 19 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        21,     0,
    /* state 20 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    22,
    /* state 21 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 22 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 23 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 24 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 25 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    22,
    /* state 26 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 27 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 28 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 29 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 30 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 31 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 32 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 33 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 34 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
};
/*
    the left hand side of each rule,used to determine goto action.
*/
static const int yylhs[] = {
         0,     1,     2,     2,     3,     3,     3,     4,     4,     5,     5,
         6,     6,     7,     8,     8,     9,    10,    10,    11,    11,    12,
        12,    12,
};
/*
    the length of the symbols on the rhs of each rule
    used to pop states from the state stack when doing
    an reduction.
*/
static const int yyruleLen[] = {
         1,     5,     2,     0,     3,     2,     2,     1,     0,     2,     1,
         2,     2,     2,     2,     0,     3,     1,     3,     2,     0,     1,
         1,     1,
};
const char *yytokenNames[] = {
    "EOF","%token","%type","%datatype","%test","%empty",
    "%%",":","|",";","name","token",
    "string","block","prologue",
};
const char *yytokenAlias[] = {
    "EOF","TOKEN_DIR","TYPE_DIR","DATATYPE_DIR","TEST_DIR","EMPTY_DIR",
    "SEPERATOR","ARROW","OR","EOL","NAME","TOKEN",
    "STRING","BLOCK","PROLOGUE",
};
const char *yynonTerminals[] = {
    "(accept)","file","options","option","prologue","body",
    "bodyItem","test","tokenList","rule","ruleBody","ruleItems",
    "ruleItem",
};
static int yyParser_reduce(yyParser *yyparser,int yyrule){
    YYCHECK_PUSH_TOKEN();
    ysptr yyval;
    YGBuilder *yydata = (YGBuilder *)yyparser->userData;
    switch(yyrule){
        case 0:
            /* (accept) -> file  */
            yyval = yyparser->sp[-1];
            /* no action. */
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 1:
            /* file -> prologue options <%%> body <%%>  */
            yyval = yyparser->sp[-5];
            /* no action. */
            yyparser->sp -= 5;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* options -> options option  */
            yyval = yyparser->sp[-2];
            /* no action. */
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* options ->  */
            yyval = yyparser->sp[0];
            /* no action. */
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* option -> <%token> <token> <string>  */
            yyval = yyparser->sp[-3];
            #line 37 "./yapc.y"
            { YGBuilder_addToken(yydata,(yyparser->sp[-2]),(yyparser->sp[-1])); }
            #line 337 "./yapc.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* option -> <%type> <string>  */
            yyval = yyparser->sp[-2];
            #line 38 "./yapc.y"
            { YGBuilder_setTokenType(yydata,(yyparser->sp[-1])); }
            #line 346 "./yapc.c"
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 6:
            /* option -> <%datatype> <string>  */
            yyval = yyparser->sp[-2];
            #line 39 "./yapc.y"
            { YGBuilder_setDataType(yydata,(yyparser->sp[-1])); }
            #line 355 "./yapc.c"
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 7:
            /* prologue -> <prologue>  */
            yyval = yyparser->sp[-1];
            /* no action. */
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 8:
            /* prologue ->  */
            yyval = yyparser->sp[0];
            /* no action. */
            *yyparser->sp++ = yyval;
            break;
        case 9:
            /* body -> body bodyItem  */
            yyval = yyparser->sp[-2];
            /* no action. */
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 10:
            /* body -> bodyItem  */
            yyval = yyparser->sp[-1];
            /* no action. */
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 11:
            /* bodyItem -> rule <;>  */
            yyval = yyparser->sp[-2];
            /* no action. */
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 12:
            /* bodyItem -> test <;>  */
            yyval = yyparser->sp[-2];
            /* no action. */
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 13:
            /* test -> <%test> tokenList  */
            yyval = yyparser->sp[-2];
            /* no action. */
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 14:
            /* tokenList -> tokenList <token>  */
            yyval = yyparser->sp[-2];
            /* no action. */
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 15:
            /* tokenList ->  */
            yyval = yyparser->sp[0];
            /* no action. */
            *yyparser->sp++ = yyval;
            break;
        case 16:
            /* rule -> <name> <:> ruleBody  */
            yyval = yyparser->sp[-3];
            /* no action. */
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 17:
            /* ruleBody -> ruleItems  */
            yyval = yyparser->sp[-1];
            /* no action. */
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 18:
            /* ruleBody -> ruleBody <|> ruleItems  */
            yyval = yyparser->sp[-3];
            /* no action. */
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 19:
            /* ruleItems -> ruleItems ruleItem  */
            yyval = yyparser->sp[-2];
            /* no action. */
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 20:
            /* ruleItems ->  */
            yyval = yyparser->sp[0];
            /* no action. */
            *yyparser->sp++ = yyval;
            break;
        case 21:
            /* ruleItem -> <token>  */
            yyval = yyparser->sp[-1];
            /* no action. */
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 22:
            /* ruleItem -> <name>  */
            yyval = yyparser->sp[-1];
            /* no action. */
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 23:
            /* ruleItem -> <block>  */
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
    yyparser->sp = yyparser->pstack = (ysptr *)YYMALLOC(yyparser,sizeof(ysptr) * yyparser->pSize);
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
