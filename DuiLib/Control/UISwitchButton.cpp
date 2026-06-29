#include "stdafx.h"
#include "UISwitchButton.h"

namespace DuiLib
{
	namespace
	{
		DWORD ParseSwitchColor(LPCTSTR pstrValue)
		{
			if( pstrValue == NULL ) return 0;
			if( *pstrValue == _T('#') ) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			return _tcstoul(pstrValue, &pstr, 16);
		}

		COLORREF ToColorRef(DWORD dwColor)
		{
			return RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
		}

		void FillRoundRect(HDC hDC, const RECT& rc, int nRound, DWORD dwColor)
		{
			HBRUSH hBrush = ::CreateSolidBrush(ToColorRef(dwColor));
			HPEN hPen = ::CreatePen(PS_SOLID, 1, ToColorRef(dwColor));
			HGDIOBJ hOldBrush = ::SelectObject(hDC, hBrush);
			HGDIOBJ hOldPen = ::SelectObject(hDC, hPen);
			::RoundRect(hDC, rc.left, rc.top, rc.right, rc.bottom, nRound, nRound);
			::SelectObject(hDC, hOldBrush);
			::SelectObject(hDC, hOldPen);
			::DeleteObject(hPen);
			::DeleteObject(hBrush);
		}
	}

	CSwitchButtonUI::CSwitchButtonUI()
		: m_bSelected(false)
		, m_dwOnColor(0xFF2E8B57)
		, m_dwOffColor(0xFF9CA3AF)
		, m_dwDisabledColor(0xFFD1D5DB)
		, m_dwThumbColor(0xFFFFFFFF)
		, m_dwHotThumbColor(0xFFF3F4F6)
	{
		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	}

	LPCTSTR CSwitchButtonUI::GetClass() const
	{
		return _T("SwitchButtonUI");
	}

