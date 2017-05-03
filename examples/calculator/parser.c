/*
    Generated by yapc -- An LR(1) parser generator made by CFY
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "parser.h"
static const int yytokenCount = 24;
static const int yyntCount = 6;
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
        yyparser->pstack = (double _Complex *)YYREALLOC(yyparser->pstack,sizeof(double _Complex) * yyparser->pSize);\
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
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 1 */
        -1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 2 */
        -2,     0,    12,    10,     8,     6,     4,     0,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 3 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 4 */
        -7,     0,    -7,    -7,    -7,    -7,     4,     0,    -7,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 5 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 6 */
        -6,     0,    -6,    -6,    -6,    -6,     4,     0,    -6,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 7 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 8 */
        -5,     0,    -5,    -5,    -5,    -5,     4,     0,    -5,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 9 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 10 */
        -4,     0,    -4,    -4,     8,     6,     4,     0,    -4,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 11 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 12 */
        -3,     0,    -3,    -3,     8,     6,     4,     0,    -3,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 13 */
       -11,     0,   -11,   -11,   -11,   -11,   -11,     0,   -11,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 14 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 15 */
       -10,     0,   -10,   -10,   -10,   -10,   -10,     0,   -10,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 16 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 17 */
        -9,     0,    -9,    -9,    -9,    -9,    -9,     0,    -9,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 18 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 19 */
        -8,     0,    -8,    -8,    -8,    -8,    -8,     0,    -8,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 20 */
       -15,     0,   -15,   -15,   -15,   -15,   -15,     0,   -15,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 21 */
       -14,     0,   -14,   -14,   -14,   -14,   -14,     0,   -14,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 22 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 23 */
         0,     0,    12,    10,     8,     6,     4,     0,    25,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 24 */
       -13,     0,   -13,   -13,   -13,   -13,   -13,     0,   -13,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 25 */
       -12,     0,   -12,   -12,   -12,   -12,   -12,     0,   -12,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 26 */
       -30,     0,   -30,   -30,   -30,   -30,   -30,     0,   -30,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 27 */
       -29,     0,   -29,   -29,   -29,   -29,   -29,     0,   -29,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 28 */
       -28,     0,   -28,   -28,   -28,   -28,   -28,     0,   -28,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 29 */
         0,     0,     0,     0,     0,     0,     0,    31,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 30 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 31 */
         0,     0,    12,    10,     8,     6,     4,     0,    33,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 32 */
       -27,     0,   -27,   -27,   -27,   -27,   -27,     0,   -27,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 33 */
         0,     0,     0,     0,     0,     0,     0,    35,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 34 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 35 */
         0,     0,    12,    10,     8,     6,     4,     0,    37,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 36 */
       -26,     0,   -26,   -26,   -26,   -26,   -26,     0,   -26,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 37 */
         0,     0,     0,     0,     0,     0,     0,    39,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 38 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 39 */
         0,     0,    12,    10,     8,     6,     4,     0,    41,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 40 */
       -25,     0,   -25,   -25,   -25,   -25,   -25,     0,   -25,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 41 */
         0,     0,     0,     0,     0,     0,     0,    43,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 42 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 43 */
         0,     0,    12,    10,     8,     6,     4,     0,    45,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 44 */
       -24,     0,   -24,   -24,   -24,   -24,   -24,     0,   -24,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 45 */
         0,     0,     0,     0,     0,     0,     0,    47,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 46 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 47 */
         0,     0,    12,    10,     8,     6,     4,     0,    49,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 48 */
       -23,     0,   -23,   -23,   -23,   -23,   -23,     0,   -23,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 49 */
         0,     0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 50 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 51 */
         0,     0,    12,    10,     8,     6,     4,     0,    53,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 52 */
       -22,     0,   -22,   -22,   -22,   -22,   -22,     0,   -22,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 53 */
         0,     0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 54 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 55 */
         0,     0,    12,    10,     8,     6,     4,     0,    57,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 56 */
       -21,     0,   -21,   -21,   -21,   -21,   -21,     0,   -21,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 57 */
         0,     0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 58 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 59 */
         0,     0,    12,    10,     8,     6,     4,     0,    61,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 60 */
       -20,     0,   -20,   -20,   -20,   -20,   -20,     0,   -20,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 61 */
         0,     0,     0,     0,     0,     0,     0,    63,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 62 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 63 */
         0,     0,    12,    10,     8,     6,     4,     0,    65,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 64 */
       -19,     0,   -19,   -19,   -19,   -19,   -19,     0,   -19,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 65 */
         0,     0,     0,     0,     0,     0,     0,    67,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 66 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 67 */
         0,     0,    12,    10,     8,     6,     4,     0,    69,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 68 */
       -18,     0,   -18,   -18,   -18,   -18,   -18,     0,   -18,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 69 */
         0,     0,     0,     0,     0,     0,     0,    71,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 70 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 71 */
         0,     0,    12,    10,     8,     6,     4,     0,    73,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 72 */
       -17,     0,   -17,   -17,   -17,   -17,   -17,     0,   -17,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 73 */
         0,     0,     0,     0,     0,     0,     0,    75,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 74 */
         0,    26,    19,    17,    15,     0,     0,    23,     0,    74,    70,
        66,    50,    46,    42,    62,    58,    54,    38,    34,    30,    29,
        28,    27,
    /* state 75 */
         0,     0,    12,    10,     8,     6,     4,     0,    77,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 76 */
       -16,     0,   -16,   -16,   -16,   -16,   -16,     0,   -16,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
};
/*
    goto table
*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     3,    14,    22,    21,
    /* state 1 */
         0,     0,     0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     0,     0,     0,
    /* state 3 */
         0,     0,     5,    14,    22,    21,
    /* state 4 */
         0,     0,     0,     0,     0,     0,
    /* state 5 */
         0,     0,     7,    14,    22,    21,
    /* state 6 */
         0,     0,     0,     0,     0,     0,
    /* state 7 */
         0,     0,     9,    14,    22,    21,
    /* state 8 */
         0,     0,     0,     0,     0,     0,
    /* state 9 */
         0,     0,    11,    14,    22,    21,
    /* state 10 */
         0,     0,     0,     0,     0,     0,
    /* state 11 */
         0,     0,    13,    14,    22,    21,
    /* state 12 */
         0,     0,     0,     0,     0,     0,
    /* state 13 */
         0,     0,     0,     0,     0,     0,
    /* state 14 */
         0,     0,    16,    14,    22,    21,
    /* state 15 */
         0,     0,     0,     0,     0,     0,
    /* state 16 */
         0,     0,    18,    14,    22,    21,
    /* state 17 */
         0,     0,     0,     0,     0,     0,
    /* state 18 */
         0,     0,    20,    14,    22,    21,
    /* state 19 */
         0,     0,     0,     0,     0,     0,
    /* state 20 */
         0,     0,     0,     0,     0,     0,
    /* state 21 */
         0,     0,     0,     0,     0,     0,
    /* state 22 */
         0,     0,    24,    14,    22,    21,
    /* state 23 */
         0,     0,     0,     0,     0,     0,
    /* state 24 */
         0,     0,     0,     0,     0,     0,
    /* state 25 */
         0,     0,     0,     0,     0,     0,
    /* state 26 */
         0,     0,     0,     0,     0,     0,
    /* state 27 */
         0,     0,     0,     0,     0,     0,
    /* state 28 */
         0,     0,     0,     0,     0,     0,
    /* state 29 */
         0,     0,     0,     0,     0,     0,
    /* state 30 */
         0,     0,    32,    14,    22,    21,
    /* state 31 */
         0,     0,     0,     0,     0,     0,
    /* state 32 */
         0,     0,     0,     0,     0,     0,
    /* state 33 */
         0,     0,     0,     0,     0,     0,
    /* state 34 */
         0,     0,    36,    14,    22,    21,
    /* state 35 */
         0,     0,     0,     0,     0,     0,
    /* state 36 */
         0,     0,     0,     0,     0,     0,
    /* state 37 */
         0,     0,     0,     0,     0,     0,
    /* state 38 */
         0,     0,    40,    14,    22,    21,
    /* state 39 */
         0,     0,     0,     0,     0,     0,
    /* state 40 */
         0,     0,     0,     0,     0,     0,
    /* state 41 */
         0,     0,     0,     0,     0,     0,
    /* state 42 */
         0,     0,    44,    14,    22,    21,
    /* state 43 */
         0,     0,     0,     0,     0,     0,
    /* state 44 */
         0,     0,     0,     0,     0,     0,
    /* state 45 */
         0,     0,     0,     0,     0,     0,
    /* state 46 */
         0,     0,    48,    14,    22,    21,
    /* state 47 */
         0,     0,     0,     0,     0,     0,
    /* state 48 */
         0,     0,     0,     0,     0,     0,
    /* state 49 */
         0,     0,     0,     0,     0,     0,
    /* state 50 */
         0,     0,    52,    14,    22,    21,
    /* state 51 */
         0,     0,     0,     0,     0,     0,
    /* state 52 */
         0,     0,     0,     0,     0,     0,
    /* state 53 */
         0,     0,     0,     0,     0,     0,
    /* state 54 */
         0,     0,    56,    14,    22,    21,
    /* state 55 */
         0,     0,     0,     0,     0,     0,
    /* state 56 */
         0,     0,     0,     0,     0,     0,
    /* state 57 */
         0,     0,     0,     0,     0,     0,
    /* state 58 */
         0,     0,    60,    14,    22,    21,
    /* state 59 */
         0,     0,     0,     0,     0,     0,
    /* state 60 */
         0,     0,     0,     0,     0,     0,
    /* state 61 */
         0,     0,     0,     0,     0,     0,
    /* state 62 */
         0,     0,    64,    14,    22,    21,
    /* state 63 */
         0,     0,     0,     0,     0,     0,
    /* state 64 */
         0,     0,     0,     0,     0,     0,
    /* state 65 */
         0,     0,     0,     0,     0,     0,
    /* state 66 */
         0,     0,    68,    14,    22,    21,
    /* state 67 */
         0,     0,     0,     0,     0,     0,
    /* state 68 */
         0,     0,     0,     0,     0,     0,
    /* state 69 */
         0,     0,     0,     0,     0,     0,
    /* state 70 */
         0,     0,    72,    14,    22,    21,
    /* state 71 */
         0,     0,     0,     0,     0,     0,
    /* state 72 */
         0,     0,     0,     0,     0,     0,
    /* state 73 */
         0,     0,     0,     0,     0,     0,
    /* state 74 */
         0,     0,    76,    14,    22,    21,
    /* state 75 */
         0,     0,     0,     0,     0,     0,
    /* state 76 */
         0,     0,     0,     0,     0,     0,
};
/*
    the left hand side of each rule,used to determine goto action.
*/
static const int yylhs[] = {
         0,     1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
         3,     3,     3,     3,     4,     4,     4,     4,     4,     4,     4,
         4,     4,     4,     4,     4,     5,     5,     5,
};
/*
    the length of the symbols on the rhs of each rule
    used to pop states from the state stack when doing
    an reduction.
*/
static const int yyruleLen[] = {
         1,     1,     3,     3,     3,     3,     3,     2,     2,     2,     1,
         1,     3,     1,     1,     4,     4,     4,     4,     4,     4,     4,
         4,     4,     4,     4,     4,     1,     1,     1,
};
const char *yytokenNames[] = {
    "EOF","num","+","-","*","/",
    "**","(",")","sin","cos","tan",
    "sinh","cosh","tanh","asin","acos","atan",
    "exp","ln","sqrt","I","pi","e",
    
};
const char *yytokenAlias[] = {
    "EOF","NUM","PLUS","MINUS","MULTIPLY","DIVIDE",
    "EXP","BRA","KET","SIN","COS","TAN",
    "SINH","COSH","TANH","ASIN","ACOS","ATAN",
    "CEXP","LN","SQRT","I","PI","E",
    
};
const char *yynonTerminals[] = {
    "(accept)","start","expr","atom","functions","consts",
    
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
            *yyparser->sp++ = yyval;
            break;
        case 1:
            /* start -> expr  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* expr -> expr <+> expr  */
            #line 48 "parser.y"
            { yyval = (yyparser->sp[-3]) + (yyparser->sp[-1]); }
            #line 576 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* expr -> expr <-> expr  */
            #line 49 "parser.y"
            { yyval = (yyparser->sp[-3]) - (yyparser->sp[-1]); }
            #line 584 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* expr -> expr <*> expr  */
            #line 50 "parser.y"
            { yyval = (yyparser->sp[-3]) * (yyparser->sp[-1]); }
            #line 592 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* expr -> expr </> expr  */
            #line 51 "parser.y"
            { yyval = (yyparser->sp[-3]) / (yyparser->sp[-1]); }
            #line 600 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 6:
            /* expr -> expr <**> expr  */
            #line 52 "parser.y"
            { yyval = cpow((yyparser->sp[-3]),(yyparser->sp[-1])); }
            #line 608 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 7:
            /* expr -> <+> expr  */
            #line 53 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 616 "parser.c"
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 8:
            /* expr -> <-> expr  */
            #line 54 "parser.y"
            { yyval = -(yyparser->sp[-1]); }
            #line 624 "parser.c"
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 9:
            /* expr -> <*> expr  */
            #line 56 "parser.y"
            { yyval = conj((yyparser->sp[-1])); }
            #line 632 "parser.c"
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 10:
            /* expr -> atom  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 11:
            /* atom -> <num>  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 12:
            /* atom -> <(> expr <)>  */
            #line 62 "parser.y"
            { yyval = (yyparser->sp[-2]); }
            #line 654 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 13:
            /* atom -> functions  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 14:
            /* atom -> consts  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 15:
            /* functions -> <sin> <(> expr <)>  */
            #line 68 "parser.y"
            { yyval = csin((yyparser->sp[-2]));   }
            #line 676 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 16:
            /* functions -> <cos> <(> expr <)>  */
            #line 69 "parser.y"
            { yyval = ccos((yyparser->sp[-2]));   }
            #line 684 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 17:
            /* functions -> <tan> <(> expr <)>  */
            #line 70 "parser.y"
            { yyval = ctan((yyparser->sp[-2]));   }
            #line 692 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 18:
            /* functions -> <asin> <(> expr <)>  */
            #line 71 "parser.y"
            { yyval = casin((yyparser->sp[-2]));  }
            #line 700 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 19:
            /* functions -> <acos> <(> expr <)>  */
            #line 72 "parser.y"
            { yyval = cacos((yyparser->sp[-2]));  }
            #line 708 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 20:
            /* functions -> <atan> <(> expr <)>  */
            #line 73 "parser.y"
            { yyval = catan((yyparser->sp[-2]));  }
            #line 716 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 21:
            /* functions -> <sinh> <(> expr <)>  */
            #line 74 "parser.y"
            { yyval = csinh((yyparser->sp[-2]));  }
            #line 724 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 22:
            /* functions -> <cosh> <(> expr <)>  */
            #line 75 "parser.y"
            { yyval = ccosh((yyparser->sp[-2]));  }
            #line 732 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 23:
            /* functions -> <tanh> <(> expr <)>  */
            #line 76 "parser.y"
            { yyval = ctanh((yyparser->sp[-2]));  }
            #line 740 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 24:
            /* functions -> <exp> <(> expr <)>  */
            #line 77 "parser.y"
            { yyval = cexp((yyparser->sp[-2]));   }
            #line 748 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 25:
            /* functions -> <ln> <(> expr <)>  */
            #line 78 "parser.y"
            { yyval = clog((yyparser->sp[-2]));   }
            #line 756 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 26:
            /* functions -> <sqrt> <(> expr <)>  */
            #line 79 "parser.y"
            { yyval = csqrt((yyparser->sp[-2]));  }
            #line 764 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 27:
            /* consts -> <I>  */
            #line 83 "parser.y"
            { yyval = I; }
            #line 772 "parser.c"
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 28:
            /* consts -> <pi>  */
            #line 84 "parser.y"
            { yyval = 3.14159265358979; }
            #line 780 "parser.c"
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 29:
            /* consts -> <e>  */
            #line 85 "parser.y"
            { yyval = 2.718281828; }
            #line 788 "parser.c"
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
    yyparser->state = (int *)YYMALLOC(sizeof(int) * yyparser->sSize);
    yyparser->state[0] = 0;
    yyparser->sp = yyparser->pstack = (double _Complex *)YYMALLOC(sizeof(double _Complex) * yyparser->pSize);
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
    YYFREE(yyparser->state);
    YYFREE(yyparser->pstack);
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
