#include "stdafx.h"
#include "UIHotKey.h"
#include <algorithm>
namespace DuiLib{
	namespace {
		struct HotKeyNameEntry
		{
			WORD key;
			LPCTSTR name;
		};

		const HotKeyNameEntry kHotKeyNames[] = {
			{ VK_SPACE, _T("Space") }, { VK_RETURN, _T("Enter") },
			{ VK_ESCAPE, _T("Esc") }, { VK_TAB, _T("Tab") },
			{ VK_BACK, _T("Backspace") }, { VK_DELETE, _T("Delete") },
			{ VK_INSERT, _T("Insert") }, { VK_HOME, _T("Home") },
			{ VK_END, _T("End") }, { VK_PRIOR, _T("PageUp") },
			{ VK_NEXT, _T("PageDown") }, { VK_LEFT, _T("Left") },
			{ VK_RIGHT, _T("Right") }, { VK_UP, _T("Up") },
			{ VK_DOWN, _T("Down") }, { VK_SNAPSHOT, _T("PrintScreen") },
			{ VK_PAUSE, _T("Pause") }, { VK_CAPITAL, _T("CapsLock") },
			{ VK_NUMLOCK, _T("NumLock") }, { VK_SCROLL, _T("ScrollLock") },
			{ VK_MULTIPLY, _T("Multiply") }, { VK_ADD, _T("Add") },
			{ VK_SUBTRACT, _T("Subtract") }, { VK_DECIMAL, _T("Decimal") },
			{ VK_DIVIDE, _T("Divide") }, { VK_OEM_PLUS, _T("Plus") },
			{ VK_OEM_MINUS, _T("Minus") }, { VK_OEM_COMMA, _T("Comma") },
			{ VK_OEM_PERIOD, _T("Period") }, { VK_OEM_1, _T("Semicolon") },
			{ VK_OEM_2, _T("Slash") }, { VK_OEM_3, _T("Backtick") },
			{ VK_OEM_4, _T("LeftBracket") }, { VK_OEM_5, _T("Backslash") },
			{ VK_OEM_6, _T("RightBracket") }, { VK_OEM_7, _T("Quote") },
			{ VK_ESCAPE, _T("Escape") }, { VK_DELETE, _T("Del") },
			{ VK_INSERT, _T("Ins") },
		};

		tstring TrimAndLower(tstring value)
		{
			const tstring whitespace = _T(" \t\r\n");
			const size_t first = value.find_first_not_of(whitespace);
			if( first == tstring::npos ) return tstring();
			const size_t last = value.find_last_not_of(whitespace);
			value = value.substr(first, last - first + 1);
			std::transform(value.begin(), value.end(), value.begin(),
				[](TCHAR ch) { return static_cast<TCHAR>(_totlower(ch)); });
			return value;
		}

		tstring NumberText(UINT value)
		{
			TCHAR buffer[16] = { 0 };
			_stprintf_s(buffer, _countof(buffer), _T("%u"), value);
			return buffer;
		}

		WORD KeyFromCanonicalName(tstring value)
		{
			value = TrimAndLower(std::move(value));
			if( value.size() == 1 ) {
				const TCHAR ch = static_cast<TCHAR>(_totupper(value[0]));
				if( (ch >= _T('A') && ch <= _T('Z')) ||
					(ch >= _T('0') && ch <= _T('9')) ) return static_cast<WORD>(ch);
			}
			if( value.size() > 1 && value[0] == _T('f') ) {
				const int functionKey = _ttoi(value.c_str() + 1);
				if( functionKey >= 1 && functionKey <= 24 &&
					value == _T("f") + NumberText(functionKey) ) {
					return static_cast<WORD>(VK_F1 + functionKey - 1);
				}
			}
			if( value.compare(0, 6, _T("numpad")) == 0 ) {
				const int digit = _ttoi(value.c_str() + 6);
				if( digit >= 0 && digit <= 9 &&
					value == _T("numpad") + NumberText(digit) ) {
					return static_cast<WORD>(VK_NUMPAD0 + digit);
				}
			}
			for( const HotKeyNameEntry& entry : kHotKeyNames ) {
				if( _tcsicmp(value.c_str(), entry.name) == 0 ) return entry.key;
			}
			return 0;
		}

