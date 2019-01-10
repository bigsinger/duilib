# duilib
- 源码源自googlecode，但是现在googlecode上已经找不到了，作者目前维护的GitHub项目：[duilib](https://github.com/duilib/duilib)

奇怪的是官方版本并不提供DuiDesigner界面UI设计工具，所以我这里维护了一个版本，并且修复了一些问题，并优化了用法。

收集的很多关于duilib开发的demo移动到了这里：[bigsinger/duilibdemo](https://github.com/bigsinger/duilibdemo)


# 技巧
## 占位符
可以用
```xml
<Control />
```
或者：
```xml
<Container />
```

水平布局（HorizontalLayout）中**宽度为1的黑色分隔条**：
```xml
<Control width="1" bkcolor="#0" />
```

## 垂直滚动条
在<Window>子节点下添加：
```xml
<Default name="VScrollBar" value="button1normalimage=&quot;file=&apos;scrollbar_v.png&apos; source=&apos;0,0,16,16&apos;&quot; button1hotimage=&quot;file=&apos;scrollbar_v.png&apos; source=&apos;16,0,32,16,16&apos;&quot; button1pushedimage=&quot;file=&apos;scrollbar_v.png&apos; source=&apos;32,0,48,16&apos;&quot; button2normalimage=&quot;file=&apos;scrollbar_v.png&apos; source=&apos;0,32,16,48&apos;&quot; button2hotimage=&quot;file=&apos;scrollbar_v.png&apos; source=&apos;16,32,32,48&apos;&quot; button2pushedimage=&quot;file=&apos;scrollbar_v.png&apos; source=&apos;32,32,48,48&apos;&quot; thumbnormalimage=&quot;file=&apos;scrollbar_v.png&apos; source=&apos;0,48,16,64&apos; corner=&apos;0,2,0,2&apos;&quot; thumbhotimage=&quot;file=&apos;scrollbar_v.png&apos; source=&apos;16,48,32,64&apos; corner=&apos;0,2,0,2&apos;&quot; thumbpushedimage=&quot;file=&apos;scrollbar_v.png&apos; source=&apos;32,48,48,64&apos; corner=&apos;0,2,0,2&apos;&quot; bknormalimage=&quot;file=&apos;scrollbar_v.png&apos; source=&apos;0,16,16,32&apos;&quot;" />
```

## 无须图片文件仅靠颜色的编辑框（Edit）
```xml
<Edit name="edtRow" width="80" height="30" bordersize="1" bkcolor="#FFFFFFFF" bordercolor="#FF4EA0D1" textpadding="4,3,40,3" textcolor="#FF000000" align="leftvcenter" />
```

## 占满剩余空间
如果在一个水平布局（HorizontalLayout）中，想要实现三部分，第一部分在左侧，第三部分在右侧，剩余空间给第二部分。则可以通过省略第二部分**width**属性的方法来达到该效果。

类似地，如果在一个垂直布局（VerticalLayout）中，想要实现三部分，第一部分在上侧，第三部分在下侧，剩余空间给第二部分。则可以通过省略第二部分**height**属性的方法来达到该效果。

## 拖放缩小窗口时隐藏优先级
如果主窗口界面允许拖放，在拖放缩小时，可以通过如下规则隐藏控件：
- 水平缩小时，优先隐藏水平布局（HorizontalLayout）中**不带width**属性的子垂直布局（VerticalLayout），其次缩小隐藏**带width**属性的子垂直布局（VerticalLayout），最后才是子水平布局。
- 垂直缩小时，优先隐藏垂直布局（VerticalLayout）中**不带height**属性的子水平布局（HorizontalLayout），其次缩小隐藏**带height**属性的子水平布局（HorizontalLayout），最后才是子垂直布局。

举例说明：
```xml
```

如果不好理解该规则，其实可以添加对OnSize的处理通过代码的方式实现控件的重新布局或隐藏：
```c
m_uiPreview->OnSize += MakeDelegate(this, &CMainWnd::OnSizeChanged);
```

## 关于duilib中的颜色
COLORREF中颜色的排列是（高位）BGR（低位），而通过Color.ToArgb()得到的数值中颜色排列是（高位）AARRGGBB（低位）。

而在duilib中关于颜色（带有Color的函数）的使用是AARRGGBB格式，对于传统Windows开发来说如果已经习惯了COLORREF的话要特别注意！

为防止误解使用，重新调整了下接口：
```c
// 设置了一个易于理解的接口，如果只是设置简单的RGB可以安装这个接口调
void SetTextColor(IN BYTE r, IN BYTE g, IN BYTE b);

// 如果是从其他地方获取的RGB格式颜色，可以直接使用，内部会转换
void SetTextColorRGB(COLORREF rgb);

// 设置的是ARGB（Photoshop格式），不是RGB（VC开发惯用格式）！
void SetTextColor(ARGB dwTextColor);

// 返回的是ARGB，注意区别于RGB的字节组织结构
ARGB GetTextColor() const;
```

## 多语言支持
我看了官方duilib的代码提交记录，是有支持了多语言的，但是官方没有说明怎么使用该多语言支持的功能。心想自己实现也不复杂，于是自己动手实现一下。

设计的预期效果：
- 用户可以通过菜单列表（或其他下拉列表控件）自由切换语言，切换后立即生效，无闪烁，无须重新运行程序。
- 多语言配置文件要足够简单。

设计过程略。

使用方法：
- 窗口类继承CAppBaseMainWnd（该类封装了多语言支持的细节）。
- 查找控件时使用CAppBaseMainWnd::FindControl，该函数会把返回的控件指针添加到须支持多语言的控件列表mMultiLanguageControls中去。
- 如果想要支持多语言，请在皮肤配置xml文件中为控件显示指定一个name，那些无须多语言支持的控件可以不填name。
- 当用户切换多语言时，CAppBaseMainWnd会自动处理mMultiLanguageControls中的控件，获取控件的name作为字符串ID来查询字符串作为控件的文本，{name}_tip作为字符串ID查询的字符串作为控件的tip文本。
- 语言文件按如下方式简单配置即可：
```
# lang_zh_cn.ini内容：
tab_setting=选项设置
tab_setting_tip=软件设置

# lang_en_us.ini内容：
tab_setting=Options
tab_setting_tip=Manage Settings
```
如果控件有name属性，但是没有出现在语言配置中，或语言配置中值是空，则不会切换控件文本和提示文本。

语言的值可以用\n来表示换行，\t表示Tab字符，内部统一处理了。使用这种语言配置文件形式为map形式，也即一个key一个value，相对于xml形式简单清晰也不用考虑其他转义字符。

- 派生类窗口如果还需要做一些特殊处理，可以重载覆盖以下函数来编写自定义代码：
```c
//刷新语言
virtual void OnFreshLang();
```