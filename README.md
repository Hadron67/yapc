# YAPC

[English Documentation](README-us_EN.md)

另一个解析器生成器

## 特点
* 使用[hanolee算法](http://david.tribble.com/text/honalee.html)生成LR(1)分析表，这是一种比LALR(1)更强的解析算法，能够解析所有的LR(1)型文法，而当文法是LALR(1)时生成的状态表的大小和LALR(1)相当。

* 不像yacc会生成一些全局变量，yapc是将所有需要的变量保存在一个结构体中的，就像c++的类一样，这就允许了**多个解析器同时运行**，不管是单线程还是多线程。

* 简单易用的API，使你可以在其他程序中方便地调用yapc。

***

## 编译及安装
用以下命令即可编译
```shell
cmake .
make
```
编译需要使用cmake2.8及以上。除此之外没有其他库的依赖。

# YAPC的文档
## 用法
yapc需要一个参数作为输入文件：
```shell
yapc example.y
```
执行后三个文件会被生成，分别是example.c，example.h，example.output。其中前两个是生成的解析器源码，最后一个为输出的其他信息，下面会继续提到。第三个.output文件可以用--output参数单独指定，如
```shell
yapc example.y --output test.txt
```
将生成
example.c，example.h，test.txt。其中test.txt和example.output内容完全一样。

## 输入文件的语法
文件的语法和yacc语法相似，只不过做了许多简化，也就是说，有些功能还没有（毕竟只有我一个人在写啊）。
文件的结构如下：
```
%{

前部

%}

选项及关键字（token）定义

%%

文法定义

%%
```
下面一一解释这些部分。
## 前部
这部分代码会被原封不动地复制到生成的.c文件的最前部。一般会将一些#include等指令放在这里。

## 选项及关键字定义
关键字定义的格式为
```
%token <关键字名> "关键字的别名"
```
其中关键字名可以是由任何除了">"的字符组成，是用于在文法定义里面的终结符（Terminal）的。
关键字别名**必须是合法的标识符**。因为别名是解析器和词法扫描器之间通信用的，在生成.h文件时会对每个关键词定义相应的宏，如
```yacc
%token <+> "PLUS"
```
会生成如下的宏：
```c
#define T_PLUS 1
```
因此如果别名不是标识符，虽然yapc不会报错，但在编译解析器时就会出错。

### 关键字类型定义
格式为
```
%type "类型"
```
关键字以被定义的类型的方式保存在解析器的语法分析栈中用于语义分析。这个类型默认是int。

### 用户数据类型定义
在实际编写解析器时可能还需要在动作块（见下）中访问用户定义的一些数据结构，这可以通过生成的解析器中的void *userData传入，在动作块中它会被定义成一个叫做yydata的局部变量，并可以被强转成其他类型方便使用。可以通过%datatype指令来指定强转的类型。格式如下
``` yacc
%datatype "类型"
```
这个类型默认是void，即不执行强转。

### 名字空间定义
在生成解析器代码的时候，所有的变量名、结构体名、函数名都加上了前缀以防止对你的命名空间的污染。这个前缀默认是“yy”。你可以用一下指令自定义这个前缀：
```yacc
%ns "命名空间名"
```
### 关键字前缀
与名字空间类似，在对所有关键字定义宏的时候都会给宏名加前缀，默认是“T_”。同样，用以下指令可修改：
```yacc
%token_prefix "前缀名"
```

### 生成分析树
生成分析树（具体语法树）更多的是为了把它打印出来。如果想让yapc在解析的过程中同时生成分析树，只需加上如下指令：
```
%enable_cst
```
这样，yapc就会生成分析树节点的数据类型（yyCst）以及用于打印分析树的函数
```c
int yyParser_printCst(yyParser **parser,FILE *out);
```
只需在完成对输入串的分析后调用即可打印。

## 文法定义
文法定义的格式和yacc相似，只不过所有的终结符（关键字）都要用尖括号"<>"括起来。如在yacc中的定义
```
%token T_NUM
...
A: T_NUM;
```
应写成
```
%token <num> "T_NUM"
...
A: <num>;
```
其中尖括号里面定义的关键字名可以是任何名称。

### 语义动作
与yacc类似，在每个文法规则最后及中间都可以定义动作块，用花括号“{}”括起来。每个终结符和非终结符都有一个语义值。在动作内部使用“$$”代表这条规则的语义值，$n代表这条规则中第n个元素（终结符或非终结符）的值。语义值的类型就是之前用%type定义的。

### 测试指令
测试指令可用于测试对一个关键字串的解析。使用了测试指令后yapc会在.output文件里面打印出测试的结果，即对这个关键字串的解析过程，以及出错信息。这样方便了对文法规则的检验和调试。
测试指令可以出现在文法定义块中文法规则外的任何位置。
用法：
``` yacc
%test ( <关键字> )* ;
```
“*”表示可以出现任意次。

## 冲突及解决
当文法存在冲突时yapc会在终端以及.output文件里面打印出冲突的信息以及解决的方式。当出现移进/规约冲突时yapc会选择移进，出现规约/规约冲突时写在前面的规则会被选择。目前暂时还没有实现%left,%right等解决方式。

## 生成的源码及使用方法
yapc会生成六个函数
```c
int yyParser_init(yyParser *yyparser);
int yyParser_reInit(yyParser *yyparser);
int yyParser_free(yyParser *yyparser);
int yyParser_acceptToken(yyParser *yyparser,int yytokenid);
int yyParser_printError(yyParser *yyparser,FILE *out);
int yyParser_clearStack(yyParser *yyparser);
```
及一个结构体yyParser。在解析器生成部分yapc和lemon比较类似，即词法扫描器调用解析器。基本使用方法是：扫描器每扫描一个关键词就调用yyParser_acceptToken使解析器移进（shift）一个关键字，如果出现了语法错误这个函数就会返回-1,这时候就调用
yyParser_printError来打印错误信息，直到解析完成。具体的例子请参考examples。

## .output输出文件
.output文件包含三个部分：文法的冲突信息、生成的LR(1)项目集合及状态转移表、测试输出。

## API
如果你想在其他程序中调用yapc，可以调用API。一般一个简单的生成器由下面几行代码组成：
```c
#include "yapc.h"
...
//创建输入及输出文件
FILE *input = fopen("example.y","ro");
FILE *source = fopen("example.c","wo");
FILE *header = fopen("example.h","wo");
FILE *out = fopen("example.output","wo");
//yNewContext用于新建一个新的yapc上下文
void *ctx = yNewContext();
//解析文法文件，如果出错会返回-1
if(yParseFile(ctx,input,"example.y",stderr)){
    ...
    //生成解析表
    yGenerateTable(ctx);
    //向输出文件中打印警告信息，即冲突等
    yPrintGenerationWarnings(ctx,out);
    //向终端打印警告信息
    yPrintGenerationWarnings(ctx,stdout);
    //向输出文件打印
    yPrintResult(ctx,out);
    //向输出文件打印测试的结果，即测试的解析过程
    yPrintTestResult(ctx,out);
    //生成解析器的源码文件
    yGenerateCCode(ctx,header,source,"header.h","source.c");
    ...
}
else{
    fprintf(stderr,"error while parse grammar file.\n");
    ...
}

...
//释放上下文的内存
yDestroyContext(ctx);
```
# Bug
欢迎大家在issues上提交bug！

# 版本记录
## v0.2，2017-4-29（最新）
* 添加了生成并打印分析树的功能（见文档）
* 将解析文法文件的解析器由之前手写的递归向下分析器改为用yapc生成的分析器（文法文件为yparser.y）

## v0.1.2，2017-4-28
* 修复了测试模块中的bug。（犯了低级错误。。）
* 修复了由于错误地合并项目集合而导致生成错误的状态转移表的bug。（查看tests/bug-2.y）
* 现在生成的冲突信息会提示相应的关键字了。
* 现在会生成未使用的关键字的警告信息。

## v0.1.1，2017-4-26
* 修复了examples/json中的几个bug。
* 重构了部分代码。
* 修改了文档中的几个错误。
* 现在会在生成的源码最前面加上“由yapc生成”的注释信息。

## v0.1-rc，2017-4-23
* 第一个版本。

# 许可证
YAPC使用GPL v3进行许可。