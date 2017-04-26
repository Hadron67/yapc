#include <stdlib.h>
#include <string.h>
#include "scanner.h"

const char *tokenNames[] = {
    "begin",
    "end",
    "integer",
    "if",
    "then",
    "else",
    "function",
    "read",
    "write",
    "(identifier)",
    "(constant)",
    "=",
    "<>",
    "<=",
    "<",
    ">=",
    ">",
    "-",
    "*",
    ":=",
    "(",
    ")",
    ";",
    ","
};

#define ymalloc malloc
#define yrealloc realloc
#define yfree free

#define C (scanner->c)
#define NC() (scanner->c = fgetc(scanner->in))
#define NC2() (yyScanner_pushChar(scanner,C),scanner->c = fgetc(scanner->in))
#define IEOF (feof(scanner->in))
#define ISS(s) (yyScanner_isS(scanner,(s)))

#define ISNUM(c) ((c) >= '0' && (c) <= '9')
#define ISNAMEHEAD(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') ||(c) == '_' || (c) == '$' )
#define ISNAME(c) (ISNAMEHEAD(c) || ISNUM(c))

static char *newString(yyStringBlock **prev,const char *s,int len){
    yyStringBlock *b = (yyStringBlock *)ymalloc(sizeof(yyStringBlock) + len * sizeof(char));
    b->next = *prev;
    *prev = b;
    strncpy(b->data,s,len);
    return b->data;
}

static int deleteStringBlock(yyStringBlock *head){
    yyStringBlock *temp;
    while(head != NULL){
        temp = head->next;
        yfree(head);
        head = temp;
    }
    return 0;
}

int yyScanner_init(yyScanner *s,FILE *in,FILE *err){
    s->len = 0;
    s->size = 16;
    s->line = 1;
    s->first = 1;
    s->buf = (char *)ymalloc(sizeof(char) * s->size);
    s->head = NULL;
    
    s->in = in;
    s->err = err;
    return 0;
}
int yyScanner_free(yyScanner *s){
    yfree(s->buf);
    deleteStringBlock(s->head);
    
    return 0;
}
int yyScanner_pushChar(yyScanner *s,char c){
    if(s->len >= s->size){
        s->size *= 2;
        s->buf = (char *)yrealloc(s->buf,sizeof(char) * s->size);
    }
    s->buf[s->len++] = c;
    return 0;
}
int yyScanner_isS(yyScanner *scanner,const char *c){
    while(*c){
        if(*c != C){
            return 0;
        }
        NC2();
        c++;
    }
    return 1;
}
int yyScanner_next(yyScanner *scanner,yyToken *t){
    if(scanner->first){
        scanner->first = 0;
        NC();
    }
    while(C == ' ' || C == '\n' || C == '\t'){
        if(C == '\n'){
            scanner->line++;
        }
        NC();
    }
    t->line = scanner->line;
    if(IEOF){
        t->id = T_EOF;
        return 0;
    }
    int found = 0;
    scanner->len = 0;
    switch(C){
        case 'b':
            NC2();
            if(ISS("egin")){
                t->id = T_BEGIN;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 'i':
            NC2();
            if(C == 'f'){
                NC2();
                t->id = T_IF;
                found = 1;
                goto yycheckname;
            }
            else if(C == 'n'){
                NC2();
                if(ISS("teger")){
                    t->id = T_INTEGER;
                    found = 1;
                    goto yycheckname;
                }
            }
            goto yycheckname;
        case 't':
            NC2();
            if(ISS("hen")){
                t->id = T_THEN;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 'e':
            NC2();
            if(C == 'n'){
                NC2();
                if(C == 'd'){
                    NC2();
                    t->id = T_END;
                    found = 1;
                    goto yycheckname;
                }
            }
            else if(C == 'l'){
                NC2();
                if(ISS("se")){
                    t->id = T_ELSE;
                    found = 1;
                    goto yycheckname;
                }
            }
            goto yycheckname;
        case 'f':
            NC2();
            if(ISS("unction")){
                t->id = T_FUNCTION;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 'r':
            NC2();
            if(ISS("ead")){
                t->id = T_READ;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 'w':
            NC2();
            if(ISS("rite")){
                t->id = T_WRITE;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case '=':
            NC();
            t->id = T_EQ;
            return 0;
        case '<':
            NC();
            if(C == '>'){
                NC();
                t->id = T_NE;
                return 0;
            }
            else if(C == '='){
                t->id = T_LTOE;
                return 0;
            }
            else {
                t->id = T_LT;
                return 0;
            }
        case '>':
            NC();
            if(C == '='){
                t->id = T_GTOE;
                return 0;
            }
            else {
                t->id = T_GT;
                return 0;
            }
        case '-':
            NC();
            t->id = T_MINUS;
            return 0;
        case '*':
            NC();
            t->id = T_MULTIPLY;
            return 0;
        case '(':
            NC();
            t->id = T_BRA;
            return 0;
        case ')':
            NC();
            t->id = T_KET;
            return 0;
        case ';':
            NC();
            t->id = T_SEMI_COLLON;
            return 0;
        case ',':
            NC();
            t->id = T_COMMA;
            return 0;
        case ':':
            NC();
            if(C == '='){
                NC();
                t->id = T_ASSIGN;
                return 0;
            }
            else{
                //TODO:err
                fprintf(scanner->err,"*** line %d unexpected character '%c',expecting '='\n",scanner->line,C);
                return -1;
            }
        default:
            if(ISNAMEHEAD(C)){
                goto yyname;
            }
            else if(ISNUM(C)){
                int num = 0;
                while(ISNUM(C)){
                    num += C - '0';
                    NC();
                }
                t->id = T_NUM;
                t->u.i = num;
                return 0;
            }
            else{
                goto yyunexpected;
            }
    }
yycheckname:
    if(!found){
        goto yyunexpected;
    }
    else if(!ISNAME(C)){
        return 0;
    }
yyname:
    while(ISNAME(C)){
        NC2();
    }
    yyScanner_pushChar(scanner,'\0');
    t->id = T_ID;
    t->u.image = newString(&scanner->head,scanner->buf,scanner->len);
    return 0;
yyunexpected:
    //TODO: print error message
    fprintf(scanner->err,"*** line %d unexpected character '%c'\n",scanner->line,C);
    return -1;
}
