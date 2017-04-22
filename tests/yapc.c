#include "./yapc.h"
static int yytokenCount = 11;
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
        yyparser->pstack = (int *)yyparser->rtor(yyparser->pstack,sizeof(int) * yyparser->pSize);\
        yyparser->sp = yyparser->pstack + offset;\
    }
/** shift action table
 * positive numbers indicate the states shift to,
 * negative numbers indicate the rules reduce with.
 * the state should be the number in the table minus one,since zero marks
 * for error.*/
static const int yyshift[] = {
    /* state 0 */
         0,    -4,     0,    -4,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 1 */
        -1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 2 */
         0,    26,     0,     4,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 3 */
         0,     0,    21,     0,     0,     0,     0,    11,     0,     0,     0,
    
    /* state 4 */
         0,     0,    21,     6,     0,     0,     0,    11,     0,     0,     0,
    
    /* state 5 */
        -2,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 6 */
         0,     0,    -6,    -6,     0,     0,     0,    -6,     0,     0,     0,
    
    /* state 7 */
         0,     0,    -7,    -7,     0,     0,     0,    -7,     0,     0,     0,
    
    /* state 8 */
         0,     0,    -8,    -8,     0,     0,     0,    -8,     0,     0,     0,
    
    /* state 9 */
         0,     0,    -9,    -9,     0,     0,     0,    -9,     0,     0,     0,
    
    /* state 10 */
         0,     0,     0,     0,    12,     0,     0,     0,     0,     0,     0,
    
    /* state 11 */
         0,     0,   -17,   -17,     0,   -17,     0,   -17,   -17,     0,   -17,
    
    /* state 12 */
         0,     0,   -13,   -13,     0,    14,     0,   -13,     0,     0,     0,
    
    /* state 13 */
         0,     0,   -17,   -17,     0,   -17,     0,   -17,   -17,     0,   -17,
    
    /* state 14 */
         0,     0,   -15,   -15,     0,   -15,     0,    18,    17,     0,    19,
    
    /* state 15 */
         0,     0,   -16,   -16,     0,   -16,     0,   -16,   -16,     0,   -16,
    
    /* state 16 */
         0,     0,   -18,   -18,     0,   -18,     0,   -18,   -18,     0,   -18,
    
    /* state 17 */
         0,     0,   -19,   -19,     0,   -19,     0,   -19,   -19,     0,   -19,
    
    /* state 18 */
         0,     0,   -20,   -20,     0,   -20,     0,   -20,   -20,     0,   -20,
    
    /* state 19 */
         0,     0,   -14,   -14,     0,   -14,     0,    18,    17,     0,    19,
    
    /* state 20 */
         0,     0,     0,     0,     0,     0,   -12,     0,   -12,     0,     0,
    
    /* state 21 */
         0,     0,     0,     0,     0,     0,    23,     0,    24,     0,     0,
    
    /* state 22 */
         0,     0,   -10,   -10,     0,     0,     0,   -10,     0,     0,     0,
    
    /* state 23 */
         0,     0,     0,     0,     0,     0,   -11,     0,   -11,     0,     0,
    
    /* state 24 */
         0,    -3,     0,    -3,     0,     0,     0,     0,     0,     0,     0,
    
    /* state 25 */
         0,     0,     0,     0,     0,     0,     0,     0,    27,     0,     0,
    
    /* state 26 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,    28,     0,
    
    /* state 27 */
         0,    -5,     0,    -5,     0,     0,     0,     0,     0,     0,     0,
    
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
};
/* the lhs of each rule. */
static const int yylhs[] = {
         0,     1,     2,     2,     3,     4,     4,     5,     5,     6,     7,
         7,     8,     9,     9,    10,    10,    11,    11,    11,
};
static const char *yytokenNames[] = {
    "EOF","%token","%test","%%",":","|",
    ";","id","token","string","block",
};
static const char *yytokenAlias[] = {
    "EOF","TOKEN_DIR","TEST_DIR","SEPERATOR","COLLON","OR",
    "SEMI_COLLON","ID","TOKEN","STRING","BLOCK",
};
static const char *yynonTerminals[] = {
    "(accept)","file","options","option","body","bodyItem",
    "test","tokenList","rule","ruleBody","ruleItems","ruleItem",
    
};
static int yyParser_reduce(yyParser *yyparser,int yyrule){
    YYCHECK_PUSH_TOKEN();
    int yyval;
    switch(yyrule){
        case 0:
            /* (accept) -> file  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 1:
            /* file -> options <%%> body <%%>  */
            /* no action. */
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* options -> options option  */
            /* no action. */
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* options ->  */
            /* no action. */
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* option -> <%token> <token> <string>  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* body -> body bodyItem  */
            /* no action. */
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 6:
            /* body -> bodyItem  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 7:
            /* bodyItem -> rule  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 8:
            /* bodyItem -> test  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 9:
            /* test -> <%test> tokenList <;>  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 10:
            /* tokenList -> tokenList <token>  */
            /* no action. */
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 11:
            /* tokenList ->  */
            /* no action. */
            *yyparser->sp++ = yyval;
            break;
        case 12:
            /* rule -> <id> <:> ruleBody  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 13:
            /* ruleBody -> ruleItems  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 14:
            /* ruleBody -> ruleBody <|> ruleItems  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 15:
            /* ruleItems -> ruleItems ruleItem  */
            /* no action. */
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 16:
            /* ruleItems ->  */
            /* no action. */
            *yyparser->sp++ = yyval;
            break;
        case 17:
            /* ruleItem -> <token>  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 18:
            /* ruleItem -> <id>  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 19:
            /* ruleItem -> <block>  */
            /* no action. */
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
    yyparser->sSize = yyparser->pSize = 16;
    yyparser->state = (int *)altor(sizeof(int) * yyparser->sSize);
    yyparser->state[0] = 0;
    yyparser->sp = yyparser->pstack = (int *)altor(sizeof(int) * yyparser->pSize);
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
        fprintf(out,"unexpected token '%s',was expecting one of:\n",yytokenNames[yyparser->errToken]);
        int i;
        for(i = 0;i < yytokenCount;i++){
            if(yyshift[index + i] != 0){
                fprintf(out,"    '%s' ...\n",yytokenNames[i]);
            }
        }
    }
    return 0;
}
