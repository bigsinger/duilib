#ifndef __UIEXTRACONTROLS_H__
#define __UIEXTRACONTROLS_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CNumberEditUI : public CEditUI
	{
	public:
		CNumberEditUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetRange(int nMin, int nMax);
		void SetText(LPCTSTR pstrText);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		int m_nMinValue;
		int m_nMaxValue;
		bool m_bUseRange;
	};

	class UILIB_API CHyperLinkUI : public CButtonUI
	{
	public:
		CHyperLinkUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetUrl(LPCTSTR pstrUrl);
		LPCTSTR GetUrl() const;
		void SetOpenUrl(bool bOpenUrl);
		bool Activate();
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		tstring m_sUrl;
		bool m_bOpenUrl;
	};

	class UILIB_API CLineUI : public CControlUI
	{
	public:
		CLineUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetHorizontal(bool bHorizontal);
		void SetLineColor(DWORD dwColor);
		void SetLineSize(int nSize);
		void SetLineStyle(int nStyle);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoPaint(HDC hDC, const RECT& rcPaint);

	private:
		bool m_bHorizontal;
		DWORD m_dwLineColor;
		int m_nLineSize;
		int m_nLineStyle;
	};

#if DUI_HAS_STANDARD_CONTROLS
	class UILIB_API CComboButtonUI : public CButtonUI
	{
	public:
		CComboButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool Activate();
		void SetArrowColor(DWORD dwColor);
		void SetArrowWidth(int nWidth);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void PaintText(HDC hDC);

	protected:
		DWORD m_dwArrowColor;
		int m_nArrowWidth;
	};

	class UILIB_API CCheckComboUI : public CComboUI
	{
	public:
		CCheckComboUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		tstring GetText() const;
		void SetSeparator(LPCTSTR pstrSeparator);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		tstring m_sSeparator;
	};

	class UILIB_API CFilterComboUI : public CComboBoxUI
	{
	public:
		CFilterComboUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetFilterText(LPCTSTR pstrText);
		LPCTSTR GetFilterText() const;
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		tstring m_sFilterText;
	};

	class UILIB_API CVirtualListBoxUI : public CListUI
	{
	public:
		CVirtualListBoxUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetVirtualCount(int nCount);
		int GetVirtualCount() const;
		void SetMaterializedCount(int nCount);
		void SetItemPrefix(LPCTSTR pstrPrefix);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		void RebuildVirtualItems();

		int m_nVirtualCount;
		int m_nMaterializedCount;
		tstring m_sItemPrefix;
	};

	class UILIB_API CSplitterUI : public CControlUI
	{
	public:
		CSplitterUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;
		void SetHorizontal(bool bHorizontal);
		void SetLineColor(DWORD dwColor);
		void SetLineSize(int nSize);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoEvent(TEventUI& event);
		void DoPaint(HDC hDC, const RECT& rcPaint);

	private:
		CControlUI* FindPreviousControl() const;

		bool m_bHorizontal;
		bool m_bDragging;
		POINT m_ptStart;
		int m_nStartSize;
		DWORD m_dwLineColor;
		int m_nLineSize;
	};

	class UILIB_API CMenuBarUI : public CHorizontalLayoutUI
	{
	public:
		CMenuBarUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	};
#endif

#if DUI_HAS_FULL_CONTROLS
	class UILIB_API CPropertyGridUI : public CListUI
	{
	public:
		CPropertyGridUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void AddProperty(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	};
#endif

#if DUI_HAS_FULL_CONTROLS && DUI_HAS_STANDARD_CONTROLS
	class UILIB_API CDirectoryTreeUI : public CTreeViewUI
	{
	public:
		CDirectoryTreeUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetRootPath(LPCTSTR pstrRootPath);
		LPCTSTR GetRootPath() const;
		void SetMaxDepth(int nMaxDepth);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		void LoadRoot();
		void LoadDirectory(CTreeNodeUI* pParentNode, const tstring& path, int nDepth);

		tstring m_sRootPath;
		int m_nMaxDepth;
	};
#endif

#if DUI_HAS_FULL_CONTROLS
	class UILIB_API CAddressBarUI : public CEditUI
	{
	public:
		CAddressBarUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetPath(LPCTSTR pstrPath);
		LPCTSTR GetPath() const;
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		tstring m_sPath;
	};
#endif
}

#endif // __UIEXTRACONTROLS_H__
