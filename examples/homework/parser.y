%token <begin> "BEGIN"//
%token <end> "END"//
%token <integer> "INTEGER"//
%token <if> "IF"//
%token <then> "THEN"//
%token <else> "ELSE"//
%token <function> "FUNCTION"//
%token <read> "READ"//
%token <write> "WRITE"//
%token <ID> "ID"//
%token <NUM> "NUM"//
%token <=> "EQ"//
%token <ne> "NE"//
%token <ltoe> "LTOE"//
%token <lt> "LT" //
%token <gtoe> "GTOE"//
%token <gt> "GT"//
%token <-> "MINUS"//
%token <*> "MULTIPLY"//
%token <:=> "ASSIGN"//
%token <(> "BRA"//
%token <)> "KET"//
%token <;> "SEMI_COLLON"//

%left <->
%left <*> 

%enable_cst

%%

//<程序>→<分程序>
program: subProgram;

//<分程序>→begin <说明语句表>；<执行语句表> end
subProgram: <begin> declList <;> stmtList <end>;

//<说明语句表>→<说明语句>│<说明语句表> ；<说明语句>
declList: declList <;> decl | decl;

//<说明语句>→<变量说明>│<函数说明>
decl: varDecl | funcDecl;

//<变量说明>→integer <变量>
varDecl: <integer> varID;

//<变量>→<标识符>
varID: <ID>;
//<标识符>→<字母>│<标识符><字母>│ <标识符><数字>
//<字母>→a│b│c│d│e│f│g│h│i│j│k│l│m│n│o │p│q │r│s│t│u│v│w│x│y│z
//<数字>→0│1│2│3│4│5│6│7│8│9

//<函数说明>→integer function <标识符>（<参数>）；<函数体>
funcDecl: <integer> <function> <ID> <(> argDefList <)> <;> funcBody;

//<参数>→<变量>
argDefList: varID;

//<函数体>→begin <说明语句表>；<执行语句表> end
funcBody: <begin> declList <;> stmtList <end>;

//<执行语句表>→<执行语句>│<执行语句表>；<执行语句>
stmtList: stmt | stmtList <;> stmt;

//<执行语句>→<读语句>│<写语句>│<赋值语句>│<条件语句>
stmt: readStmt | writeStmt | assignStmt | ifStmt;

//<读语句>→read(<变量>)
readStmt: <read> <(> varID <)>;

//<写语句>→write(<变量>)
writeStmt: <write> <(> varID <)>;

//<赋值语句>→<变量>:=<算术表达式>
assignStmt: varID <:=> expr;

//<算术表达式>→<算术表达式>-<项>│<项>
//<项>→<项>*<因子>│<因子>
//<因子>→<变量>│<常数>│<函数调用>
expr: expr <-> expr | expr <*> expr | constant | varID | funcCall;


//<常数>→<无符号整数>
constant: <NUM>;
//<无符号整数>→<数字>│<无符号整数><数字>
//<条件语句>→if<条件表达式>then<执行语句>else <执行语句>
ifStmt: <if> cond <then> stmt <else> stmt;

//<条件表达式>→<算术表达式><关系运算符><算术表达式>
cond: expr condOptr expr;

//<关系运算符> →<│<=│>│>=│=│<>
condOptr: <lt> | <gt> | <ltoe> | <gtoe> | <=> | <ne>;

funcCall: <ID> <(> expr <)>;

%%