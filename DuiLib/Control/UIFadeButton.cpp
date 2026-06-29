#include "StdAfx.h"
#include "UIFadeButton.h"

#if DUI_HAS_FULL_CONTROLS

namespace DuiLib {
	IMPLEMENT_DUICONTROL(CFadeButtonUI)

	CFadeButtonUI::CFadeButtonUI()
	{
	}

	CFadeButtonUI::~CFadeButtonUI()
	{
	}

	LPCTSTR CFadeButtonUI::GetClass() const
	{
		return _T("FadeButtonUI");
	}

	LPVOID CFadeButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("FadeButton")) == 0 ) 
			return static_cast<CFadeButtonUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}

	void CFadeButtonUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		m_sLastImage = m_sNormalImage;
	}

	void CFadeButtonUI::DoEvent(TEventUI& event)
	{
		CButtonUI::DoEvent( event );
	}

	void CFadeButtonUI::OnTimer( int nTimerID )
	{
	}

	void CFadeButtonUI::PaintStatusImage(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if( !m_sDisabledImage.empty() ) {
				if( !DrawImage(hDC, m_sDisabledImage.c_str()) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if( !m_sPushedImage.empty() ) {
				if( !DrawImage(hDC, m_sPushedImage.c_str()) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( !m_sFocusedImage.empty() ) {
				if( !DrawImage(hDC, m_sFocusedImage.c_str()) ) {}
				else return;
			}
		}
		if( !m_sNormalImage.empty() ) {
			LPCTSTR pstrImage = m_sNormalImage.c_str();
			if( (m_uButtonState & UISTATE_HOT) != 0 && !m_sHotImage.empty() ) {
				pstrImage = m_sHotImage.c_str();
			}
			if( !DrawImage(hDC, pstrImage) ) {}
				return;
		}
	}

}

#endif
