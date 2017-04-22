#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__
#include <stdio.h>

typedef struct _YRuleItem YRuleItem;
typedef struct _YRule YRule;

struct _YRuleItem{
    int id;
    int isTerminal;
    //const char *actionBlock;
};

struct _YRule{
    int index;
    int lhs;
    int length;
    const char *actionBlock;
    int line;

    //hasValue only makes sense when isGen is true,
    //i.e.,user-defined rules always has a value.
    int hasValue,isGen;

    //the number of sematic variables on the stack when this rule is recongnized.
    
    int stackOffset;
    
    
    YRuleItem *rule;
};

typedef struct _YGrammar{
    const char **tokenNames;
    const char **tokenAlias;
    int tokenCount;

    const char **ntNames;
    int ntCount;

    int ruleCount;
    YRule *rules;

    int *tests;
    int testCount;

    int actionCount;
    
    const char *nameSpace;
    const char *tokenPrefix;
    const char *stype;

    const char *prologue;

    char *spool;
    char data[1];
}YGrammar;

//keep in mind that the index of a terminal in this first set is its origin index plus 1,
//bucause token with index 0 is reserved for epsilon,i.e.,empty.
typedef struct _YFirstSets{
    YGrammar *g;
    int size;
    char data[1];
}YFirstSets;

int YRule_dump(YGrammar *g,YRule *rule,FILE *out);
int YGrammar_dump(YGrammar *g,FILE *out);
int YGrammar_free(YGrammar *g);

//token set helper functions
int YGrammar_getTokenSetSize(YGrammar *g);
int YTokenSet_add(YGrammar *g,char *set,int token);
int YTokenSet_removeToken(YGrammar *g,char *set,int token);
int YTokenSet_contains(YGrammar *g,char *set,int token);
int YTokenSet_union(YGrammar *g,char *dest,char *src);
int YTokenSet_hasIntersection(YGrammar *g,char *s1,char *s2);
int YTokenSet_isIdentical(char *s1,char *s2,YGrammar *g);


YFirstSets *YGrammar_generateFirstSets(YGrammar *g);

int YFirstSets_contains(YFirstSets *sets,int nt,int t);
char *YFirstSets_getSet(YFirstSets *sets,int nt);
int YFirstSets_dump(YFirstSets *sets,FILE *out);
int YFirstSets_free(YFirstSets *sets);



#endif
