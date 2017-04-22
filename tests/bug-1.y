/*
    note: this bug has been fixed.
    
    XXX:the program would be caught into a dead loop when parsing this grammar.
    
    reason: cannot merge two identical item sets if they have r/r internal conflict.

*/

%%

A: B A | ;

B:;

%%

