# duilib

- 源码源自googlecode，但是现在googlecode上已经找不到了，作者目前维护的GitHub项目：[duilib](https://github.com/duilib/duilib)

奇怪的是官方版本并不提供DuiDesigner界面UI设计工具，所以我这里维护了一个版本，并且修复了一些问题，并优化了用法。

收集的很多关于duilib开发的demo移动到了这里：[bigsinger/duilibdemo](https://github.com/bigsinger/duilibdemo)

当前维护方向是 VS2026 + C++20，并提供三档控件 profile。默认头文件启用轻量版、标准版和全量版全部控件，开关集中在 [UIProfile.h](DuiLib/Core/UIProfile.h)：

- 轻量版：`DUI_CONTROLS_LIGHT`，包含基础布局、表单、菜单、列表、Progress、Slider、DateTime、GroupBox、PageControl、HotKey、SwitchButton、NumberEdit、HyperLink、Line。
- 标准版：`DUI_CONTROLS_STANDARD`，在轻量版基础上增加 GifAnim、IPAddress、TreeView/TreeNode、RollText、ComboButton、CheckCombo、FilterCombo、VirtualListBox、Splitter、MenuBar。
- 全量版：`DUI_CONTROLS_FULL`，在标准版基础上增加 RichEdit、ListEx/ListTextExtElement、FadeButton、PropertyGrid、DirectoryTree、AddressBar。

```cpp
#define DUI_CONTROLS_FLAG (DUI_CONTROLS_LIGHT | DUI_CONTROLS_STANDARD | DUI_CONTROLS_FULL)
```

当前 DuiLib 主库工程不再启用 MFC，主库源码也已清理 MFC 字符串和集合类型残留。本地字符串统一使用 `tstring`，在多字节构建下基于 `std::string`，在 Unicode 构建下基于 `std::wstring`；外部文本交换继续使用 UTF-8 接口和 `std::string_view`。
DuiLib 主库只保留四个配置：`Debug/Release` 是 Unicode DLL 配置，输出到 `bin`/`bin\x64`；`lib_debug/lib_release` 是多字节静态库配置，输出到 `Lib`/`Lib\x64`。四个配置均使用 `/MT` 或 `/MTd`。
DLL 配置默认使用全量档 `0x07`，静态 lib 配置默认使用轻量档 `0x01`。
工程的 PCH 和编译 PDB 已隔离到各自中间目录，项目引用交替构建时不再共用旧 `DuiLib\Build` 预编译头。

不再进入支持 profile 的旧控件包括 Animation、AnimationTabLayout、ColorPalette、Loading、Ring。ActiveX、WebBrowser、Flash 及其事件处理辅助类已从新版主库源码和工程入口移除。

构建示例：

```powershell
# DuiLib DLL Release Win32
& "D:\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" DuiLib.sln /p:Configuration=Release /p:Platform=Win32

# DuiLib 静态库 x64
& "D:\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" DuiLib.sln /p:Configuration=lib_release /p:Platform=x64

# DuiDesigner 默认验证配置
& "D:\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" DuiDesigner.sln /m:1 /p:Configuration=Release /p:Platform=Win32

# 也可以直接双击或命令行运行自动编译脚本
build\build_all.bat
build\build_all.bat all

# 自动化调用时可关闭脚本末尾暂停
cmd /c "set DUILIB_BUILD_NO_PAUSE=1&& build\build_all.bat designer Release Win32"
```

编辑器改造当前恢复到 `DuiDesigner` 项目继续推进，暂停非 MFC `DuiEditor` 路线。`DuiDesigner` 可以继续使用 MFC 框架，并只保留 `Debug/Release` 两个配置；默认验证使用 `Release|Win32`。设计器始终面向全量控件清单，Win32 输出到 `bin`，x64 输出到 `bin\x64`。已移除 ActiveX、WebBrowser、Flash 等主库已删除控件入口。

新版文档：

- [新版使用教程](doc/新版使用教程.md)
- [控件与属性参考](doc/控件与属性参考.md)
- [DuiDesigner/DuiEditor 状态说明](doc/DuiEditor使用说明.md)
- [控件裁剪与分档方案](doc/modernization-control-evaluation.md)
- [现代化改造状态](doc/modernization-status.md)
- [历史修复同步清单](doc/历史修复同步清单.md)

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

当前主库已提供轻量多语言工具 `CMultiLanguageUI`，不要求窗口类继承特定基类。

使用约定：

- XML 中需要翻译的控件设置唯一 `name`。
- 语言文件使用 UTF-8 `key=value` 格式。
- `name` 对应控件文本，`name_tip` 对应控件提示文本。
- 值为空或缺失时，不覆盖控件当前文本。
- 值中可使用 `\n`、`\r`、`\t` 转义。

语言文件示例：

```
# lang_zh_cn.ini内容：
tab_setting=选项设置
tab_setting_tip=软件设置

# lang_en_us.ini内容：
tab_setting=Options
tab_setting_tip=Manage Settings
```

代码示例：

```cpp
DuiLib::CMultiLanguageUI lang;
if (lang.LoadFromFile(_T("lang_zh_cn.ini"))) {
    lang.Apply(&m_PaintManager);
}
```


