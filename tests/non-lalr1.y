/**
    this grammar is LR(1),but not LALR(1),so that it cannot be handled by pure LALR(1) parsers.
    but Honalee algorithm can.
*/

%token  <a> "A"
%token  <b> "B"
%token  <c> "C"
%token  <d> "D"
%token  <e> "E"

%%

S : <a> A <d>;
S : <a> B <e>;
S : <b> A <e>;
S : <b> B <d>;
A : <c>;
B : <c>;

%test <a><c><d>;
%test <b><c><e>;

%%
