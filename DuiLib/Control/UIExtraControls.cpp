#include "stdafx.h"
#include "UIExtraControls.h"

#include <algorithm>
#include <filesystem>
#include <shellapi.h>

namespace DuiLib
{
	namespace
	{
		DWORD ParseExtraColor(LPCTSTR pstrValue, DWORD dwDefault)
		{
			if( pstrValue == NULL || *pstrValue == _T('\0') ) return dwDefault;
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#') ) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			return _tcstoul(pstrValue, &pstr, 16);
		}

		bool IsTrueValue(LPCTSTR pstrValue)
		{
			return pstrValue != NULL && (_tcsicmp(pstrValue, _T("true")) == 0 || _tcscmp(pstrValue, _T("1")) == 0);
		}

		tstring FileNameFromPath(const std::filesystem::path& path)
		{
#ifdef _UNICODE
			return path.filename().wstring();
#else
			return path.filename().string();
#endif
		}

		tstring PathToTString(const std::filesystem::path& path)
		{
#ifdef _UNICODE
			return path.wstring();
#else
			return path.string();
#endif
		}
	}

	CNumberEditUI::CNumberEditUI()
		: m_nMinValue(0)
		, m_nMaxValue(0)
		, m_bUseRange(false)
	{
		SetNumberOnly(true);
	}

	LPCTSTR CNumberEditUI::GetClass() const
	{
		return _T("NumberEditUI");
	}

