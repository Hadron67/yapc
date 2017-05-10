#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "ydef.h"
#include "scanner.h"
#include "parser.h"

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

    ",",
    "+",
    "/",
    ":"
};

#define C (scanner->c)
#define NC() (yyScanner_getChar(scanner))
#define NC2() (yyScanner_pushChar(scanner,C),yyScanner_getChar(scanner))
#define IEOF (feof(scanner->in))
#define ISS(s) (yyScanner_isS(scanner,(s)))

#define ISNUM(c) ((c) >= '0' && (c) <= '9')
#define ISNAMEHEAD(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') ||(c) == '_' || (c) == '$' )
#define ISNAME(c) (ISNAMEHEAD(c) || ISNUM(c))

static int yyScanner_vaerr(yyScanner *s,const char *fmt,va_list args){
    if(s->err != NULL){
        fprintf(s->err," *** lexical error: %d:",s->line);
        vfprintf(s->err,fmt,args);
        fprintf(s->err,"\n");
    }
    return 0;
}

static int yyScanner_err(yyScanner *s,const char *fmt,...){
    va_list args;
    va_start(args,fmt);
    yyScanner_vaerr(s,fmt,args);
    va_end(args);
    return 0;
}

int yyScanner_init(yyScanner *s,FILE *in,FILE *err){
    YSPool_init(&s->pool,64,16,NULL);
    s->len = 0;
    s->size = 16;
    s->buf = (char *)ya_malloc(NULL,sizeof(char) * s->size);
    s->line = 1;
    s->first = 1;
    
    s->in = in;
    s->err = err;
    return 0;
}
int yyScanner_free(yyScanner *s,char **buf){
    ya_free(NULL,s->buf);
    YSPool_free(&s->pool,buf);
    
    return 0;
}
int yyScanner_getChar(yyScanner *s){
    s->c = fgetc(s->in);
    if(s->c >= 'A' && s->c <= 'Z'){
        s->c += 'a' - 'A';
    }
    return 0;
}
int yyScanner_pushChar(yyScanner *s,char c){
    if(s->len >= s->size){
        s->size *= 2;
        s->buf = (char *)ya_realloc(NULL,s->buf,sizeof(char) * s->size);
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
int yyScanner_next(yyScanner *scanner,yynode *t){
    if(scanner->first){
        scanner->first = 0;
        NC();
    }
    t->lineCount = 0;
    restart:
    while(C == ' ' || C == '\n' || C == '\t'){
        if(C == '\n'){
            scanner->line++;
            t->lineCount++;
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
        case 'b'://begin
            NC2();
            if(ISS("egin")){
                t->id = T_BEGIN;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 'i'://if,integer,in
            NC2();
            if(C == 'f'){
                NC2();
                t->id = T_IF;
                found = 1;
                goto yycheckname;
            }
            else if(ISS("nteger")){
                t->id = T_INTEGER;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 't'://then,to,type
            NC2();
            if(ISS("hen")){
                t->id = T_THEN;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 'e'://end,else
            NC2();
            if(ISS("nd")){
                t->id = T_END;
                found = 1;
                goto yycheckname;
            }
            else if(ISS("lse")){
                t->id = T_ELSE;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 'f'://function
            NC2();
            if(ISS("unction")){
                t->id = T_FUNCTION;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 'r'://read
            NC2();
            if(ISS("ead")){
                t->id = T_READ;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 'w'://write,while,with
            NC2();
            if(ISS("rite")){
                t->id = T_WRITE;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case '_':
            NC2();
            if(ISS("_halt_compiler")){
                t->id = T_HALT_COMPILER;
                found = 1;
                goto yycheckname;
            }
            goto yycheckname;
        case 'm':
            NC2();
            if(ISS("od")){
                t->id = T_MOD;
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
                NC();
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
        case ',':
            NC();
            t->id = T_COMMA;
            return 0;
        case '(':
            NC();
            if(C == '*'){
                NC();
                while(1){
                    if(C == '*'){
                        NC();
                        if(C == ')'){
                            NC();
                            goto restart;
                        }
                        else if(IEOF){
                            t->id = T_EOF;
                            return 0;
                        }
                        else if(C == '\n'){
                            NC();
                            scanner->line++;
                            t->lineCount++;
                        }
                    }
                    else if(C == '\n'){
                        NC();
                        scanner->line++;
                        t->lineCount++;
                    }
                    else if(IEOF){
                        t->id = T_EOF;
                        return 0;
                    }
                    else{
                        NC();
                    }
                }
            }
            else{
                t->id = T_BRA;
                return 0;
            }
        case ')':
            NC();
            t->id = T_KET;
            return 0;
        case '{':
            NC();
            while(C != '}'){
                if(IEOF){
                    t->id = T_EOF;
                    return 0;
                }
                else if(C == '\n'){
                    NC();
                    scanner->line++;
                    t->lineCount++;
                }
                else if(C == '}'){
                    NC();
                    goto restart;
                }
                else{
                    NC();
                }
            }
        case ';':
            NC();
            t->id = T_SEMI_COLLON;
            return 0;
        case '+':
            NC();
            t->id = T_PLUS;
            return 0;
        case '/':
            NC();
            t->id = T_DIVIDE;
            return 0;
        case ':':
            NC();
            if(C == '='){
                NC();
                t->id = T_ASSIGN;
                return 0;
            }
            else{
                goto yyunexpected;
                return 0;
            }
        default:
            if(ISNAMEHEAD(C)){
                NC2();
                goto yyname;
            }
            else if(ISNUM(C)){
                int num = 0;
                while(ISNUM(C)){
                    num *= 10;
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
    if(found && !ISNAME(C)){
        return 0;
    }
yyname:
    while(ISNAME(C)){
        NC2();
    }
    yyScanner_pushChar(scanner,'\0');
    t->id = T_ID;
    t->u.image.s = YSPool_addString(&scanner->pool,scanner->buf);
    t->u.image.len = scanner->len - 1;
    return 0;
yyunexpected:
    //TODO: print error message
    if(IEOF){
        yyScanner_err(scanner,"unexpected end of file");
    }
    else{
        yyScanner_err(scanner,"unexpected character '%c'",C);
    }
    NC();
    return -1;
}