	LPVOID CSwitchButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_SWITCHBUTTON) == 0 ) return static_cast<CSwitchButtonUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}

	bool CSwitchButtonUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		SetSelected(!m_bSelected);
		if( m_pManager != NULL ) {
			m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
			m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		}
		return true;
	}

	bool CSwitchButtonUI::IsSelected() const
	{
		return m_bSelected;
	}

	void CSwitchButtonUI::SetSelected(bool bSelected)
	{
		if( m_bSelected == bSelected ) return;
		m_bSelected = bSelected;
		Invalidate();
	}

	void CSwitchButtonUI::SetCheck(bool bCheck)
	{
		SetSelected(bCheck);
	}

	bool CSwitchButtonUI::GetCheck() const
	{
		return IsSelected();
	}

	void CSwitchButtonUI::SetOnText(LPCTSTR pstrText)
	{
		m_sOnText = pstrText == NULL ? _T("") : pstrText;
		Invalidate();
	}

	LPCTSTR CSwitchButtonUI::GetOnText() const
	{
		return m_sOnText.c_str();
	}

	void CSwitchButtonUI::SetOffText(LPCTSTR pstrText)
	{
		m_sOffText = pstrText == NULL ? _T("") : pstrText;
		Invalidate();
	}

	LPCTSTR CSwitchButtonUI::GetOffText() const
	{
		return m_sOffText.c_str();
	}

	void CSwitchButtonUI::SetOnColor(DWORD dwColor)
	{
		m_dwOnColor = dwColor;
		Invalidate();
	}

	DWORD CSwitchButtonUI::GetOnColor() const
	{
		return m_dwOnColor;
	}

	void CSwitchButtonUI::SetOffColor(DWORD dwColor)
	{
		m_dwOffColor = dwColor;
		Invalidate();
	}

	DWORD CSwitchButtonUI::GetOffColor() const
	{
		return m_dwOffColor;
	}

	void CSwitchButtonUI::SetDisabledColor(DWORD dwColor)
	{
		m_dwDisabledColor = dwColor;
		Invalidate();
	}

	DWORD CSwitchButtonUI::GetDisabledColor() const
	{
		return m_dwDisabledColor;
	}

	void CSwitchButtonUI::SetThumbColor(DWORD dwColor)
	{
		m_dwThumbColor = dwColor;
		Invalidate();
	}

	DWORD CSwitchButtonUI::GetThumbColor() const
	{
		return m_dwThumbColor;
	}

	void CSwitchButtonUI::SetHotThumbColor(DWORD dwColor)
	{
		m_dwHotThumbColor = dwColor;
		Invalidate();
	}

	DWORD CSwitchButtonUI::GetHotThumbColor() const
	{
		return m_dwHotThumbColor;
	}

	SIZE CSwitchButtonUI::EstimateSize(SIZE szAvailable)
	{
		SIZE sz = CButtonUI::EstimateSize(szAvailable);
		if( m_cxyFixed.cx == 0 ) sz.cx = 48;
		if( m_cxyFixed.cy == 0 ) sz.cy = 24;
		return sz;
	}

	void CSwitchButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("selected")) == 0 || _tcscmp(pstrName, _T("checked")) == 0 ) {
			SetSelected(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("ontext")) == 0 ) SetOnText(pstrValue);
		else if( _tcscmp(pstrName, _T("offtext")) == 0 ) SetOffText(pstrValue);
		else if( _tcscmp(pstrName, _T("oncolor")) == 0 ) SetOnColor(ParseSwitchColor(pstrValue));
		else if( _tcscmp(pstrName, _T("offcolor")) == 0 ) SetOffColor(ParseSwitchColor(pstrValue));
		else if( _tcscmp(pstrName, _T("disabledcolor")) == 0 ) SetDisabledColor(ParseSwitchColor(pstrValue));
		else if( _tcscmp(pstrName, _T("thumbcolor")) == 0 ) SetThumbColor(ParseSwitchColor(pstrValue));
		else if( _tcscmp(pstrName, _T("hotthumbcolor")) == 0 ) SetHotThumbColor(ParseSwitchColor(pstrValue));
		else CButtonUI::SetAttribute(pstrName, pstrValue);
	}

	void CSwitchButtonUI::PaintStatusImage(HDC hDC)
	{
		RECT rcTrack = m_rcItem;
		const int nHeight = rcTrack.bottom - rcTrack.top;
		const int nWidth = rcTrack.right - rcTrack.left;
		if( nHeight <= 0 || nWidth <= 0 ) return;

		const int nInset = max(2, nHeight / 12);
		const int nThumb = max(1, nHeight - nInset * 2);
		const DWORD dwTrackColor = !IsEnabled() ? m_dwDisabledColor : (m_bSelected ? m_dwOnColor : m_dwOffColor);
		const DWORD dwThumbColor = ((m_uButtonState & UISTATE_HOT) != 0 && IsEnabled()) ? m_dwHotThumbColor : m_dwThumbColor;

		FillRoundRect(hDC, rcTrack, nHeight, GetAdjustColor(dwTrackColor));

		RECT rcThumb = { 0 };
		rcThumb.top = rcTrack.top + nInset;
		rcThumb.bottom = rcThumb.top + nThumb;
		if( m_bSelected ) {
			rcThumb.right = rcTrack.right - nInset;
			rcThumb.left = rcThumb.right - nThumb;
		}
		else {
			rcThumb.left = rcTrack.left + nInset;
			rcThumb.right = rcThumb.left + nThumb;
		}
		FillRoundRect(hDC, rcThumb, nThumb, GetAdjustColor(dwThumbColor));
	}

	void CSwitchButtonUI::PaintText(HDC hDC)
	{
		const tstring& sPaintText = m_bSelected ? m_sOnText : m_sOffText;
		if( sPaintText.empty() ) return;

		DWORD dwTextColor = IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor;
		if( dwTextColor == 0 ) {
			dwTextColor = IsEnabled() ? 0xFFFFFFFF : m_pManager->GetDefaultDisabledColor();
		}

		RECT rc = m_rcItem;
		if( m_bSelected ) rc.right -= (rc.bottom - rc.top);
		else rc.left += (rc.bottom - rc.top);

		CRenderEngine::DrawText(hDC, m_pManager, rc, sPaintText.c_str(), GetAdjustColor(dwTextColor), m_iFont, m_uTextStyle);
	}
}
