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

#ifndef __PARSER_H__
#define __PARSER_H__

#include "spool.h"
#include "grammar_builder.h"
#include "yparser.h"

typedef struct _YGParser{
    FILE *in,*err;
    yheap_t *heap;
    char c;
    char *buf;
    int len,size;
    int line;

    YToken token;

    YGBuilder builder;
    yyParser parser;
}YGParser;

int YGParser_init(YGParser *parser,FILE *err,yheap_t *heap);
YGrammar *YGParser_parse(YGParser *parser,FILE *in);

#endif