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

#define T (parser->token)
#define NT() YGParser_scan(parser)
#define EXPECT(id) YGParser_expect(parser,(id))
#define STR(image) (parser->buf + (image))

static const char *tokenNames[] = { 
    NULL,"end of file","name","string","token",
    "%token","arrow","|",";","%%","{...}","%test","%empty","%{","%type" };
static int YGParser_scan(YGParser *parser);

static void YGParser_vaerr(YGParser *parser,const char *fmt,va_list args){
    fprintf(parser->err,"error: ");
    vfprintf(parser->err,fmt,args);
    fprintf(parser->err,"\n at line %d\n",parser->line);
    longjmp(parser->catcher,1);
    abort();
}

static void YGParser_err(YGParser *parser,const char *fmt,...){
    va_list args;
    va_start(args,fmt);
    YGParser_vaerr(parser,fmt,args);
    va_end(args);
    //never reached
    abort();
}
static void YGParser_pushChar(YGParser *parser,char c){
    if(parser->len >= parser->size){
        parser->size *= 2;
        parser->buf = (char *)ya_realloc(parser->buf,sizeof(char) * parser->size);
    }
    parser->buf[parser->len++] = c;
}
static void YGParser_popString(YGParser *parser){
    if(parser->len > 0){
        parser->len -= 2;
        while(parser->len > 0 && parser->buf[parser->len]){
            parser->len--;
        }
        if(parser->len > 0){
            parser->len++;
        }
    }
}
static void YGParser_expect(YGParser *parser,ytoken_t id){
    if(id != -1 && id != T.id){
        YGParser_err(parser,"unexpected token '%s',expecting '%s'",tokenNames[T.id],tokenNames[id]);
    }
}
static int YGParser_isBlock(YGParser *parser){
    while(C == ' ' || C == '\n' || C == '\t'){
        if(C == '\n'){
            parser->line++;
        }
        NC();
    }
    return parser->c == '{';
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
static int YGParser_scan(YGParser *parser){
    
    YToken *tk = &parser->token;
    size_t sptr = parser->len;
    restart:
    while(C == ' ' || C == '\n' || C == '\t'){
        if(C == '\n'){
            parser->line++;
        }
        NC();
    }
    if(YGParser_eof(parser)){
        tk->id = T_EOF;
        return 0;
    }
    tk->line = parser->line;

    switch(C){
        case '%':
            NC();
            if(C == 't'){
                NC();
                if(YGParser_isS(parser,"oken")){
                    if(C == '_'){
                        NC();
                        if(YGParser_isS(parser,"prefix")){
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
                if(YGParser_isS(parser,"atatype")){
                    tk->id = T_DATATYPE_DIR;
                    return 0;
                }
            }
            else if(C == 'e'){
                NC();
                if(YGParser_isS(parser,"mpty")){
                    tk->id = T_EMPTY_DIR;
                    return 0;
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
                    if(YGParser_eof(parser)){
                        YGParser_err(parser,"unexpected end of file:unclosed prologue block");
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
                tk->image = sptr;
                return 0;
            }
            YGParser_err(parser,"unexpected character:%c",C);
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
            YGParser_err(parser,"unexpected character:%c",C);
            break;
        case '<':
            NC();
            if(C == '>'){
                YGParser_err(parser,"empty tokens are not allowed");
            }
            while(C != '>'){
                if(YGParser_eof(parser)){
                    YGParser_err(parser,"unexpected end of file: unclosed token literal");
                }
                PUSHC(C);
                NC();
            }
            PUSHC('\0');
            NC();
            tk->id = T_TOKEN;
            tk->image = sptr;
            return 0;
        case '{':
            NC();
            int level = 1;
            while(level > 0){
                if(YGParser_eof(parser)){
                    YGParser_err(parser,"unexpected end of file: unclosed block");
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
            tk->image = sptr;
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
                    YGParser_err(parser,"unexpected end of file: unclosed string");
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
            tk->image = sptr;
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
                    }
                    else {
                        if(C == '\n'){
                            parser->line++;
                        }
                        NC();
                    }
                }
            }
            YGParser_err(parser,"unexpected character '%c' at start of comment",C);
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
                tk->image = sptr;
            }
            else {
                YGParser_err(parser,"unexpected character '%c'",C);
            }
    }
}

static void YGParser_file(YGParser *parser);
static void YGParser_options(YGParser *parser);
static void YGParser_rules(YGParser *parser);
static void YGParser_rule(YGParser *parser,size_t lhs);
static void YGParser_testList(YGParser *parser);


/**
    file() ->

    options() '%%' rules() '%%' EOF
*/
static void YGParser_file(YGParser *parser){
    YGParser_options(parser);
    EXPECT(T_SEPERATOR);
    NT();
    if(T.id == T_SEPERATOR){
        YGParser_err(parser,"empty grammars are not allowed.\n");
    }
    YGParser_rules(parser);
    EXPECT(T_SEPERATOR);
    NT();
    EXPECT(T_EOF);
    NT();
}

/** 
    options() ->
    [ <PROLOGUE> ]
    ( %token <TOKEN> <STRING> |
    %type <STRING> |
    %datatype <STRING> |
    %ns <STRING>
    %token_prefix <STRING> )*
*/
static void YGParser_options(YGParser *parser){
    if(T.id == T_PROLOGUE){
        YGBuilder_setPrologue(&parser->builder,STR(T.image));
        YGParser_popString(parser);
        NT();
    }
    while(T.id != T_SEPERATOR){
        switch(T.id){
            case T_TOKEN_DIR:
                NT();
                EXPECT(T_TOKEN);
                size_t tname = T.image;
                NT();
                EXPECT(T_STRING);
                size_t alias = T.image;
                if(YGBuilder_addToken(&parser->builder,STR(tname),STR(alias))){
                    YGParser_err(parser,"cannot redefine token '%s' or its alias '%s'",STR(tname),STR(alias));
                }
                YGParser_popString(parser);
                YGParser_popString(parser);
                NT();
                break;
            case T_TYPE_DIR:
                NT();
                EXPECT(T_STRING);
                YGBuilder_setTokenType(&parser->builder,STR(T.image));
                YGParser_popString(parser);
                NT();
                break;
            case T_DATATYPE_DIR:
                NT();
                EXPECT(T_STRING);
                YGBuilder_setDataType(&parser->builder,STR(T.image));
                YGParser_popString(parser);
                NT();
                break;
            case T_NS_DIR:
                NT();
                EXPECT(T_STRING);
                YGBuilder_setNameSpace(&parser->builder,STR(T.image));
                YGParser_popString(parser);
                NT();
                break;
            case T_TOKEN_PREFIX_DIR:
                NT();
                EXPECT(T_STRING);
                YGBuilder_setTokenPrefix(&parser->builder,STR(T.image));
                YGParser_popString(parser);
                NT();
                break;
            default:
                YGParser_err(parser,"unexpected token '%s'",tokenNames[T.id]);
        }
    }
}
/**
    rules() ->

    ( <NAME> '->' rule() ('|' rule())* ';' | testList() )*
*/
static void YGParser_rules(YGParser *parser){
    while(T.id != T_SEPERATOR){
        switch(T.id){
            case T_NAME:{
                size_t lhs;
                EXPECT(T_NAME);
                lhs = T.image;
                NT();
                EXPECT(T_ARROW);
                NT();

                YGParser_rule(parser,lhs);
                while(T.id == T_OR){
                    NT();
                    YGParser_rule(parser,lhs);
                }
                YGParser_popString(parser);//lhs
                EXPECT(T_EOL);
                NT();
                break;
            }
            case T_TEST_DIR:
                YGParser_testList(parser);
                break;
            default:
                YGParser_err(parser,"unexpected token '%d'",tokenNames[T.id]);
        }
        
    }
}
/**
    testList() ->

    '%test' (<TOKEN>)* ';'

*/
static void YGParser_testList(YGParser *parser){
    NT();
    while(T.id != T_EOL){
        EXPECT(T_TOKEN);
        if(YGBuilder_addTestToken(&parser->builder,STR(T.image))){
            YGParser_err(parser,"use of undefined token '%s' in test",STR(T.image));
        }
        YGParser_popString(parser);
        NT();
    }

    EXPECT(T_EOL);
    NT();
    YGBuilder_commitTest(&parser->builder);
}

/**
    rule() ->

    ( <TOKEN> | <NAME> | <BLOCK> )*
*/
static void YGParser_rule(YGParser *parser,size_t lhs){
    YGBuilder_prepareRule(&parser->builder,STR(lhs));
    while(T.id != T_OR && T.id != T_EOL){
        size_t name = T.image;
        int isTerminal;
        if(T.id == T_TOKEN){
            isTerminal = 1;
        }
        else if(T.id == T_NAME){
            isTerminal = 0;
        }
        else if(T.id == T_BLOCK){
            YGBuilder_addBlockItem(&parser->builder,STR(T.image),T.line);
            YGParser_popString(parser);
            NT();
            continue;
        }
        else {
            YGParser_err(parser,"unexpected token '%s',expected name or token",tokenNames[T.id]);
        }
        YGBuilder_addRuleItem(&parser->builder,STR(name),isTerminal);
        YGParser_popString(parser);
        NT();
    }
    YGBuilder_commitRule(&parser->builder);
}

int YGParser_init(YGParser *parser,FILE *err){
    memset(parser,0,sizeof(YGParser));
    YGBuilder_init(&parser->builder);
    parser->err = err;

    parser->size = 16;
    parser->buf = (char *)ya_malloc(sizeof(char) * parser->size);

    return 0;   
}

YGrammar *YGParser_parse(YGParser *parser,FILE *in){
    if(setjmp(parser->catcher)){
        goto err;
    }
    parser->line = 1;
    parser->in = in;
    parser->c = fgetc(in);
    NT();

    YGParser_file(parser);

    YGrammar *ret = YGBuilder_build(&parser->builder,parser->err);
    if(ret == NULL){
        goto err;
    }
    YGBuilder_free(&parser->builder,&ret->spool);
    ya_free(parser->buf);
    return ret;
err:
    YGBuilder_free(&parser->builder,NULL);
    ya_free(parser->buf);
    return NULL;
}
