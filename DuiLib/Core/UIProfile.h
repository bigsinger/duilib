#pragma once

// 控件档位开关：1=轻量版，2=标准版，4=全量版。
#ifndef DUI_CONTROLS_LIGHT
#define DUI_CONTROLS_LIGHT    0x01
#endif

#ifndef DUI_CONTROLS_STANDARD
#define DUI_CONTROLS_STANDARD 0x02
#endif

#ifndef DUI_CONTROLS_FULL
#define DUI_CONTROLS_FULL     0x04
#endif

// 默认启用三档全部控件；工程文件或使用者代码可在包含 UIlib.h 前覆盖该宏。
#ifndef DUI_CONTROLS_FLAG
#define DUI_CONTROLS_FLAG (DUI_CONTROLS_LIGHT | DUI_CONTROLS_STANDARD | DUI_CONTROLS_FULL)
#endif
