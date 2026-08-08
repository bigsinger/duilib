// ToolBoxWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiDesigner.h"
#include "ToolBoxWnd.h"

//////////////////////////////////////////////////////////////////////////
// CToolBoxWnd

IMPLEMENT_DYNAMIC(CToolBoxWnd, CDockablePane)

CToolBoxWnd::CToolBoxWnd()
{
	g_pToolBoxWnd=this;
}

CToolBoxWnd::~CToolBoxWnd()
{
}


BEGIN_MESSAGE_MAP(CToolBoxWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CToolBoxWnd 消息处理程序

int CToolBoxWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_ctlToolList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 12))
	{
		TRACE0("Failed to create ToolBox\n");
		return -1;      // fail to create
	}
	InitToolList();

	return 0;
}

void CToolBoxWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AdjustLayout();
}

void CToolBoxWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_ctlToolList.SetWindowPos(NULL, rectClient.left,rectClient.top,rectClient.Width(),rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CToolBoxWnd::InitToolList()
{
	CToolElement* pControlTab=new CToolElement(_T("基础控件"));
	pControlTab->AddSubTool(new CToolElement(_T("指针"),classPointer,IDI_TOOLBOX_POINTER));
	pControlTab->AddSubTool(new CToolElement(_T("Control"),classControl,IDI_TOOLBOX_CONTROL));
	pControlTab->AddSubTool(new CToolElement(_T("Label"),classLabel,IDI_TOOLBOX_LABEL));
	pControlTab->AddSubTool(new CToolElement(_T("Text"),classText,IDI_TOOLBOX_TEXT));
	pControlTab->AddSubTool(new CToolElement(_T("Line"),classLine,IDI_TOOLBOX_CONTROL));
	pControlTab->AddSubTool(new CToolElement(_T("Button"),classButton,IDI_TOOLBOX_BUTTON));
	pControlTab->AddSubTool(new CToolElement(_T("FadeButton"),classFadeButton,IDI_TOOLBOX_BUTTON));
	pControlTab->AddSubTool(new CToolElement(_T("ComboButton"),classComboButton,IDI_TOOLBOX_BUTTON));
	pControlTab->AddSubTool(new CToolElement(_T("SwitchButton"),classSwitchButton,IDI_TOOLBOX_BUTTON));
	pControlTab->AddSubTool(new CToolElement(_T("HyperLink"),classHyperLink,IDI_TOOLBOX_LABEL));
	pControlTab->AddSubTool(new CToolElement(_T("Edit"),classEdit,IDI_TOOLBOX_EDIT));
	pControlTab->AddSubTool(new CToolElement(_T("NumberEdit"),classNumberEdit,IDI_TOOLBOX_EDIT));
	pControlTab->AddSubTool(new CToolElement(_T("AddressBar"),classAddressBar,IDI_TOOLBOX_EDIT));
	pControlTab->AddSubTool(new CToolElement(_T("RichEdit"),classRichEdit,IDI_TOOLBOX_EDIT));
	pControlTab->AddSubTool(new CToolElement(_T("HotKey"),classHotKey,IDI_TOOLBOX_EDIT));
	pControlTab->AddSubTool(new CToolElement(_T("DateTime"),classDateTime,IDI_TOOLBOX_EDIT));
	pControlTab->AddSubTool(new CToolElement(_T("IPAddress"),classIPAddress,IDI_TOOLBOX_EDIT));
	pControlTab->AddSubTool(new CToolElement(_T("Option"),classOption,IDI_TOOLBOX_OPTION));
	pControlTab->AddSubTool(new CToolElement(_T("CheckBox"),classCheckBox,IDI_TOOLBOX_OPTION));
	pControlTab->AddSubTool(new CToolElement(_T("Progress"),classProgress,IDI_TOOLBOX_PROGRESS));
	pControlTab->AddSubTool(new CToolElement(_T("Slider"),classSlider,IDI_TOOLBOX_SLIDER));
	pControlTab->AddSubTool(new CToolElement(_T("GifAnim"),classGifAnim,IDI_TOOLBOX_CONTROL));
	pControlTab->AddSubTool(new CToolElement(_T("RollText"),classRollText,IDI_TOOLBOX_TEXT));
	m_ctlToolList.AddToolTab(pControlTab);

	CToolElement* pListTab=new CToolElement(_T("选择与列表"));
	pListTab->AddSubTool(new CToolElement(_T("Combo"),classCombo,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("ComboBox"),classComboBox,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("CheckCombo"),classCheckCombo,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("FilterCombo"),classFilterCombo,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("List"),classList,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("ListEx"),classListEx,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("VirtualListBox"),classVirtualListBox,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("GroupedList"),classGroupedList,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("ListHeader"),classListHeader,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("ListHeaderItem"),classListHeaderItem,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("ListTextElement"),classListTextElement,IDI_TOOLBOX_TEXT));
	pListTab->AddSubTool(new CToolElement(_T("ListTextExtElement"),classListTextExtElement,IDI_TOOLBOX_TEXT));
	pListTab->AddSubTool(new CToolElement(_T("ListLabelElement"),classListLabelElement,IDI_TOOLBOX_LABEL));
	pListTab->AddSubTool(new CToolElement(_T("ListContainerElement"),classListContainerElement,IDI_TOOLBOX_CONTAINER));
	pListTab->AddSubTool(new CToolElement(_T("GroupedListRow"),classGroupedListRow,IDI_TOOLBOX_CONTAINER));
	pListTab->AddSubTool(new CToolElement(_T("TreeView"),classTreeView,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("TreeNode"),classTreeNode,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("DirectoryTree"),classDirectoryTree,IDI_TOOLBOX_COMBO));
	pListTab->AddSubTool(new CToolElement(_T("PropertyGrid"),classPropertyGrid,IDI_TOOLBOX_COMBO));
	m_ctlToolList.AddToolTab(pListTab);

	CToolElement* pLayoutTab=new CToolElement(_T("布局与菜单"));
	pLayoutTab->AddSubTool(new CToolElement(_T("Container"),classContainer,IDI_TOOLBOX_CONTAINER));
	pLayoutTab->AddSubTool(new CToolElement(_T("GroupBox"),classGroupBox,IDI_TOOLBOX_CONTAINER));
	pLayoutTab->AddSubTool(new CToolElement(_T("VerticalLayout"),classVerticalLayout,IDI_TOOLBOX_VERTICAL_LAYOUT));
	pLayoutTab->AddSubTool(new CToolElement(_T("HorizontalLayout"),classHorizontalLayout,IDI_TOOLBOX_HORIZONTAL_LAYOUT));
	pLayoutTab->AddSubTool(new CToolElement(_T("TileLayout"),classTileLayout,IDI_TOOLBOX_TILE_LAYOUT));
	pLayoutTab->AddSubTool(new CToolElement(_T("TabLayout"),classTabLayout,IDI_TOOLBOX_TAB_LAYOUT));
	pLayoutTab->AddSubTool(new CToolElement(_T("PageControl"),classPageControl,IDI_TOOLBOX_TAB_LAYOUT));
	pLayoutTab->AddSubTool(new CToolElement(_T("ChildLayout"),classChildLayout,IDI_TOOLBOX_CONTAINER));
	pLayoutTab->AddSubTool(new CToolElement(_T("Splitter"),classSplitter,IDI_TOOLBOX_SCROLLBAR));
	pLayoutTab->AddSubTool(new CToolElement(_T("ScrollBar"),classScrollBar,IDI_TOOLBOX_SCROLLBAR));
	pLayoutTab->AddSubTool(new CToolElement(_T("Menu"),classMenu,IDI_TOOLBOX_CONTAINER));
	pLayoutTab->AddSubTool(new CToolElement(_T("MenuBar"),classMenuBar,IDI_TOOLBOX_HORIZONTAL_LAYOUT));
	pLayoutTab->AddSubTool(new CToolElement(_T("MenuElement"),classMenuElement,IDI_TOOLBOX_BUTTON));
	m_ctlToolList.AddToolTab(pLayoutTab);

	m_ctlToolList.SetCurSel(classPointer);
}