	LPVOID CNumberEditUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_NUMBEREDIT) == 0 ) return static_cast<CNumberEditUI*>(this);
		return CEditUI::GetInterface(pstrName);
	}

	void CNumberEditUI::SetRange(int nMin, int nMax)
	{
		m_nMinValue = nMin;
		m_nMaxValue = nMax;
		if( m_nMinValue > m_nMaxValue ) std::swap(m_nMinValue, m_nMaxValue);
		m_bUseRange = true;
		SetText(GetText().c_str());
	}

	void CNumberEditUI::SetText(LPCTSTR pstrText)
	{
		if( pstrText == NULL || *pstrText == _T('\0') ) {
			CEditUI::SetText(_T(""));
			return;
		}

		if( !m_bUseRange ) {
			CEditUI::SetText(pstrText);
			return;
		}

		LPTSTR pEnd = NULL;
		int nValue = static_cast<int>(_tcstol(pstrText, &pEnd, 10));
		nValue = CLAMP(nValue, m_nMinValue, m_nMaxValue);
		tstring sValue;
		DuiStringSmallFormat(sValue, _T("%d"), nValue);
		CEditUI::SetText(sValue.c_str());
	}

	void CNumberEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("min")) == 0 ) {
			SetRange(_ttoi(pstrValue), m_bUseRange ? m_nMaxValue : _ttoi(pstrValue));
		}
		else if( _tcscmp(pstrName, _T("max")) == 0 ) {
			SetRange(m_bUseRange ? m_nMinValue : _ttoi(pstrValue), _ttoi(pstrValue));
		}
		else {
			CEditUI::SetAttribute(pstrName, pstrValue);
		}
	}

	CHyperLinkUI::CHyperLinkUI()
		: m_bOpenUrl(false)
	{
		SetTextColor(0xFF1264D8);
		SetHotTextColor(0xFF0B50B5);
		SetTextStyle(DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);
	}

	LPCTSTR CHyperLinkUI::GetClass() const
	{
		return _T("HyperLinkUI");
	}

	LPVOID CHyperLinkUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_HYPERLINK) == 0 ) return static_cast<CHyperLinkUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}

	void CHyperLinkUI::SetUrl(LPCTSTR pstrUrl)
	{
		m_sUrl = DuiStringAssign(pstrUrl);
	}

	LPCTSTR CHyperLinkUI::GetUrl() const
	{
		return m_sUrl.c_str();
	}

	void CHyperLinkUI::SetOpenUrl(bool bOpenUrl)
	{
		m_bOpenUrl = bOpenUrl;
	}

	bool CHyperLinkUI::Activate()
	{
		bool bResult = CButtonUI::Activate();
		if( bResult && m_bOpenUrl && !m_sUrl.empty() ) {
			::ShellExecute(NULL, _T("open"), m_sUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
		return bResult;
	}

	void CHyperLinkUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("url")) == 0 ) SetUrl(pstrValue);
		else if( _tcscmp(pstrName, _T("openurl")) == 0 ) SetOpenUrl(IsTrueValue(pstrValue));
		else CButtonUI::SetAttribute(pstrName, pstrValue);
	}

	CLineUI::CLineUI()
		: m_bHorizontal(true)
		, m_dwLineColor(0xFFE5E7EB)
		, m_nLineSize(1)
		, m_nLineStyle(PS_SOLID)
	{
		SetFixedHeight(1);
	}

	LPCTSTR CLineUI::GetClass() const
	{
		return _T("LineUI");
	}

	LPVOID CLineUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_LINE) == 0 ) return static_cast<CLineUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CLineUI::SetHorizontal(bool bHorizontal)
	{
		m_bHorizontal = bHorizontal;
		if( m_bHorizontal && GetFixedHeight() == 0 ) SetFixedHeight(m_nLineSize);
		if( !m_bHorizontal && GetFixedWidth() == 0 ) SetFixedWidth(m_nLineSize);
		Invalidate();
	}

	void CLineUI::SetLineColor(DWORD dwColor)
	{
		m_dwLineColor = dwColor;
		Invalidate();
	}

	void CLineUI::SetLineSize(int nSize)
	{
		m_nLineSize = max(1, nSize);
		if( m_bHorizontal ) SetFixedHeight(m_nLineSize);
		else SetFixedWidth(m_nLineSize);
		Invalidate();
	}

	void CLineUI::SetLineStyle(int nStyle)
	{
		m_nLineStyle = nStyle;
		Invalidate();
	}

	void CLineUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("hor")) == 0 ) SetHorizontal(IsTrueValue(pstrValue));
		else if( _tcscmp(pstrName, _T("linecolor")) == 0 ) SetLineColor(ParseExtraColor(pstrValue, m_dwLineColor));
		else if( _tcscmp(pstrName, _T("linesize")) == 0 ) SetLineSize(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("dash")) == 0 ) SetLineStyle(IsTrueValue(pstrValue) ? PS_DASH : PS_SOLID);
		else CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CLineUI::DoPaint(HDC hDC, const RECT& rcPaint)
	{
		if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
		RECT rcLine = m_rcItem;
		if( m_bHorizontal ) {
			int y = rcLine.top + (rcLine.bottom - rcLine.top) / 2;
			rcLine.top = y;
			rcLine.bottom = y;
		}
		else {
			int x = rcLine.left + (rcLine.right - rcLine.left) / 2;
			rcLine.left = x;
			rcLine.right = x;
		}
		CRenderEngine::DrawLine(hDC, rcLine, m_nLineSize, GetAdjustColor(m_dwLineColor), m_nLineStyle);
	}

