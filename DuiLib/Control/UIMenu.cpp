#include "StdAfx.h"
#include "UIMenu.h"

namespace DuiLib {

IMPLEMENT_DUICONTROL(CMenuUI)
IMPLEMENT_DUICONTROL(CMenuElementUI)

CMenuUI::CMenuUI()
{
    if( GetHeader() != NULL ) GetHeader()->SetVisible(false);
}

CMenuUI::~CMenuUI()
{
}

LPCTSTR CMenuUI::GetClass() const
{
    return _T("MenuUI");
}

LPVOID CMenuUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, DUI_CTR_MENU) == 0 ) return static_cast<CMenuUI*>(this);
    return CListUI::GetInterface(pstrName);
}

bool CMenuUI::Add(CControlUI* pControl)
{
    if( pControl == NULL || pControl->GetInterface(DUI_CTR_MENUELEMENT) == NULL ) return false;
    return CListUI::Add(pControl);
}

bool CMenuUI::AddAt(CControlUI* pControl, int iIndex)
{
    if( pControl == NULL || pControl->GetInterface(DUI_CTR_MENUELEMENT) == NULL ) return false;
    return CListUI::AddAt(pControl, iIndex);
}

int CMenuUI::GetItemIndex(CControlUI* pControl) const
{
    if( pControl == NULL || pControl->GetInterface(DUI_CTR_MENUELEMENT) == NULL ) return -1;
    return CListUI::GetItemIndex(pControl);
}

bool CMenuUI::SetItemIndex(CControlUI* pControl, int iIndex)
{
    if( pControl == NULL || pControl->GetInterface(DUI_CTR_MENUELEMENT) == NULL ) return false;
    return CListUI::SetItemIndex(pControl, iIndex);
}

bool CMenuUI::Remove(CControlUI* pControl)
{
    if( pControl == NULL || pControl->GetInterface(DUI_CTR_MENUELEMENT) == NULL ) return false;
    return CListUI::Remove(pControl);
}

CMenuElementUI::CMenuElementUI() :
    m_bLineType(false),
    m_bCheckItem(false),
    m_bChecked(false),
    m_bShowExplandIcon(false),
    m_dwLineColor(0xFFBCBFC4)
{
    ::SetRect(&m_rcLinePadding, 29, 2, 7, 2);
    m_szIconSize.cx = 16;
    m_szIconSize.cy = 16;
}

CMenuElementUI::~CMenuElementUI()
{
}

LPCTSTR CMenuElementUI::GetClass() const
{
    return _T("MenuElementUI");
}

LPVOID CMenuElementUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcsicmp(pstrName, DUI_CTR_MENUELEMENT) == 0 ) return static_cast<CMenuElementUI*>(this);
    return CListContainerElementUI::GetInterface(pstrName);
}

void CMenuElementUI::SetLineType()
{
    m_bLineType = true;
}

void CMenuElementUI::SetLineColor(DWORD color)
{
    m_dwLineColor = color;
    Invalidate();
}

DWORD CMenuElementUI::GetLineColor() const
{
    return m_dwLineColor;
}

void CMenuElementUI::SetLinePadding(RECT rcPadding)
{
    m_rcLinePadding = rcPadding;
    Invalidate();
}

RECT CMenuElementUI::GetLinePadding() const
{
    return m_rcLinePadding;
}

void CMenuElementUI::SetIcon(LPCTSTR strIcon)
{
    m_strIcon = strIcon;
    Invalidate();
}

void CMenuElementUI::SetIconSize(LONG cx, LONG cy)
{
    m_szIconSize.cx = cx;
    m_szIconSize.cy = cy;
    Invalidate();
}

SIZE CMenuElementUI::GetIconSize() const
{
    return m_szIconSize;
}

void CMenuElementUI::SetChecked(bool bCheck)
{
    m_bChecked = bCheck;
    Invalidate();
}

bool CMenuElementUI::GetChecked() const
{
    return m_bChecked;
}

void CMenuElementUI::SetCheckItem(bool bCheckItem)
{
    m_bCheckItem = bCheckItem;
    Invalidate();
}

bool CMenuElementUI::GetCheckItem() const
{
    return m_bCheckItem;
}

void CMenuElementUI::SetShowExplandIcon(bool bShow)
{
    m_bShowExplandIcon = bShow;
    Invalidate();
}

void CMenuElementUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcsicmp(pstrName, _T("linetype")) == 0 ) {
        if( _tcsicmp(pstrValue, _T("true")) == 0 ) SetLineType();
        else m_bLineType = false;
    }
    else if( _tcsicmp(pstrName, _T("linecolor")) == 0 ) {
        if( *pstrValue == _T('#') ) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        SetLineColor(_tcstoul(pstrValue, &pstr, 16));
    }
    else if( _tcsicmp(pstrName, _T("linepadding")) == 0 ) {
        RECT rcPadding = { 0 };
        LPTSTR pstr = NULL;
        rcPadding.left = _tcstol(pstrValue, &pstr, 10); ASSERT(pstr);
        rcPadding.top = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
        rcPadding.right = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
        rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
        SetLinePadding(rcPadding);
    }
    else if( _tcsicmp(pstrName, _T("icon")) == 0 ) SetIcon(pstrValue);
    else if( _tcsicmp(pstrName, _T("iconsize")) == 0 ) {
        LPTSTR pstr = NULL;
        LONG cx = _tcstol(pstrValue, &pstr, 10); ASSERT(pstr);
        LONG cy = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
        SetIconSize(cx, cy);
    }
    else if( _tcsicmp(pstrName, _T("checked")) == 0 ) SetChecked(_tcsicmp(pstrValue, _T("true")) == 0);
    else if( _tcsicmp(pstrName, _T("checkitem")) == 0 ) SetCheckItem(_tcsicmp(pstrValue, _T("true")) == 0);
    else if( _tcsicmp(pstrName, _T("showexpandicon")) == 0 ) SetShowExplandIcon(_tcsicmp(pstrValue, _T("true")) == 0);
    else CListContainerElementUI::SetAttribute(pstrName, pstrValue);
}

void CMenuElementUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
    DrawItemBk(hDC, m_rcItem);
    if( m_bLineType ) {
        RECT rcLine = {
            m_rcItem.left + m_rcLinePadding.left,
            m_rcItem.top + ((m_rcItem.bottom - m_rcItem.top) / 2),
            m_rcItem.right - m_rcLinePadding.right,
            m_rcItem.top + ((m_rcItem.bottom - m_rcItem.top) / 2)
        };
        CRenderEngine::DrawLine(hDC, rcLine, 1, GetAdjustColor(m_dwLineColor));
        return;
    }
    DrawItemIcon(hDC, m_rcItem);
    DrawItemText(hDC, m_rcItem);
    CContainerUI::DoPaint(hDC, rcPaint);
}

SIZE CMenuElementUI::EstimateSize(SIZE szAvailable)
{
    SIZE cxy = CListContainerElementUI::EstimateSize(szAvailable);
    if( cxy.cy == 0 ) cxy.cy = m_bLineType ? 6 : 30;
    if( cxy.cx == 0 ) cxy.cx = 150;
    return cxy;
}

void CMenuElementUI::DrawItemIcon(HDC hDC, const RECT& rcItem)
{
    if( m_strIcon.empty() ) return;
    RECT rcDest = {
        rcItem.left + 6,
        rcItem.top + ((rcItem.bottom - rcItem.top - m_szIconSize.cy) / 2),
        rcItem.left + 6 + m_szIconSize.cx,
        rcItem.top + ((rcItem.bottom - rcItem.top - m_szIconSize.cy) / 2) + m_szIconSize.cy
    };
    tstring sImageModify;
    DuiStringFormat(sImageModify, _T("dest='%d,%d,%d,%d'"), rcDest.left - rcItem.left, rcDest.top - rcItem.top, rcDest.right - rcItem.left, rcDest.bottom - rcItem.top);
    DrawImage(hDC, m_strIcon, sImageModify);
}

void CMenuElementUI::DrawItemText(HDC hDC, const RECT& rcItem)
{
    if( m_pOwner == NULL ) return;
    tstring sText = GetText();
    if( sText.empty() ) return;

    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iTextColor = pInfo->dwTextColor;
    if( (m_uButtonState & UISTATE_HOT) != 0 ) iTextColor = pInfo->dwHotTextColor;
    if( IsSelected() ) iTextColor = pInfo->dwSelectedTextColor;
    if( !IsEnabled() ) iTextColor = pInfo->dwDisabledTextColor;

    RECT rcText = rcItem;
    rcText.left += 30;
    rcText.right -= m_bShowExplandIcon ? 24 : 8;
    rcText.top += 2;
    rcText.bottom -= 2;
    CRenderEngine::DrawText(hDC, m_pManager, rcText, sText.c_str(), iTextColor, pInfo->nFont, DT_SINGLELINE | DT_VCENTER | pInfo->uTextStyle);
}

}
