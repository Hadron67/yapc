# YAPC

另一个解析器生成器
Yet Another Parser Compiler

使用[hanolee算法](http://david.tribble.com/text/honalee.html)生成LR(1)分析表，这是一种比LALR(1)更强的解析算法，能够解析所有的LR(1)型文法，而当文法是LALR(1)时生成的状态表的大小和LALR(1)相当。

## 编译及安装
用一下命令即可编译
```
cmake .
make
```
编译需要使用cmake2.8及以上。除此之外没有其他库的依赖。

## YAPC的文档
### 用法
yapc需要一个参数作为输入文件：
```
yapc example.y
```
执行后三个文件会被生成，分别是example.c，example.h，example.output。其中前两个是生成的解析器源码，最后一个为输出的其他信息，下面会继续提到。第三个.output文件可以用--output参数单独指定，如
```
yapc example.y --output test.txt
```
将生成
example.c，example.h，test.txt。其中test.txt和example.output内容完全一样。

### 输入文件的语法
文件的语法和yacc语法相似，只不过做了许多简化，也就是说，有些功能还没有（毕竟只有我一个人在写啊）。
文件的结构如下：
%{

    前部

%}

选项及关键字（token）定义

%%

文法定义

%%

下面一一解释这些部分。
### 前部
这部分代码会被原封不动地复制到生成的.c文件的最前部。一般会将一些#include等指令放在这里。

### 选项及关键字定义
#### 关键词定义
关键字定义的格式为
%token <关键字名> "关键字的别名"
其中关键字名可以是由任何除了">"的字符组成，是用于在文法定义里面的终结符（Terminal）的。
关键字别名**必须是合法的标识符**。因为别名是解析器和词法扫描器之间通信用的，在生成.h文件时会对每个关键词定义相应的宏，如
```
%token <+> "PLUS"
```
会生成如下的宏：
```
\#define T_PLUS 1
```
因此如果别名不是标识符，虽然yapc不会报错，但在编译解析器时就会出错。

#### 关键字类型定义
格式为
```
%type "类型"
```
关键字以被定义的类型的方式保存在解析器的语法分析栈中用于语义分析。这个类型默认是int。

### 文法定义
文法定义的格式和yacc相似，只不过所有的终结符（关键字）都要用尖括号"<>"括起来。如在yacc中的定义
```
A: T_NUM;
```
应写成
```
A: <T_NUM>;
```
其中尖括号里面的内容就是之前定义的关键字名。

#### 动作（action）
与yacc类似，在每个文法规则最后及中间都可以定义动作块，用花括号“{}”括起来。在动作内部使用“$$”代表这条规则的返回值，$+整数代表这条规则中的各个元素的返回值，其中返回值的类型就是之前用%type定义的。

#### 测试指令 %test
测试指令可用于测试对一个关键字串的解析。使用了测试指令后yapc会在.output文件里面打印出测试的结果，即对这个关键字串的解析过程，以及出错信息。这样方便了对文法规则的检验和调试。
测试指令可以出现在文法定义块中文法规则外的任何位置。用法：%test ( <关键字> )* ;“*”表示可以出现任意次。

### 冲突及解决
当文法存在冲突时yapc会在终端以及.output文件里面打印出冲突的信息以及解决的方式。当出现移进/规约冲突时yapc会选择移进，出现规约/规约冲突时写在前面的规则会被选择。目前暂时还没有实现%left,%right等解决方式。

### 生成的源码及使用方法
yapc会生成四个函数
```
int yyParser_init(yyParser *yyparser,yyalloc altor,yyrealloc rtor,yyfree dtor);
int yyParser_free(yyParser *yyparser);
int yyParser_acceptToken(yyParser *yyparser,int yytokenid);
int yyParser_printError(yyParser *yyparser,FILE *out);
```
及一个结构体yyParser。在解析器生成部分yapc和lemon比较类似，即词法扫描器调用解析器。基本使用方法是：扫描器每扫描一个关键词就调用yyParser_acceptToken使解析器移进（shift）一个关键字，如果出现了语法错误这个函数就会返回-1,这时候就调用
yyParser_printError来打印错误信息，直到解析完成。具体的例子请参考examples。

### API

## License
YAPC is licenced under GPL v2.0