#if DUI_HAS_STANDARD_CONTROLS
	CComboButtonUI::CComboButtonUI()
		: m_dwArrowColor(0xFF4B5563)
		, m_nArrowWidth(18)
	{
		SetTextPadding(CDuiRect(8, 0, 24, 0));
	}

	LPCTSTR CComboButtonUI::GetClass() const
	{
		return _T("ComboButtonUI");
	}

	LPVOID CComboButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_COMBOBUTTON) == 0 ) return static_cast<CComboButtonUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}

	bool CComboButtonUI::Activate()
	{
		bool bResult = CButtonUI::Activate();
		if( bResult && m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_MENU);
		return bResult;
	}

	void CComboButtonUI::SetArrowColor(DWORD dwColor)
	{
		m_dwArrowColor = dwColor;
		Invalidate();
	}

	void CComboButtonUI::SetArrowWidth(int nWidth)
	{
		m_nArrowWidth = max(8, nWidth);
		Invalidate();
	}

	void CComboButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("arrowcolor")) == 0 ) SetArrowColor(ParseExtraColor(pstrValue, m_dwArrowColor));
		else if( _tcscmp(pstrName, _T("arrowwidth")) == 0 ) SetArrowWidth(_ttoi(pstrValue));
		else CButtonUI::SetAttribute(pstrName, pstrValue);
	}

	void CComboButtonUI::PaintText(HDC hDC)
	{
		CButtonUI::PaintText(hDC);
		const int cx = min(m_nArrowWidth, max(8, m_rcItem.right - m_rcItem.left));
		const int centerX = m_rcItem.right - cx / 2;
		const int centerY = m_rcItem.top + (m_rcItem.bottom - m_rcItem.top) / 2;
		POINT pts[3] = {
			{ centerX - 4, centerY - 2 },
			{ centerX + 4, centerY - 2 },
			{ centerX, centerY + 3 },
		};
		HBRUSH hBrush = ::CreateSolidBrush(RGB(GetBValue(m_dwArrowColor), GetGValue(m_dwArrowColor), GetRValue(m_dwArrowColor)));
		HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(GetBValue(m_dwArrowColor), GetGValue(m_dwArrowColor), GetRValue(m_dwArrowColor)));
		HGDIOBJ hOldBrush = ::SelectObject(hDC, hBrush);
		HGDIOBJ hOldPen = ::SelectObject(hDC, hPen);
		::Polygon(hDC, pts, 3);
		::SelectObject(hDC, hOldPen);
		::SelectObject(hDC, hOldBrush);
		::DeleteObject(hPen);
		::DeleteObject(hBrush);
	}

	CCheckComboUI::CCheckComboUI()
		: m_sSeparator(_T(", "))
	{
	}

	LPCTSTR CCheckComboUI::GetClass() const
	{
		return _T("CheckComboUI");
	}

	LPVOID CCheckComboUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_CHECKCOMBO) == 0 ) return static_cast<CCheckComboUI*>(this);
		return CComboUI::GetInterface(pstrName);
	}

	tstring CCheckComboUI::GetText() const
	{
		tstring sText;
		for( int i = 0; i < m_items.GetSize(); ++i ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[i]);
			CCheckBoxUI* pCheckBox = pControl == NULL ? NULL : static_cast<CCheckBoxUI*>(pControl->GetInterface(DUI_CTR_CHECKBOX));
			if( pCheckBox != NULL && pCheckBox->GetCheck() ) {
				if( !sText.empty() ) sText += m_sSeparator;
				sText += pCheckBox->GetText();
			}
		}
		if( !sText.empty() ) return sText;
		return CComboUI::GetText();
	}

	void CCheckComboUI::SetSeparator(LPCTSTR pstrSeparator)
	{
		m_sSeparator = DuiStringAssign(pstrSeparator);
	}

	void CCheckComboUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("separator")) == 0 ) SetSeparator(pstrValue);
		else CComboUI::SetAttribute(pstrName, pstrValue);
	}

	CFilterComboUI::CFilterComboUI()
	{
	}

	LPCTSTR CFilterComboUI::GetClass() const
	{
		return _T("FilterComboUI");
	}

	LPVOID CFilterComboUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_FILTERCOMBO) == 0 ) return static_cast<CFilterComboUI*>(this);
		return CComboUI::GetInterface(pstrName);
	}

	void CFilterComboUI::SetFilterText(LPCTSTR pstrText)
	{
		m_sFilterText = DuiStringAssign(pstrText);
		Invalidate();
	}

	LPCTSTR CFilterComboUI::GetFilterText() const
	{
		return m_sFilterText.c_str();
	}

	void CFilterComboUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("filter")) == 0 ) SetFilterText(pstrValue);
		else CComboBoxUI::SetAttribute(pstrName, pstrValue);
	}

	CVirtualListBoxUI::CVirtualListBoxUI()
		: m_nVirtualCount(0)
		, m_nMaterializedCount(200)
		, m_sItemPrefix(_T("Item "))
	{
	}

	LPCTSTR CVirtualListBoxUI::GetClass() const
	{
		return _T("VirtualListBoxUI");
	}

	LPVOID CVirtualListBoxUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_VIRTUALLISTBOX) == 0 ) return static_cast<CVirtualListBoxUI*>(this);
		return CListUI::GetInterface(pstrName);
	}

	void CVirtualListBoxUI::SetVirtualCount(int nCount)
	{
		m_nVirtualCount = max(0, nCount);
		RebuildVirtualItems();
	}

	int CVirtualListBoxUI::GetVirtualCount() const
	{
		return m_nVirtualCount;
	}

	void CVirtualListBoxUI::SetMaterializedCount(int nCount)
	{
		m_nMaterializedCount = max(1, nCount);
		RebuildVirtualItems();
	}

	void CVirtualListBoxUI::SetItemPrefix(LPCTSTR pstrPrefix)
	{
		m_sItemPrefix = DuiStringAssign(pstrPrefix);
		RebuildVirtualItems();
	}

	void CVirtualListBoxUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("virtualcount")) == 0 ) SetVirtualCount(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("materializedcount")) == 0 ) SetMaterializedCount(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("itemprefix")) == 0 ) SetItemPrefix(pstrValue);
		else CListUI::SetAttribute(pstrName, pstrValue);
	}

	void CVirtualListBoxUI::RebuildVirtualItems()
	{
		if( m_nVirtualCount <= 0 ) return;
		RemoveAll();
		const int nCount = min(m_nVirtualCount, m_nMaterializedCount);
		for( int i = 0; i < nCount; ++i ) {
			CListLabelElementUI* pItem = new CListLabelElementUI();
			tstring sText;
			DuiStringSmallFormat(sText, _T("%s%d"), m_sItemPrefix.c_str(), i + 1);
			pItem->SetText(sText.c_str());
			Add(pItem);
		}
	}

	CSplitterUI::CSplitterUI()
		: m_bHorizontal(false)
		, m_bDragging(false)
		, m_nStartSize(0)
		, m_dwLineColor(0xFFD1D5DB)
		, m_nLineSize(4)
	{
		m_ptStart.x = m_ptStart.y = 0;
		SetFixedWidth(m_nLineSize);
	}

	LPCTSTR CSplitterUI::GetClass() const
	{
		return _T("SplitterUI");
	}

	LPVOID CSplitterUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_SPLITTER) == 0 ) return static_cast<CSplitterUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	UINT CSplitterUI::GetControlFlags() const
	{
		return UIFLAG_SETCURSOR;
	}

	void CSplitterUI::SetHorizontal(bool bHorizontal)
	{
		m_bHorizontal = bHorizontal;
		if( m_bHorizontal ) {
			SetFixedHeight(m_nLineSize);
			SetFixedWidth(0);
		}
		else {
			SetFixedWidth(m_nLineSize);
			SetFixedHeight(0);
		}
		NeedParentUpdate();
	}

	void CSplitterUI::SetLineColor(DWORD dwColor)
	{
		m_dwLineColor = dwColor;
		Invalidate();
	}

	void CSplitterUI::SetLineSize(int nSize)
	{
		m_nLineSize = max(1, nSize);
		SetHorizontal(m_bHorizontal);
	}

	void CSplitterUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("hor")) == 0 ) SetHorizontal(IsTrueValue(pstrValue));
		else if( _tcscmp(pstrName, _T("linecolor")) == 0 ) SetLineColor(ParseExtraColor(pstrValue, m_dwLineColor));
		else if( _tcscmp(pstrName, _T("linesize")) == 0 ) SetLineSize(_ttoi(pstrValue));
		else CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CSplitterUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_SETCURSOR ) {
			::SetCursor(::LoadCursor(NULL, m_bHorizontal ? IDC_SIZENS : IDC_SIZEWE));
			return;
		}
		if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() ) {
			CControlUI* pTarget = FindPreviousControl();
			if( pTarget != NULL ) {
				m_bDragging = true;
				m_ptStart = event.ptMouse;
				m_nStartSize = m_bHorizontal ? pTarget->GetFixedHeight() : pTarget->GetFixedWidth();
				if( m_nStartSize <= 0 ) {
					RECT rc = pTarget->GetPos();
					m_nStartSize = m_bHorizontal ? rc.bottom - rc.top : rc.right - rc.left;
				}
				m_pManager->SetCapture();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE && m_bDragging ) {
			CControlUI* pTarget = FindPreviousControl();
			if( pTarget != NULL ) {
				int nDelta = m_bHorizontal ? event.ptMouse.y - m_ptStart.y : event.ptMouse.x - m_ptStart.x;
				int nSize = max(0, m_nStartSize + nDelta);
				if( m_bHorizontal ) pTarget->SetFixedHeight(nSize);
				else pTarget->SetFixedWidth(nSize);
				NeedParentUpdate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) {
			if( m_bDragging ) {
				m_bDragging = false;
				m_pManager->ReleaseCapture();
			}
			return;
		}
		CControlUI::DoEvent(event);
	}

	void CSplitterUI::DoPaint(HDC hDC, const RECT& rcPaint)
	{
		if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
		CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwLineColor));
	}

	CControlUI* CSplitterUI::FindPreviousControl() const
	{
		CControlUI* pParent = GetParent();
		IContainerUI* pContainer = pParent == NULL ? NULL : static_cast<IContainerUI*>(pParent->GetInterface(_T("IContainer")));
		if( pContainer == NULL ) return NULL;
		for( int i = 0; i < pContainer->GetCount(); ++i ) {
			if( pContainer->GetItemAt(i) == this && i > 0 ) return pContainer->GetItemAt(i - 1);
		}
		return NULL;
	}

	CMenuBarUI::CMenuBarUI()
	{
		SetFixedHeight(28);
	}

	LPCTSTR CMenuBarUI::GetClass() const
	{
		return _T("MenuBarUI");
	}

	LPVOID CMenuBarUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_MENUBAR) == 0 ) return static_cast<CMenuBarUI*>(this);
		return CHorizontalLayoutUI::GetInterface(pstrName);
	}
