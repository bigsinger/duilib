
#pragma once

#include "../Utils/UIStringUtil.h"

namespace DuiLib {

enum class ControlHitTestRole { Default, Client, Caption, Transparent };

/////////////////////////////////////////////////////////////////////////////////////
//
enum {
	Left = 1, Top = 2, Right = 4, Bottom = 8,
	LeftTop = Left | Top,
	LeftRight = Left | Right,
	LeftBottom = Left | Bottom,
	TopRight = Top | Right,
	TopBottom = Top | Bottom,
	RightBottom = Right | Bottom,
	LeftTopRight = Left | Top | Right,
	LeftTopBottom = Left | Top | Bottom,
	LeftRightBottom = Left | Right | Bottom,
	TopRightBottom = Top | Right | Bottom,
	LeftTopRightBottom = Left | Top | Right | Bottom,
};

typedef CControlUI* (CALLBACK* FINDCONTROLPROC)(CControlUI*, LPVOID);

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
class UILIB_API CControlUI
{
public:
    CControlUI();
    virtual ~CControlUI();

public:
    virtual tstring GetName() const;
    virtual void SetName(LPCTSTR pstrName);
    virtual DuiUtf8String GetNameUtf8() const;
    virtual void SetNameUtf8(std::string_view name);
    virtual LPCTSTR GetClass() const;
    virtual LPVOID GetInterface(LPCTSTR pstrName);
    virtual UINT GetControlFlags() const;

    virtual bool Activate();
    virtual ControlHitTestRole GetHitTestRole() const;
    virtual void SetHitTestRole(ControlHitTestRole role);
    virtual const tstring& GetNavigationTarget() const;
    virtual int GetNavigationPageIndex() const;
    virtual void SetNavigationTarget(LPCTSTR target, int pageIndex);
    virtual CPaintManagerUI* GetManager() const;
    virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
    virtual CControlUI* GetParent() const;

    // 文本相关
    virtual tstring GetText() const;
    virtual void SetText(LPCTSTR pstrText);
    virtual DuiUtf8String GetTextUtf8() const;
    virtual void SetTextUtf8(std::string_view text);
    virtual void BindTextUtf8(DuiTextGetter getter, DuiTextSetter setter = {});
    virtual void BindTextUtf8(DuiUtf8String& text);
    virtual void ClearTextBinding();
    virtual bool HasTextBinding() const;
    virtual void RefreshTextBinding();
    virtual void CommitTextBinding() const;
	virtual void SetTextV(const LPCTSTR lpszFormat, ...) final;

    // 图形相关
    DWORD GetBkColor() const;
    void SetBkColor(DWORD dwBackColor);
    DWORD GetBkColor2() const;
    void SetBkColor2(DWORD dwBackColor);
    DWORD GetBkColor3() const;
    void SetBkColor3(DWORD dwBackColor);
    LPCTSTR GetBkImage();
    void SetBkImage(LPCTSTR pStrImage);
	DWORD GetFocusBorderColor() const;
	void SetFocusBorderColor(DWORD dwBorderColor);
    bool IsColorHSL() const;
    void SetColorHSL(bool bColorHSL);
    SIZE GetBorderRound() const;
    void SetBorderRound(SIZE cxyRound);
    bool DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
    bool DrawImage(HDC hDC, const tstring& sImage, LPCTSTR pStrModify = NULL) { return DrawImage(hDC, sImage.c_str(), pStrModify); }
    bool DrawImage(HDC hDC, const tstring& sImage, const tstring& sModify) { return DrawImage(hDC, sImage.c_str(), sModify.c_str()); }

	//边框相关
	int GetBorderSize() const;
	void SetBorderSize(int nSize);
	DWORD GetBorderColor() const;
	void SetBorderColor(DWORD dwBorderColor);

	void SetBorderSize(RECT rc);
	int GetLeftBorderSize() const;
	void SetLeftBorderSize(int nSize);
	int GetTopBorderSize() const;
	void SetTopBorderSize(int nSize);
	int GetRightBorderSize() const;
	void SetRightBorderSize(int nSize);
	int GetBottomBorderSize() const;
	void SetBottomBorderSize(int nSize);
	int GetBorderStyle() const;
	void SetBorderStyle(int nStyle);

    // 位置相关
	int  GetAnchorMode();
	void SetAnchorMode(int nAnchorMode);
    virtual const RECT& GetPos() const;
    virtual void SetPos(RECT rc);
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual int GetX() const;
    virtual int GetY() const;
    virtual RECT GetPadding() const;
    virtual void SetPadding(RECT rcPadding); // 设置外边距，由上层窗口绘制
    virtual SIZE GetFixedXY() const;         // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedXY(SIZE szXY);      // 仅float为true时有效
    virtual int GetFixedWidth() const;       // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedWidth(int cx);      // 预设的参考值
    virtual int GetFixedHeight() const;      // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedHeight(int cy);     // 预设的参考值
    virtual int GetMinWidth() const;
    virtual void SetMinWidth(int cx);
    virtual int GetMaxWidth() const;
    virtual void SetMaxWidth(int cx);
    virtual int GetMinHeight() const;
    virtual void SetMinHeight(int cy);
    virtual int GetMaxHeight() const;
    virtual void SetMaxHeight(int cy);

    // 提示文本

