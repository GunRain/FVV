# Fresh View for Viewer (FVV/FW/废物/廢物)


This page is written in Chinese, please use the translation if you do not understand.


苓师傅在闲余时间搓出来的文本格式，目前仅支持`C++`(最低支持C++`17`)，以后可能会支持`Go`、`Kotlin`、`JavaScript`等常用语言，反正目前是还没有写的。

不会支持的语言: `Python`、`Java`、`Rust`等

TODO:

 - 支持数学运算(需要显式定义)
 - 支持值组内的赋值(需要显式定义)
 - 支持将组赋值给值


已知问题: ~~解析器无法存留赋值定义，举例:~~

``` fvv
{
    a = 1;
    b = a;
}
```

~~解析器解析后只能解析出`b`等于`1`，不能在`a`被修改的情况下让`b`的值跟随变动，`b`的值仍为`1`~~

此问题已修复


名字显而易见，是个~~废物~~清新的文本格式，那么有多清新呢，请看示例:

``` fvv
{
  ValueName1 = "114514\"" <字符串与转义\>>;
  ValueName2 = true       <布尔值>;
  ValueName3 = 114514     <整数>  ;
  ValueName4 = 114.514    <浮点数>;

  ValueName5 = ["1", "1", "4", "5", "1", "4"] <字符串组>;
  ValueName6 = [1, 1, 4, 5, 1, 4, ] <整数组(这个“,”是故意多打的)>;

  GroupName = {
    SubGroupName = {
        a = ValueName3 <支持赋值操作(这个是跨组赋值)>;
    } <子组>;
    c = SubGroupName.a <这个是跨组赋值>;
  } <组>;

  GroupName.SubGroupName.b = a                        <还支持用“.”连接组名(这个是同组赋值)>;
  GroupName.SubGroupName.b = GroupName.SubGroupName.a <这个是同组赋值，只不过用“.”表示了完整名称>;
}
```

显而易见，支持`字符串`、`布尔值`、`整数`、`浮点数`以及它们的`组`的存储，还支持`值组`以及`赋值`

值的命名也是没有什么忌口的，请看示例:

``` fvv
{
    114514 = 114514;
    一一四五一四 = 114514;
    ()()(((())))((((()))))()(((()))) = 114514;
}
```

需要注意的是，如果一个值被命名为`纯数字`(整数和浮点数都算)，那么这个值将`无法用于赋值`，因为没法判断到底给的是值还是值的名称

还需要注意的是正常命名不要带“.”啊喂，会被认为是组的名称的


本文本格式的值的存储使用`;`进行分割，所有的值都必须至于根路径的`{}`里面，注释使用`<>`进行存储

注释是本文本格式的一个比较特色的功能，它可以放到任何地方，请看示例:

``` fvv
<注释>{<注释>a<注释>=<注释>1<注释>;<注释>}<注释>
```

可以非常直接地看出来，注释完全是想怎么写就怎么写，但是它的作用不止于此

介于`=`与`;`之间的`最后一个`注释，将会被认定为是该值的`描述`，苓师傅认为这是一个非常不错的功能


## C++

以下是`C++`的代码示例:

``` cpp
#include <iostream>
#include <string>
#include <string_view>

#include "fvv.hpp"

using namespace std;
using namespace literals;

const string fvv_txt = R"(
    {
        ValueName1 = "114514\"" <字符串与转义\>>;
        ValueName2 = true       <布尔值>;
        ValueName3 = 114514     <整数>  ;
        ValueName4 = 114.514    <浮点数>;

        ValueName5 = ["1", "1", "4", "5", "1", "4"] <字符串组>;
        ValueName6 = [1, 1, 4, 5, 1, 4, ] <整数组(这个“,”是故意多打的)>;

        GroupName = {
            SubGroupName = {
                a = ValueName3 <支持赋值操作(这个是跨组赋值)>;
            } <子组>;
            c = SubGroupName.a <这个是跨组赋值>;
        } <组>;

        GroupName.SubGroupName.b = a                        <还支持用“.”连接组名(这个是同组赋值)>;
        GroupName.SubGroupName.b = GroupName.SubGroupName.a <这个是同组赋值，只不过用“.”表示了完整名称>;

        114514 = 114514;
        一一四五一四 = 114514;
        ()()(((())))((((()))))()(((()))) = 114514;
    }
)"s;

signed main(void) {
    FVV::FVVV fvv; // 这里初始化个struct
    // “FVVV”指的是FVV的Value，非常简单的命名

    FVV::Parser::ReadString(fvv_txt, fvv); // 从字符串解析

    cout << "FVV API: "s << FVV_API << endl; // 输出FVV的API版本

    if (fvv["GroupName"s]["SubGroupName"s]["114514"s].isEmpty()) // 判断值是否存在
        cout << "值 GroupName.SubGroupName.114514 不存在"s << endl;
    else
        cout << "值 GroupName.SubGroupName.114514 存在"s << endl;

    if (fvv["GroupName"s]["SubGroupName"s]["a"s].isType<string>()) // 判断值是否为指定类型
        cout << "值 GroupName.SubGroupName.a (string): "s << fvv["GroupName"s]["SubGroupName"s]["a"s].as<string>().value() << endl;
    if (fvv["GroupName"s]["SubGroupName"s]["a"s].isType<int>()) // 判断值是否为指定类型
        cout << "值 GroupName.SubGroupName.a (int): "s << fvv["GroupName"s]["SubGroupName"s]["a"s].asInt() << endl;

    cout << fvv.print() << endl << fvv.print("min"sv) << endl; // 输出正常格式和缩减格式

    cout << "值 GroupName.SubGroupName.a (描述): "s << fvv["GroupName"s]["SubGroupName"s]["a"s].getDesc() << endl; // 输出描述
    fvv["GroupName"s]["SubGroupName"s]["a"s].setDesc("114514"sv); // 修改描述
    cout << "值 GroupName.SubGroupName.a (描述): "s << fvv["GroupName"s]["SubGroupName"s]["a"s].getDesc() << endl; // 输出描述

    return 0;
}
```

