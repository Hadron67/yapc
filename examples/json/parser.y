%{

#include "json.h"

#define YYDESTRUCTOR(a) JSON_free((a))

%}

%token <string> "STRING"
%token <num> "NUM"
%token <true> "TRUE"
%token <false> "FALSE"
%token <null> "NULL"
%token <undefined> "UNDEFINED"

%token <(> "BRA"
%token <)> "KET"
%token <[> "CBRA"
%token <]> "CKET"
%token <{> "BBRA"
%token <}> "BKET"
%token <:> "COLLON"
%token <,> "COMMA"

%type "jsonval"

%%

json: 
    <{> obj <}> { $$ = $2; }
|   <[> array <]> { $$ = $2; }
|   primitives
;
    
obj: nonEmptyObj possibleComma | /* empty */ { JSON_init(&$$,JTYPE_OBJ); }; 
nonEmptyObj: 
    nonEmptyObj <,> <string> <:> json { jsonObj_add(&$1,$3.u.stringv.s,$3.u.stringv.length,&$5);$$ = $1; }
|   <string> <:> json { JSON_init(&$$,JTYPE_OBJ);jsonObj_add(&$$,$1.u.stringv.s,$1.u.stringv.length,&$3); } 
;
    

array:nonEmptyArray possibleComma | /* empty */ { JSON_init(&$$,JTYPE_ARRAY); } ;

nonEmptyArray:
    nonEmptyArray <,> json { jsonArray_push(&$1,&$3);$$ = $1; }
|   json { JSON_init(&$$,JTYPE_ARRAY);jsonArray_push(&$$,&$1); } 
;

primitives: <string> | <num> | <true> | <false> | <null> | <undefined>;

possibleComma: <,> | /* empty */;

%%


