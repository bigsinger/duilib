#ifndef __UIHOTKEY_H__
#define __UIHOTKEY_H__
#pragma once

namespace DuiLib{
	class CHotKeyUI;

	class UILIB_API CHotKeyWnd : public CWindowWnd
	{
	public:
		CHotKeyWnd(void);

	public:
		void Init(CHotKeyUI * pOwner);
		RECT CalPos();
		LPCTSTR GetWindowClassName() const;
		void OnFinalMessage(HWND hWnd);
		LPCTSTR GetSuperClassName() const;
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	public:
		void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers);
		void GetHotKey(WORD &wVirtualKeyCode, WORD &wModifiers) const;
		DWORD GetHotKey(void) const;
		tstring GetHotKeyName();
		void SetRules(WORD wInvalidComb, WORD wModifiers);
		tstring GetKeyName(UINT vk, BOOL fExtended);
	protected:
		CHotKeyUI * m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
	};

	class UILIB_API CHotKeyUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CHotKeyUI)
		friend CHotKeyWnd;
	public:
		static constexpr WORD kHotKeyModifierWin = 0x10;
		static bool ParseHotKey(LPCTSTR pstrText, WORD& wVirtualKeyCode, WORD& wModifiers);
		static tstring FormatHotKey(WORD wVirtualKeyCode, WORD wModifiers);

		CHotKeyUI();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;
		void SetEnabled(bool bEnable = true);
		void SetText(LPCTSTR pstrText);
		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		void SetNativeBkColor(DWORD dwBkColor);
		DWORD GetNativeBkColor() const;

		void SetPos(RECT rc);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		SIZE EstimateSize(SIZE szAvailable);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);
	public:
		void GetHotKey(WORD &wVirtualKeyCode, WORD &wModifiers) const;
		DWORD GetHotKey(void) const;
		void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers);

	protected:
		CHotKeyWnd * m_pWindow;
		UINT m_uButtonState;
		tstring m_sNormalImage;
		tstring m_sHotImage;
		tstring m_sFocusedImage;
		tstring m_sDisabledImage;
		DWORD m_dwHotKeybkColor;

	protected:
		WORD m_wVirtualKeyCode;
		WORD m_wModifiers;
	};
}


#endif
