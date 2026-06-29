# DuiLib 控件裁剪与分档方案

本文记录本次改造后的控件保留、合并、删除和分档规则。后续新增控件必须先进入本文评估，再决定是否进入轻量版、标准版或全量版。

## 改造目标

- 只支持 Windows，不做跨平台抽象。
- 同时支持 Win32 和 x64。
- 使用 Visual Studio 2026、MSVC v145、C++20。
- DuiLib 主库不再依赖 MFC。
- 新接口逐步回归 `tstring`、UTF-8 `std::string_view` 和标准库容器。
- 控件按轻量版、标准版、全量版三档维护，默认启用三档全部控件。
- 面向中小企业常见桌面工具，优先保留表单、列表、菜单、设置页、属性页、进度状态等高频能力。

## 分档开关

开关集中在 [UIProfile.h](../DuiLib/Core/UIProfile.h)：

```cpp
#define DUI_CONTROLS_LIGHT    0x01
#define DUI_CONTROLS_STANDARD 0x02
#define DUI_CONTROLS_FULL     0x04
#define DUI_CONTROLS_FLAG (DUI_CONTROLS_LIGHT | DUI_CONTROLS_STANDARD | DUI_CONTROLS_FULL)
```

常用组合：

- 轻量版：`DUI_CONTROLS_LIGHT`
- 标准版：`DUI_CONTROLS_LIGHT | DUI_CONTROLS_STANDARD`
- 全量版：`DUI_CONTROLS_LIGHT | DUI_CONTROLS_STANDARD | DUI_CONTROLS_FULL`

项目预处理器中不再配置控件档位宏。

## 轻量版控件

轻量版目标是满足常见业务工具的窗口、表单、菜单、基础列表和设置页。

基础节点和布局：

- `Window`
- `Default`
- `Font`
- `Image`
- `Include`
- `Style`
- `Styles`
- `Control`
- `Container`
- `VerticalLayout`
- `HorizontalLayout`
- `TabLayout`
- `TileLayout`
- `ChildLayout`

基础控件：

- `Label`
- `Text`
- `Button`
- `Option`
- `CheckBox`
- `SwitchButton`
- `Edit`
- `Combo`
- `ComboBox`
- `List`
- `ListHeader`
- `ListHeaderItem`
- `ListTextElement`
- `ListLabelElement`
- `ListContainerElement`
- `ScrollBar`
- `Menu`
- `MenuElement`
- `HotKey`
- `PageControl`
- `Progress`
- `Slider`
- `DateTime`
- `GroupBox`

## 标准版控件

标准版在轻量版基础上增加依赖较轻但能明显提升业务界面表达能力的控件：

- `GifAnim`
- `IPAddress`
- `TreeView`
- `TreeNode`
- `RollText`

适用场景：

- 树形资源视图
- IP 地址输入
- 简单 GIF 状态展示
- 滚动文字提示

## 全量版控件

全量版在标准版基础上增加维护成本更高、依赖关系更重或更偏复杂业务场景的控件：

- `RichEdit`
- `ListEx`
- `ListTextExtElement`
- `FadeButton`

`ListEx` 直接依赖 `RichEdit` 做单元格编辑，因此与 `RichEdit` 同属全量版。

## 合并规则

- `GifAnimEx` 的能力合并到 `GifAnim`，XML 构建器兼容旧节点名。
- `IPAddressEx` 的能力合并到 `IPAddress`，XML 构建器兼容旧节点名。

## 删除组件

以下组件不再进入任何档位：

- `Animation`
- `AnimationTabLayout`
- `ColorPalette`
- `Loading`
- `Ring`
- `ActiveX`
- `WebBrowser`
- `Flash`

删除原因：

- 使用频率低，且增加库体积和维护成本。
- WebBrowser/Flash 已过时，新项目不应继续依赖。
- 加载状态可用 `Progress`、`Text`、`GifAnim` 或普通图片组合实现。
- 动画能力应作为后续独立效果层重新设计。

## DuiDesigner 对齐要求

DuiDesigner 是设计器，控件面板和属性面板必须始终按全量控件清单提供：

- 展示轻量版、标准版和全量版全部保留控件。
- 删除组件不再出现在工具箱和属性面板中。
- `SwitchButton` 已进入轻量版，也应在设计器基础控件中可创建。
- `ActiveX`、`WebBrowser`、`Flash` 不再出现在新版编辑器中。

## 后续评估规则

新增控件进入轻量版前必须满足：

- 高频业务场景需要。
- 不依赖浏览器、ActiveX、Flash 或大型第三方运行时。
- 不显著增加默认库体积。
- 能在 Win32/x64 下通过 VS2026 构建。

新增控件进入标准版前必须满足：

- 可明显提升通用业务界面能力。
- 依赖较轻，不强制引入复杂编辑器或大体积组件。
- 可通过 `DUI_CONTROLS_STANDARD` 完整隔离。

新增控件进入全量版前必须满足：

- 有明确业务场景。
- 维护边界清晰。
- 可通过 `DUI_CONTROLS_FULL` 完整隔离。
- 属性文档和 DuiDesigner 元数据同步更新。
