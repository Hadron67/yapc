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

#include <stdarg.h>
#include <string.h>
#include "parser.h"


#define C (parser->c)
#define NC() (parser->c = fgetc(parser->in))
#define PUSHC(c) YGParser_pushChar(parser,(c))
#define ISS(s) (YGParser_isS(parser,(s)))
#define IEOF (YGParser_eof(parser))

static int YGParser_scan(YGParser *parser,YToken *tk);

static void YGParser_vaerr(YGParser *parser,int line,const char *fmt,va_list args){
    fprintf(parser->err,"error: ");
    vfprintf(parser->err,fmt,args);
    fprintf(parser->err,"\n at line %d\n",line);
}

static void YGParser_err(YGParser *parser,int line,const char *fmt,...){
    va_list args;
    va_start(args,fmt);
    YGParser_vaerr(parser,line,fmt,args);
    va_end(args);
}
static void YGParser_pushChar(YGParser *parser,char c){
    if(parser->len >= parser->size){
        parser->size *= 2;
        parser->buf = (char *)ya_realloc(parser->buf,sizeof(char) * parser->size);
    }
    parser->buf[parser->len++] = c;
}

static int YGParser_eof(YGParser *parser){
    return feof(parser->in);
}
static int YGParser_isS(YGParser *parser,const char *s){
    while(*s){
        if(*s != C){
            return 0;
        }
        NC();
        s++;
    }
    return 1;
}
static int YGParser_scan(YGParser *parser,YToken *tk){
    
    //YToken *tk = &parser->token;
    restart:
    parser->len = 0;
    while(C == ' ' || C == '\n' || C == '\t'){
        if(C == '\n'){
            parser->line++;
        }
        NC();
    }
    if(IEOF){
        tk->id = T_EOF;
        return 0;
    }
    tk->line = parser->line;

    switch(C){
        case '%':
            NC();
            if(C == 't'){
                NC();
                if(ISS("oken")){
                    if(C == '_'){
                        NC();
                        if(ISS("prefix")){
                            tk->id = T_TOKEN_PREFIX_DIR;
                            return 0;
                        }
                    }
                    else {
                        tk->id = T_TOKEN_DIR;
                        return 0;
                    }
                }
                else if(C == 'e'){
                    NC();
                    if(C == 's'){
                        NC();
                        if(C == 't'){
                            NC();
                            tk->id = T_TEST_DIR;
                            return 0;
                        }
                    }
                }
                else if(C == 'y'){
                    NC();
                    if(C == 'p'){
                        NC();
                        if(C == 'e'){
                            NC();
                            tk->id = T_TYPE_DIR;
                            return 0;
                        }
                    }
                }
            }
            else if(C == 'd'){
                NC();
                if(ISS("atatype")){
                    tk->id = T_DATATYPE_DIR;
                    return 0;
                }
            }
            else if(C == 'e'){
                NC();
                if(C == 'm'){
                    NC();
                    if(ISS("pty")){
                        tk->id = T_EMPTY_DIR;
                        return 0;
                    }
                }
                else if(C == 'n'){
                    NC();
                    if(ISS("able_cst")){
                        tk->id = T_ENABLE_CST_DIR;
                        return 0;
                    }
                }
            }
            else if(C == 'n'){
                NC();
                if(C == 's'){
                    NC();
                    tk->id = T_NS_DIR;
                    return 0;
                }
            }
            else if(C == '%'){
                NC();
                tk->id = T_SEPERATOR;
                return 0;
            }
            else if(C == '{'){
                NC();
                while(1){
                    if(C == '\n'){
                        parser->line++;
                    }
                    if(IEOF){
                        YGParser_err(parser,parser->line,"unexpected end of file:unclosed prologue block");
                    }
                    else if(C == '%'){
                        NC();
                        if(C == '}'){
                            NC();
                            break;
                        }
                        else {
                            PUSHC('%');
                            PUSHC(C);
                        }
                    }
                    else{
                        PUSHC(C);
                    }
                    NC();
                }
                PUSHC('\0');
                tk->id = T_PROLOGUE;
                tk->image = YGBuilder_addString(&parser->builder,parser->buf);
                return 0;
            }
            YGParser_err(parser,parser->line,"unexpected character:%c",C);
            break;
        case ':':
            NC();
            tk->id = T_ARROW;
            return 0;
        case '-':
            NC();
            if(C == '>'){
                NC();
                tk->id = T_ARROW;
                return 0;
            }
            YGParser_err(parser,parser->line,"unexpected character:%c",C);
            break;
        case '<':
            NC();
            if(C == '>'){
                YGParser_err(parser,parser->line,"empty tokens are not allowed");
            }
            while(C != '>'){
                if(IEOF){
                    YGParser_err(parser,parser->line,"unexpected end of file: unclosed token literal");
                }
                PUSHC(C);
                NC();
            }
            PUSHC('\0');
            NC();
            tk->id = T_TOKEN;
            tk->image = YGBuilder_addString(&parser->builder,parser->buf);
            return 0;
        case '{':
            NC();
            int level = 1;
            while(level > 0){
                if(YGParser_eof(parser)){
                    YGParser_err(parser,parser->line,"unexpected end of file: unclosed block");
                }
                else if(C == '{'){
                    level++;
                }
                else if(C == '}'){
                    level--;
                }
                if(C == '\n'){
                    parser->line++;
                }
                PUSHC(C);
                NC();
            }
            parser->len--;
            PUSHC('\0');
            tk->id = T_BLOCK;
            tk->image = YGBuilder_addString(&parser->builder,parser->buf);
            return 0;
        case '|':
            NC();
            tk->id = T_OR;
            return 0;
        case ';':
            NC();
            tk->id = T_EOL;
            return 0;
        case '"':
            NC();
            while(C != '"'){
                if(feof(parser->in)){
                    YGParser_err(parser,parser->line,"unexpected end of file: unclosed string");
                }
                if(C == '\n'){
                    parser->line++;
                }
                PUSHC(C);
                NC();
            }
            PUSHC('\0');
            NC();
            tk->id = T_STRING;
            tk->image = YGBuilder_addString(&parser->builder,parser->buf);
            return 0;
        case '/':
            NC();
            if(C == '/'){
                NC();
                while(C != '\n'){
                    NC();
                }
                NC();
                parser->line++;
                goto restart;
            }
            else if(C == '*'){
                NC();
                while(1){
                    if(C == '*'){
                        NC();
                        if(C == '/'){
                            NC();
                            goto restart;
                        }
                        else if(C == '\n'){
                            parser->line++;
                        }
                    }
                    else {
                        if(C == '\n'){
                            parser->line++;
                        }
                        NC();
                    }
                }
            }
            goto yyunexpected;
        default:
            if(C >= 'a' && C <= 'z' || C >= 'A' && C <= 'Z' || C == '_' || C == '$'){
                PUSHC(C);
                NC();
                while(C >= 'a' && C <= 'z' || 
                    C >= 'A' && C <= 'Z' || 
                    C == '_' || 
                    C == '$' ||
                    C >= '0' && C <= '9'){
                        PUSHC(C);
                        NC();
                }
                PUSHC('\0');
                tk->id = T_NAME;
                tk->image = YGBuilder_addString(&parser->builder,parser->buf);
                return 0;
            }
            else {
                goto yyunexpected;
            }
    }
yyunexpected:
    YGParser_err(parser,parser->line,"unexpected character '%c'",C);
    return -1;
}


