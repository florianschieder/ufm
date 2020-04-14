#include "Button.h"

METHOD Button::Button(Window* parent) : Control(parent)
{
    this->m_controlHandle = { 0 };
    this->SetText(L"");

    this->iconID = -1;
    this->iconWidth = 0;
    this->iconHeight = 0;
}

METHOD Button::~Button()
{
    DestroyIcon(this->iconHandle);
}

METHOD void Button::SetDimensions(int x, int y, int w, int h)
{
    this->m_x = x;
    this->m_y = y;
    this->m_width = w;
    this->m_height = h;
}

METHOD void Button::Show()
{
    this->m_controlHandle = CreateWindowEx(
        0,
        L"BUTTON",
        this->Text,
        WS_VISIBLE | WS_CHILD | BS_ICON | BS_NOTIFY,
        this->m_x,
        this->m_y,
        this->m_width,
        this->m_height,
        this->m_parentWindow->GetHandle(),
        (HMENU) this->m_ctrlID,
        this->m_parentWindow->GetApplication()->GetInstance(),
        this);

    SetWindowLongPtr(
        m_controlHandle,
        GWLP_USERDATA,
        (LONG_PTR) this);

    SetWindowSubclass(
        m_controlHandle,
        this->MessageLoopForwarder,
        0,
        0);

    this->iconHandle = (HICON) LoadImage(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCEW(this->iconID),
        IMAGE_ICON,
        this->iconWidth, this->iconHeight,
        LR_DEFAULTCOLOR);

    SendMessage(
        this->m_controlHandle,
        BM_SETIMAGE,
        IMAGE_ICON,
        (LPARAM) this->iconHandle);

    SendMessage(
        this->m_controlHandle,
        WM_SETFONT,
        (WPARAM) this->defaultFont,
        TRUE);
}

METHOD void Button::SetText(const wchar_t* Text)
{
    this->Text = (wchar_t*) Text;
}

METHOD void Button::SetIcon(int iconID, int w, int h)
{
    this->iconID = iconID;
    this->iconWidth = w;
    this->iconHeight = h;
}

METHOD LRESULT Button::MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    Button* button = (Button*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    return button->MessageLoop(hwnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);
}

METHOD LRESULT Button::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg)
    {
        case WM_COMMAND:
            switch (HIWORD(wParam))
            {
                case BN_CLICKED:
                    if (this->OnClick != nullptr)
                        this->OnClick(this->m_parentWindow);
                    break;
                case BN_DBLCLK:
                    break;
            }
    }
    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}
