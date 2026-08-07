#ifndef __UIGROUPEDLIST_H__
#define __UIGROUPEDLIST_H__

#pragma once

#if DUI_HAS_FULL_CONTROLS

namespace DuiLib
{
	class CGroupedListUI;

	class UILIB_API CGroupedListRowUI : public CHorizontalLayoutUI
	{
	public:
		CGroupedListRowUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;
		void SetPos(RECT rc);
		bool Activate();
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void PaintBkColor(HDC hDC);

		void SetSortValue(int nColumn, LPCTSTR pstrValue);
		LPCTSTR GetSortValue(int nColumn) const;
		void SetHotBkColor(DWORD dwColor);
		DWORD GetHotBkColor() const;

	private:
		std::vector<tstring> m_sortValues;
		DWORD m_dwHotBkColor;
		UINT m_uButtonState;
	};

	class UILIB_API CGroupedListGroupUI : public CVerticalLayoutUI
	{
	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool IsExpanded() const;
		void SetExpanded(bool bExpanded);
		void ToggleExpanded();
		CContainerUI* GetBody() const;
		LPCTSTR GetTitle() const;
		int GetItemCount() const;

	private:
		friend class CGroupedListUI;
		class CGroupHeaderButtonUI;

		CGroupedListGroupUI(CGroupedListUI* pOwner, LPCTSTR pstrTitle,
			int nItemCount, CContainerUI* pBody, int nBodyHeight);
		void UpdateHeaderText();
		void UpdateHeight();

		CGroupedListUI* m_pGroupedOwner;
		CGroupHeaderButtonUI* m_pHeader;
		CContainerUI* m_pBody;
		tstring m_sTitle;
		int m_nItemCount;
		int m_nBodyHeight;
		bool m_bExpanded;
	};

	class UILIB_API CGroupedListUI : public CVerticalLayoutUI
	{
	public:
		CGroupedListUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		bool Remove(CControlUI* pControl);
		bool RemoveAt(int iIndex);
		void RemoveAll();

		CButtonUI* AddColumn(LPCTSTR pstrText, int nWidth = 0);
		void RemoveAllColumns();
		CGroupedListGroupUI* AddGroup(LPCTSTR pstrTitle, int nItemCount,
			CContainerUI* pBody, int nBodyHeight);
		CTileLayoutUI* AddTileGroup(LPCTSTR pstrTitle, int nItemCount,
			SIZE szItem, int nColumns, int nChildPadding = 6);
		CVerticalLayoutUI* AddListGroup(LPCTSTR pstrTitle, int nItemCount,
			int nRowHeight, int nChildPadding = 5);

		void SetSortColumn(int nColumn, bool bAscending);
		int GetSortColumn() const;
		bool IsSortAscending() const;
		void SetCollapsible(bool bCollapsible);
		bool IsCollapsible() const;
		void SetAutoHeight(bool bAutoHeight);
		bool IsAutoHeight() const;
		void RefreshLayoutHeight();

		static void ApplyStyle(CPaintManagerUI* pManager,
			CControlUI* pControl, LPCTSTR pstrStyleName);
		static void ApplyStyle(CPaintManagerUI& manager,
			CControlUI* pControl, LPCTSTR pstrStyleName)
		{
			ApplyStyle(&manager, pControl, pstrStyleName);
		}

	private:
		class CColumnButtonUI;
		friend class CColumnButtonUI;
		friend class CGroupedListGroupUI;

		void OnColumnClicked(int nColumn);
		void SortGroup(CGroupedListGroupUI* pGroup);
		static tstring SortText(CControlUI* pControl, int nColumn);

		CHorizontalLayoutUI* m_pColumnHeader;
		std::vector<CGroupedListGroupUI*> m_groups;
		int m_nSortColumn;
		bool m_bSortAscending;
		bool m_bCollapsible;
		bool m_bAutoHeight;
	};
}

#endif // DUI_HAS_FULL_CONTROLS
#endif // __UIGROUPEDLIST_H__
