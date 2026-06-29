#ifndef __UISWITCHBUTTON_H__
#define __UISWITCHBUTTON_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CSwitchButtonUI : public CButtonUI
	{
	public:
		CSwitchButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool Activate();
		bool IsSelected() const;
		void SetSelected(bool bSelected);
		void SetCheck(bool bCheck);
		bool GetCheck() const;

		void SetOnText(LPCTSTR pstrText);
		LPCTSTR GetOnText() const;
		void SetOffText(LPCTSTR pstrText);
		LPCTSTR GetOffText() const;

		void SetOnColor(DWORD dwColor);
		DWORD GetOnColor() const;
		void SetOffColor(DWORD dwColor);
		DWORD GetOffColor() const;
		void SetDisabledColor(DWORD dwColor);
		DWORD GetDisabledColor() const;
		void SetThumbColor(DWORD dwColor);
		DWORD GetThumbColor() const;
		void SetHotThumbColor(DWORD dwColor);
		DWORD GetHotThumbColor() const;

		SIZE EstimateSize(SIZE szAvailable);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);

	protected:
		bool m_bSelected;
		tstring m_sOnText;
		tstring m_sOffText;
		DWORD m_dwOnColor;
		DWORD m_dwOffColor;
		DWORD m_dwDisabledColor;
		DWORD m_dwThumbColor;
		DWORD m_dwHotThumbColor;
	};
}

#endif // __UISWITCHBUTTON_H__
