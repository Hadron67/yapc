

#include "json.h"

#define YYDESTRUCTOR(a) JSON_free((a))

#include "parser.h"
static const int yytokenCount = 15;
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
        yyparser->pstack = (jsonval *)YYREALLOC(yyparser->pstack,sizeof(jsonval) * yyparser->pSize);\
        yyparser->sp = yyparser->pstack + offset;\
    }
/** shift action table
 * positive numbers indicate the states shift to,
 * negative numbers indicate the rules reduce with.
 * the state should be the number in the table minus one,since zero marks
 * for error.*/
static const int yyshift[] = {
    /* state 0 */
         0,    24,    25,    26,    27,    28,    29,     0,     0,    20,     0,
        17,     0,     0,     0,
    /* state 1 */
        -1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,   -20,     0,     5,
    /* state 3 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    -5,     0,     0,
    /* state 4 */
         0,     6,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,   -19,     0,     0,
    /* state 5 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,     7,     0,
    /* state 6 */
         0,    24,    25,    26,    27,    28,    29,     0,     0,    20,     0,
        17,     0,     0,     0,
    /* state 7 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    -7,     0,    -7,
    /* state 8 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,    10,     0,
    /* state 9 */
         0,    24,    25,    26,    27,    28,    29,     0,     0,    20,     0,
        17,     0,     0,     0,
    /* state 10 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    -8,     0,    -8,
    /* state 11 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,   -20,
         0,     0,     0,    14,
    /* state 12 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    -9,
         0,     0,     0,     0,
    /* state 13 */
         0,    24,    25,    26,    27,    28,    29,     0,     0,    20,   -19,
        17,     0,     0,     0,
    /* state 14 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,   -11,
         0,     0,     0,   -11,
    /* state 15 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,   -12,
         0,     0,     0,   -12,
    /* state 16 */
         0,     9,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    -6,     0,     0,
    /* state 17 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,    19,     0,     0,
    /* state 18 */
        -2,     0,     0,     0,     0,     0,     0,     0,     0,     0,    -2,
         0,    -2,     0,    -2,
    /* state 19 */
         0,    24,    25,    26,    27,    28,    29,     0,     0,    20,   -10,
        17,     0,     0,     0,
    /* state 20 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
         0,     0,     0,     0,
    /* state 21 */
        -3,     0,     0,     0,     0,     0,     0,     0,     0,     0,    -3,
         0,    -3,     0,    -3,
    /* state 22 */
        -4,     0,     0,     0,     0,     0,     0,     0,     0,     0,    -4,
         0,    -4,     0,    -4,
    /* state 23 */
       -13,     0,     0,     0,     0,     0,     0,     0,     0,     0,   -13,
         0,   -13,     0,   -13,
    /* state 24 */
       -14,     0,     0,     0,     0,     0,     0,     0,     0,     0,   -14,
         0,   -14,     0,   -14,
    /* state 25 */
       -15,     0,     0,     0,     0,     0,     0,     0,     0,     0,   -15,
         0,   -15,     0,   -15,
    /* state 26 */
       -16,     0,     0,     0,     0,     0,     0,     0,     0,     0,   -16,
         0,   -16,     0,   -16,
    /* state 27 */
       -17,     0,     0,     0,     0,     0,     0,     0,     0,     0,   -17,
         0,   -17,     0,   -17,
    /* state 28 */
       -18,     0,     0,     0,     0,     0,     0,     0,     0,     0,   -18,
         0,   -18,     0,   -18,
};
/** goto table,
 * zero iff there is an error*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     0,     0,     0,     0,    23,     0,
    /* state 1 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     0,     0,     0,     0,     4,
    /* state 3 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 4 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 5 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 6 */
         0,     8,     0,     0,     0,     0,    23,     0,
    /* state 7 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 8 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 9 */
         0,    11,     0,     0,     0,     0,    23,     0,
    /* state 10 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 11 */
         0,     0,     0,     0,     0,     0,     0,    13,
    /* state 12 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 13 */
         0,    15,     0,     0,     0,     0,    23,     0,
    /* state 14 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 15 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 16 */
         0,     0,    18,     3,     0,     0,     0,     0,
    /* state 17 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 18 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 19 */
         0,    16,     0,     0,    21,    12,    23,     0,
    /* state 20 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 21 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 22 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 23 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 24 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 25 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 26 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 27 */
         0,     0,     0,     0,     0,     0,     0,     0,
    /* state 28 */
         0,     0,     0,     0,     0,     0,     0,     0,
};
/* the lhs of each rule. */
static const int yylhs[] = {
         0,     1,     1,     1,     2,     2,     3,     3,     4,     4,     5,
         5,     6,     6,     6,     6,     6,     6,     7,     7,
};
static const char *yytokenNames[] = {
    "EOF","string","num","true","false","null",
    "undefined","(",")","[","]","{",
    "}",":",",",
};
static const char *yytokenAlias[] = {
    "EOF","STRING","NUM","TRUE","FALSE","NULL",
    "UNDEFINED","BRA","KET","CBRA","CKET","BBRA",
    "BKET","COLLON","COMMA",
};
static const char *yynonTerminals[] = {
    "(accept)","json","obj","nonEmptyObj","array","nonEmptyArray",
    "primitives","possibleComma",
};
static int yyParser_reduce(yyParser *yyparser,int yyrule){
    YYCHECK_PUSH_TOKEN();
    jsonval yyval;
    void *yydata = (void *)yyparser->userData;
    switch(yyrule){
        case 0:
            /* (accept) -> json  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 1:
            /* json -> <{> obj <}>  */
            #line 30 "parser.y"
            { yyval = (yyparser->sp[-2]); }
            #line 230 "parser.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* json -> <[> array <]>  */
            #line 31 "parser.y"
            { yyval = (yyparser->sp[-2]); }
            #line 239 "parser.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* json -> primitives  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* obj -> nonEmptyObj possibleComma  */
            /* no action. */
            yyval = yyparser->sp[-2];
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* obj ->  */
            #line 34 "parser.y"
            { JSON_init(&yyval,JTYPE_OBJ); }
            #line 264 "parser.c"
            *yyparser->sp++ = yyval;
            break;
        case 6:
            /* nonEmptyObj -> nonEmptyObj <,> <string> <:> json  */
            #line 36 "parser.y"
            { jsonObj_add(&(yyparser->sp[-5]),(yyparser->sp[-3]).u.stringv.s,(yyparser->sp[-3]).u.stringv.length,&(yyparser->sp[-1]));yyval = (yyparser->sp[-5]); }
            #line 271 "parser.c"
            yyparser->sp -= 5;
            yyparser->sLen -= 5;
            *yyparser->sp++ = yyval;
            break;
        case 7:
            /* nonEmptyObj -> <string> <:> json  */
            #line 37 "parser.y"
            { JSON_init(&yyval,JTYPE_OBJ);jsonObj_add(&yyval,(yyparser->sp[-3]).u.stringv.s,(yyparser->sp[-3]).u.stringv.length,&(yyparser->sp[-1])); }
            #line 280 "parser.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 8:
            /* array -> nonEmptyArray possibleComma  */
            /* no action. */
            yyval = yyparser->sp[-2];
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 9:
            /* array ->  */
            #line 40 "parser.y"
            { JSON_init(&yyval,JTYPE_ARRAY); }
            #line 297 "parser.c"
            *yyparser->sp++ = yyval;
            break;
        case 10:
            /* nonEmptyArray -> nonEmptyArray <,> json  */
            #line 43 "parser.y"
            { jsonArray_push(&(yyparser->sp[-3]),&(yyparser->sp[-1]));yyval = (yyparser->sp[-3]); }
            #line 304 "parser.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 11:
            /* nonEmptyArray -> json  */
            #line 44 "parser.y"
            { JSON_init(&yyval,JTYPE_ARRAY);jsonArray_push(&yyval,&(yyparser->sp[-1])); }
            #line 313 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 12:
            /* primitives -> <string>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 13:
            /* primitives -> <num>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 14:
            /* primitives -> <true>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 15:
            /* primitives -> <false>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 16:
            /* primitives -> <null>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 17:
            /* primitives -> <undefined>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 18:
            /* possibleComma -> <,>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 19:
            /* possibleComma ->  */
            /* no action. */
            yyval = yyparser->sp[0];
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
    yyparser->sp = yyparser->pstack = (jsonval *)YYMALLOC(sizeof(jsonval) * yyparser->pSize);
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