#include "./expr.h"
static int yytokenCount = 6;
static int yyntCount = 4;
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
         0,     0,     0,     9,     0,    12,
    /* state 1 */
        -1,     5,     0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     9,     0,    12,
    /* state 3 */
        -4,    -4,    -4,     0,     0,     0,
    /* state 4 */
         0,     0,     0,     9,     0,    12,
    /* state 5 */
        -2,    -2,     3,     0,    -2,     0,
    /* state 6 */
        -3,    -3,     3,     0,    -3,     0,
    /* state 7 */
        -5,    -5,    -5,     0,    -5,     0,
    /* state 8 */
         0,     0,     0,     9,     0,    12,
    /* state 9 */
         0,     5,     0,     0,    11,     0,
    /* state 10 */
        -6,    -6,    -6,     0,    -6,     0,
    /* state 11 */
        -7,    -7,    -7,     0,    -7,     0,
};
/** goto table,
 * zero iff there is an error*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     7,     8,
    /* state 1 */
         0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     4,
    /* state 3 */
         0,     0,     0,     0,
    /* state 4 */
         0,     0,     6,     8,
    /* state 5 */
         0,     0,     0,     0,
    /* state 6 */
         0,     0,     0,     0,
    /* state 7 */
         0,     0,     0,     0,
    /* state 8 */
         0,    10,     7,     8,
    /* state 9 */
         0,     0,     0,     0,
    /* state 10 */
         0,     0,     0,     0,
    /* state 11 */
         0,     0,     0,     0,
};
/* the lhs of each rule. */
static const int yylhs[] = {
         0,     1,     1,     2,     2,     3,     3,
};
static const char *yytokenNames[] = {
    "EOF","+","*","(",")","id",
    
};
static const char *yytokenAlias[] = {
    "EOF","PLUS","TIMES","BRA","KET","ID",
    
};
static const char *yynonTerminals[] = {
    "(accept)","Expr","Term","Factor",
};
static int yyParser_reduce(yyParser *yyparser,int yyrule){
    YYCHECK_PUSH_TOKEN();
    int yyval;
    switch(yyrule){
        case 0:
            /* (accept) -> Expr  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 1:
            /* Expr -> Expr <+> Term  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* Expr -> Term  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* Term -> Term <*> Factor  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* Term -> Factor  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* Factor -> <(> Expr <)>  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 6:
            /* Factor -> <id>  */
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