int YGParser_init(YGParser *parser,FILE *err){
    memset(parser,0,sizeof(YGParser));
    YGBuilder_init(&parser->builder,err);
    parser->err = err;

    parser->size = 16;
    parser->buf = (char *)ya_malloc(sizeof(char) * parser->size);

    yyParser_init(&parser->parser);
    parser->parser.userData = &parser->builder;

    return 0;   
}

YGrammar *YGParser_parse(YGParser *parser,FILE *in){
    parser->line = 1;
    parser->in = in;
    parser->c = fgetc(in);
    YToken *tk = &parser->parser.token;

    while(!parser->parser.done){
        if(YGParser_scan(parser,tk)){
            goto err;
        }
        if(yyParser_acceptToken(&parser->parser,tk->id) != YY_OK){
            yyParser_printError(&parser->parser,parser->err);
            YGParser_err(parser,tk->line,"syntax error detected");
            goto err;
        }
        if(parser->builder.status){
            YGParser_err(parser,tk->line,"semantic error detected");
            goto err;
        }
    }

    YGrammar *ret = YGBuilder_build(&parser->builder,parser->err);
    if(ret == NULL){
        YGParser_err(parser,0,"semantic error detected while building grammar");
        goto err;
    }
    YGBuilder_free(&parser->builder,&ret->spool);
    yyParser_free(&parser->parser);
    ya_free(parser->buf);
    return ret;
err:
    YGBuilder_free(&parser->builder,NULL);
    yyParser_free(&parser->parser);
    ya_free(parser->buf);
    return NULL;
}
