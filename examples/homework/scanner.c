#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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

    ",",
    "+",
    "/",
    ":"
};

#define ymalloc malloc
#define yrealloc realloc
#define yfree free

#define C (scanner->c)
#define NC() (yyScanner_getChar(scanner))
#define NC2() (yyScanner_pushChar(scanner,C),yyScanner_getChar(scanner))
#define IEOF (feof(scanner->in))
#define ISS(s) (yyScanner_isS(scanner,(s)))

#define ISNUM(c) ((c) >= '0' && (c) <= '9')
#define ISNAMEHEAD(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') ||(c) == '_' || (c) == '$' )
#define ISNAME(c) (ISNAMEHEAD(c) || ISNUM(c))

static char *newString(yyStringBlock **prev,const char *s,int len){
    static const int isize = 128;
    yyStringBlock *yb = *prev;
    if(yb == NULL || len > yb->size - yb->len){
        int si = len > isize ? len : isize;
        yyStringBlock *b = (yyStringBlock *)ymalloc(sizeof(yyStringBlock) + si * sizeof(char));
        b->size = si;
        b->len = 0;
        b->next = *prev;
        yb = *prev = b;
    }
    char *ret = yb->data + yb->len;
    strncpy(ret,s,len);
    yb->len += len;
    return ret;
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
            else if(C == 'n'){
                NC2();
                if(C == 't'){
                    NC2();
                    if(ISS("eger")){
                        t->id = T_INTEGER;
                        found = 1;
                        goto yycheckname;
                    }
                }
            }
            goto yycheckname;
        case 't'://then,to,type
            NC2();
            if(C == 'h'){
                NC2();
                if(ISS("en")){
                    t->id = T_THEN;
                    found = 1;
                    goto yycheckname;
                }
            }
            goto yycheckname;
        case 'e'://end,else
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
        case 'f'://function,for,file
            NC2();
            if(C == 'u'){
                NC2();
                if(ISS("nction")){
                    t->id = T_FUNCTION;
                    found = 1;
                    goto yycheckname;
                }
            }
            goto yycheckname;
        case 'r'://read
            NC2();
            if(C == 'e'){
                NC2();
                if(C == 'a'){
                    NC2();
                    if(C == 'd'){
                        NC2();
                        t->id = T_READ;
                        found = 1;
                        goto yycheckname;
                    }
                }
            }
            goto yycheckname;
        case 'w'://write,while,with
            NC2();
            if(C == 'r'){
                NC2();
                if(ISS("ite")){
                    t->id = T_WRITE;
                    found = 1;
                    goto yycheckname;
                }
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
    t->u.image.s = newString(&scanner->head,scanner->buf,scanner->len);
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