    virtual tstring GetToolTip() const;
    virtual void SetToolTip(LPCTSTR pstrText);
    virtual DuiUtf8String GetToolTipUtf8() const;
    virtual void SetToolTipUtf8(std::string_view text);
	virtual void SetToolTipWidth(int nWidth);
	virtual int	  GetToolTipWidth(void);	// 多行ToolTip单行最长宽度

    // 快捷键
    virtual TCHAR GetShortcut() const;
    virtual void SetShortcut(TCHAR ch);

    // 菜单
    virtual bool IsContextMenuUsed() const;
    virtual void SetContextMenuUsed(bool bMenuUsed);

    // 用户数据

    virtual const tstring& GetUserData(); // 辅助函数，供用户使用
    virtual void SetUserData(LPCTSTR pstrText); // 辅助函数，供用户使用
    virtual DuiUtf8String GetUserDataUtf8() const;
    virtual void SetUserDataUtf8(std::string_view text);
    virtual UINT_PTR GetTag() const; // 辅助函数，供用户使用
    virtual void SetTag(UINT_PTR pTag); // 辅助函数，供用户使用

    // 一些重要的属性
    virtual bool IsVisible() const;
    virtual void SetVisible(bool bVisible = true);
    virtual void SetInternVisible(bool bVisible = true); // 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数
    virtual bool IsEnabled() const;
    virtual void SetEnabled(bool bEnable = true);
    virtual bool IsMouseEnabled() const;
    virtual void SetMouseEnabled(bool bEnable = true);
    virtual bool IsKeyboardEnabled() const;
    virtual void SetKeyboardEnabled(bool bEnable = true);
    virtual bool IsFocused() const;
    virtual void SetFocus();
    virtual bool IsFloat() const;
    virtual void SetFloat(bool bFloat = true);

    virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

    void Invalidate();
    bool IsUpdateNeeded() const;
    void NeedUpdate();
    void NeedParentUpdate();
    DWORD GetAdjustColor(DWORD dwColor);

    virtual void Init();
    virtual void DoInit();

    virtual void Event(TEventUI& event);
    virtual void DoEvent(TEventUI& event);

    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void SetAttribute(const tstring& sName, const tstring& sValue) { SetAttribute(sName.c_str(), sValue.c_str()); }
    virtual void SetAttributeUtf8(std::string_view name, std::string_view value);
    CControlUI* ApplyAttributeList(LPCTSTR pstrList);
    CControlUI* ApplyAttributeList(const tstring& sList) { return ApplyAttributeList(sList.c_str()); }
    CControlUI* ApplyAttributeListUtf8(std::string_view attributes);

    virtual SIZE EstimateSize(SIZE szAvailable);

    virtual void DoPaint(HDC hDC, const RECT& rcPaint);
    virtual void PaintBkColor(HDC hDC);
    virtual void PaintBkImage(HDC hDC);
    virtual void PaintStatusImage(HDC hDC);
    virtual void PaintText(HDC hDC);
    virtual void PaintBorder(HDC hDC);

    virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);

	//虚拟窗口参数
	void SetVirtualWnd(LPCTSTR pstrValue);
	tstring GetVirtualWnd() const;

public:
    CEventSource OnInit;
    CEventSource OnDestroy;
    CEventSource OnSize;
    CEventSource OnEvent;
    CEventSource OnNotify;

protected:
    CPaintManagerUI* m_pManager;
    CControlUI* m_pParent;
	tstring m_sVirtualWnd;
    tstring m_sName;
    bool m_bUpdateNeeded;
    bool m_bMenuUsed;
    RECT m_rcItem;
    RECT m_rcPadding;
    SIZE m_cXY;
    SIZE m_cxyFixed;
    SIZE m_cxyMin;
    SIZE m_cxyMax;
    bool m_bVisible;
    bool m_bInternVisible;
    bool m_bEnabled;
    bool m_bMouseEnabled;
	bool m_bKeyboardEnabled ;
    bool m_bFocused;
    bool m_bFloat;
    ControlHitTestRole m_hitTestRole;
    tstring m_sNavigationTarget;
    int m_iNavigationPageIndex;
    bool m_bSetPos; // 防止SetPos循环调用

    tstring m_sText;
    DuiTextGetter m_textGetter;
    DuiTextSetter m_textSetter;
    tstring m_sToolTip;
    TCHAR m_chShortcut;
    tstring m_sUserData;
    UINT_PTR m_pTag;

    DWORD m_dwBackColor;
    DWORD m_dwBackColor2;
    DWORD m_dwBackColor3;
    tstring m_sBkImage;
	tstring m_sForeImage;
    DWORD m_dwBorderColor;
	DWORD m_dwFocusBorderColor;
    bool m_bColorHSL;
    int m_nBorderSize;
	int m_nBorderStyle;
	int m_nTooltipWidth;
    SIZE m_cxyBorderRound;
    RECT m_rcPaint;
	RECT m_rcBorderSize;

public:
	int m_nAnchorMode;
	int		m_nMarginLeft;
	int		m_nMarginTop;
	int		m_nMarginRight;
	int		m_nMarginBottom;
	int		m_nWidth;
	int		m_nHeight;
};
#ifdef _MSC_VER
#pragma warning(pop)
#endif

} // namespace DuiLib