代码执行后，将会输出:

```
FVV API: 1
值 GroupName.SubGroupName.114514 不存在
值 GroupName.SubGroupName.a (int): 114514
{
  ()()(((())))((((()))))()(((()))) = 114514;
  114514 = 114514;
  GroupName = {
    SubGroupName = {
      a = ValueName3 <支持赋值操作(这个是跨组赋值)>;
      b = GroupName.SubGroupName.a <这个是同组赋值，只不过用“.”表示了完整名称>;
    } <子组>;
    c = SubGroupName.a <这个是跨组赋值>;
  } <组>;
  ValueName1 = "114514\"" <字符串与转义\>>;
  ValueName2 = true <布尔值>;
  ValueName3 = 114514 <整数>;
  ValueName4 = 114.514000 <浮点数>;
  ValueName5 = ["1", "1", "4", "5", "1", "4"] <字符串组>;
  ValueName6 = [1, 1, 4, 5, 1, 4] <整数组(这个“,”是故意多打的)>;
  一一四五一四 = 114514;
}
{()()(((())))((((()))))()(((())))=114514;114514=114514;GroupName={SubGroupName={a=ValueName3;b=GroupName.SubGroupName.a;};c=SubGroupName.a;};ValueName1="114514\"";ValueName2=true;ValueName3=114514;ValueName4=114.514000;ValueName5=["1","1","4","5","1","4"];ValueName6=[1,1,4,5,1,4];一一四五一四=114514;}
值 GroupName.SubGroupName.a (描述): 支持赋值操作(这个是跨组赋值)
值 GroupName.SubGroupName.a (描述): 114514
```

~~有人可能会有疑问，为什么会有`114514 = ;`呢？~~

~~这是因为代码里进行了`if (fvv["GroupName"s]["SubGroupName"s]["114514"s].isEmpty())`的判断操作，导致创建了`GroupName.SubGroupName.114514`这样一个空值~~

此问题被认为是个bug，已修复


接下来就详细解释一下代码吧:


`FVV::FVVV`是一个`struct`类型，使用它来定义一个FVVV以存储解析得到的值

`FVV::Parser::ReadString`是将FVV格式文本解析为FVVV struct的函数

下面是FVVV struct的用法:
 - `asBool()`、`asInt()`、`asDouble()`、`asString()`、`asBools()`、`asInts()`、`asDoubles()`、`asStrings()`: 分别会返回`bool`、`int`、`double`、`std::string`、`std::vector<bool>`、`std::vector<int>`、`std::vector<double>`、`std::vector<std::string>`类型的值，如果值不存在，会分别返回`false`、`0`、`0.0`、`""`、`{}`、`{}`、`{}`、`{}`
 - `as<typename>()`: 会返回一个`std::optional`类型的值
 - `hasDesc()`: 用于判断值是否有描述，会返回一个`bool`类型的值
 - `getDesc()`: 会返回值的描述(`std::string`)，如果没有，会返回空的字符串
 - `setDesc(str)`: 需要传入一个`std::string`类型的值，用于设置值的描述，没有返回值
 - `delDesc()`: 会删除值的描述，没有返回值
 - `isLink()`: 用于判断值是否是链接，会返回一个`bool`类型的值
 - `getLink()`: 会返回值的链接(`FVV::FVVV`)，如果没有，会返回`nullptr`
 - `getLinkName()`: 会返回值的链接名称(`std::string`)，如果没有，会返回空的字符串(返回的链接名称取决于解析时原始文本的定义，如果是相对链接，则仍是相对链接，如果是绝对链接，则仍是绝对链接)
 - `setLink(fvvv)`: 需要传入一个`FVV::FVVV`类型的值，用于设置值的链接，没有返回值
 - `setLinkName(str)`: 需要传入一个`std::string`类型的值，用于设置值的链接名称，没有返回值
 - `delLink()`: 会删除值的链接(包括链接名称)，没有返回值
 - `isEmpty()`、`isNotEmpty()`: 用于判断值是否存在，会返回一个`bool`类型的值
 - `isType<typename>()`: 用于判断值是否为指定类型，会返回一个`bool`类型的值(如果值不存在，会返回`false`)
 - `print()`或`print("min")`: 会把当前struct内所有值输出为FVV格式文本，传入“min”时会去除掉所有值的描述(只是在输出时去除，不会影响struct内的值)，并去除所有空格和换行


## 注意点
 - 注释和字符串赋值支持`转义`，但是`仅支持转义“>”、“"”、“{”、“}”`，例如`<\>>`、`"\""`，解析时会自动去除里面的“\”，`直接使用“\”不需要重复两个“\”`，因为根本没做多复杂的转义检测逻辑
 - `仅支持UTF-8文本`，`支持UTF-8 with BOM文本`，`支持LF和CRLF文本`