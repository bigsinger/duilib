#include "stdafx.h"
#include "UIGroupedList.h"
#include <algorithm>

#if DUI_HAS_FULL_CONTROLS

namespace DuiLib
{
	namespace
	{
		const int kColumnHeaderHeight = 30;
		const int kGroupHeaderHeight = 30;
		const int kGroupVerticalInset = 6;
		const int kGroupBodyTopInset = 3;

		DWORD ParseColor(LPCTSTR pstrValue)
		{
			if( pstrValue == NULL ) return 0;
			if( *pstrValue == _T('#') ) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstrEnd = NULL;
			return _tcstoul(pstrValue, &pstrEnd, 16);
		}

		bool TryParseNumber(const tstring& text, double& value)
		{
			tstring normalized;
			normalized.reserve(text.size());
			for( TCHAR ch : text ) {
				if( ch != _T(',') && ch != _T('%') && !_istspace(ch) ) normalized += ch;
			}
			if( normalized.empty() ) return false;
			LPTSTR end = NULL;
			value = _tcstod(normalized.c_str(), &end);
			return end != normalized.c_str() && end != NULL && *end == _T('\0');
		}
	}

	class CGroupedListUI::CColumnButtonUI : public CButtonUI
	{
	public:
		CColumnButtonUI(CGroupedListUI* pOwner, int nColumn)
			: m_pOwner(pOwner), m_nColumn(nColumn) {}

		bool Activate()
		{
			if( !CControlUI::Activate() ) return false;
			if( m_pOwner != NULL ) m_pOwner->OnColumnClicked(m_nColumn);
			return true;
		}

	private:
		CGroupedListUI* m_pOwner;
		int m_nColumn;
	};

	class CGroupedListGroupUI::CGroupHeaderButtonUI : public CButtonUI
	{
	public:
		explicit CGroupHeaderButtonUI(CGroupedListGroupUI* pOwner)
			: m_pOwner(pOwner) {}

		bool Activate()
		{
			if( !CControlUI::Activate() ) return false;
			if( m_pOwner != NULL ) m_pOwner->ToggleExpanded();
			return true;
		}

	private:
		CGroupedListGroupUI* m_pOwner;
	};

	CGroupedListRowUI::CGroupedListRowUI()
		: m_dwHotBkColor(0), m_uButtonState(0)
	{
		SetMouseChildEnabled(false);
	}

	LPCTSTR CGroupedListRowUI::GetClass() const
	{
		return _T("GroupedListRowUI");
	}

