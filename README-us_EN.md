# YAPC

[中文文档](README.md)

Yet Another Parser Compiler

## Features

* Use [hanolee algorithm](http://david.tribble.com/text/honalee.html) to generate LR(1) parse table.It is a stronger parse algroithm than LALR(1),it can handle all LR(1) grammars,with its parse table size the same as LALR(1) if this grammar is LALR(1).

* Unlike yacc will generate some global variables,yapc store all the variables in a structure block,like a class of c++.This allows **multiple parsers running at once**,despites single thread or multiple thread.

* Simple APIs makes it easier to embed yapc in another programme(if necesary).

# Compile and install
You can use the following command to compile and install yapc:
```shell
cmake .
make
sudo make install
```
this requires cmake2.8 or latter.There is no more dependences on other libraries.

# Documentation for YAPC
## Usage
yapc requires one argument as input file:
```shell
yapc example.y
```
three files will be generated,they are example.c,example.h,example.output.the first two files are source for the parser,the last one contains informations for of result,which will be mentioned latter.This output can be --output,for example:
```shell
yapc example.y --output test.txt
```
will generate
example.c,example.h,test.txt.where test.txt is identical to example.output.

## Input file
The syntax of the file is simmilar to yacc,with some simplyfies.Which means some functions is not present yet(i'm the only contributer,after all).
Following is the file's structure:
```yacc
%{

Prologue

%}

options and token definations

%%

Grammar definations

%%
```
These will be explained below.
## Prologue
This section would be copied to  the beginning of the generated .c source file without modification.So declarations such as #include can be written here.

## Options and token defination
The syntax for token defination is
```
%token <token name> "token alias"
```
token name can contains any character except ">",it is used to define to terminals in the grammar defination.
token alias **must be a valid identifier**.Because token alias is used in the tokenizer to tell the parser what token it has seen,so for each token yapc will define a macro with its alias.For example:
```yacc
%token <+> "PLUS"
```
Will generate a macro:
```c
#define T_PLUS 1
```
Therefore if token alias isn't a valid identifier,even though yapc won't report an error,error will occur when compile the generated parser source.

### Token type defination
The syntax is
```
%type "token type"
```
tokens will be stored in the semantic stack of the parser so that they can be accessed by the semantic actions(see below),and user can use this command to define the type of the stored token.This type is "int" by default.

### User data type defination
When writting a parser it could be necesary to access some other objects or data structures in the semantic actions(see below),this can be done by assigning the required object to the "void \*userData" field of the parser. Before the semantic action blocks it will be assigned to another local variable called "yydata",and can be cast to other types,making it more convenient to use.You can use %datatype to define what type it will be cast to.Syntax is
```yacc
%datatype "type"
```
By default,this type is "void".

### Name space defination
When generating the source code,all the names of the variables,structures,functions are all have a prefix,in order not to introduce name space pollution to your source code.By default,this prefix is "yy".You can change it using the %ns directive:
```yacc
%ns "name space name"
```

### Token prefix
Similar to name spaces,when defining macros for every token,a prefix is added to the macro names,it is "T_" by default.You can change it by:
```yacc
%token_prefix "prefix"
```

### Printing concrete syntax tree
It could sometimes be useful to print out the concrete syntax tree of the input stream.To enable it,use the following directive:
```
%enable_cst
```
Then yapc will generate a structure(yyCst) of the tree node and a function that prints the tree:
```c
int yyParser_printCst(yyParser *parser,FILE *out);
```
Just invoke it when an input sequence is accepted.

## Grammar defination
Grammar defination is simmilar to that of yacc,except that all the terminals should be surrounded by "<>".For example,in yacc,we write:
```
%token T_NUM
...
A: T_NUM;
```
In yapc,it should be written as
```
%token <num> "T_NUM"
...
A: <num>;
```
Where the name in "<>" can be any string.

### Semantic action
Similar to yacc,semantic actions can appear at the and or middle of each production rule.Each terminal and non terminal has a semantic value,in the action block,"$$" is the value of this rule,you can set it by assign value to it;$n denotes the value of the n-th element(terminal or non terminal).The type of the semantic value it defined by %type at before.

### Test directive
Test directives can be used to test the parsing proccess of a given token sequence.When a test directive is used,yapc will print the parsing steps of the sequence,and error message of detected.This makes it easier to test and debug the grammar.
Test directives can appear at grammar defination section,but not at the middle of a production rule.
Usage:
``` yacc
%test ( <token> )* ;
```
Where "\*" indicates an arbitary number of tokens.

## Conflicts and resolution
If conflicts are detected yapc will print informations of the conflicts and resolution to the terminal and .output file.When a shift/reduce conflict is encountered yapc will chose to shift,as for reduce/reduce conflicts,yapc will accept the production rule which was written earlier.You can also resolve conflicts using operator precedence,see below.

### Operator precedence
Operator precedence was added since v0.3,and is similar to that of yacc.You can use operator precedence directives %left,%right,%nonassoc in the options block to define the associatives and precedence levels of the operators to resolve the conflicts.The syntax is
```yacc
%left (<token>)*
%right (<token>)*
%nonassoc (<token>)*
```
The operators in one direcive have the same precedence level,and among different directives,the one appears **latter** has higher level.The precedence level of a rule is the level of the last token appears in the production rule.Therefore,when a shift/reduce conflict is encountered,the one with higher precedence level will be chosen.

You can also define the precedence levels of the rules manually.To do this,add following to the rule:
```
%prec <token> offset(optional)
```
Then the level of this rule will be assigned to the level of the given operator.What's more,you can add offset values to the level,just follow a number to it,then the level is the token plus the given number.

For more specified examples,see tests/optr_test.y or examples/calculator.

## Usage of generated code
yapc will generate some functions and a structure type yyParser.yapc is simmilar to lemon when generate source code,which means in yapc,the tokenizer calls the parser.Basic usage is:when every token is recognized by the tokenizer,call yyParser\_acceptToken to shift that token,this function returns -1 if a syntax error is detected,when that happens,call yyParser\_printError to print the error message.And when the input sequence is accepted,the "done" field will be set to 1.

### int yyParser_init(yyParser \*yyparser);
* Function
Used to initialize a parser object,similar to constructor.When a yyParser is declared,invoke this function to initialize it.
* Parameters
yyparser:The parser object to be initialized.
* Returns
Always YY_OK.

### int yyParser_reInit(yyParser \*yyparser);
* Function
Used to initialize a parser again.When a parsing session is finished,if you want it to parser another input sequence,you'll need to call this function first.
* Parameters
yyparser:The parser to be reinitialized.
* Returns
Always YY_OK.

### int yyParser_free(yyParser \*yyparser);
* Function
Used to destroy a parser object,similar to destructors.When you no longer need a parser,call this function to release memory,or leak will occur.
* Parameters
yyparser:The parser object to be destroyed.
* Returns
Always YY_OK

### int yyParser_acceptToken(yyParser \*yyparser,int yytokenid);
* Function
This is the core of the generated code.When a token is recognized by the tokenizer,call this function to pass that token to the parser to proccess.
* Parameters
yyparser:The parser object;
yytokenid:The id of the token recognized by the tokenizer,ids for each token is defined in parser.h
* Returns
YY_OK:normal;
YY_ERR:syntax error detected.

### int yyParser_printError(yyParser \*yyparser,FILE \*out);
* Function
Print error message when a syntax error is encountered.
* Parameters
yyparser:The parser.
out:output.

### int yyParser_printCst(yyParser \*parser,FILE \*out);
* Functions
Print the generated concrete syntax tree.This function will only be generated when the %enable_cst directive is present.
* Parameters
parser:The parser;
out:output of the printing.
* Returns
Always YY_OK。

### Members in yyParser that user can use
* int yyParser.done:Initially it is 0,when the input is accepted by the parser,it will be set to 1.
* void *yyParser.userData:User data,use can access other data structures in the sementic blocks through it.
* yyParser.token:The semantic value,which will be pushed into the semantic stack and accessed by the code in the sementic blocks when yyParser_acceptToken is called.The type of this member is mensioned [before](#Token type defination).

For more concrete examples of using the parser,see examples.

### The .output file
the .output file consists three sections:conflict informations of the grammar,the generated LR(1) token sets and state table,test output.

## API
If you want to use yapc in another programme,you can invoke the APIs.A basic parser generater is like this:
```c
#include "yapc.h"
...
//create input and output file
FILE *input = fopen("example.y","ro");
FILE *source = fopen("example.c","wo");
FILE *header = fopen("example.h","wo");
FILE *out = fopen("example.output","wo");
//yNewContext is used to create a new context.
void *ctx = yNewContext();
//parse grammar file,this function will return -1 if there's an error.
if(yParseFile(ctx,input,"example.y",stderr)){
    ...
    //generate parse table
    yGenerateTable(ctx);
    //print conflicts and warnings to the output file
    yPrintGenerationWarnings(ctx,out);
    //print conflicts and warnings to the terminal
    yPrintGenerationWarnings(ctx,stdout);
    //print LR(1) item sets to the output file
    yPrintResult(ctx,out);
    //print test result to the output file
    yPrintTestResult(ctx,out);
    //generate parser source code
    yGenerateCCode(ctx,header,source,"header.h","source.c");
    ...
}
else{
    fprintf(stderr,"error while parse grammar file.\n");
    ...
}

...
//release the context
yDestroyContext(ctx);
```
# Bug
Please submit bugs on the issues page if you found!

# Change logs
## v0.3,2017-4-30(latest)
* Operator precedence added.(see [documentation](#Operator precedence))
* Modified the usage of generated code section of the documentation.
* Changed the namespace directive from %ns to %namespace.

## v0.2,2017-4-29
* Added printing concrete syntax tree(see [above](#Printing concrete syntax tree)).
* Changed the grammar file parser used by yapc from hand-written recursive desent parser to the parser generated by yapc(grammar file is yparser.y).

## v0.1.2,2017-4-28
* Fixed the bug that will cause segmentation fault occasionally in the testing module.
* Fixed the bug that generates wrong parsing table due to merging item sets improperly.(see tests/bug-2.y)
* Now the warnings for the conflicts will contain the corresponding tokens.
* Tokens that are not used in the grammar will be reported now.

## v0.1.1,2017-4-26
* Fixed some bugs in examples/json.
* Rearranged part of the code.
* Fixed some errors in the documentation.
* Now a comment "generated by yapc" will be inserted to the beginning of the generated file.

## v0.1-rc，2017-4-23
* The first version

# License
YAPC is licenced under GPL v3
