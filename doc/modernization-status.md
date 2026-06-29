# DuiLib 现代化改造状态

本文记录当前仓库已经完成的改造、验证结果和仍需继续迁移的范围。

## 构建基线

- 工具链：Visual Studio 2026，MSVC v145。
- C++ 标准：C++20。
- C 标准：C17。
- 平台：Win32、x64。
- DuiLib 主库不启用 MFC，源码已清理 `CString`、`CStringArray`、`CArray`、`CMap` 等 MFC 字符串和集合类型残留。
- DuiLib 主库只保留四个配置：`Debug/Release` 为 Unicode DLL，`lib_debug/lib_release` 为多字节静态库。
- DuiDesigner 只保留 `Debug/Release` 两个解决方案配置。
- 根目录 `Demo.sln` 已移除，demo 后续在独立仓库维护。
- 当前默认 profile 是全量版；控件档位通过 `DuiLib/Core/UIProfile.h` 中的 `DUI_CONTROLS_FLAG` 控制。
- DuiLib 的 PCH、编译 PDB 和 Debug PDB 已隔离到各自中间目录，避免并行构建和项目引用构建互相污染。
- `DuiLib/CMakeLists.txt` 旧入口已移除，VS2026 构建以 `DuiLib/DuiLib.vcxproj` 为准。
- 非 MFC `DuiEditor` 路线已暂停；当前编辑器后续以恢复后的 `DuiDesigner` 项目为准。

## 已验证构建

当前默认验证策略：

- DuiDesigner 默认使用 `Release|Win32` 验证。
- DuiLib 需要完整验证时使用 `Debug/Release/lib_debug/lib_release` 乘以 `Win32/x64` 的八个组合。

## 控件 profile

控件 profile 已改为三档标志位：

- `DUI_CONTROLS_LIGHT`：轻量版。
- `DUI_CONTROLS_STANDARD`：标准版。
- `DUI_CONTROLS_FULL`：全量版。

轻量版包含基础窗口、布局、表单、菜单、列表，并额外包含：

- `Menu`
- `HotKey`
- `PageControl`
- `Progress`
- `Slider`
- `DateTime`
- `GroupBox`
- `SwitchButton`

标准版额外包含：

- `GifAnim`
- `IPAddress`
- `TreeView`
- `TreeNode`
- `RollText`

全量版额外包含：

- `RichEdit`
- `ListEx`
- `ListTextExtElement`
- `FadeButton`

已排除：

- `Animation`
- `AnimationTabLayout`
- `ColorPalette`
- `Loading`
- `Ring`
- `ActiveX`
- `WebBrowser`
- `Flash`

这些控件不进入新版 DuiLib 工程、不进入 `UIlib.h` 聚合头，也不再通过 `UIDefine.h` 暴露控件名宏。ActiveX、WebBrowser、Flash、Flash 类型库及其事件处理辅助类已从新版主库源码目录移除，迁移对照以 git 历史为准。

## 标准字符串与绑定

新增标准字符串工具：

- `DuiUtf8String`
- `DuiWideString`
- `DuiNativeString`
- `DuiUtf8ToWide`
- `DuiWideToUtf8`
- `DuiUtf8ToNative`
- `DuiNativeToUtf8`
- `DuiStringToUtf8`
- `DuiStringFromUtf8`

`CControlUI` 新增 UTF-8 接口：

- `GetNameUtf8`
- `SetNameUtf8`
- `GetTextUtf8`
- `SetTextUtf8`
- `GetToolTipUtf8`
- `SetToolTipUtf8`
- `GetUserDataUtf8`
- `SetUserDataUtf8`
- `SetAttributeUtf8`
- `ApplyAttributeListUtf8`
- `BindTextUtf8`
- `ClearTextBinding`
- `HasTextBinding`
- `RefreshTextBinding`
- `CommitTextBinding`

源码中的旧自定义字符串类型已经替换为 `tstring`。`tstring` 是标准库字符串别名，在多字节构建下对应 `std::string`，在 Unicode 构建下对应 `std::wstring`。旧 `CDuiString` 成员式接口已经从 DuiLib 主库移除，新增代码应直接使用标准库方法和 UTF-8 接口；需要旧式查找、截取、替换、格式化语义时，使用 `DuiStringFind`、`DuiStringMid`、`DuiStringReplace`、`DuiStringFormat` 等过渡辅助函数。

`CStdStringPtrMap` 仍保留旧接口名用于兼容现有管理器和旧 DuiDesigner 迁移代码，但内部手写哈希链表已经替换为 `std::unordered_map<tstring, void*>` 和 `std::vector<tstring>`，后续公共 API 可逐步收敛到明确类型的标准库容器。

新增和合并控件中的明显英文尾注、英文说明注释和注释掉的旧代码已清理；旧第三方源码中保留的 GBK 注释仍按历史编码保存，避免在未统一源文件编码前引入 UTF-8 中文导致编译器误判。

