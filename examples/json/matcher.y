%{

#include "json.h"
#include "json_match.h"
        
%}
%token <[> "CBRA"
%token <]> "CKET"
%token <member> "MEMBER"
%token <num> "NUM"
%token <string> "STRING"

%type "jnode"
%namespace "jm"
%token_prefix "MT_"
%datatype "jMatch"

%%

start: 
    start <[> <num> <]> { jMatch_doNum(jmdata,&$3); }|
    start <[> <string> <]> { jMatch_doMember(jmdata,&$3); } |
    start <member> { jMatch_doMember(jmdata,&$2); } |
    /* empty */;
    

%%
