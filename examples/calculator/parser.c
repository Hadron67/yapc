#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
static int yytokenCount = 20;
static int yyntCount = 9;
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
        yyparser->pstack = (double _Complex *)yyparser->rtor(yyparser->pstack,sizeof(double _Complex) * yyparser->pSize);\
        yyparser->sp = yyparser->pstack + offset;\
    }
/** shift action table
 * positive numbers indicate the states shift to,
 * negative numbers indicate the rules reduce with.
 * the state should be the number in the table minus one,since zero marks
 * for error.*/
static const int yyshift[] = {
    /* state 0 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 1 */
        -1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 2 */
        -2,     0,    57,    58,     0,     0,     0,     0,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 3 */
       -10,     0,   -10,   -10,   -10,   -10,   -10,     0,   -10,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 4 */
       -11,     0,   -11,   -11,   -11,   -11,   -11,     0,   -11,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 5 */
       -12,     0,   -12,   -12,   -12,   -12,   -12,     0,   -12,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 6 */
       -13,     0,   -13,   -13,   -13,   -13,   -13,     0,   -13,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 7 */
       -14,     0,   -14,   -14,   -14,   -14,   -14,     0,   -14,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 8 */
       -15,     0,   -15,   -15,   -15,   -15,   -15,     0,   -15,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 9 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 10 */
         0,     0,    57,    58,     0,     0,     0,     0,    12,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 11 */
       -16,     0,   -16,   -16,   -16,   -16,   -16,     0,   -16,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 12 */
       -17,     0,   -17,   -17,   -17,   -17,   -17,     0,   -17,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 13 */
       -18,     0,   -18,   -18,   -18,   -18,   -18,     0,   -18,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 14 */
         0,     0,     0,     0,     0,     0,     0,    16,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 15 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 16 */
         0,     0,    57,    58,     0,     0,     0,     0,    18,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 17 */
       -19,     0,   -19,   -19,   -19,   -19,   -19,     0,   -19,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 18 */
        -6,     0,    -6,    -6,    -6,    -6,    30,     0,    -6,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 19 */
        -9,     0,    -9,    -9,    -9,    -9,    -9,     0,    -9,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 20 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 21 */
        -8,     0,    -8,    -8,    -8,    -8,    30,     0,    -8,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 22 */
         0,     0,     0,     0,     0,     0,     0,    24,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 23 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 24 */
         0,     0,    57,    58,     0,     0,     0,     0,    26,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 25 */
       -20,     0,   -20,   -20,   -20,   -20,   -20,     0,   -20,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 26 */
        -3,     0,    -3,    -3,    21,    32,     0,     0,    -3,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 27 */
        -4,     0,    -4,    -4,    21,    32,     0,     0,    -4,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 28 */
        -7,     0,    -7,    -7,    -7,    -7,    30,     0,    -7,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 29 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 30 */
        -5,     0,    -5,    -5,    21,    32,     0,     0,    -5,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 31 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 32 */
         0,     0,     0,     0,     0,     0,     0,    34,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 33 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 34 */
         0,     0,    57,    58,     0,     0,     0,     0,    36,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 35 */
       -21,     0,   -21,   -21,   -21,   -21,   -21,     0,   -21,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 36 */
         0,     0,     0,     0,     0,     0,     0,    38,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 37 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 38 */
         0,     0,    57,    58,     0,     0,     0,     0,    40,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 39 */
       -22,     0,   -22,   -22,   -22,   -22,   -22,     0,   -22,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 40 */
         0,     0,     0,     0,     0,     0,     0,    42,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 41 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 42 */
         0,     0,    57,    58,     0,     0,     0,     0,    44,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 43 */
       -23,     0,   -23,   -23,   -23,   -23,   -23,     0,   -23,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 44 */
         0,     0,     0,     0,     0,     0,     0,    46,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 45 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 46 */
         0,     0,    57,    58,     0,     0,     0,     0,    48,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 47 */
       -24,     0,   -24,   -24,   -24,   -24,   -24,     0,   -24,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 48 */
         0,     0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 49 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 50 */
         0,     0,    57,    58,     0,     0,     0,     0,    52,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 51 */
       -25,     0,   -25,   -25,   -25,   -25,   -25,     0,   -25,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 52 */
         0,     0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 53 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 54 */
         0,     0,    57,    58,     0,     0,     0,     0,    56,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 55 */
       -26,     0,   -26,   -26,   -26,   -26,   -26,     0,   -26,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 56 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 57 */
         0,     9,    59,    60,    61,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 58 */
         0,     9,     0,     0,     0,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 59 */
         0,     9,     0,     0,     0,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 60 */
         0,     9,     0,     0,     0,     0,     0,    10,     0,    15,    23,
        33,    37,    41,    45,    49,    53,    62,    63,    64,
    /* state 61 */
       -27,     0,   -27,   -27,   -27,   -27,   -27,     0,   -27,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 62 */
       -28,     0,   -28,   -28,   -28,   -28,   -28,     0,   -28,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 63 */
       -29,     0,   -29,   -29,   -29,   -29,   -29,     0,   -29,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,
};
/** goto table,
 * zero iff there is an error*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     3,    31,    22,     4,     5,    13,    14,
    /* state 1 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
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
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 9 */
         0,     0,    11,    31,    22,     4,     5,    13,    14,
    /* state 10 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 11 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 12 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 13 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 14 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 15 */
         0,     0,    17,    31,    22,     4,     5,    13,    14,
    /* state 16 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 17 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 18 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 19 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 20 */
         0,     0,     0,     0,    19,     4,     5,    13,    14,
    /* state 21 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 22 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 23 */
         0,     0,    25,    31,    22,     4,     5,    13,    14,
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
    /* state 29 */
         0,     0,     0,     0,     0,    20,     5,    13,    14,
    /* state 30 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 31 */
         0,     0,     0,     0,    29,     4,     5,    13,    14,
    /* state 32 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 33 */
         0,     0,    35,    31,    22,     4,     5,    13,    14,
    /* state 34 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 35 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 36 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 37 */
         0,     0,    39,    31,    22,     4,     5,    13,    14,
    /* state 38 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 39 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 40 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 41 */
         0,     0,    43,    31,    22,     4,     5,    13,    14,
    /* state 42 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 43 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 44 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 45 */
         0,     0,    47,    31,    22,     4,     5,    13,    14,
    /* state 46 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 47 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 48 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 49 */
         0,     0,    51,    31,    22,     4,     5,    13,    14,
    /* state 50 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 51 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 52 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 53 */
         0,     0,    55,    31,    22,     4,     5,    13,    14,
    /* state 54 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 55 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 56 */
         0,     0,     0,    27,    22,     4,     5,    13,    14,
    /* state 57 */
         0,     0,     0,    28,    22,     4,     5,    13,    14,
    /* state 58 */
         0,     0,     0,     0,     0,     0,     6,    13,    14,
    /* state 59 */
         0,     0,     0,     0,     0,     0,     7,    13,    14,
    /* state 60 */
         0,     0,     0,     0,     0,     0,     8,    13,    14,
    /* state 61 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 62 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
    /* state 63 */
         0,     0,     0,     0,     0,     0,     0,     0,     0,
};
/* the lhs of each rule. */
static const int yylhs[] = {
         0,     1,     2,     2,     2,     3,     3,     3,     4,     4,     5,
         5,     5,     5,     6,     6,     6,     6,     7,     7,     7,     7,
         7,     7,     7,     7,     8,     8,     8,
};
static const char *yytokenNames[] = {
    "EOF","num","+","-","*","/",
    "**","(",")","sin","cos","tan",
    "asin","acos","atan","exp","ln","I",
    "pi","e",
};
static const char *yytokenAlias[] = {
    "EOF","NUM","PLUS","MINUS","MULTIPLY","DIVIDE",
    "EXP","BRA","KET","SIN","COS","TAN",
    "ASIN","ACOS","ATAN","CEXP","LN","I",
    "PI","E",
};
static const char *yynonTerminals[] = {
    "(accept)","start","expr","multiplyExpr","powerExpr","atomicExpr",
    "atom","functions","consts",
};
static int yyParser_reduce(yyParser *yyparser,int yyrule){
    YYCHECK_PUSH_TOKEN();
    double _Complex yyval;
    int *yydata = (int *)yyparser->userData;
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
            #line 37 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 395 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* expr -> expr <+> multiplyExpr  */
            #line 40 "parser.y"
            { yyval = (yyparser->sp[-3]) + (yyparser->sp[-1]); }
            #line 404 "parser.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* expr -> expr <-> multiplyExpr  */
            #line 41 "parser.y"
            { yyval = (yyparser->sp[-3]) - (yyparser->sp[-1]); }
            #line 413 "parser.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* expr -> multiplyExpr  */
            #line 42 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 422 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* multiplyExpr -> multiplyExpr <*> powerExpr  */
            #line 45 "parser.y"
            { yyval = (yyparser->sp[-3]) * (yyparser->sp[-1]); }
            #line 431 "parser.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 6:
            /* multiplyExpr -> multiplyExpr </> powerExpr  */
            #line 46 "parser.y"
            { yyval = (yyparser->sp[-3]) / (yyparser->sp[-1]); }
            #line 440 "parser.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 7:
            /* multiplyExpr -> powerExpr  */
            #line 47 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 449 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 8:
            /* powerExpr -> powerExpr <**> atomicExpr  */
            #line 50 "parser.y"
            { yyval = cpow((yyparser->sp[-3]),(yyparser->sp[-1])); }
            #line 458 "parser.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 9:
            /* powerExpr -> atomicExpr  */
            #line 51 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 467 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 10:
            /* atomicExpr -> atom  */
            #line 54 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 476 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 11:
            /* atomicExpr -> <+> atom  */
            #line 55 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 485 "parser.c"
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 12:
            /* atomicExpr -> <-> atom  */
            #line 56 "parser.y"
            { yyval = -(yyparser->sp[-1]); }
            #line 494 "parser.c"
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 13:
            /* atomicExpr -> <*> atom  */
            #line 58 "parser.y"
            { yyval = conj((yyparser->sp[-1])); }
            #line 503 "parser.c"
            yyparser->sp -= 2;
            yyparser->sLen -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 14:
            /* atom -> <num>  */
            #line 61 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 512 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 15:
            /* atom -> <(> expr <)>  */
            #line 62 "parser.y"
            { yyval = (yyparser->sp[-2]); }
            #line 521 "parser.c"
            yyparser->sp -= 3;
            yyparser->sLen -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 16:
            /* atom -> functions  */
            #line 63 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 530 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 17:
            /* atom -> consts  */
            #line 64 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 539 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 18:
            /* functions -> <sin> <(> expr <)>  */
            #line 67 "parser.y"
            { yyval = csin((yyparser->sp[-2])); }
            #line 548 "parser.c"
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 19:
            /* functions -> <cos> <(> expr <)>  */
            #line 68 "parser.y"
            { yyval = ccos((yyparser->sp[-2])); }
            #line 557 "parser.c"
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 20:
            /* functions -> <tan> <(> expr <)>  */
            #line 69 "parser.y"
            { yyval = ctan((yyparser->sp[-2])); }
            #line 566 "parser.c"
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 21:
            /* functions -> <asin> <(> expr <)>  */
            #line 70 "parser.y"
            { yyval = casin((yyparser->sp[-2])); }
            #line 575 "parser.c"
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 22:
            /* functions -> <acos> <(> expr <)>  */
            #line 71 "parser.y"
            { yyval = cacos((yyparser->sp[-2])); }
            #line 584 "parser.c"
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 23:
            /* functions -> <atan> <(> expr <)>  */
            #line 72 "parser.y"
            { yyval = catan((yyparser->sp[-2])); }
            #line 593 "parser.c"
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 24:
            /* functions -> <exp> <(> expr <)>  */
            #line 73 "parser.y"
            { yyval = cexp((yyparser->sp[-2])); }
            #line 602 "parser.c"
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 25:
            /* functions -> <ln> <(> expr <)>  */
            #line 74 "parser.y"
            { yyval = clog((yyparser->sp[-2])); }
            #line 611 "parser.c"
            yyparser->sp -= 4;
            yyparser->sLen -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 26:
            /* consts -> <I>  */
            #line 77 "parser.y"
            { yyval = I; }
            #line 620 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 27:
            /* consts -> <pi>  */
            #line 78 "parser.y"
            { yyval = 3.14159265358979; }
            #line 629 "parser.c"
            yyparser->sp -= 1;
            yyparser->sLen -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 28:
            /* consts -> <e>  */
            #line 79 "parser.y"
            { yyval = 2.718281828; }
            #line 638 "parser.c"
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
    yyparser->sp = yyparser->pstack = (double _Complex *)altor(sizeof(double _Complex) * yyparser->pSize);
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
