

%token <begin> "BEGIN"
%token <end> "END"
%token <integer> "INTEGER"
%token <if> "IF"
%token <then> "THEN"
%token <else> "ELSE"
%token <function> "FUNCTION"
%token <read> "READ"
%token <write> "WRITE"
%token <id> "ID"
%token <num> "NUM"
%token <=> "EQ"
%token <ne> "NE"
%token <ltoe> "LTOE"
%token <lt> "LT" 
%token <gtoe> "GTOE"
%token <gt> "GT"
%token <-> "MINUS"
%token <*> "MULTIPLY"
%token <:=> "ASSIGN"
%token <(> "BRA"
%token <)> "KET"
%token <eol> "SEMI_COLLON"
%token <,> "COMMA"


%%

program: subprogram;
// <分程序>→begin <说明语句表>；<执行语句表> end
subprogram: <begin> nameList <eol> execList <end>;
//<说明语句表>→<说明语句>│<说明语句表> ；<说明语句>
nameList: nameStatement | nameList nameStatement;
//<说明语句>→<变量说明>│<函数说明>
nameStatement: variableDecl <eol> | functionDecl <eol>;
//<变量说明>→integer <变量>
variableDecl: <integer> variable;
//<变量>→<标识符>
//<标识符>→<字母>│<标识符><字母>│ <标识符><数字>
//<字母>→a│b│c│d│e│f│g│h│i│j│k│l│m│n│o │p│q │r│s│t│u│v│w│x│y│z
//<数字>→0│1│2│3│4│5│6│7│8│9
variable: <id>;
//<函数说明>→integer function <标识符>（<参数>）；<函数体>
functionDecl: <integer> <function> <id> <(> argDefList <)> <eol> functionBody;
//<函数体>→begin <说明语句表>；<执行语句表> end
functionBody: <begin> nameList <eol> execList <end>;
//<执行语句表>→<执行语句>│<执行语句表>；<执行语句>
execList: execStatement | execList execStatement;
//<执行语句>→<读语句>│<写语句>│<赋值语句>│<条件语句>
execStatement: 
    readStatement <eol> | 
    writeStatement <eol> | 
    assignStatement <eol> | 
    conditionalStatement <eol> | 
    <begin> execList <end> |
    <eol>;
//<读语句>→read(<变量>)
readStatement: <read> <(> variable <)>;
//<写语句>→write(<变量>)
writeStatement: <write> <(> variable <)>;
//<赋值语句>→<变量>:=<算术表达式>
assignStatement: variable <:=> expr;
//<算术表达式>→<算术表达式>-<项>│<项>
expr: expr <-> item | item;
//<项>→<项>*<因子>│<因子>
item: item <*> factor | factor;
//<因子>→<变量>│<常数>│<函数调用>
factor: variable | consts | functionCall;
//<常数>→<无符号整数>
consts: unsignedNum;
//<无符号整数>→<数字>│<无符号整数><数字>
unsignedNum: <num> | unsignedNum <num>;
//<条件语句>→if<条件表达式>then<执行语句>else <执行语句>
conditionalStatement: <if> conditionalExpr <then> execList <else> execList;
//<条件表达式>→<算术表达式><关系运算符><算术表达式>
conditionalExpr: expr conditionOptr expr;
//<关系运算符> →<│<=│>│>=│=│<>
conditionOptr: <gt> | <lt> | <gtoe> | <ltoe> | <=> | <ne>;

functionCall: <id> <(> argList <)>;

argList: 
    nonEmptyArgList | /* empty */;
    
nonEmptyArgList:
    nonEmptyArgList <,> expr | expr;
    
argDefList:
    nonEmptyArgDefList | /* empty */;

nonEmptyArgDefList:
    nonEmptyArgDefList <,> variable | variable;
    
%%
