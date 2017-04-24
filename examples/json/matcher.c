

#include "json.h"
#include "json_match.h"

#include "matcher.h"
static const int jmtokenCount = 6;
static const int jmntCount = 2;
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
    if(jmparser->sLen >= jmparser->sSize){ \
        jmparser->sSize *= 2; \
        jmparser->state = (int *)YYREALLOC(jmparser->state,sizeof(int) * jmparser->sSize); \
    } \
    jmparser->state[jmparser->sLen++] = (s);

#define YYSTATE() (jmparser->state[jmparser->sLen - 1])
#define YYCHECK_PUSH_TOKEN() \
    if(jmparser->sp - jmparser->pstack >= jmparser->pSize){\
        size_t offset = jmparser->sp - jmparser->pstack;\
        jmparser->pSize *= 2;\
        jmparser->pstack = (jnode *)YYREALLOC(jmparser->pstack,sizeof(jnode) * jmparser->pSize);\
        jmparser->sp = jmparser->pstack + offset;\
    }
/** shift action table
 * positive numbers indicate the states shift to,
 * negative numbers indicate the rules reduce with.
 * the state should be the number in the table minus one,since zero marks
 * for error.*/
static const int jmshift[] = {
    /* state 0 */
        -5,    -5,     0,    -5,     0,     0,
    /* state 1 */
        -1,     3,     0,     8,     0,     0,
    /* state 2 */
         0,     0,     0,     0,     4,     6,
    /* state 3 */
         0,     0,     5,     0,     0,     0,
    /* state 4 */
        -2,    -2,     0,    -2,     0,     0,
    /* state 5 */
         0,     0,     7,     0,     0,     0,
    /* state 6 */
        -3,    -3,     0,    -3,     0,     0,
    /* state 7 */
        -4,    -4,     0,    -4,     0,     0,
};
/** goto table,
 * zero iff there is an error*/
static const int jmgoto[] = {
    /* state 0 */
         0,     2,
    /* state 1 */
         0,     0,
    /* state 2 */
         0,     0,
    /* state 3 */
         0,     0,
    /* state 4 */
         0,     0,
    /* state 5 */
         0,     0,
    /* state 6 */
         0,     0,
    /* state 7 */
         0,     0,
};
/* the lhs of each rule. */
static const int jmlhs[] = {
         0,     1,     1,     1,     1,
};
static const char *jmtokenNames[] = {
    "EOF","[","]","member","num","string",
    
};
static const char *jmtokenAlias[] = {
    "EOF","CBRA","CKET","MEMBER","NUM","STRING",
    
};
static const char *jmnonTerminals[] = {
    "(accept)","start",
};
static int jmParser_reduce(jmParser *jmparser,int jmrule){
    YYCHECK_PUSH_TOKEN();
    jnode jmval;
    jMatch *jmdata = (jMatch *)jmparser->userData;
    switch(jmrule){
        case 0:
            /* (accept) -> start  */
            /* no action. */
            jmval = jmparser->sp[-1];
            jmparser->sp -= 1;
            jmparser->sLen -= 1;
            *jmparser->sp++ = jmval;
            break;
        case 1:
            /* start -> start <[> <num> <]>  */
            #line 21 "matcher.y"
            { jMatch_doNum(jmdata,&(jmparser->sp[-2])); }
            #line 112 "matcher.c"
            jmparser->sp -= 4;
            jmparser->sLen -= 4;
            *jmparser->sp++ = jmval;
            break;
        case 2:
            /* start -> start <[> <string> <]>  */
            #line 22 "matcher.y"
            { jMatch_doMember(jmdata,&(jmparser->sp[-2])); }
            #line 121 "matcher.c"
            jmparser->sp -= 4;
            jmparser->sLen -= 4;
            *jmparser->sp++ = jmval;
            break;
        case 3:
            /* start -> start <member>  */
            #line 23 "matcher.y"
            { jMatch_doMember(jmdata,&(jmparser->sp[-1])); }
            #line 130 "matcher.c"
            jmparser->sp -= 2;
            jmparser->sLen -= 2;
            *jmparser->sp++ = jmval;
            break;
        case 4:
            /* start ->  */
            /* no action. */
            jmval = jmparser->sp[0];
            *jmparser->sp++ = jmval;
            break;
    }
    int jmindex = YYSTATE() * jmntCount + jmlhs[jmrule];
    YYPUSH_STATE(jmgoto[jmindex] - 1);
    return 0;
}
int jmParser_init(jmParser *jmparser){
    jmparser->sLen = 1;
    jmparser->done = 0;
    jmparser->sSize = jmparser->pSize = 16;
    jmparser->state = (int *)YYMALLOC(sizeof(int) * jmparser->sSize);
    jmparser->state[0] = 0;
    jmparser->sp = jmparser->pstack = (jnode *)YYMALLOC(sizeof(jnode) * jmparser->pSize);
    return 0;
}
int jmParser_reInit(jmParser *jmparser){
    jmparser->sLen = 0;
    jmparser->done = 0;
    jmparser->state[0] = 0;
    jmparser->sp = jmparser->pstack;
    return 0;
}
int jmParser_free(jmParser *jmparser){
    YYFREE(jmparser->state);
    YYFREE(jmparser->pstack);
    return 0;
}
int jmParser_acceptToken(jmParser *jmparser,int jmtokenid){
    int shifted = 0;
    while(!shifted){
        int jmaction = jmshift[YYSTATE() * jmtokenCount + jmtokenid];
        if(jmaction > 0){
            YYCHECK_PUSH_TOKEN();
            *jmparser->sp++ = jmparser->token;
            YYPUSH_STATE(jmaction - 1);
            shifted = 1;
        }
        else if(jmaction < 0){
            if(jmaction == -1){
                jmparser->done = 1;
                return 0;
            }
            jmParser_reduce(jmparser,-1 - jmaction);
        }
        else {
            jmparser->error = 1;
            jmparser->errToken = jmtokenid;
            return -1;
        }
    }
    return 0;
}
int jmParser_printError(jmParser *jmparser,FILE *out){
    if(jmparser->error){
        int index = YYSTATE() * jmtokenCount;
        fprintf(out,"unexpected token '%s' (%s),was expecting one of:\n",jmtokenNames[jmparser->errToken],jmtokenAlias[jmparser->errToken]);
        int i;
        for(i = 0;i < jmtokenCount;i++){
            if(jmshift[index + i] != 0){
                fprintf(out,"    '%s' (%s) ...\n",jmtokenNames[i],jmtokenAlias[i]);
            }
        }
    }
    return 0;
}
int jmParser_clearStack(jmParser *jmparser){
    while(jmparser->sp != jmparser->pstack){
        YYDESTRUCTOR(--jmparser->sp);
    }
    return 0;
}