#endif

#if DUI_HAS_FULL_CONTROLS
	CPropertyGridUI::CPropertyGridUI()
	{
		SetFixedHeight(180);
	}

	LPCTSTR CPropertyGridUI::GetClass() const
	{
		return _T("PropertyGridUI");
	}

	LPVOID CPropertyGridUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_PROPERTYGRID) == 0 ) return static_cast<CPropertyGridUI*>(this);
		return CListUI::GetInterface(pstrName);
	}

	void CPropertyGridUI::AddProperty(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		CListLabelElementUI* pItem = new CListLabelElementUI();
		tstring sText = DuiStringAssign(pstrName);
		sText += _T("    ");
		sText += DuiStringAssign(pstrValue);
		pItem->SetText(sText.c_str());
		Add(pItem);
	}

	void CPropertyGridUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("items")) == 0 ) {
			RemoveAll();
			tstring sItems = DuiStringAssign(pstrValue);
			size_t start = 0;
			while( start < sItems.size() ) {
				size_t end = sItems.find(_T(';'), start);
				tstring sItem = sItems.substr(start, end == tstring::npos ? tstring::npos : end - start);
				size_t mid = sItem.find(_T('='));
				if( mid != tstring::npos ) AddProperty(sItem.substr(0, mid).c_str(), sItem.substr(mid + 1).c_str());
				if( end == tstring::npos ) break;
				start = end + 1;
			}
		}
		else CListUI::SetAttribute(pstrName, pstrValue);
	}
