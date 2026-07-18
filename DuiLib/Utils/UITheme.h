#pragma once

#include <functional>

namespace DuiLib {

class CControlUI;
class CPaintManagerUI;

using DuiColorMapper = std::function<DWORD(DWORD)>;
using DuiThemeControlCustomizer = std::function<void(CControlUI*)>;

class UILIB_API CThemeEngine final
{
public:
    static bool Apply(CPaintManagerUI* manager, const DuiColorMapper& mapper,
        const DuiThemeControlCustomizer& customizer = {});
    static bool Apply(CControlUI* root, const DuiColorMapper& mapper,
        const DuiThemeControlCustomizer& customizer = {});
};

} // namespace DuiLib
