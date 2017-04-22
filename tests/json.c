#include "./json.h"
static int yytokenCount = 12;
static int yyntCount = 10;
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
         0,    18,     0,     0,     0,    21,     0,    25,    29,    26,    27,
        28,
    /* state 1 */
        -1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 2 */
         0,     0,   -22,     0,     5,     0,     0,     0,     0,     0,     0,
         0,
    /* state 3 */
         0,     0,    -5,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 4 */
         0,     0,   -21,     0,     0,     0,     0,     0,    11,     0,     0,
        12,
    /* state 5 */
         0,     0,    -7,     0,    -7,     0,     0,     0,     0,     0,     0,
         0,
    /* state 6 */
         0,     0,    -8,     0,    -8,     0,     0,     0,     0,     0,     0,
         0,
    /* state 7 */
         0,     0,     0,     9,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 8 */
         0,    18,     0,     0,     0,    21,     0,    25,    29,    26,    27,
        28,
    /* state 9 */
         0,     0,    -9,     0,    -9,     0,     0,     0,     0,     0,     0,
         0,
    /* state 10 */
         0,     0,     0,   -10,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 11 */
         0,     0,     0,   -11,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 12 */
         0,     0,     0,     0,    15,     0,   -22,     0,     0,     0,     0,
         0,
    /* state 13 */
         0,     0,     0,     0,     0,     0,   -12,     0,     0,     0,     0,
         0,
    /* state 14 */
         0,    18,     0,     0,     0,    21,   -21,    25,    29,    26,    27,
        28,
    /* state 15 */
         0,     0,     0,     0,   -14,     0,   -14,     0,     0,     0,     0,
         0,
    /* state 16 */
         0,     0,     0,     0,   -15,     0,   -15,     0,     0,     0,     0,
         0,
    /* state 17 */
         0,     0,    -6,     0,     0,     0,     0,     0,    11,     0,     0,
        12,
    /* state 18 */
         0,     0,    20,     0,     0,     0,     0,     0,     0,     0,     0,
         0,
    /* state 19 */
        -2,     0,    -2,     0,    -2,     0,    -2,     0,     0,     0,     0,
         0,
    /* state 20 */
         0,    18,     0,     0,     0,    21,   -13,    25,    29,    26,    27,
        28,
    /* state 21 */
         0,     0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
         0,
    /* state 22 */
        -3,     0,    -3,     0,    -3,     0,    -3,     0,     0,     0,     0,
         0,
    /* state 23 */
        -4,     0,    -4,     0,    -4,     0,    -4,     0,     0,     0,     0,
         0,
    /* state 24 */
       -16,     0,   -16,     0,   -16,     0,   -16,     0,     0,     0,     0,
         0,
    /* state 25 */
       -17,     0,   -17,     0,   -17,     0,   -17,     0,     0,     0,     0,
         0,
    /* state 26 */
       -18,     0,   -18,     0,   -18,     0,   -18,     0,     0,     0,     0,
         0,
    /* state 27 */
       -19,     0,   -19,     0,   -19,     0,   -19,     0,     0,     0,     0,
         0,
    /* state 28 */
       -20,     0,   -20,     0,   -20,     0,   -20,     0,     0,     0,     0,
         0,
};
/** goto table,
 * zero iff there is an error*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     0,     0,     0,     0,     0,     0,    24,     0,
    /* state 1 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
    /* state 3 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 4 */
         0,     0,     0,     0,     6,     8,     0,     0,     0,     0,
    /* state 5 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 6 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 7 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 8 */
         0,    10,     0,     0,     0,     0,     0,     0,    24,     0,
    /* state 9 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 10 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 11 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 12 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
    /* state 13 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 14 */
         0,    16,     0,     0,     0,     0,     0,     0,    24,     0,
    /* state 15 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 16 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 17 */
         0,     0,    19,     3,     7,     8,     0,     0,     0,     0,
    /* state 18 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 19 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 20 */
         0,    17,     0,     0,     0,     0,    22,    13,    24,     0,
    /* state 21 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 22 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 23 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 24 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 25 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 26 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 27 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 28 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
};
/* the lhs of each rule. */
static const int yylhs[] = {
         0,     1,     1,     1,     2,     2,     3,     3,     4,     5,     5,
         6,     6,     7,     7,     8,     8,     8,     8,     8,     9,     9,
    
};
static const char *yytokenNames[] = {
    "EOF","{","}",":",",","[",
    "]","num","id","true","false","string",
    
};
static const char *yytokenAlias[] = {
    "EOF","OPEN_CURLY","CLOSE_CULRY","COLLON","COMMA","CBRA",
    "CKET","NUM","ID","TRUE","FALSE","STRING",
    
};
static const char *yynonTerminals[] = {
    "(accept)","json","obj","nonEmptyObj","objItem","objKey",
    "array","nonEmptyArray","atom","possibleComma",
};
static int yyParser_reduce(yyParser *yyparser,int yyrule){
    YYCHECK_PUSH_TOKEN();
    int yyval;
    switch(yyrule){
        case 0:
            /* (accept) -> json  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 1:
            /* json -> <{> obj <}>  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* json -> <[> array <]>  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* json -> atom  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* obj -> nonEmptyObj possibleComma  */
            /* no action. */
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* obj ->  */
            /* no action. */
            *yyparser->sp++ = yyval;
            break;
        case 6:
            /* nonEmptyObj -> nonEmptyObj <,> objItem  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 7:
            /* nonEmptyObj -> objItem  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 8:
            /* objItem -> objKey <:> json  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 9:
            /* objKey -> <id>  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 10:
            /* objKey -> <string>  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 11:
            /* array -> nonEmptyArray possibleComma  */
            /* no action. */
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 12:
            /* array ->  */
            /* no action. */
            *yyparser->sp++ = yyval;
            break;
        case 13:
            /* nonEmptyArray -> nonEmptyArray <,> json  */
            /* no action. */
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 14:
            /* nonEmptyArray -> json  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 15:
            /* atom -> <num>  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 16:
            /* atom -> <true>  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 17:
            /* atom -> <false>  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 18:
            /* atom -> <string>  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 19:
            /* atom -> <id>  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 20:
            /* possibleComma -> <,>  */
            /* no action. */
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 21:
            /* possibleComma ->  */
            /* no action. */
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