	LPVOID CGroupedListRowUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_GROUPEDLISTROW) == 0 ) return static_cast<CGroupedListRowUI*>(this);
		return CHorizontalLayoutUI::GetInterface(pstrName);
	}

	UINT CGroupedListRowUI::GetControlFlags() const
	{
		return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) |
			(IsEnabled() ? UIFLAG_SETCURSOR : 0);
	}

	bool CGroupedListRowUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		return true;
	}

	void CGroupedListRowUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_DBLCLICK && IsEnabled() &&
			::PtInRect(&m_rcItem, event.ptMouse) ) {
			if( m_pManager != NULL ) {
				m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMDBCLICK,
					event.wParam, event.lParam);
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() &&
			::PtInRect(&m_rcItem, event.ptMouse) ) {
			m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
			Invalidate();
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE &&
			(m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			if( ::PtInRect(&m_rcItem, event.ptMouse) ) m_uButtonState |= UISTATE_PUSHED;
			else m_uButtonState &= ~UISTATE_PUSHED;
			Invalidate();
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP &&
			(m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();
			m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
			Invalidate();
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER && IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		else if( event.Type == UIEVENT_MOUSELEAVE && IsEnabled() ) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		else if( event.Type == UIEVENT_CONTEXTMENU && IsContextMenuUsed() ) {
			if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
			return;
		}
		else if( event.Type == UIEVENT_SETCURSOR && IsEnabled() ) {
			::SetCursor(::LoadCursor(NULL, IDC_HAND));
			return;
		}
		CHorizontalLayoutUI::DoEvent(event);
	}

	void CGroupedListRowUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("hotbkcolor")) == 0 ) SetHotBkColor(ParseColor(pstrValue));
		else CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
	}

	void CGroupedListRowUI::PaintBkColor(HDC hDC)
	{
		if( (m_uButtonState & (UISTATE_HOT | UISTATE_PUSHED)) != 0 &&
			m_dwHotBkColor != 0 ) {
			CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
			return;
		}
		CHorizontalLayoutUI::PaintBkColor(hDC);
	}

	void CGroupedListRowUI::SetSortValue(int nColumn, LPCTSTR pstrValue)
	{
		if( nColumn < 0 ) return;
		if( static_cast<int>(m_sortValues.size()) <= nColumn ) m_sortValues.resize(nColumn + 1);
		m_sortValues[nColumn] = pstrValue == NULL ? _T("") : pstrValue;
	}

	LPCTSTR CGroupedListRowUI::GetSortValue(int nColumn) const
	{
		if( nColumn < 0 || nColumn >= static_cast<int>(m_sortValues.size()) ) return _T("");
		return m_sortValues[nColumn].c_str();
	}

	void CGroupedListRowUI::SetHotBkColor(DWORD dwColor)
	{
		m_dwHotBkColor = dwColor;
		Invalidate();
	}

	DWORD CGroupedListRowUI::GetHotBkColor() const
	{
		return m_dwHotBkColor;
	}

	CGroupedListGroupUI::CGroupedListGroupUI(CGroupedListUI* pOwner,
		LPCTSTR pstrTitle, int nItemCount, CContainerUI* pBody, int nBodyHeight)
		: m_pGroupedOwner(pOwner), m_pHeader(NULL), m_pBody(pBody),
		m_sTitle(pstrTitle == NULL ? _T("") : pstrTitle),
		m_nItemCount(max(0, nItemCount)), m_nBodyHeight(max(0, nBodyHeight)),
		m_bExpanded(true)
	{
		m_pHeader = new CGroupHeaderButtonUI(this);
		m_pHeader->SetFixedHeight(kGroupHeaderHeight);
		CGroupedListUI::ApplyStyle(pOwner == NULL ? NULL : pOwner->GetManager(),
			m_pHeader, _T("GroupedListGroupHeader"));
		UpdateHeaderText();
		Add(m_pHeader);
		if( m_pBody != NULL ) {
			m_pBody->SetFixedHeight(m_nBodyHeight);
			CGroupedListUI::ApplyStyle(pOwner == NULL ? NULL : pOwner->GetManager(),
				m_pBody, _T("GroupedListBody"));
			Add(m_pBody);
		}
		UpdateHeight();
	}

	LPCTSTR CGroupedListGroupUI::GetClass() const
	{
		return _T("GroupedListGroupUI");
	}

	LPVOID CGroupedListGroupUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_GROUPEDLISTGROUP) == 0 ) return static_cast<CGroupedListGroupUI*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

	bool CGroupedListGroupUI::IsExpanded() const { return m_bExpanded; }

	void CGroupedListGroupUI::SetExpanded(bool bExpanded)
	{
		if( m_pGroupedOwner != NULL && !m_pGroupedOwner->IsCollapsible() ) bExpanded = true;
		if( m_bExpanded == bExpanded ) return;
		m_bExpanded = bExpanded;
		if( m_pBody != NULL ) m_pBody->SetVisible(m_bExpanded);
		UpdateHeaderText();
		UpdateHeight();
		if( m_pGroupedOwner != NULL ) {
			m_pGroupedOwner->RefreshLayoutHeight();
			if( m_pManager != NULL ) m_pManager->SendNotify(this,
				m_bExpanded ? DUI_MSGTYPE_ITEMEXPAND : DUI_MSGTYPE_ITEMCOLLAPSE);
		}
	}

	void CGroupedListGroupUI::ToggleExpanded() { SetExpanded(!m_bExpanded); }
	CContainerUI* CGroupedListGroupUI::GetBody() const { return m_pBody; }
	LPCTSTR CGroupedListGroupUI::GetTitle() const { return m_sTitle.c_str(); }
	int CGroupedListGroupUI::GetItemCount() const { return m_nItemCount; }

	void CGroupedListGroupUI::UpdateHeaderText()
	{
		if( m_pHeader == NULL ) return;
		tstring text = m_bExpanded ? _T("\x25BC  ") : _T("\x25B6  ");
		text += m_sTitle.empty() ? _T("Default") : m_sTitle;
		text += _T("    ");
		tstring count;
		DuiStringSmallFormat(count, _T("%d"), m_nItemCount);
		text += count;
		m_pHeader->SetText(text.c_str());
	}

	void CGroupedListGroupUI::UpdateHeight()
	{
		SetFixedHeight(kGroupHeaderHeight + kGroupVerticalInset +
			(m_bExpanded ? m_nBodyHeight : 0));
	}

	CGroupedListUI::CGroupedListUI()
		: m_pColumnHeader(NULL), m_nSortColumn(-1), m_bSortAscending(true),
		m_bCollapsible(true), m_bAutoHeight(true)
	{
		SetChildPadding(6);
	}

	LPCTSTR CGroupedListUI::GetClass() const { return _T("GroupedListUI"); }

	LPVOID CGroupedListUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_GROUPEDLIST) == 0 ) return static_cast<CGroupedListUI*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

	void CGroupedListUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("collapsible")) == 0 ) SetCollapsible(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("autoheight")) == 0 ) SetAutoHeight(_tcscmp(pstrValue, _T("true")) == 0);
		else CVerticalLayoutUI::SetAttribute(pstrName, pstrValue);
	}

	bool CGroupedListUI::Remove(CControlUI* pControl)
	{
		if( pControl == NULL ) return false;
		if( pControl == m_pColumnHeader ) {
			m_pColumnHeader = NULL;
			m_nSortColumn = -1;
		}
		CGroupedListGroupUI* pGroup = static_cast<CGroupedListGroupUI*>(
			pControl->GetInterface(DUI_CTR_GROUPEDLISTGROUP));
		if( pGroup != NULL ) {
			m_groups.erase(std::remove(m_groups.begin(), m_groups.end(), pGroup),
				m_groups.end());
		}
		const bool removed = CVerticalLayoutUI::Remove(pControl);
		if( removed ) RefreshLayoutHeight();
		return removed;
	}

	bool CGroupedListUI::RemoveAt(int iIndex)
	{
		return Remove(GetItemAt(iIndex));
	}

	void CGroupedListUI::RemoveAll()
	{
		m_pColumnHeader = NULL;
		m_groups.clear();
		m_nSortColumn = -1;
		CVerticalLayoutUI::RemoveAll();
		RefreshLayoutHeight();
	}

	CButtonUI* CGroupedListUI::AddColumn(LPCTSTR pstrText, int nWidth)
	{
		if( m_pColumnHeader == NULL ) {
			m_pColumnHeader = new CHorizontalLayoutUI;
			m_pColumnHeader->SetFixedHeight(kColumnHeaderHeight);
			ApplyStyle(m_pManager, m_pColumnHeader, _T("GroupedListColumnHeader"));
			AddAt(m_pColumnHeader, 0);
		}
		const int nColumn = m_pColumnHeader->GetCount();
		CColumnButtonUI* pButton = new CColumnButtonUI(this, nColumn);
		pButton->SetText(pstrText == NULL ? _T("") : pstrText);
		if( nWidth > 0 ) pButton->SetFixedWidth(nWidth);
		ApplyStyle(m_pManager, pButton, _T("GroupedListColumn"));
		m_pColumnHeader->Add(pButton);
		RefreshLayoutHeight();
		return pButton;
	}

	void CGroupedListUI::RemoveAllColumns()
	{
		if( m_pColumnHeader == NULL ) return;
		Remove(m_pColumnHeader);
	}

	CGroupedListGroupUI* CGroupedListUI::AddGroup(LPCTSTR pstrTitle,
		int nItemCount, CContainerUI* pBody, int nBodyHeight)
	{
		CGroupedListGroupUI* pGroup = new CGroupedListGroupUI(
			this, pstrTitle, nItemCount, pBody, nBodyHeight);
		ApplyStyle(m_pManager, pGroup, _T("GroupedListGroup"));
		Add(pGroup);
		m_groups.push_back(pGroup);
		if( m_nSortColumn >= 0 ) SortGroup(pGroup);
		RefreshLayoutHeight();
		return pGroup;
	}

	CTileLayoutUI* CGroupedListUI::AddTileGroup(LPCTSTR pstrTitle,
		int nItemCount, SIZE szItem, int nColumns, int nChildPadding)
	{
		CTileLayoutUI* pTiles = new CTileLayoutUI;
		pTiles->SetItemSize(szItem);
		pTiles->SetChildPadding(nChildPadding);
		const int columns = max(1, nColumns);
		const int rows = nItemCount <= 0 ? 1 : (nItemCount + columns - 1) / columns;
		const int bodyHeight = kGroupBodyTopInset + (nItemCount <= 0 ? 30 :
			rows * szItem.cy + max(0, rows - 1) * nChildPadding);
		AddGroup(pstrTitle, nItemCount, pTiles, bodyHeight);
		if( nItemCount <= 0 ) {
			CTextUI* pEmpty = new CTextUI;
			pEmpty->SetText(_T("No items"));
			pEmpty->SetFixedHeight(30);
			ApplyStyle(m_pManager, pEmpty, _T("GroupedListEmpty"));
			pTiles->Add(pEmpty);
		}
		return pTiles;
	}

	CVerticalLayoutUI* CGroupedListUI::AddListGroup(LPCTSTR pstrTitle,
		int nItemCount, int nRowHeight, int nChildPadding)
	{
		CVerticalLayoutUI* pList = new CVerticalLayoutUI;
		pList->SetChildPadding(nChildPadding);
		const int bodyHeight = kGroupBodyTopInset + (nItemCount <= 0 ? 30 :
			nItemCount * nRowHeight + max(0, nItemCount - 1) * nChildPadding);
		AddGroup(pstrTitle, nItemCount, pList, bodyHeight);
		if( nItemCount <= 0 ) {
			CTextUI* pEmpty = new CTextUI;
			pEmpty->SetText(_T("No items"));
			pEmpty->SetFixedHeight(30);
			ApplyStyle(m_pManager, pEmpty, _T("GroupedListEmpty"));
			pList->Add(pEmpty);
		}
		return pList;
	}

	void CGroupedListUI::SetSortColumn(int nColumn, bool bAscending)
	{
		if( nColumn < 0 || m_pColumnHeader == NULL || nColumn >= m_pColumnHeader->GetCount() ) return;
		m_nSortColumn = nColumn;
		m_bSortAscending = bAscending;
		for( CGroupedListGroupUI* pGroup : m_groups ) SortGroup(pGroup);
		NeedUpdate();
	}

	int CGroupedListUI::GetSortColumn() const { return m_nSortColumn; }
	bool CGroupedListUI::IsSortAscending() const { return m_bSortAscending; }

	void CGroupedListUI::SetCollapsible(bool bCollapsible)
	{
		m_bCollapsible = bCollapsible;
		if( !m_bCollapsible ) {
			for( CGroupedListGroupUI* pGroup : m_groups ) pGroup->SetExpanded(true);
		}
	}

	bool CGroupedListUI::IsCollapsible() const { return m_bCollapsible; }
	void CGroupedListUI::SetAutoHeight(bool bAutoHeight) { m_bAutoHeight = bAutoHeight; RefreshLayoutHeight(); }
	bool CGroupedListUI::IsAutoHeight() const { return m_bAutoHeight; }

	void CGroupedListUI::RefreshLayoutHeight()
	{
		if( !m_bAutoHeight ) return;
		int height = m_pColumnHeader == NULL ? 0 : m_pColumnHeader->GetFixedHeight();
		int visible = m_pColumnHeader == NULL ? 0 : 1;
		for( CGroupedListGroupUI* pGroup : m_groups ) {
			if( pGroup == NULL || !pGroup->IsVisible() ) continue;
			height += pGroup->GetFixedHeight();
			++visible;
		}
		height += max(0, visible - 1) * GetChildPadding();
		SetFixedHeight(height);
		NeedParentUpdate();
	}

	void CGroupedListUI::ApplyStyle(CPaintManagerUI* pManager,
		CControlUI* pControl, LPCTSTR pstrStyleName)
	{
		if( pManager == NULL || pControl == NULL || pstrStyleName == NULL ) return;
		LPCTSTR pstrAttributes = pManager->GetDefaultAttributeList(pstrStyleName);
		if( pstrAttributes != NULL ) pControl->ApplyAttributeList(pstrAttributes);
	}

	void CGroupedListUI::OnColumnClicked(int nColumn)
	{
		const bool ascending = nColumn == m_nSortColumn ? !m_bSortAscending : true;
		SetSortColumn(nColumn, ascending);
		if( m_pManager != NULL && m_pColumnHeader != NULL ) {
			CControlUI* pColumn = m_pColumnHeader->GetItemAt(nColumn);
			if( pColumn != NULL ) m_pManager->SendNotify(pColumn,
				DUI_MSGTYPE_HEADERCLICK, static_cast<WPARAM>(nColumn),
				static_cast<LPARAM>(ascending));
		}
	}

	void CGroupedListUI::SortGroup(CGroupedListGroupUI* pGroup)
	{
		if( pGroup == NULL || pGroup->GetBody() == NULL || m_nSortColumn < 0 ) return;
		CContainerUI* pBody = pGroup->GetBody();
		std::vector<CControlUI*> items;
		items.reserve(pBody->GetCount());
		for( int i = 0; i < pBody->GetCount(); ++i ) items.push_back(pBody->GetItemAt(i));
		std::stable_sort(items.begin(), items.end(), [this](CControlUI* left, CControlUI* right) {
			const tstring leftText = SortText(left, m_nSortColumn);
			const tstring rightText = SortText(right, m_nSortColumn);
			double leftNumber = 0.0;
			double rightNumber = 0.0;
			int compare = 0;
			if( TryParseNumber(leftText, leftNumber) && TryParseNumber(rightText, rightNumber) ) {
				compare = leftNumber < rightNumber ? -1 : (leftNumber > rightNumber ? 1 : 0);
			}
			else compare = _tcsicmp(leftText.c_str(), rightText.c_str());
			return m_bSortAscending ? compare < 0 : compare > 0;
		});
		for( int i = 0; i < static_cast<int>(items.size()); ++i ) pBody->SetItemIndex(items[i], i);
	}

	tstring CGroupedListUI::SortText(CControlUI* pControl, int nColumn)
	{
		if( pControl == NULL ) return tstring();
		CGroupedListRowUI* pRow = static_cast<CGroupedListRowUI*>(
			pControl->GetInterface(DUI_CTR_GROUPEDLISTROW));
		if( pRow != NULL ) {
			LPCTSTR value = pRow->GetSortValue(nColumn);
			if( value != NULL && *value != _T('\0') ) return value;
		}
		IContainerUI* pContainer = static_cast<IContainerUI*>(pControl->GetInterface(_T("IContainer")));
		if( pContainer != NULL && nColumn >= 0 && nColumn < pContainer->GetCount() ) {
			CControlUI* pCell = pContainer->GetItemAt(nColumn);
			if( pCell != NULL ) return pCell->GetText();
		}
		return pControl->GetText();
	}
}

#endif // DUI_HAS_FULL_CONTROLS
