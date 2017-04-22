#include "./yapc.h"


#include <stdio.h>
#include "grammar_builder.h"
#include "grammar.h"
#include "spool.h"

static int yytokenCount = 15;
static int yyntCount = 12;
#define YYPUSH_STATE(parser,s) \
    if(parser->sLen >= parser->sSize){ \
        parser->sSize *= 2; \
        parser->state = (int *)parser->rtor(parser->state,sizeof(int) * parser->sSize); \
    } \
    parser->state[parser->sLen++] = (s);

#define YYSTATE() (yyparser->state[yyparser->sLen - 1])
#define YYCHECK_PUSH_TOKEN() \
    if(yyparser->sp - yyparser->pstack >= yyparser->pSize){\
        size_t offset = yyparser->sp - yyparser->pstack;\
        yyparser->pSize *= 2;\
        yyparser->pstack = (size_t *)yyparser->rtor(yyparser->pstack,sizeof(size_t) * yyparser->pSize);\
        yyparser->sp = yyparser->pstack + offset;\
    }
/** shift action table
 * positive numbers indicate the states shift to,
 * negative numbers indicate the rules reduce with.
 * the state should be the number in the table minus one,since zero marks
 * for error.*/
static const int yyshift[] = {
    /* state 0 */
         0,    -4,    -4,    -4,     0,     0,    -4,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 1 */
        -1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 2 */
         0,    26,    29,    31,     0,     0,     4,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 3 */
         0,     0,     0,     0,    21,     0,     0,     0,     0,     0,    11,
         0,     0,     0,     0,
    /* state 4 */
         0,     0,     0,     0,    21,     0,     6,     0,     0,     0,    11,
         0,     0,     0,     0,
    /* state 5 */
        -2,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 6 */
         0,     0,     0,     0,    -8,     0,    -8,     0,     0,     0,    -8,
         0,     0,     0,     0,
    /* state 7 */
         0,     0,     0,     0,    -9,     0,    -9,     0,     0,     0,    -9,
         0,     0,     0,     0,
    /* state 8 */
         0,     0,     0,     0,   -10,     0,   -10,     0,     0,     0,   -10,
         0,     0,     0,     0,
    /* state 9 */
         0,     0,     0,     0,   -11,     0,   -11,     0,     0,     0,   -11,
         0,     0,     0,     0,
    /* state 10 */
         0,     0,     0,     0,     0,     0,     0,    12,     0,     0,     0,
         0,     0,     0,     0,
    /* state 11 */
         0,     0,     0,     0,   -19,     0,   -19,     0,   -19,     0,   -19,
       -19,     0,   -19,     0,
    /* state 12 */
         0,     0,     0,     0,   -15,     0,   -15,     0,    14,     0,   -15,
         0,     0,     0,     0,
    /* state 13 */
         0,     0,     0,     0,   -19,     0,   -19,     0,   -19,     0,   -19,
       -19,     0,   -19,     0,
    /* state 14 */
         0,     0,     0,     0,   -17,     0,   -17,     0,   -17,     0,    18,
        17,     0,    19,     0,
    /* state 15 */
         0,     0,     0,     0,   -18,     0,   -18,     0,   -18,     0,   -18,
       -18,     0,   -18,     0,
    /* state 16 */
         0,     0,     0,     0,   -20,     0,   -20,     0,   -20,     0,   -20,
       -20,     0,   -20,     0,
    /* state 17 */
         0,     0,     0,     0,   -21,     0,   -21,     0,   -21,     0,   -21,
       -21,     0,   -21,     0,
    /* state 18 */
         0,     0,     0,     0,   -22,     0,   -22,     0,   -22,     0,   -22,
       -22,     0,   -22,     0,
    /* state 19 */
         0,     0,     0,     0,   -16,     0,   -16,     0,   -16,     0,    18,
        17,     0,    19,     0,
    /* state 20 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,   -14,     0,
       -14,     0,     0,     0,
    /* state 21 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,    23,     0,
        24,     0,     0,     0,
    /* state 22 */
         0,     0,     0,     0,   -12,     0,   -12,     0,     0,     0,   -12,
         0,     0,     0,     0,
    /* state 23 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,   -13,     0,
       -13,     0,     0,     0,
    /* state 24 */
         0,    -3,    -3,    -3,     0,     0,    -3,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 25 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        27,     0,     0,     0,
    /* state 26 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    28,     0,     0,
    /* state 27 */
         0,    -5,    -5,    -5,     0,     0,    -5,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 28 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    30,     0,     0,
    /* state 29 */
         0,    -6,    -6,    -6,     0,     0,    -6,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 30 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    32,     0,     0,
    /* state 31 */
         0,    -7,    -7,    -7,     0,     0,    -7,     0,     0,     0,     0,
         0,     0,     0,     0,
};
/** goto table,
 * zero iff there is an error*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     3,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 1 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 2 */
         0,     0,     0,    25,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 3 */
         0,     0,     0,     0,     5,     8,    10,     0,     9,     0,     0,
         0,
    /* state 4 */
         0,     0,     0,     0,     0,     7,    10,     0,     9,     0,     0,
         0,
    /* state 5 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 6 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 7 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 8 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 9 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 10 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 11 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,    13,    20,
         0,
    /* state 12 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 13 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    15,
         0,
    /* state 14 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        16,
    /* state 15 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 16 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 17 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 18 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 19 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        16,
    /* state 20 */
         0,     0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
         0,
    /* state 21 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 22 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 23 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 24 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 25 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 26 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 27 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 28 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 29 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 30 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 31 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
};
/* the lhs of each rule. */
static const int yylhs[] = {
         0,     1,     2,     2,     3,     3,     3,     4,     4,     5,     5,
         6,     7,     7,     8,     9,     9,    10,    10,    11,    11,    11,
    
};
static const char *yytokenNames[] = {
    "EOF","%token","%type","%datatype","%test","%empty",
    "%%",":","|",";","name","token",
    "string","block","prologue",
};
static const char *yytokenAlias[] = {
    "EOF","TOKEN_DIR","TYPE_DIR","DATATYPE_DIR","TEST_DIR","EMPTY_DIR",
    "SEPERATOR","ARROW","OR","EOL","NAME","TOKEN",
    "STRING","BLOCK","PROLOGUE",
};
static const char *yynonTerminals[] = {
    "(accept)","file","options","option","body","bodyItem",
    "test","tokenList","rule","ruleBody","ruleItems","ruleItem",
    
};
static int yyParser_reduce(yyParser *yyparser,int yyrule){
    YYCHECK_PUSH_TOKEN();
    size_t yyval;
    YGBuilder *yydata = (YGBuilder *)yyparser->userData;
    switch(yyrule){
        case 0:
            /* (accept) -> file  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 1:
            /* file -> options <%%> body <%%>  */
            /* no action. */
            yyval = yyparser->sp[-4];
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* options -> options option  */
            /* no action. */
            yyval = yyparser->sp[-2];
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* options ->  */
            /* no action. */
            yyval = yyparser->sp[0];
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* option -> <%token> <token> <string>  */
            #line 37 "./yapc.y"
            { YGBuilder_addToken(yydata,(yyparser->sp[-2]),(yyparser->sp[-1])); }
            #line 290 "./yapc.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* option -> <%type> <string>  */
            #line 38 "./yapc.y"
            { YGBuilder_setTokenType(yydata,(yyparser->sp[-1])); }
            #line 299 "./yapc.c"
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 6:
            /* option -> <%datatype> <string>  */
            #line 39 "./yapc.y"
            { YGBuilder_setDataType(yydata,(yyparser->sp[-1])); }
            #line 308 "./yapc.c"
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 7:
            /* body -> body bodyItem  */
            /* no action. */
            yyval = yyparser->sp[-2];
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 8:
            /* body -> bodyItem  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 9:
            /* bodyItem -> rule  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 10:
            /* bodyItem -> test  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 11:
            /* test -> <%test> tokenList <;>  */
            /* no action. */
            yyval = yyparser->sp[-3];
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 12:
            /* tokenList -> tokenList <token>  */
            /* no action. */
            yyval = yyparser->sp[-2];
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 13:
            /* tokenList ->  */
            /* no action. */
            yyval = yyparser->sp[0];
            *yyparser->sp++ = yyval;
            break;
        case 14:
            /* rule -> <name> <:> ruleBody  */
            /* no action. */
            yyval = yyparser->sp[-3];
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 15:
            /* ruleBody -> ruleItems  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 16:
            /* ruleBody -> ruleBody <|> ruleItems  */
            /* no action. */
            yyval = yyparser->sp[-3];
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 17:
            /* ruleItems -> ruleItems ruleItem  */
            /* no action. */
            yyval = yyparser->sp[-2];
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 18:
            /* ruleItems ->  */
            /* no action. */
            yyval = yyparser->sp[0];
            *yyparser->sp++ = yyval;
            break;
        case 19:
            /* ruleItem -> <token>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 20:
            /* ruleItem -> <name>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 21:
            /* ruleItem -> <block>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
    }
    int yyindex = YYSTATE() * yyntCount + yylhs[yyrule];
    YYPUSH_STATE(yyparser,yygoto[yyindex] - 1);
    return 0;
}
int yyParser_init(yyParser *yyparser,yyalloc altor,yyrealloc rtor,yyfree dtor){
    yyparser->altor = altor;
    yyparser->dtor = dtor;
    yyparser->rtor = rtor;
    yyparser->sLen = 1;
    yyparser->done = 0;
    yyparser->sSize = yyparser->pSize = 16;
    yyparser->state = (int *)altor(sizeof(int) * yyparser->sSize);
    yyparser->state[0] = 0;
    yyparser->sp = yyparser->pstack = (size_t *)altor(sizeof(size_t) * yyparser->pSize);
    return 0;
}
int yyParser_free(yyParser *yyparser){
    yyparser->dtor(yyparser->state);
    yyparser->dtor(yyparser->pstack);
    return 0;
}
int yyParser_acceptToken(yyParser *yyparser,int yytokenid){
    int shifted = 0;
    while(!shifted){
        int yyaction = yyshift[YYSTATE() * yytokenCount + yytokenid];
        if(yyaction > 0){
            YYCHECK_PUSH_TOKEN();
            *yyparser->sp++ = yyparser->token;
            YYPUSH_STATE(yyparser,yyaction - 1);
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
