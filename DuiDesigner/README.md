# DuiDesigner

`DuiDesigner` 是当前 DuiLib XML 界面的可视化编辑器。工程文件已从仓库历史中的 `9c6f261` 提交恢复，并继续按当前 DuiLib 接口同步维护。

## 编译环境

- Visual Studio 2026（工具集 `v145`）
- C++20
- Unicode
- x64
- Debug 使用 MTd，Release 使用 MT
- MFC 静态链接
- DuiLib 全量控件：`DUI_CONTROLS_FLAG=0x07`

编译 Debug：

```powershell
& "D:\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" DuiDesigner.sln /m /p:Configuration=Debug /p:Platform=x64 /v:minimal
```

编译 Release：

```powershell
& "D:\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" DuiDesigner.sln /m /p:Configuration=Release /p:Platform=x64 /v:minimal
```

输出目录为仓库根目录下的 `bin\x64`。Debug 可执行文件名为 `DuiDesigner_d.exe`，Release 可执行文件名为 `DuiDesigner.exe`。

## 字符串约定

DuiDesigner 与 DuiLib 交互时使用 `DuiLib::tstring`。当前工程固定为 Unicode，因此 `tstring` 对应 `std::wstring`；UTF-8 边界使用 `std::string`、`std::string_view` 与 DuiLib 提供的 UTF-8 转换函数。MFC 窗口和属性表内部仍可使用 `CString`，但不再引入或使用 `CDuiString`。

## 控件同步

工具箱按“基础控件”“选择与列表”“布局与菜单”分组，列出当前 DuiLib 可由 XML 创建的全量控件。已经移除的 `ActiveX`、`WebBrowser`、`Animation`、`AnimationTabLayout`、`ColorPalette`、`Loading` 和 `Ring` 不再提供工具箱入口。

新增控件后需要同步检查以下位置：

1. `stdafx.h` 中的 `UIClass` 和 DuiLib 类型声明。
2. `stdafx.cpp` 中控件类名到 `UIClass` 的映射。
3. `ToolBoxWnd.cpp` 中的工具箱入口。
4. `LayoutManager.cpp` 中的创建、克隆和 XML 保存分派。

第三方编辑器项目的经验表明，用外部控件属性描述文件驱动工具箱和属性表更容易长期维护。当前版本先保持现有编辑器架构，并将控件映射改为表驱动，后续可再逐步迁移属性描述机制。

## 参考项目

- [JonYangUi/DuiLibUiEditor](https://github.com/JonYangUi/DuiLibUiEditor)：重点参考其同步提交和 x64 编译修复记录。
- [xfcanyue/DuiLib_DuiEditor](https://github.com/xfcanyue/DuiLib_DuiEditor)：重点参考其设计器缺陷修复、控件扩展和 UTF-8 编码整理记录。

本项目不直接依赖上述仓库代码，当前控件清单和接口仍以本仓库的 DuiLib 为准。
