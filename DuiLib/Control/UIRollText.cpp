#include "stdafx.h"
#include "UIRollText.h"

#if DUI_HAS_STANDARD_CONTROLS

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CRollTextUI)

	CRollTextUI::CRollTextUI(void)
	{
		m_nScrollPos = 0;
		m_nText_W_H = 0;
		m_nStep = 5;
		m_bUseRoll = FALSE;
		m_nRollDirection = ROLLTEXT_LEFT;
	}

	CRollTextUI::~CRollTextUI(void)
	{
		if (m_pManager != NULL) {
			m_pManager->KillTimer(this, ROLLTEXT_ROLL_END);
			m_pManager->KillTimer(this, ROLLTEXT_TIMERID);
		}
	}

	LPCTSTR CRollTextUI::GetClass() const
	{
		return _T("RollTextUI");
	}

	LPVOID CRollTextUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("RollText")) == 0 ) return static_cast<CRollTextUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	void CRollTextUI::BeginRoll(int nDirect, LONG lTimeSpan, LONG lMaxTimeLimited)
	{
		m_nRollDirection = nDirect;
		if (m_bUseRoll)
		{
			EndRoll();
		}
		m_nText_W_H = 0;
		if (m_pManager == NULL) return;

		m_pManager->KillTimer(this, ROLLTEXT_TIMERID);
		m_pManager->SetTimer(this, ROLLTEXT_TIMERID, lTimeSpan);

		m_pManager->KillTimer(this, ROLLTEXT_ROLL_END);
		m_pManager->SetTimer(this, ROLLTEXT_ROLL_END, lMaxTimeLimited*1000);

		m_bUseRoll = TRUE;
	}

	void CRollTextUI::EndRoll()
	{
		if (!m_bUseRoll) return;

		if (m_pManager != NULL) {
			m_pManager->KillTimer(this, ROLLTEXT_ROLL_END);
			m_pManager->KillTimer(this, ROLLTEXT_TIMERID);
		}

		m_bUseRoll = FALSE;
	}

	void CRollTextUI::SetPos(RECT rc)
	{
		CLabelUI::SetPos(rc);
		m_nText_W_H = 0;			// 控件尺寸变化时重新计算文本滚动范围
	}

	void CRollTextUI::SetText( LPCTSTR pstrText )
	{
		CLabelUI::SetText(pstrText);
		m_nText_W_H = 0;			// 文本变化时重新计算文本滚动范围
	}

	void CRollTextUI::DoEvent(TEventUI& event)
	{
		if (event.Type == UIEVENT_TIMER && event.wParam == ROLLTEXT_ROLL_END)
		{
			if (m_pManager != NULL) {
				m_pManager->KillTimer(this, ROLLTEXT_ROLL_END);
				m_pManager->SendNotify(this, DUI_MSGTYPE_TEXTROLLEND);
			}
		}
		else if( event.Type == UIEVENT_TIMER && event.wParam == ROLLTEXT_TIMERID )
		{
			Invalidate();
			return;
		}
		CLabelUI::DoEvent(event);
	}

	void CRollTextUI::PaintText(HDC hDC)
	{
		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();
		DWORD dwTextColor = IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor;
		tstring sText = GetText();
		if( sText.empty() ) return;

		RECT rcTextPadding = GetTextPadding();
		CDuiRect  rcClient;
		rcClient = m_rcItem;
		rcClient.left += rcTextPadding.left;
		rcClient.right -= rcTextPadding.right;
		rcClient.top += rcTextPadding.top;
		rcClient.bottom -= rcTextPadding.bottom;

		if (m_nText_W_H == 0) {
			RECT rcMeasure = rcClient;
			UINT uMeasureStyle = DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT;
			if (m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT) {
				rcMeasure.right += 10000;
			}
			else {
				rcMeasure.bottom += 10000;
			}

			if (m_bShowHtml) {
				int nLinks = 0;
				CRenderEngine::DrawHtmlText(hDC, m_pManager, rcMeasure, sText, dwTextColor, NULL, NULL, nLinks, uMeasureStyle);
			}
			else {
				CRenderEngine::DrawText(hDC, m_pManager, rcMeasure, sText, dwTextColor, m_iFont, uMeasureStyle);
			}

			m_nText_W_H = (m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT)
				? (rcMeasure.right - rcMeasure.left)
				: (rcMeasure.bottom - rcMeasure.top);
		}

		RECT rc = rcClient;
		if (m_bUseRoll && m_nText_W_H > 0) {
			int nScrollRange = 0;

			if (m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT) {	// 水平滚动
				nScrollRange = m_nText_W_H + rcClient.GetWidth();

				rcClient.Offset((m_nRollDirection == ROLLTEXT_LEFT?rcClient.GetWidth():-rcClient.GetWidth()), 0);
				rcClient.Offset((m_nRollDirection == ROLLTEXT_LEFT?-m_nScrollPos:m_nScrollPos), 0);
				rcClient.right += (m_nText_W_H - rcClient.GetWidth());
			}
			else {															// 垂直滚动
				nScrollRange = m_nText_W_H + rcClient.GetHeight();
				rcClient.Offset(0, (m_nRollDirection == ROLLTEXT_UP?rcClient.GetHeight():-rcClient.GetHeight()));
				rcClient.Offset(0, (m_nRollDirection == ROLLTEXT_UP?-m_nScrollPos:m_nScrollPos));
				rcClient.bottom += (m_nText_W_H - rcClient.GetHeight());
			}

			m_nScrollPos += m_nStep;
			if (m_nScrollPos > nScrollRange) {
				m_nScrollPos = 0;
			}
		}

		UINT uTextStyle = DT_WORDBREAK | DT_EDITCONTROL;

		CRenderClip clip;
		CRenderClip::GenerateClip(hDC, rcClient, clip);
		if( m_bShowHtml ) {
			int nLinks = 0;
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText.c_str(), dwTextColor, NULL, NULL, nLinks, uTextStyle);
		}
		else {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText.c_str(), dwTextColor, m_iFont, uTextStyle);
		}

	}
}

#endif
