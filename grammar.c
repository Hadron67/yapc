/*  
    YAPC - Yet Another Parser Compiler - An LR(1) parser generator

    Copyright (C) 2017  Chen FeiYu

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>
#include "ydef.h"
#include "grammar.h"

#define SBITS 8

int YRule_dump(YGrammar *g,YRule *rule,FILE *out){
    int j;
    fprintf(out,"%s -> ",g->ntNames[rule->lhs]);
    for(j = 0;j < rule->length;j++){
        YRuleItem *item = rule->rule + j;
        if(item->isTerminal){
            fprintf(out,"<%s> ",g->tokens[item->id].name);
        }
        else {
            fprintf(out,"%s ",g->ntNames[item->id]);
        }
    }
    return 0;
}
int YGrammar_dump(YGrammar *g,FILE *out){
    int i,j;
    for(i = 0;i < g->ruleCount;i++){
        YRule *rule = g->rules + i;
        YRule_dump(g,rule,out);
        fprintf(out,"\n");
    }
    
    return 0;
}
int YGrammar_free(YGrammar *g){
    ya_free(g->tests);
    ya_free(g->spool);
    ya_free(g);
    return 0;
}

int YGrammar_getTokenSetSize(YGrammar *g){
    int count = g->tokenCount + 1;
    int size = count / SBITS;
    if(count % SBITS != 0){
        size++;
    }
    
    return size;
}
int YTokenSet_add(YGrammar *g,char *set,int token){
    int i = token % SBITS;
    char *item = set + (token / SBITS);
    char orig = *item;
    *item |= 1 << i;
    return orig != *item;
}
int YTokenSet_removeToken(YGrammar *g,char *set,int token){
    int i = token % SBITS;
    char *item = set + (token / SBITS);
    char orig = *item;
    *item &= ~(1 << i);
    return orig != *item;
}
int YTokenSet_contains(YGrammar *g,char *set,int token){
    int i = token % SBITS;
    char *item = set + (token / SBITS);
    return *item & (1 << i);
}
int YTokenSet_union(YGrammar *g,char *dest,char *src){
    int ret = 0;
    int size = YGrammar_getTokenSetSize(g);
    int i;
    for(i = 0;i < size;i++){
        char orig = dest[i];
        dest[i] |= src[i];
        ret = ret || orig != dest[i];
    }
    return ret;
}
int YTokenSet_hasIntersection(YGrammar *g,char *s1,char *s2){
    int size = YGrammar_getTokenSetSize(g);
    int i;
    for(i = 0;i < size;i++){
        if(s1[i] & s2[i]){
            return 1;
        }
    }
    return 0;
}
int YTokenSet_isIdentical(char *s1,char *s2,YGrammar *g){
    int size = YGrammar_getTokenSetSize(g);
    int i;
    for(i = 0;i < size;i++){
        if(s1[i] != s2[i]){
            return 0;
        }
    }
    return 1;
}
static int YFirstSets_add(YFirstSets *sets,int nt,int t){
    return YTokenSet_add(sets->g,sets->data + nt * sets->size,t);
}

static int YFirstSets_addAll(YFirstSets *sets,int nt1,int nt2){
    int ret = 0;
    char *item1 = sets->data + nt1 * sets->size;
    char *item2 = sets->data + nt2 * sets->size;
    
    return YTokenSet_union(sets->g,item1,item2);
}

int YFirstSets_contains(YFirstSets *sets,int nt,int t){
    char *item = sets->data + nt * sets->size + (t / SBITS) * sizeof(char);
    return YTokenSet_contains(sets->g,sets->data + nt * sets->size,t);
}

char *YFirstSets_getSet(YFirstSets *sets,int nt){
    return sets->data + nt * sets->size;
}

YFirstSets *YGrammar_generateFirstSets(YGrammar *g){
    int count = g->tokenCount + 1;
    int size = count / SBITS;
    if(count % SBITS != 0){
        size++;
    }
    YFirstSets *sets = (YFirstSets *)ya_malloc(sizeof(YFirstSets) + g->ntCount * size * sizeof(char));
    memset(sets->data,0,g->ntCount * size * sizeof(char));
    sets->g = g;
    sets->size = size;
    
    int changed = 1;
    while(changed){
        changed = 0;
        int i,j,k;
        for(i = 0;i < g->ntCount;i++){
            for(j = 0;j < g->ruleCount;j++){
                YRule *rule = g->rules + j;
                if(rule->lhs == i){                
                    if(rule->length == 0){
                        changed = changed || YFirstSets_add(sets,i,0);                    
                    }
                    else {
                        for(k = 0;k < rule->length;k++){
                            YRuleItem *item = rule->rule + k;
                            if(item->isTerminal){
                                //token index must plus 1.
                                changed = changed || YFirstSets_add(sets,i,item->id + 1);
                                break;
                            }
                            else {
                                if(item->id != i)
                                    changed = changed || YFirstSets_addAll(sets,i,item->id);
                                if(!YFirstSets_contains(sets,item->id,0)){
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return sets;
}

int YFirstSets_dump(YFirstSets *sets,FILE *out){
    int i,j;
    YGrammar *g = sets->g;
    for(i = 0;i < g->ntCount;i++){
        fprintf(out,"FIRST(%s) = { ",g->ntNames[i]);
        if(YFirstSets_contains(sets,i,0)){
            fprintf(out,"<>,");
        }
        for(j = 0;j < g->tokenCount;j++){
            if(YFirstSets_contains(sets,i,j + 1)){
                fprintf(out,"<%s>,",g->tokens[j].name);
            }
        }
        fprintf(out," }\n");
    }
    return 0;
}
int YFirstSets_free(YFirstSets *sets){
    ya_free(sets);
    return 0;
}
