#include "StdAfx.h"
#include "UIPopup.h"

namespace DuiLib {

CPopupWindow::CPopupWindow() : anchor_{0, 0}, builderCallback_(NULL) {}
CPopupWindow::~CPopupWindow() = default;

bool CPopupWindow::ShowPopup(HWND owner, POINT anchor, LPCTSTR xml,
    IDialogBuilderCallback* builderCallback, bool takeFocus)
{
    if( owner == NULL || xml == NULL || *xml == _T('\0') ) return false;
    anchor_ = anchor;
    xml_ = xml;
    builderCallback_ = builderCallback;
    if( Create(owner, NULL, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        WS_EX_TOOLWINDOW) == NULL ) return false;
    ShowWindow(true, takeFocus);
    if( takeFocus ) ::SetForegroundWindow(m_hWnd);
    return true;
}

CPaintManagerUI* CPopupWindow::GetPopupManager() { return &manager_; }
void CPopupWindow::ClosePopup() { Close(); }
LPCTSTR CPopupWindow::GetWindowClassName() const { return _T("DuiPopupWindow"); }
void CPopupWindow::OnFinalMessage(HWND) { delete this; }
void CPopupWindow::Notify(TNotifyUI& message) { OnPopupNotify(message); }
void CPopupWindow::OnPopupCreated(CControlUI*) {}
void CPopupWindow::OnPopupNotify(TNotifyUI&) {}
bool CPopupWindow::CloseOnFocusLost() const { return true; }

RECT CPopupWindow::FitToMonitor(RECT window, POINT anchor, HWND reference)
{
    const int width = window.right - window.left;
    const int height = window.bottom - window.top;
    MONITORINFO monitor{sizeof(monitor)};
    ::GetMonitorInfo(::MonitorFromWindow(reference, MONITOR_DEFAULTTONEAREST), &monitor);
    window = {anchor.x, anchor.y, anchor.x + width, anchor.y + height};
    if( window.right > monitor.rcWork.right ) { window.right = monitor.rcWork.right; window.left = window.right - width; }
    if( window.bottom > monitor.rcWork.bottom ) { window.bottom = monitor.rcWork.bottom; window.top = window.bottom - height; }
    if( window.left < monitor.rcWork.left ) { window.left = monitor.rcWork.left; window.right = window.left + width; }
    if( window.top < monitor.rcWork.top ) { window.top = monitor.rcWork.top; window.bottom = window.top + height; }
    return window;
}

void CPopupWindow::AdjustPosition()
{
    RECT window{};
    ::GetWindowRect(m_hWnd, &window);
    window = FitToMonitor(window, anchor_, m_hWnd);
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, window.left, window.top, 0, 0,
        SWP_NOSIZE | SWP_NOACTIVATE);
}

LRESULT CPopupWindow::OnCreate()
{
    manager_.Init(m_hWnd);
    CDialogBuilder builder;
    CControlUI* root = builder.Create(xml_.c_str(), NULL, builderCallback_, &manager_);
    if( root == NULL ) return -1;
    manager_.AttachDialog(root);
    manager_.AddNotifier(this);
    OnPopupCreated(root);
    AdjustPosition();
    return 0;
}

LRESULT CPopupWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    if( message == WM_CREATE ) return OnCreate();
    if( message == WM_KEYDOWN && wParam == VK_ESCAPE ) { ClosePopup(); return 0; }
    if( message == WM_KILLFOCUS && CloseOnFocusLost() ) {
        HWND focus = reinterpret_cast<HWND>(wParam);
        if( focus != m_hWnd && !::IsChild(m_hWnd, focus) ) ClosePopup();
    }
    LRESULT result = 0;
    if( manager_.MessageHandler(message, wParam, lParam, result) ) return result;
    return CWindowWnd::HandleMessage(message, wParam, lParam);
}

} // namespace DuiLib