本轮继续补充中文化清理：已移除新增控件和字符串工具路径中的英文参数注释、默认参数尾注及 `UIDlgBuilder` 中注释掉的旧示例代码。

已梳理 `F:\bigsinger\zhupite.github.io\_posts\dev\duilib` 下 29 篇历史修复文档，新增 `doc/历史修复同步清单.md` 作为同步跟踪表。当前已核对 TinyXML 属性保存引号修复，确认 `TiXmlAttribute::Print` 固定使用双引号输出；并同步 `CComboUI::Add/AddAt` 的内部可见性修复，避免 Combo 在隐藏 Tab 页初始化后 `SelectItem` 因子项不可见而失败。已用 `E:\temp\duilib-combo-test` 临时工程验证隐藏状态下 `CComboUI::Add` 后执行 `SelectItem(0)` 返回成功。`CRichEditUI::AppendMsg` 已按历史修复文档补回，ModernProfileDemo 全量版按钮路径已验证追加彩色日志。

当前多语言能力已在主库补充 `CMultiLanguageUI`：读取 UTF-8 `key=value` 文件后，可按控件 `name` 和 `name_tip` 递归刷新控件文本与提示。

已新增 `build/build_all.bat`，用于双击菜单或命令行构建 DuiLib 和 DuiDesigner。自动化调用时可设置 `DUILIB_BUILD_NO_PAUSE=1` 跳过脚本末尾暂停。

## DuiDesigner 状态

现代化 `DuiEditor` 路线已暂停。当前编辑器基线恢复为 `DuiDesigner` 现状，后续改造按以下约束推进：

- 保留 `DuiDesigner` 项目和现有 MFC 框架，不再把它替换为非 MFC 的 `DuiEditor` 外壳。
- 与 DuiLib 的 `tstring` 方案保持一致：多字节构建使用 `std::string`，Unicode 构建使用 `std::wstring`，移除 `CDuiString` 依赖。
- 设计器始终面向全量控件清单，不需要轻量版/标准版/全量版切换。
- `SwitchButton` 已进入轻量版，设计器基础工具箱已加入创建入口。
- 去掉 DuiLib 已移除的 ActiveX、WebBrowser、Flash 以及 Animation、AnimationTabLayout、ColorPalette、Loading、Ring 等控件入口。
- 只保留 `Debug/Release` 两个配置，使用 VS2026 / C++20 编译。
- `DuiDesigner` 始终面向全量控件清单，Win32 输出到 `bin`，x64 输出到 `bin\x64`；默认验证使用 `Release|Win32`。
- x64 下保留 `CreateFileW` 的 IAT 路径重定向钩子，禁用旧 32 位裸汇编的 `Invalidate/GetImageEx` 跳转钩子。

默认验证命令：

```powershell
& "D:\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" DuiDesigner.sln /m:1 /p:Configuration=Release /p:Platform=Win32 /v:minimal
```

## 旧修复点保留要求

参考 `F:\bigsinger\zhupite.github.io\_posts\dev\duilib` 下的历史修复文档，后续迁移必须保留这些行为：

- 文件被外部修改后，DuiDesigner 必须能提示重新载入，并在保存后刷新文件时间戳。
- “打开皮肤文件所在位置”应定位到具体文件，而不是只打开目录。
- Label、Button、ListHeaderItem 等文本对齐属性必须能编辑和保存。
- `wrap`、`textpadding`、CheckBox 状态等控件属性不能在保存时丢失。
- RichEdit 属性保存不能退化成空 `<RichEdit />`。
- `VScrollBar`、`HScrollBar` 默认属性必须能完整读写。
- TinyXML 处理带转义引号的属性时不能破坏原值。
- `anchor` 语义继续可用，旧 `relativepos` 只作为读取兼容入口，不再由新版 DuiDesigner 写出。
- Tab/PageControl 选中状态必须能保存并重新加载。
- `Combo` 在隐藏 Tab 页内调用 `SelectItem` 的行为不能回退。
- `List` Ctrl/Shift 多选必须保留条目回调和 owner 批量选择分离，程序性 `Select(..., false)` 不应受当前键盘状态影响。
- 旧自定义字符串类型的历史比较问题由 `tstring` 标准字符串过渡层逐步替代，新增代码不再依赖旧比较语义。

## 下一阶段任务

1. 从旧 MFC 源码继续核对项目树筛选、XML 读写、更完整的拖放插入和资源操作逻辑。
2. 继续按 `doc/属性列表.xml` 补齐低频属性和属性分组，让 DuiDesigner 的全量属性面板更接近旧设计器。
3. 处理 x64 构建中剩余的旧 MFC 窄化警告，优先清理指针到 `DWORD/int` 的转换。
4. 同步 demo 仓库，移除已删除控件示例。

## 当前保存限制

DuiDesigner 后续保存逻辑需要继续保持 XML 原始空白、缩进、属性顺序和注释布局。遇到无法可靠定位原始标签的 XML 结构时，才允许回退到 TinyXML 重新输出 XML。


