// stdafx.cpp : 只包括标准包含文件的源文件
// UIDesigner.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

CMainFrame* g_pMainFrame = NULL;
CToolBoxWnd* g_pToolBoxWnd = NULL;
CPropertiesWnd* g_pPropertiesWnd = NULL;
CFileView* g_pFileView = NULL;
CClassView* g_pClassView = NULL;
CResourceViewBar* g_pResourceView = NULL;
CHookAPI g_pHookAPI;

int gGetUIClass(LPCTSTR pstrClass)
{
	if (pstrClass == NULL || *pstrClass == _T('\0')) return classPointer;

	struct UIClassEntry
	{
		LPCTSTR name;
		int type;
	};

	// 采用表驱动映射，新增控件时不会再受类名长度分支影响。
	static const UIClassEntry entries[] = {
		{ _T("FormUI"), classWindow },
		{ _T("ControlUI"), classControl },
		{ _T("ButtonUI"), classButton },
		{ _T("FadeButtonUI"), classFadeButton },
		{ _T("ComboButtonUI"), classComboButton },
		{ _T("SwitchButtonUI"), classSwitchButton },
		{ _T("HyperLinkUI"), classHyperLink },
		{ _T("EditUI"), classEdit },
		{ _T("NumberEditUI"), classNumberEdit },
		{ _T("AddressBarUI"), classAddressBar },
		{ _T("RichEditUI"), classRichEdit },
		{ _T("HotKeyUI"), classHotKey },
		{ _T("DateTimeUI"), classDateTime },
		{ _T("IPAddressUI"), classIPAddress },
		{ _T("LabelUI"), classLabel },
		{ _T("TextUI"), classText },
		{ _T("LineUI"), classLine },
		{ _T("RollTextUI"), classRollText },
		{ _T("GifAnimUI"), classGifAnim },
		{ _T("OptionUI"), classOption },
		{ _T("CheckBoxUI"), classCheckBox },
		{ _T("ComboUI"), classCombo },
		{ _T("ComboBoxUI"), classComboBox },
		{ _T("CheckComboUI"), classCheckCombo },
		{ _T("FilterComboUI"), classFilterCombo },
		{ _T("ListUI"), classList },
		{ _T("ListExUI"), classListEx },
		{ _T("VirtualListBoxUI"), classVirtualListBox },
		{ _T("GroupedListUI"), classGroupedList },
		{ _T("SliderUI"), classSlider },
		{ _T("ProgressUI"), classProgress },
		{ _T("ContainerUI"), classContainer },
		{ _T("GroupBoxUI"), classGroupBox },
		{ _T("VerticalLayoutUI"), classVerticalLayout },
		{ _T("HorizontalLayoutUI"), classHorizontalLayout },
		{ _T("TabLayoutUI"), classTabLayout },
		{ _T("PageControlUI"), classPageControl },
		{ _T("TileLayoutUI"), classTileLayout },
		{ _T("ChildLayoutUI"), classChildLayout },
		{ _T("SplitterUI"), classSplitter },
		{ _T("MenuUI"), classMenu },
		{ _T("MenuBarUI"), classMenuBar },
		{ _T("MenuElementUI"), classMenuElement },
		{ _T("ScrollBarUI"), classScrollBar },
		{ _T("VScrollBarUI"), classScrollBar },
		{ _T("HScrollBarUI"), classScrollBar },
		{ _T("TreeViewUI"), classTreeView },
		{ _T("TreeNodeUI"), classTreeNode },
		{ _T("DirectoryTreeUI"), classDirectoryTree },
		{ _T("PropertyGridUI"), classPropertyGrid },
		{ _T("ListHeaderUI"), classListHeader },
		{ _T("ListHeaderItemUI"), classListHeaderItem },
		{ _T("ListElementUI"), classItem },
		{ _T("ListTextElementUI"), classListTextElement },
		{ _T("ListTextExtElementUI"), classListTextExtElement },
		{ _T("ListLabelElementUI"), classListLabelElement },
		{ _T("ListContainerElementUI"), classListContainerElement },
		{ _T("GroupedListRowUI"), classGroupedListRow },
		{ _T("GroupedListGroupUI"), classVerticalLayout },
	};

	for (const UIClassEntry& entry : entries) {
		if (_tcscmp(pstrClass, entry.name) == 0) return entry.type;
	}
	return classPointer;
}
int gGetUIClass(CControlUI* pControl)
{
	if(pControl==NULL)
		return classPointer;

	return gGetUIClass(pControl->GetClass());
}
