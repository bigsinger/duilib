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

