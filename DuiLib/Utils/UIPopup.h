#pragma once

namespace DuiLib {

class UILIB_API CPopupWindow : public CWindowWnd, public INotifyUI
{
public:
    CPopupWindow();
    virtual ~CPopupWindow();

    bool ShowPopup(HWND owner, POINT anchor, LPCTSTR xml,
        IDialogBuilderCallback* builderCallback = NULL, bool takeFocus = true);
    CPaintManagerUI* GetPopupManager();
    void ClosePopup();
    static RECT FitToMonitor(RECT window, POINT anchor, HWND reference);

    LPCTSTR GetWindowClassName() const override;
    void OnFinalMessage(HWND hWnd) override;
    void Notify(TNotifyUI& message) override;
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
    virtual void OnPopupCreated(CControlUI* root);
    virtual void OnPopupNotify(TNotifyUI& message);
    virtual bool CloseOnFocusLost() const;

private:
    LRESULT OnCreate();
    void AdjustPosition();

    POINT anchor_;
    tstring xml_;
    IDialogBuilderCallback* builderCallback_;
    CPaintManagerUI manager_;
};

} // namespace DuiLib
