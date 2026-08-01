#include "StdAfx.h"
#include "UITheme.h"

namespace DuiLib {
namespace {

DWORD Map(const DuiColorMapper& mapper, DWORD color)
{
    return mapper && color != 0 ? mapper(color) : color;
}

void ApplyOne(CControlUI* control, const DuiColorMapper& mapper,
    const DuiThemeControlCustomizer& customizer)
{
    if( control == NULL ) return;
    control->SetBkColor(Map(mapper, control->GetBkColor()));
    control->SetBkColor2(Map(mapper, control->GetBkColor2()));
    control->SetBkColor3(Map(mapper, control->GetBkColor3()));
    control->SetBorderColor(Map(mapper, control->GetBorderColor()));
    control->SetFocusBorderColor(Map(mapper, control->GetFocusBorderColor()));

    if( CLabelUI* label = static_cast<CLabelUI*>(control->GetInterface(_T("Label"))) ) {
        label->SetTextColor(Map(mapper, label->GetTextColor()));
        label->SetDisabledTextColor(Map(mapper, label->GetDisabledTextColor()));
    }
    if( CEditUI* edit = static_cast<CEditUI*>(
        control->GetInterface(DUI_CTR_EDIT)) ) {
        edit->SetHintTextColor(Map(mapper, edit->GetHintTextColor()));
    }
    if( CButtonUI* button = static_cast<CButtonUI*>(control->GetInterface(DUI_CTR_BUTTON)) ) {
        button->SetHotBkColor(Map(mapper, button->GetHotBkColor()));
        button->SetHotTextColor(Map(mapper, button->GetHotTextColor()));
        button->SetPushedTextColor(Map(mapper, button->GetPushedTextColor()));
        button->SetFocusedTextColor(Map(mapper, button->GetFocusedTextColor()));
    }
    if( COptionUI* option = static_cast<COptionUI*>(control->GetInterface(DUI_CTR_OPTION)) ) {
        option->SetSelectedBkColor(Map(mapper, option->GetSelectBkColor()));
        option->SetSelectedTextColor(Map(mapper, option->GetSelectedTextColor()));
    }
    if( CSwitchButtonUI* toggle = static_cast<CSwitchButtonUI*>(control->GetInterface(DUI_CTR_SWITCHBUTTON)) ) {
        toggle->SetOnColor(Map(mapper, toggle->GetOnColor()));
        toggle->SetOffColor(Map(mapper, toggle->GetOffColor()));
        toggle->SetDisabledColor(Map(mapper, toggle->GetDisabledColor()));
        toggle->SetThumbColor(Map(mapper, toggle->GetThumbColor()));
        toggle->SetHotThumbColor(Map(mapper, toggle->GetHotThumbColor()));
    }
    if( CListUI* list = static_cast<CListUI*>(control->GetInterface(DUI_CTR_LIST)) ) {
        list->SetItemTextColor(Map(mapper, list->GetItemTextColor()));
        list->SetItemBkColor(Map(mapper, list->GetItemBkColor()));
        list->SetSelectedItemTextColor(Map(mapper, list->GetSelectedItemTextColor()));
        list->SetSelectedItemBkColor(Map(mapper, list->GetSelectedItemBkColor()));
        list->SetHotItemTextColor(Map(mapper, list->GetHotItemTextColor()));
        list->SetHotItemBkColor(Map(mapper, list->GetHotItemBkColor()));
        list->SetDisabledItemTextColor(Map(mapper, list->GetDisabledItemTextColor()));
        list->SetDisabledItemBkColor(Map(mapper, list->GetDisabledItemBkColor()));
        list->SetItemLineColor(Map(mapper, list->GetItemLineColor()));
    }
    if( CComboUI* combo = static_cast<CComboUI*>(control->GetInterface(DUI_CTR_COMBO)) ) {
        combo->SetItemTextColor(Map(mapper, combo->GetItemTextColor()));
        combo->SetItemBkColor(Map(mapper, combo->GetItemBkColor()));
        combo->SetSelectedItemTextColor(Map(mapper, combo->GetSelectedItemTextColor()));
        combo->SetSelectedItemBkColor(Map(mapper, combo->GetSelectedItemBkColor()));
        combo->SetHotItemTextColor(Map(mapper, combo->GetHotItemTextColor()));
        combo->SetHotItemBkColor(Map(mapper, combo->GetHotItemBkColor()));
        combo->SetDisabledItemTextColor(Map(mapper, combo->GetDisabledItemTextColor()));
        combo->SetDisabledItemBkColor(Map(mapper, combo->GetDisabledItemBkColor()));
        combo->SetItemLineColor(Map(mapper, combo->GetItemLineColor()));
    }
#if DUI_HAS_FULL_CONTROLS
    if( CRichEditUI* richEdit = static_cast<CRichEditUI*>(
        control->GetInterface(DUI_CTR_RICHEDIT)) ) {
        richEdit->SetTextColor(Map(mapper, richEdit->GetTextColor()));
    }
    if( CGroupedListRowUI* row = static_cast<CGroupedListRowUI*>(
        control->GetInterface(DUI_CTR_GROUPEDLISTROW)) ) {
        row->SetHotBkColor(Map(mapper, row->GetHotBkColor()));
    }
#endif
    if( customizer ) customizer(control);

    IContainerUI* container = static_cast<IContainerUI*>(control->GetInterface(_T("IContainer")));
    if( container != NULL ) {
        for( int index = 0; index < container->GetCount(); ++index )
            ApplyOne(container->GetItemAt(index), mapper, customizer);
    }
    control->Invalidate();
}

} // namespace

bool CThemeEngine::Apply(CPaintManagerUI* manager, const DuiColorMapper& mapper,
    const DuiThemeControlCustomizer& customizer)
{
    if( manager == NULL || manager->GetRoot() == NULL || !mapper ) return false;
    ApplyOne(manager->GetRoot(), mapper, customizer);
    manager->NeedUpdate();
    return true;
}

bool CThemeEngine::Apply(CControlUI* root, const DuiColorMapper& mapper,
    const DuiThemeControlCustomizer& customizer)
{
    if( root == NULL || !mapper ) return false;
    ApplyOne(root, mapper, customizer);
    return true;
}

} // namespace DuiLib
