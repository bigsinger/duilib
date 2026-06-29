#pragma once

// 控件档位开关：1=轻量版，2=标准版，4=全量版。
#define DUI_CONTROLS_LIGHT    0x01
#define DUI_CONTROLS_STANDARD 0x02
#define DUI_CONTROLS_FULL     0x04

// 默认启用三档全部控件；如需裁剪，请把下一行改为需要的标志位组合。
#define DUI_CONTROLS_FLAG (DUI_CONTROLS_LIGHT | DUI_CONTROLS_STANDARD | DUI_CONTROLS_FULL)