#endif

#if DUI_HAS_FULL_CONTROLS && DUI_HAS_STANDARD_CONTROLS
	CDirectoryTreeUI::CDirectoryTreeUI()
		: m_nMaxDepth(1)
	{
	}

	LPCTSTR CDirectoryTreeUI::GetClass() const
	{
		return _T("DirectoryTreeUI");
	}

	LPVOID CDirectoryTreeUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_DIRECTORYTREE) == 0 ) return static_cast<CDirectoryTreeUI*>(this);
		return CTreeViewUI::GetInterface(pstrName);
	}

	void CDirectoryTreeUI::SetRootPath(LPCTSTR pstrRootPath)
	{
		m_sRootPath = DuiStringAssign(pstrRootPath);
		LoadRoot();
	}

	LPCTSTR CDirectoryTreeUI::GetRootPath() const
	{
		return m_sRootPath.c_str();
	}

	void CDirectoryTreeUI::SetMaxDepth(int nMaxDepth)
	{
		m_nMaxDepth = max(0, nMaxDepth);
		LoadRoot();
	}

	void CDirectoryTreeUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("root")) == 0 || _tcscmp(pstrName, _T("path")) == 0 ) SetRootPath(pstrValue);
		else if( _tcscmp(pstrName, _T("maxdepth")) == 0 ) SetMaxDepth(_ttoi(pstrValue));
		else CTreeViewUI::SetAttribute(pstrName, pstrValue);
	}

	void CDirectoryTreeUI::LoadRoot()
	{
		RemoveAll();
		if( m_sRootPath.empty() ) return;
		std::error_code ec;
		std::filesystem::path root(m_sRootPath);
		if( !std::filesystem::exists(root, ec) || !std::filesystem::is_directory(root, ec) ) return;

		CTreeNodeUI* pRoot = new CTreeNodeUI();
		tstring sText = FileNameFromPath(root);
		if( sText.empty() ) sText = PathToTString(root);
		pRoot->SetItemText(sText.c_str());
		Add(pRoot);
		LoadDirectory(pRoot, PathToTString(root), 0);
	}

	void CDirectoryTreeUI::LoadDirectory(CTreeNodeUI* pParentNode, const tstring& path, int nDepth)
	{
		if( pParentNode == NULL || nDepth >= m_nMaxDepth ) return;
		std::error_code ec;
		for( const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(path), ec) ) {
			if( ec ) break;
			if( !entry.is_directory(ec) ) continue;
			CTreeNodeUI* pNode = new CTreeNodeUI(pParentNode);
			tstring sName = FileNameFromPath(entry.path());
			pNode->SetItemText(sName.c_str());
			pParentNode->Add(pNode);
			LoadDirectory(pNode, PathToTString(entry.path()), nDepth + 1);
		}
	}
#endif

#if DUI_HAS_FULL_CONTROLS
	CAddressBarUI::CAddressBarUI()
	{
		SetTextPadding(CDuiRect(8, 3, 8, 3));
	}

	LPCTSTR CAddressBarUI::GetClass() const
	{
		return _T("AddressBarUI");
	}

	LPVOID CAddressBarUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_ADDRESSBAR) == 0 ) return static_cast<CAddressBarUI*>(this);
		return CEditUI::GetInterface(pstrName);
	}

	void CAddressBarUI::SetPath(LPCTSTR pstrPath)
	{
		m_sPath = DuiStringAssign(pstrPath);
		SetText(m_sPath.c_str());
	}

	LPCTSTR CAddressBarUI::GetPath() const
	{
		return m_sPath.c_str();
	}

	void CAddressBarUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("path")) == 0 ) SetPath(pstrValue);
		else CEditUI::SetAttribute(pstrName, pstrValue);
	}
#endif
}
