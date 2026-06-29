#pragma once
//////////////BEGIN��Ϣӳ��궨��////////////////////////////////////////////////////
///

#include "UIProfile.h"

namespace DuiLib
{

enum DuiSig
{
	DuiSig_end = 0, // [marks end of message map]
	DuiSig_lwl,     // LRESULT (WPARAM, LPARAM)
	DuiSig_vn,      // void (TNotifyUI)
};

class CControlUI;

#ifndef DUI_CONTROLS_LIGHT
#define DUI_CONTROLS_LIGHT    0x01
#endif
#ifndef DUI_CONTROLS_STANDARD
#define DUI_CONTROLS_STANDARD 0x02
#endif
#ifndef DUI_CONTROLS_FULL
#define DUI_CONTROLS_FULL     0x04
#endif

#ifndef DUI_CONTROLS_FLAG
#define DUI_CONTROLS_FLAG DUI_CONTROLS_LIGHT
#endif

#define DUI_HAS_LIGHT_CONTROLS    ((DUI_CONTROLS_FLAG & DUI_CONTROLS_LIGHT) != 0)
#define DUI_HAS_STANDARD_CONTROLS ((DUI_CONTROLS_FLAG & DUI_CONTROLS_STANDARD) != 0)
#define DUI_HAS_FULL_CONTROLS     ((DUI_CONTROLS_FLAG & DUI_CONTROLS_FULL) != 0)

#define DECLARE_DUICONTROL(class_name) \
public: \
    static CControlUI* CreateControl();

#define IMPLEMENT_DUICONTROL(class_name) \
    CControlUI* class_name::CreateControl() \
    { \
        return new class_name; \
    }

// Structure for notifications to the outside world
typedef struct tagTNotifyUI 
{
	tstring sType;
	tstring sVirtualWnd;
	CControlUI* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	WPARAM wParam;
	LPARAM lParam;
} TNotifyUI;

class CNotifyPump;
typedef void (CNotifyPump::*DUI_PMSG)(TNotifyUI& msg);  //ָ������

union DuiMessageMapFunctions
{
	DUI_PMSG pfn;   // generic member function pointer
	LRESULT (CNotifyPump::*pfn_Notify_lwl)(WPARAM, LPARAM);
	void (CNotifyPump::*pfn_Notify_vn)(TNotifyUI&);
};

//����������Ϣ����
//////////////////////////////////////////////////////////////////////////

#define DUI_MSGTYPE_MENU                   (_T("menu"))
#define DUI_MSGTYPE_LINK                   (_T("link"))

#define DUI_MSGTYPE_TIMER                  (_T("timer"))
#define DUI_MSGTYPE_CLICK                  (_T("click"))

#define DUI_MSGTYPE_RETURN                 (_T("return"))
#define DUI_MSGTYPE_SCROLL                 (_T("scroll"))

#define DUI_MSGTYPE_DROPDOWN               (_T("dropdown"))
#define DUI_MSGTYPE_SETFOCUS               (_T("setfocus"))

#define DUI_MSGTYPE_KILLFOCUS              (_T("killfocus"))
#define DUI_MSGTYPE_ITEMCLICK 		   	   (_T("itemclick"))
#define DUI_MSGTYPE_LISTITEMCHECKED        (_T("listitemchecked"))
#define DUI_MSGTYPE_LISTITEMSELECT         (_T("listitemselect"))
#define DUI_MSGTYPE_LISTHEADITEMCHECKED    (_T("listheaditemchecked"))
#define DUI_MSGTYPE_LISTHEADERCLICK        (_T("listheaderclick"))
#define DUI_MSGTYPE_TABSELECT              (_T("tabselect"))

#define DUI_MSGTYPE_ITEMSELECT 		   	   (_T("itemselect"))
#define DUI_MSGTYPE_ITEMEXPAND             (_T("itemexpand"))
#define DUI_MSGTYPE_WINDOWINIT             (_T("windowinit"))
#define DUI_MSGTYPE_BUTTONDOWN 		   	   (_T("buttondown"))
#define DUI_MSGTYPE_MOUSEENTER			   (_T("mouseenter"))
#define DUI_MSGTYPE_MOUSELEAVE			   (_T("mouseleave"))

#define DUI_MSGTYPE_TEXTCHANGED            (_T("textchanged"))
#define DUI_MSGTYPE_HEADERCLICK            (_T("headerclick"))
#define DUI_MSGTYPE_ITEMDBCLICK            (_T("itemdbclick"))
#define DUI_MSGTYPE_SHOWACTIVEX            (_T("showactivex"))

#define DUI_MSGTYPE_ITEMCOLLAPSE           (_T("itemcollapse"))
#define DUI_MSGTYPE_ITEMACTIVATE           (_T("itemactivate"))
#define DUI_MSGTYPE_VALUECHANGED           (_T("valuechanged"))

#define DUI_MSGTYPE_SELECTCHANGED 		   (_T("selectchanged"))
#define DUI_MSGTYPE_TEXTROLLEND            (_T("textrollend"))
#define DUI_MSGTYPE_PAGECHANED             (_T("pagechaned"))
#define DUI_MSGTYPE_PAGECHANGED            (_T("pagechanged"))


//////////////////////////////////////////////////////////////////////////



struct DUI_MSGMAP_ENTRY;
struct DUI_MSGMAP
{
#ifndef UILIB_STATIC
	const DUI_MSGMAP* (PASCAL* pfnGetBaseMap)();
#else
	const DUI_MSGMAP* pBaseMap;
#endif
	const DUI_MSGMAP_ENTRY* lpEntries;
};

//�ṹ����
struct DUI_MSGMAP_ENTRY //����һ���ṹ�壬�������Ϣ��Ϣ
{
	tstring sMsgType;          // DUI��Ϣ����
	tstring sCtrlName;         // �ؼ�����
	UINT       nSig;              // ��Ǻ���ָ������
	DUI_PMSG   pfn;               // ָ������ָ��
};

//����
#ifndef UILIB_STATIC
#define DUI_DECLARE_MESSAGE_MAP()                                         \
private:                                                                  \
	static const DUI_MSGMAP_ENTRY _messageEntries[];                      \
protected:                                                                \
	static const DUI_MSGMAP messageMap;                                   \
	static const DUI_MSGMAP* PASCAL _GetBaseMessageMap();                 \
	virtual const DUI_MSGMAP* GetMessageMap() const;                      \

#else
#define DUI_DECLARE_MESSAGE_MAP()                                         \
private:                                                                  \
	static const DUI_MSGMAP_ENTRY _messageEntries[];                      \
protected:                                                                \
	static  const DUI_MSGMAP messageMap;				                  \
	virtual const DUI_MSGMAP* GetMessageMap() const;                      \

#endif


//����������ʼ
#ifndef UILIB_STATIC
#define DUI_BASE_BEGIN_MESSAGE_MAP(theClass)                              \
	const DUI_MSGMAP* PASCAL theClass::_GetBaseMessageMap()               \
		{ return NULL; }                                                  \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
		{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
		{  &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] };\
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#else
#define DUI_BASE_BEGIN_MESSAGE_MAP(theClass)                              \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
		{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
		{  NULL, &theClass::_messageEntries[0] };                         \
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#endif


//����������ʼ
#ifndef UILIB_STATIC
#define DUI_BEGIN_MESSAGE_MAP(theClass, baseClass)                        \
	const DUI_MSGMAP* PASCAL theClass::_GetBaseMessageMap()               \
		{ return &baseClass::messageMap; }                                \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
		{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
		{ &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#else
#define DUI_BEGIN_MESSAGE_MAP(theClass, baseClass)                        \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
		{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
		{ &baseClass::messageMap, &theClass::_messageEntries[0] };        \
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#endif


//��������
#define DUI_END_MESSAGE_MAP()                                             \
	{ _T(""), _T(""), DuiSig_end, (DUI_PMSG)0 }                           \
};                                                                        \


//������Ϣ����--ִ�к�����
#define DUI_ON_MSGTYPE(msgtype, memberFxn)                                \
	{ msgtype, _T(""), DuiSig_vn, (DUI_PMSG)&memberFxn},                  \


//������Ϣ����--�ؼ�����--ִ�к�����
#define DUI_ON_MSGTYPE_CTRNAME(msgtype,ctrname,memberFxn)                 \
	{ msgtype, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },                \


//����click��Ϣ�Ŀؼ�����--ִ�к�����
#define DUI_ON_CLICK_CTRNAME(ctrname,memberFxn)                           \
	{ DUI_MSGTYPE_CLICK, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },      \


//����selectchanged��Ϣ�Ŀؼ�����--ִ�к�����
#define DUI_ON_SELECTCHANGED_CTRNAME(ctrname,memberFxn)                   \
    { DUI_MSGTYPE_SELECTCHANGED,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn }, \


//����killfocus��Ϣ�Ŀؼ�����--ִ�к�����
#define DUI_ON_KILLFOCUS_CTRNAME(ctrname,memberFxn)                       \
	{ DUI_MSGTYPE_KILLFOCUS,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },     \


//����menu��Ϣ�Ŀؼ�����--ִ�к�����
#define DUI_ON_MENU_CTRNAME(ctrname,memberFxn)                            \
	{ DUI_MSGTYPE_MENU,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },          \


//������ؼ������޹ص���Ϣ��

  //����timer��Ϣ--ִ�к�����
#define DUI_ON_TIMER()                                                    \
	{ DUI_MSGTYPE_TIMER, _T(""), DuiSig_vn,(DUI_PMSG)&OnTimer },          \


///
//////////////END��Ϣӳ��궨��////////////////////////////////////////////////////


//////////////BEGIN�ؼ����ƺ궨��//////////////////////////////////////////////////
///

#define  DUI_CTR_EDIT                            (_T("Edit"))
#define  DUI_CTR_LIST                            (_T("List"))
#define  DUI_CTR_TEXT                            (_T("Text"))

#define  DUI_CTR_COMBO                           (_T("Combo"))
#define  DUI_CTR_LABEL                           (_T("Label"))
#define  DUI_CTR_MENU                            (_T("Menu"))

#define  DUI_CTR_BUTTON                          (_T("Button"))
#define  DUI_CTR_OPTION                          (_T("Option"))
#define  DUI_CTR_SLIDER                          (_T("Slider"))
#define  DUI_CTR_HOTKEY                          (_T("HotKey"))
#define  DUI_CTR_LISTEX                          (_T("ListEx"))

#define  DUI_CTR_CONTROL                         (_T("Control"))
#define  DUI_CTR_GIFANIM                         (_T("GifAnim"))
#define  DUI_CTR_ROLLTEXT                        (_T("RollText"))

#define  DUI_CTR_LISTITEM                        (_T("ListItem"))
#define  DUI_CTR_PROGRESS                        (_T("Progress"))
#define  DUI_CTR_RICHEDIT                        (_T("RichEdit"))
#define  DUI_CTR_CHECKBOX                        (_T("CheckBox"))
#define  DUI_CTR_COMBOBOX                        (_T("ComboBox"))
#define  DUI_CTR_DATETIME                        (_T("DateTime"))
#define  DUI_CTR_TREEVIEW                        (_T("TreeView"))
#define  DUI_CTR_TREENODE                        (_T("TreeNode"))
#define  DUI_CTR_GROUPBOX                        (_T("GroupBox"))

#define  DUI_CTR_CONTAINER                       (_T("Container"))
#define  DUI_CTR_TABLAYOUT                       (_T("TabLayout"))
#define  DUI_CTR_SCROLLBAR                       (_T("ScrollBar"))
#define  DUI_CTR_IPADDRESS                       (_T("IPAddress"))

#define  DUI_CTR_LISTHEADER                      (_T("ListHeader"))
#define  DUI_CTR_TILELAYOUT                      (_T("TileLayout"))
#define  DUI_CTR_FADEBUTTON                      (_T("FadeButton"))

#define  DUI_CTR_CHILDLAYOUT                     (_T("ChildLayout"))
#define  DUI_CTR_LISTELEMENT                     (_T("ListElement"))
#define  DUI_CTR_MENUELEMENT                     (_T("MenuElement"))
#define  DUI_CTR_PAGECONTROL                     (_T("PageControl"))
#define  DUI_CTR_SWITCHBUTTON                    (_T("SwitchButton"))

#define  DUI_CTR_DIALOGLAYOUT                    (_T("DialogLayout"))

#define  DUI_CTR_VERTICALLAYOUT                  (_T("VerticalLayout"))
#define  DUI_CTR_LISTHEADERITEM                  (_T("ListHeaderItem"))
#define  DUI_CTR_LISTTEXTEXTELEMENT              (_T("ListTextExtElement"))

#define  DUI_CTR_LISTTEXTELEMENT                 (_T("ListTextElement"))

#define  DUI_CTR_HORIZONTALLAYOUT                (_T("HorizontalLayout"))
#define  DUI_CTR_LISTLABELELEMENT                (_T("ListLabelElement"))

#define  DUI_CTR_LISTCONTAINERELEMENT            (_T("ListContainerElement"))

///
//////////////END�ؼ����ƺ궨��//////////////////////////////////////////////////


}// namespace DuiLib

