#ifndef __UIMENU_H__
#define __UIMENU_H__

#pragma once

namespace DuiLib {

class UILIB_API CMenuElementUI;

class UILIB_API CMenuUI : public CListUI
{
    DECLARE_DUICONTROL(CMenuUI)
public:
    CMenuUI();
    virtual ~CMenuUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    int GetItemIndex(CControlUI* pControl) const;
    bool SetItemIndex(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl);
};

class UILIB_API CMenuElementUI : public CListContainerElementUI
{
    DECLARE_DUICONTROL(CMenuElementUI)
public:
    CMenuElementUI();
    virtual ~CMenuElementUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetLineType();
    void SetLineColor(DWORD color);
    DWORD GetLineColor() const;
    void SetLinePadding(RECT rcPadding);
    RECT GetLinePadding() const;

    void SetIcon(LPCTSTR strIcon);
    void SetIconSize(LONG cx, LONG cy);
    SIZE GetIconSize() const;

    void SetChecked(bool bCheck = true);
    bool GetChecked() const;
    void SetCheckItem(bool bCheckItem = false);
    bool GetCheckItem() const;
    void SetShowExplandIcon(bool bShow);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void DoPaint(HDC hDC, const RECT& rcPaint);
    SIZE EstimateSize(SIZE szAvailable);

protected:
    void DrawItemIcon(HDC hDC, const RECT& rcItem);
    void DrawItemText(HDC hDC, const RECT& rcItem);

private:
    bool m_bLineType;
    bool m_bCheckItem;
    bool m_bChecked;
    bool m_bShowExplandIcon;
    DWORD m_dwLineColor;
    RECT m_rcLinePadding;
    tstring m_strIcon;
    SIZE m_szIconSize;
};

}

#endif
