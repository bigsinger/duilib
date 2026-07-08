#include "stdafx.h"
#include "UISwitchButton.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

namespace DuiLib
{
	namespace
	{
		class SharedGdiplusScope
		{
		public:
			SharedGdiplusScope()
			{
				Gdiplus::GdiplusStartup(&token_, &input_, NULL);
			}

			~SharedGdiplusScope()
			{
				if( token_ != 0 ) {
					Gdiplus::GdiplusShutdown(token_);
					token_ = 0;
				}
			}

		private:
			ULONG_PTR token_ = 0;
			Gdiplus::GdiplusStartupInput input_;
		};

		void EnsureSharedGdiplus()
		{
			static SharedGdiplusScope scope;
		}

		DWORD ParseSwitchColor(LPCTSTR pstrValue)
		{
			if( pstrValue == NULL ) return 0;
			if( *pstrValue == _T('#') ) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			return _tcstoul(pstrValue, &pstr, 16);
		}

		BYTE ColorChannel(DWORD dwColor, int shift)
		{
			return static_cast<BYTE>((dwColor >> shift) & 0xFF);
		}

		Gdiplus::Color BlendArgb(DWORD baseColor, DWORD overlayColor, float overlayWeight)
		{
			const BYTE baseA = ColorChannel(baseColor, 24);
			const BYTE baseR = ColorChannel(baseColor, 16);
			const BYTE baseG = ColorChannel(baseColor, 8);
			const BYTE baseB = ColorChannel(baseColor, 0);
			const BYTE overR = ColorChannel(overlayColor, 16);
			const BYTE overG = ColorChannel(overlayColor, 8);
			const BYTE overB = ColorChannel(overlayColor, 0);
			const float baseWeight = 1.0f - overlayWeight;
			return Gdiplus::Color(
				baseA,
				static_cast<BYTE>(baseR * baseWeight + overR * overlayWeight),
				static_cast<BYTE>(baseG * baseWeight + overG * overlayWeight),
				static_cast<BYTE>(baseB * baseWeight + overB * overlayWeight));
		}

		void AddRoundRect(Gdiplus::GraphicsPath& path, const Gdiplus::RectF& rc, float radius)
		{
			const float diameter = radius * 2.0f;
			path.AddArc(rc.X, rc.Y, diameter, diameter, 180.0f, 90.0f);
			path.AddArc(rc.X + rc.Width - diameter, rc.Y, diameter, diameter, 270.0f, 90.0f);
			path.AddArc(rc.X + rc.Width - diameter, rc.Y + rc.Height - diameter, diameter, diameter, 0.0f, 90.0f);
			path.AddArc(rc.X, rc.Y + rc.Height - diameter, diameter, diameter, 90.0f, 90.0f);
			path.CloseFigure();
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
		EnsureSharedGdiplus();

		RECT rcTrack = m_rcItem;
		const int nHeight = rcTrack.bottom - rcTrack.top;
		const int nWidth = rcTrack.right - rcTrack.left;
		if( nHeight <= 0 || nWidth <= 0 ) return;

		const float nInset = static_cast<float>(max(3, nHeight / 8));
		const float nThumb = max(1.0f, static_cast<float>(nHeight) - nInset * 2.0f);
		const DWORD dwTrackColor = !IsEnabled() ? m_dwDisabledColor : (m_bSelected ? m_dwOnColor : m_dwOffColor);
		const DWORD dwThumbColor = ((m_uButtonState & UISTATE_HOT) != 0 && IsEnabled()) ? m_dwHotThumbColor : m_dwThumbColor;

		Gdiplus::Graphics graphics(hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);

		Gdiplus::RectF track(
			static_cast<Gdiplus::REAL>(rcTrack.left) + 0.5f,
			static_cast<Gdiplus::REAL>(rcTrack.top) + 0.5f,
			static_cast<Gdiplus::REAL>(nWidth) - 1.0f,
			static_cast<Gdiplus::REAL>(nHeight) - 1.0f);
		Gdiplus::GraphicsPath trackPath;
		AddRoundRect(trackPath, track, track.Height / 2.0f);

		const DWORD adjustedTrack = GetAdjustColor(dwTrackColor);
		Gdiplus::LinearGradientBrush trackBrush(
			track,
			BlendArgb(adjustedTrack, 0xFFFFFFFF, IsEnabled() ? 0.10f : 0.04f),
			BlendArgb(adjustedTrack, 0xFF000000, IsEnabled() ? 0.08f : 0.02f),
			Gdiplus::LinearGradientModeVertical);
		graphics.FillPath(&trackBrush, &trackPath);

		Gdiplus::Pen trackBorder(
			BlendArgb(adjustedTrack, m_bSelected ? 0xFFFFFFFF : 0xFF000000, m_bSelected ? 0.22f : 0.18f),
			1.0f);
		graphics.DrawPath(&trackBorder, &trackPath);

		float thumbLeft = static_cast<float>(rcTrack.left) + nInset;
		if( m_bSelected ) {
			thumbLeft = static_cast<float>(rcTrack.right) - nInset - nThumb;
		}

		Gdiplus::RectF shadow(
			thumbLeft,
			static_cast<float>(rcTrack.top) + nInset + 1.0f,
			nThumb,
			nThumb);
		Gdiplus::SolidBrush shadowBrush(Gdiplus::Color(IsEnabled() ? 55 : 28, 0, 0, 0));
		graphics.FillEllipse(&shadowBrush, shadow);

		Gdiplus::RectF thumb(
			thumbLeft,
			static_cast<float>(rcTrack.top) + nInset,
			nThumb,
			nThumb);
		const DWORD adjustedThumb = GetAdjustColor(dwThumbColor);
		Gdiplus::LinearGradientBrush thumbBrush(
			thumb,
			BlendArgb(adjustedThumb, 0xFFFFFFFF, 0.18f),
			BlendArgb(adjustedThumb, 0xFF000000, 0.08f),
			Gdiplus::LinearGradientModeVertical);
		graphics.FillEllipse(&thumbBrush, thumb);

		Gdiplus::Pen thumbBorder(Gdiplus::Color(IsEnabled() ? 90 : 40, 0, 0, 0), 1.0f);
		graphics.DrawEllipse(&thumbBorder, thumb);
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

		RECT rcText = rc;
		SIZE szText = CRenderEngine::GetTextSize(hDC, m_pManager, sPaintText.c_str(), m_iFont, DT_SINGLELINE);
		int cyItem = rc.bottom - rc.top;
		if( szText.cy > 0 && cyItem > szText.cy ) {
			rcText.top = rc.top + (cyItem - szText.cy) / 2;
			rcText.bottom = rcText.top + szText.cy;
		}

		CRenderEngine::DrawText(hDC, m_pManager, rcText, sPaintText.c_str(), GetAdjustColor(dwTextColor), m_iFont, m_uTextStyle | DT_VCENTER);
	}
}
