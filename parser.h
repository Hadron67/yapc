/*  
    YAPC - Yet Another Parser Compiler - An LR(1) parser generater

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

#ifndef __PARSER_H__
#define __PARSER_H__
#include <setjmp.h>
#include "grammar_builder.h"

typedef enum _ytoken_t{
    T_EOF = 1,
    T_NAME,
    T_STRING,
    T_TOKEN,
    T_TOKEN_DIR,
    T_ARROW,
    T_OR,
    T_EOL,
    T_SEPERATOR,
    T_BLOCK,
    T_TEST_DIR,
    T_EMPTY_DIR,
    T_PROLOGUE,
    T_TYPE_DIR,
    T_DATATYPE_DIR
}ytoken_t;

typedef struct _YToken{
    int line;
    ytoken_t id;
    size_t image;
}YToken;

typedef struct _YGParser{
    FILE *in,*err;
    
    char c;
    char *buf;
    int len,size;
    int line;

    YToken token;

    jmp_buf catcher;
    
    YGBuilder builder;

}YGParser;

int YGParser_init(YGParser *parser,FILE *err);
YGrammar *YGParser_parse(YGParser *parser,FILE *in);

#endif