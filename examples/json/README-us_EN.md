# JSON parser

This is a simple JSON processor,it can read a JSON file and extract subparts of the JSON object.Usage:
```shell
./main <JSON file> <format string>
```
The format string is used to extract object from JSON.Use "." followed by name or \["name"\] to extract a corresponding member of a JSON object;Use \[number\] to extract a member of an array object.For example,given a JSON:
```json
{
	"test":[4,5,6,{"test1" : true},["sdfsd",5,6,8]],
	"hkm":"ggghhh",
	"rfnj": false
}
```
and ".test" will get
```json
[4,5,6,{"test1" : true},["sdfsd",5,6,8]]
```
you can continue to extract from an extracted object,for example ".test[3].test1" will get true。

In this example i used two .y files,one for the JSON parser and the latter for parsing format string.Most features mentioned in the documentation is used.
