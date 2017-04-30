/*
    Generated by yapc -- An LR(1) parser generator made by CFY
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "parser.h"
static const int yytokenCount = 24;
static const int yyntCount = 7;
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
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 1 */
        -1,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 2 */
        -2,     0,     4,     6,     8,    10,    12,     0,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 3 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 4 */
        -3,     0,    -3,    -3,     8,    10,    12,     0,    -3,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 5 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 6 */
        -4,     0,    -4,    -4,     8,    10,    12,     0,    -4,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 7 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 8 */
        -5,     0,    -5,    -5,    -5,    -5,    12,     0,    -5,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 9 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 10 */
        -6,     0,    -6,    -6,    -6,    -6,    12,     0,    -6,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 11 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 12 */
        -7,     0,    -7,    -7,    -7,    -7,    -7,     0,    -7,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 13 */
        -8,     0,    -8,    -8,    -8,    -8,    -8,     0,    -8,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 14 */
        -9,     0,    -9,    -9,    -9,    -9,    -9,     0,    -9,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 15 */
         0,    22,     0,     0,     0,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 16 */
       -10,     0,   -10,   -10,   -10,   -10,   -10,     0,   -10,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 17 */
         0,    22,     0,     0,     0,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 18 */
       -11,     0,   -11,   -11,   -11,   -11,   -11,     0,   -11,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 19 */
         0,    22,     0,     0,     0,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 20 */
       -12,     0,   -12,   -12,   -12,   -12,   -12,     0,   -12,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 21 */
       -13,     0,   -13,   -13,   -13,   -13,   -13,     0,   -13,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 22 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 23 */
         0,     0,     4,     6,     8,    10,    12,     0,    25,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 24 */
       -14,     0,   -14,   -14,   -14,   -14,   -14,     0,   -14,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 25 */
       -15,     0,   -15,   -15,   -15,   -15,   -15,     0,   -15,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 26 */
       -16,     0,   -16,   -16,   -16,   -16,   -16,     0,   -16,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 27 */
         0,     0,     0,     0,     0,     0,     0,    29,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 28 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 29 */
         0,     0,     4,     6,     8,    10,    12,     0,    31,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 30 */
       -17,     0,   -17,   -17,   -17,   -17,   -17,     0,   -17,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 31 */
         0,     0,     0,     0,     0,     0,     0,    33,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 32 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 33 */
         0,     0,     4,     6,     8,    10,    12,     0,    35,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 34 */
       -18,     0,   -18,   -18,   -18,   -18,   -18,     0,   -18,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 35 */
         0,     0,     0,     0,     0,     0,     0,    37,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 36 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 37 */
         0,     0,     4,     6,     8,    10,    12,     0,    39,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 38 */
       -19,     0,   -19,   -19,   -19,   -19,   -19,     0,   -19,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 39 */
         0,     0,     0,     0,     0,     0,     0,    41,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 40 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 41 */
         0,     0,     4,     6,     8,    10,    12,     0,    43,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 42 */
       -20,     0,   -20,   -20,   -20,   -20,   -20,     0,   -20,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 43 */
         0,     0,     0,     0,     0,     0,     0,    45,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 44 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 45 */
         0,     0,     4,     6,     8,    10,    12,     0,    47,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 46 */
       -21,     0,   -21,   -21,   -21,   -21,   -21,     0,   -21,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 47 */
         0,     0,     0,     0,     0,     0,     0,    49,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 48 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 49 */
         0,     0,     4,     6,     8,    10,    12,     0,    51,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 50 */
       -22,     0,   -22,   -22,   -22,   -22,   -22,     0,   -22,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 51 */
         0,     0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 52 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 53 */
         0,     0,     4,     6,     8,    10,    12,     0,    55,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 54 */
       -23,     0,   -23,   -23,   -23,   -23,   -23,     0,   -23,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 55 */
         0,     0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 56 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 57 */
         0,     0,     4,     6,     8,    10,    12,     0,    59,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 58 */
       -24,     0,   -24,   -24,   -24,   -24,   -24,     0,   -24,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 59 */
         0,     0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 60 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 61 */
         0,     0,     4,     6,     8,    10,    12,     0,    63,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 62 */
       -25,     0,   -25,   -25,   -25,   -25,   -25,     0,   -25,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 63 */
         0,     0,     0,     0,     0,     0,     0,    65,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 64 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 65 */
         0,     0,     4,     6,     8,    10,    12,     0,    67,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 66 */
       -26,     0,   -26,   -26,   -26,   -26,   -26,     0,   -26,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 67 */
         0,     0,     0,     0,     0,     0,     0,    69,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 68 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 69 */
         0,     0,     4,     6,     8,    10,    12,     0,    71,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 70 */
       -27,     0,   -27,   -27,   -27,   -27,   -27,     0,   -27,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 71 */
         0,     0,     0,     0,     0,     0,     0,    73,     0,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 72 */
         0,    22,    16,    18,    20,     0,     0,    23,     0,    28,    32,
        36,    52,    56,    60,    40,    44,    48,    64,    68,    72,    76,
        77,    78,
    /* state 73 */
         0,     0,     4,     6,     8,    10,    12,     0,    75,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 74 */
       -28,     0,   -28,   -28,   -28,   -28,   -28,     0,   -28,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 75 */
       -29,     0,   -29,   -29,   -29,   -29,   -29,     0,   -29,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 76 */
       -30,     0,   -30,   -30,   -30,   -30,   -30,     0,   -30,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
    /* state 77 */
       -31,     0,   -31,   -31,   -31,   -31,   -31,     0,   -31,     0,     0,
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
         0,     0,
};
/*
    goto table
*/
static const int yygoto[] = {
    /* state 0 */
         0,     2,     3,    14,    15,    26,    27,
    /* state 1 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 2 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 3 */
         0,     0,     5,    14,    15,    26,    27,
    /* state 4 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 5 */
         0,     0,     7,    14,    15,    26,    27,
    /* state 6 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 7 */
         0,     0,     9,    14,    15,    26,    27,
    /* state 8 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 9 */
         0,     0,    11,    14,    15,    26,    27,
    /* state 10 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 11 */
         0,     0,    13,    14,    15,    26,    27,
    /* state 12 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 13 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 14 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 15 */
         0,     0,     0,     0,    17,    26,    27,
    /* state 16 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 17 */
         0,     0,     0,     0,    19,    26,    27,
    /* state 18 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 19 */
         0,     0,     0,     0,    21,    26,    27,
    /* state 20 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 21 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 22 */
         0,     0,    24,    14,    15,    26,    27,
    /* state 23 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 24 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 25 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 26 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 27 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 28 */
         0,     0,    30,    14,    15,    26,    27,
    /* state 29 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 30 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 31 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 32 */
         0,     0,    34,    14,    15,    26,    27,
    /* state 33 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 34 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 35 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 36 */
         0,     0,    38,    14,    15,    26,    27,
    /* state 37 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 38 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 39 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 40 */
         0,     0,    42,    14,    15,    26,    27,
    /* state 41 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 42 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 43 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 44 */
         0,     0,    46,    14,    15,    26,    27,
    /* state 45 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 46 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 47 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 48 */
         0,     0,    50,    14,    15,    26,    27,
    /* state 49 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 50 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 51 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 52 */
         0,     0,    54,    14,    15,    26,    27,
    /* state 53 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 54 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 55 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 56 */
         0,     0,    58,    14,    15,    26,    27,
    /* state 57 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 58 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 59 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 60 */
         0,     0,    62,    14,    15,    26,    27,
    /* state 61 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 62 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 63 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 64 */
         0,     0,    66,    14,    15,    26,    27,
    /* state 65 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 66 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 67 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 68 */
         0,     0,    70,    14,    15,    26,    27,
    /* state 69 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 70 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 71 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 72 */
         0,     0,    74,    14,    15,    26,    27,
    /* state 73 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 74 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 75 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 76 */
         0,     0,     0,     0,     0,     0,     0,
    /* state 77 */
         0,     0,     0,     0,     0,     0,     0,
};
/*
    the left hand side of each rule,used to determine goto action.
*/
static const int yylhs[] = {
         0,     1,     2,     2,     2,     2,     2,     2,     3,     3,     3,
         3,     4,     4,     4,     4,     5,     5,     5,     5,     5,     5,
         5,     5,     5,     5,     5,     5,     6,     6,     6,
};
/*
    the length of the symbols on the rhs of each rule
    used to pop states from the state stack when doing
    an reduction.
*/
static const int yyruleLen[] = {
         1,     1,     3,     3,     3,     3,     3,     1,     1,     2,     2,
         2,     1,     3,     1,     1,     4,     4,     4,     4,     4,     4,
         4,     4,     4,     4,     4,     4,     1,     1,     1,
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
    "(accept)","start","expr","atomicExpr","atom","functions",
    "consts",
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
            #line 45 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 575 "parser.c"
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 2:
            /* expr -> expr <+> expr  */
            #line 48 "parser.y"
            { yyval = (yyparser->sp[-3]) + (yyparser->sp[-1]); }
            #line 583 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 3:
            /* expr -> expr <-> expr  */
            #line 49 "parser.y"
            { yyval = (yyparser->sp[-3]) - (yyparser->sp[-1]); }
            #line 591 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 4:
            /* expr -> expr <*> expr  */
            #line 50 "parser.y"
            { yyval = (yyparser->sp[-3]) * (yyparser->sp[-1]); }
            #line 599 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 5:
            /* expr -> expr </> expr  */
            #line 51 "parser.y"
            { yyval = (yyparser->sp[-3]) / (yyparser->sp[-1]); }
            #line 607 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 6:
            /* expr -> expr <**> expr  */
            #line 52 "parser.y"
            { yyval = cpow((yyparser->sp[-3]),(yyparser->sp[-1])); }
            #line 615 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 7:
            /* expr -> atomicExpr  */
            /* no action. */
            yyval = yyparser->sp[-1];
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 8:
            /* atomicExpr -> atom  */
            #line 56 "parser.y"
            { yyval = (yyparser->sp[-1]);       }
            #line 630 "parser.c"
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 9:
            /* atomicExpr -> <+> atom  */
            #line 57 "parser.y"
            { yyval = (yyparser->sp[-1]);       }
            #line 638 "parser.c"
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 10:
            /* atomicExpr -> <-> atom  */
            #line 58 "parser.y"
            { yyval = -(yyparser->sp[-1]);      }
            #line 646 "parser.c"
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 11:
            /* atomicExpr -> <*> atom  */
            #line 60 "parser.y"
            { yyval = conj((yyparser->sp[-1])); }
            #line 654 "parser.c"
            yyparser->sp -= 2;
            *yyparser->sp++ = yyval;
            break;
        case 12:
            /* atom -> <num>  */
            #line 63 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 662 "parser.c"
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 13:
            /* atom -> <(> expr <)>  */
            #line 64 "parser.y"
            { yyval = (yyparser->sp[-2]); }
            #line 670 "parser.c"
            yyparser->sp -= 3;
            *yyparser->sp++ = yyval;
            break;
        case 14:
            /* atom -> functions  */
            #line 65 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 678 "parser.c"
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 15:
            /* atom -> consts  */
            #line 66 "parser.y"
            { yyval = (yyparser->sp[-1]); }
            #line 686 "parser.c"
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 16:
            /* functions -> <sin> <(> expr <)>  */
            #line 69 "parser.y"
            { yyval = csin((yyparser->sp[-2]));   }
            #line 694 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 17:
            /* functions -> <cos> <(> expr <)>  */
            #line 70 "parser.y"
            { yyval = ccos((yyparser->sp[-2]));   }
            #line 702 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 18:
            /* functions -> <tan> <(> expr <)>  */
            #line 71 "parser.y"
            { yyval = ctan((yyparser->sp[-2]));   }
            #line 710 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 19:
            /* functions -> <asin> <(> expr <)>  */
            #line 72 "parser.y"
            { yyval = casin((yyparser->sp[-2]));  }
            #line 718 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 20:
            /* functions -> <acos> <(> expr <)>  */
            #line 73 "parser.y"
            { yyval = cacos((yyparser->sp[-2]));  }
            #line 726 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 21:
            /* functions -> <atan> <(> expr <)>  */
            #line 74 "parser.y"
            { yyval = catan((yyparser->sp[-2]));  }
            #line 734 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 22:
            /* functions -> <sinh> <(> expr <)>  */
            #line 75 "parser.y"
            { yyval = csinh((yyparser->sp[-2]));  }
            #line 742 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 23:
            /* functions -> <cosh> <(> expr <)>  */
            #line 76 "parser.y"
            { yyval = ccosh((yyparser->sp[-2]));  }
            #line 750 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 24:
            /* functions -> <tanh> <(> expr <)>  */
            #line 77 "parser.y"
            { yyval = ctanh((yyparser->sp[-2]));  }
            #line 758 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 25:
            /* functions -> <exp> <(> expr <)>  */
            #line 78 "parser.y"
            { yyval = cexp((yyparser->sp[-2]));   }
            #line 766 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 26:
            /* functions -> <ln> <(> expr <)>  */
            #line 79 "parser.y"
            { yyval = clog((yyparser->sp[-2]));   }
            #line 774 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 27:
            /* functions -> <sqrt> <(> expr <)>  */
            #line 80 "parser.y"
            { yyval = csqrt((yyparser->sp[-2]));  }
            #line 782 "parser.c"
            yyparser->sp -= 4;
            *yyparser->sp++ = yyval;
            break;
        case 28:
            /* consts -> <I>  */
            #line 83 "parser.y"
            { yyval = I; }
            #line 790 "parser.c"
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 29:
            /* consts -> <pi>  */
            #line 84 "parser.y"
            { yyval = 3.14159265358979; }
            #line 798 "parser.c"
            yyparser->sp -= 1;
            *yyparser->sp++ = yyval;
            break;
        case 30:
            /* consts -> <e>  */
            #line 85 "parser.y"
            { yyval = 2.718281828; }
            #line 806 "parser.c"
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
                return YY_OK;
            }
            yyParser_reduce(yyparser,-1 - yyaction);
        }
        else {
            yyparser->error = 1;
            yyparser->errToken = yytokenid;
            return YY_ERR;
        }
    }
    return YY_OK;
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
