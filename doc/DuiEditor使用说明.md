# DuiDesigner / DuiEditor 状态说明

当前暂停非 MFC `DuiEditor` 路线，编辑器改造恢复到现有 `DuiDesigner` 项目继续推进。

## 当前基线

- 继续使用 `DuiDesigner` 项目和用户已恢复的 `DuiDesigner/DuiDesigner.vcxproj`。
- `DuiDesigner` 可以继续使用 MFC 框架。
- 后续改造需要与 DuiLib 的 `tstring`、C++20、Win32/x64 和控件裁剪策略保持一致。
- 设计器用于编辑界面，默认应暴露全量控件清单，不需要轻量版/标准版/全量版切换。
- 当前 `DuiDesigner` 只保留 `Debug/Release` 两个配置，默认验证使用 `Release|Win32`。
- 设计器输出到常用运行目录：Win32 为 `bin`，x64 为 `bin\x64`。
- 设计器引用 DuiLib 的同名 DLL 配置，默认全量 profile 由 [UIProfile.h](../DuiLib/Core/UIProfile.h) 中的 `DUI_CONTROLS_FLAG` 控制。
- `SwitchButton` 已进入轻量版控件，并可在设计器基础工具箱中创建。
- 已移除 ActiveX、WebBrowser、Flash 等主库已删除控件的工具箱、菜单、类识别和属性页入口。

## 构建命令

```powershell
& "D:\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" DuiDesigner\DuiDesigner.vcxproj /m:1 /p:Configuration=Debug /p:Platform=Win32
& "D:\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" DuiDesigner\DuiDesigner.vcxproj /m:1 /p:Configuration=Release /p:Platform=Win32
```

旧 MFC 工程仍建议使用 `/m:1` 构建设计器，避免多 CL 进程同时写入同一批 `.tlog` 或编译 PDB。

Win32 调试版输出为 `bin\DuiDesigner_d.exe`，发布版输出为 `bin\DuiDesigner.exe`。x64 输出到 `bin\x64`。如果使用解决方案构建，`DuiDesigner.sln` 会先构建同平台同配置的 DuiLib。

也可以双击 `build\build_all.bat`，选择“编译 DuiDesigner Release Win32”。

## 后续改造要求

1. 继续同步 DuiLib 已合并控件：`GifAnimEx` 归并到 `GifAnim`，`IPAddressEx` 归并到 `IPAddress`。
2. 继续补齐属性面板、控件清单和 XML 创建逻辑。
3. 清理 x64 下旧 MFC 代码的窄化警告，尤其是指针、`DWORD_PTR`、`INT_PTR` 到 `DWORD/int` 的转换。

## 备注

旧的现代 `DuiEditor` 资料仅作为历史尝试保留，不作为当前实现依据。当前编辑器交付以 `DuiDesigner` 编译通过和运行正常为准。