		tstring CanonicalKeyName(WORD key)
		{
			if( (key >= _T('A') && key <= _T('Z')) ||
				(key >= _T('0') && key <= _T('9')) ) {
				return tstring(1, static_cast<TCHAR>(key));
			}
			if( key >= VK_F1 && key <= VK_F24 ) {
				return _T("F") + NumberText(key - VK_F1 + 1);
			}
			if( key >= VK_NUMPAD0 && key <= VK_NUMPAD9 ) {
				return _T("Numpad") + NumberText(key - VK_NUMPAD0);
			}
			for( const HotKeyNameEntry& entry : kHotKeyNames ) {
				if( key == entry.key ) return entry.name;
			}
			return tstring();
		}
	}
	CHotKeyWnd::CHotKeyWnd(void) : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false)
	{
	}
	void CHotKeyWnd::Init(CHotKeyUI * pOwner)
	{
		m_pOwner = pOwner;
		do  {
			if (NULL == m_pOwner) {
				break;
			}
			RECT rcPos = CalPos();
			UINT uStyle = WS_CHILD | ES_AUTOHSCROLL;
			HWND hWnd = Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
			if (!IsWindow(hWnd)) {
				break;
			}
			SetWindowFont(m_hWnd, m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->hFont, TRUE);
			SetHotKey(m_pOwner->m_wVirtualKeyCode, m_pOwner->m_wModifiers);
			m_pOwner->m_sText = GetHotKeyName();
			::EnableWindow(m_hWnd, m_pOwner->IsEnabled() == true);
			::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
			::SetFocus(m_hWnd);
			m_bInit = true;
		} while (0);
	}


	RECT CHotKeyWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		RECT rcInset = m_pOwner->GetTextPadding();
		rcPos.left += rcInset.left;
		rcPos.top += rcInset.top;
		rcPos.right -= rcInset.right;
		rcPos.bottom -= rcInset.bottom;
		LONG lHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
		if( lHeight < rcPos.GetHeight() ) {
			rcPos.top += (rcPos.GetHeight() - lHeight) / 2;
			rcPos.bottom = rcPos.top + lHeight;
		}
		return rcPos;
	}


	LPCTSTR CHotKeyWnd::GetWindowClassName() const
	{
		return _T("HotKeyClass");
	}

	void CHotKeyWnd::OnFinalMessage(HWND)
	{
		if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CHotKeyWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if( uMsg == WM_KILLFOCUS ) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
		else if( uMsg == OCM_COMMAND ) {
			if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
			else if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE ) {
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
			}
		}
		else if( uMsg == WM_KEYDOWN && (wParam == VK_DELETE || wParam == VK_BACK) ) {
			SetHotKey(0, 0);
			if( m_pOwner != NULL ) {
				m_pOwner->m_wVirtualKeyCode = 0;
				m_pOwner->m_wModifiers = 0;
				m_pOwner->m_sText = _T("无");
				m_pOwner->Invalidate();
				m_pOwner->GetManager()->SendNotify(m_pOwner, _T("textchanged"));
			}
			::InvalidateRect(m_hWnd, NULL, FALSE);
		}
		else if ( (uMsg == WM_NCACTIVATE) || (uMsg == WM_NCACTIVATE) || (uMsg == WM_NCCALCSIZE) )
		{
			return 0;
		}
		else if (uMsg == WM_PAINT)
		{
			PAINTSTRUCT ps = { 0 };
			HDC hDC = ::BeginPaint(m_hWnd, &ps);
			DWORD dwTextColor = m_pOwner->GetTextColor();
			DWORD dwBkColor = m_pOwner->GetNativeBkColor();
			tstring strText = GetHotKeyName();
			::RECT rect;
			::GetClientRect(m_hWnd, &rect);
			::SetBkMode(hDC, TRANSPARENT);
			::SetTextColor(hDC, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));
			HBRUSH hBrush =  CreateSolidBrush( RGB(GetBValue(dwBkColor), GetGValue(dwBkColor), GetRValue(dwBkColor)) );
			::FillRect(hDC, &rect, hBrush);
			::DeleteObject(hBrush);
			HFONT hOldFont = (HFONT)SelectObject(hDC, GetWindowFont(m_hWnd));
			::SIZE size = { 0 };
			::GetTextExtentPoint32(hDC, strText.c_str(), static_cast<int>(strText.length()), &size) ;
			::DrawText(hDC, strText.c_str(), -1, &rect, DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
			::SelectObject(hDC, hOldFont);
			::SetCaretPos(size.cx, 0);
			::EndPaint(m_hWnd, &ps);
			bHandled = TRUE;
		}
		else bHandled = FALSE;
		if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}


	LPCTSTR CHotKeyWnd::GetSuperClassName() const
	{
		return HOTKEY_CLASS;
	}


	LRESULT CHotKeyWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		::SendMessage(m_hWnd, WM_CLOSE, 0, 0);
		return lRes;
	}


	LRESULT CHotKeyWnd::OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( !m_bInit ) return 0;
		if( m_pOwner == NULL ) return 0;
		GetHotKey(m_pOwner->m_wVirtualKeyCode, m_pOwner->m_wModifiers);
		if (m_pOwner->m_wVirtualKeyCode == 0) {
			m_pOwner->m_sText = _T("无");
			m_pOwner->m_wModifiers = 0;
		}
		else {
			m_pOwner->m_sText = GetHotKeyName();
		}
		m_pOwner->GetManager()->SendNotify(m_pOwner, _T("textchanged"));
		return 0;
	}


	void CHotKeyWnd::SetHotKey(WORD wVirtualKeyCode, WORD wModifiers)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, HKM_SETHOTKEY, MAKEWORD(wVirtualKeyCode, wModifiers), 0L);
	}

	DWORD CHotKeyWnd::GetHotKey() const
	{
		ASSERT(::IsWindow(m_hWnd));
		const LRESULT result = ::SendMessage(m_hWnd, HKM_GETHOTKEY, 0, 0L);
		return static_cast<DWORD>(LOWORD(static_cast<DWORD_PTR>(result)));
	}

	void CHotKeyWnd::GetHotKey(WORD &wVirtualKeyCode, WORD &wModifiers) const
	{
		DWORD dw = GetHotKey();
		wVirtualKeyCode = LOBYTE(LOWORD(dw));
		wModifiers = HIBYTE(LOWORD(dw));
	}

	void CHotKeyWnd::SetRules(WORD wInvalidComb, WORD wModifiers)
	{
		ASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, HKM_SETRULES, wInvalidComb, MAKELPARAM(wModifiers, 0));
	}


	tstring CHotKeyWnd::GetKeyName(UINT vk, BOOL fExtended)
	{
		UINT nScanCode = ::MapVirtualKeyEx( vk, 0, ::GetKeyboardLayout( 0 ) );
		switch( vk )
		{
		case VK_INSERT:
		case VK_DELETE:
		case VK_HOME:
		case VK_END:
		case VK_NEXT:
		case VK_PRIOR:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			nScanCode |= 0x100;
		}
		if (fExtended)
			nScanCode |= 0x100;

		TCHAR szStr[ MAX_PATH ] = {0};
		::GetKeyNameText( nScanCode << 16, szStr, MAX_PATH );

		return tstring(szStr);
	}


	tstring CHotKeyWnd::GetHotKeyName()
	{
		ASSERT(::IsWindow(m_hWnd));
		WORD wCode = 0;
		WORD wModifiers = 0;
		GetHotKey(wCode, wModifiers);
		return CHotKeyUI::FormatHotKey(wCode, wModifiers);
	}


	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CHotKeyUI)

	bool CHotKeyUI::ParseHotKey(
		LPCTSTR pstrText, WORD& wVirtualKeyCode, WORD& wModifiers)
	{
		if( pstrText == NULL ) return false;
		WORD key = 0;
		WORD modifiers = 0;
		const tstring text(pstrText);
		size_t start = 0;
		while( start <= text.size() ) {
			const size_t separator = text.find(_T('+'), start);
			const tstring token = TrimAndLower(text.substr(start,
				separator == tstring::npos ? tstring::npos : separator - start));
			if( token.empty() ) return false;
			if( token == _T("ctrl") || token == _T("control") ) modifiers |= HOTKEYF_CONTROL;
			else if( token == _T("alt") ) modifiers |= HOTKEYF_ALT;
			else if( token == _T("shift") ) modifiers |= HOTKEYF_SHIFT;
			else if( token == _T("win") || token == _T("windows") ) modifiers |= kHotKeyModifierWin;
			else {
				if( key != 0 ) return false;
				key = KeyFromCanonicalName(token);
				if( key == 0 ) return false;
			}
			if( separator == tstring::npos ) break;
			start = separator + 1;
		}
		if( key == 0 ) return false;
		wVirtualKeyCode = key;
		wModifiers = modifiers;
		return true;
	}

	tstring CHotKeyUI::FormatHotKey(WORD wVirtualKeyCode, WORD wModifiers)
	{
		const tstring keyName = CanonicalKeyName(wVirtualKeyCode);
		if( keyName.empty() ) return tstring();
		tstring result;
		const auto append = [&result](LPCTSTR text) {
			if( !result.empty() ) result += _T(" + ");
			result += text;
		};
		if( wModifiers & HOTKEYF_CONTROL ) append(_T("Ctrl"));
		if( wModifiers & HOTKEYF_ALT ) append(_T("Alt"));
		if( wModifiers & HOTKEYF_SHIFT ) append(_T("Shift"));
		if( wModifiers & kHotKeyModifierWin ) append(_T("Win"));
		append(keyName.c_str());
		return result;
	}

	CHotKeyUI::CHotKeyUI() : m_pWindow(NULL), m_wVirtualKeyCode(0), m_wModifiers(0), m_uButtonState(0), m_dwHotKeybkColor(0xFFFFFFFF)
	{
		SetTextPadding(CDuiRect(4, 3, 4, 3));
		SetBkColor(0xFFFFFFFF);
		SetToolTip(_T("单击后直接按组合键修改，按 Delete 或 Backspace 清除"));
	}

	LPCTSTR CHotKeyUI::GetClass() const
	{
		return _T("HotKeyUI");
	}

	LPVOID CHotKeyUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("HotKey")) == 0 ) return static_cast<CHotKeyUI *>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CHotKeyUI::GetControlFlags() const
	{
		if( !IsEnabled() ) return CControlUI::GetControlFlags();

		return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
	}

	void CHotKeyUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
			return;
		}
		if( event.Type == UIEVENT_WINDOWSIZE )
		{
			if( m_pWindow != NULL ) m_pManager->SetFocusNeeded(this);
		}
		if( event.Type == UIEVENT_SCROLLWHEEL )
		{
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}
		if( event.Type == UIEVENT_SETFOCUS && IsEnabled() )
		{
			if( m_pWindow ) return;
			m_pWindow = new CHotKeyWnd();
			ASSERT(m_pWindow);
			m_pWindow->Init(this);
			Invalidate();
		}

		if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() )
		{
			Invalidate();
		}

		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN)
		{
			if( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if( IsFocused() && m_pWindow == NULL ) {
					m_pWindow = new CHotKeyWnd();
					ASSERT(m_pWindow);
					m_pWindow->Init(this);
				}
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		CLabelUI::DoEvent(event);
	}

	void CHotKeyUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	void CHotKeyUI::SetText(LPCTSTR pstrText)
	{
		m_sText = pstrText;
		if( m_pWindow != NULL ) Edit_SetText(*m_pWindow, m_sText.c_str());
		Invalidate();
	}

	LPCTSTR CHotKeyUI::GetNormalImage()
	{
		return m_sNormalImage.c_str();
	}

	void CHotKeyUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CHotKeyUI::GetHotImage()
	{
		return m_sHotImage.c_str();
	}

	void CHotKeyUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CHotKeyUI::GetFocusedImage()
	{
		return m_sFocusedImage.c_str();
	}

	void CHotKeyUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CHotKeyUI::GetDisabledImage()
	{
		return m_sDisabledImage.c_str();
	}

	void CHotKeyUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		m_sDisabledImage = pStrImage;
		Invalidate();
	}

	void CHotKeyUI::SetNativeBkColor(DWORD dwBkColor)
	{
		if( m_dwHotKeybkColor == dwBkColor ) return;
		m_dwHotKeybkColor = dwBkColor;
		if( m_pWindow != NULL && ::IsWindow(m_pWindow->GetHWND()) ) {
			::InvalidateRect(m_pWindow->GetHWND(), NULL, TRUE);
		}
		Invalidate();
	}

	DWORD CHotKeyUI::GetNativeBkColor() const
	{
		return m_dwHotKeybkColor;
	}

	void CHotKeyUI::SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left,
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	void CHotKeyUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	void CHotKeyUI::SetInternVisible(bool bVisible)
	{
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	SIZE CHotKeyUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) {
			SIZE cxy = { m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 6 };
			return cxy;
		}
		return CControlUI::EstimateSize(szAvailable);
	}

	void CHotKeyUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcscmp(pstrName, _T("nativebkcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetNativeBkColor(clrColor);
		}
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CHotKeyUI::PaintStatusImage(HDC hDC)
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
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( !m_sFocusedImage.empty() ) {
				if( !DrawImage(hDC, m_sFocusedImage.c_str()) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( !m_sHotImage.empty() ) {
				if( !DrawImage(hDC, m_sHotImage.c_str()) ) {}
				else return;
			}
		}

		if( !m_sNormalImage.empty() ) {
			if( !DrawImage(hDC, m_sNormalImage.c_str()) ) {}
			else return;
		}
	}

	void CHotKeyUI::PaintText(HDC hDC)
	{
		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();
		if( m_sText.empty() ) return;
		tstring sText = m_sText;
		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;
		DWORD dwTextColor = m_dwTextColor;
		if(!IsEnabled())dwTextColor = m_dwDisabledTextColor;

		CRenderEngine::DrawText(hDC, m_pManager, rc, sText.c_str(), dwTextColor, m_iFont, DT_SINGLELINE | m_uTextStyle);
	}

	DWORD CHotKeyUI::GetHotKey() const
	{
		return (MAKEWORD(m_wVirtualKeyCode, m_wModifiers));
	}

	void CHotKeyUI::GetHotKey(WORD &wVirtualKeyCode, WORD &wModifiers) const
	{
		wVirtualKeyCode = m_wVirtualKeyCode;
		wModifiers = m_wModifiers;
	}

	void CHotKeyUI::SetHotKey(WORD wVirtualKeyCode, WORD wModifiers)
	{
		m_wVirtualKeyCode = wVirtualKeyCode;
		m_wModifiers = wModifiers;
		if( m_pWindow != NULL ) {
			m_pWindow->SetHotKey(wVirtualKeyCode, wModifiers);
			m_sText = m_pWindow->GetHotKeyName();
		}
		else {
			m_sText = FormatHotKey(wVirtualKeyCode, wModifiers);
		}
		if( wVirtualKeyCode == 0 ) m_sText = _T("无");
		Invalidate();
	}

}
