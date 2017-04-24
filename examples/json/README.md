# JSON解析器 - JSON parser

[English documentation](README-us_EN.md)

这是一个简单的JSON处理器，可以从文件读取JSON并解析，然后从中提取出想要的部分。用法如下：
```shell
./main <JSON文件> <格式字符串>
```
其中格式字符串用于从读取到的JSON中提取一个成分，用“.”加名称或"[]"中加字符串表示从对象中提取出相应的成员；“[]”中间加数字代表从数组中提取。如JSON文件的内容是：
```json
{
	"test":[4,5,6,{"test1" : true},["sdfsd",5,6,8]],
	"hkm":"ggghhh",
	"rfnj": false
}
```
则“.test”就会返回
```json
[4,5,6,{"test1" : true},["sdfsd",5,6,8]]
```
可以从提取到的对象中继续提取，如“.test[3].test1”就会得到true。

在这个例子中我用了两个.y文件，分别是JSON解析器和格式字符串的解析器。用到了文档中提到的大部分功能。这个程序的代码比较多，大家可以重点看两个.y文件。

