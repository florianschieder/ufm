#include "InputBox.h"

METHOD InputBox::InputBox(Window* parent) : Control(parent)
{
    this->m_controlHandle = { 0 };
}

METHOD InputBox::InputBox(Window* parent, int x, int y, int w, int h) : InputBox(parent)
{
    this->SetDimensions(x, y, w, h);
}

METHOD void InputBox::Show()
{
    HFONT hFont = CreateFont(
        PT(10),
        0,
        0,
        0,
        FW_DONTCARE,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Courier New"));

    this->m_controlHandle = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | specificStyles,
        this->m_x,
        this->m_y,
        this->m_width,
        this->m_height,
        this->m_parentWindow->GetHandle(),
        (HMENU)0,
        this->m_parentWindow->GetApplication()->GetInstance(),
        NULL);

    SetWindowLongPtr(
        m_controlHandle,
        GWLP_USERDATA,
        (LONG_PTR)this);

    SetWindowSubclass(
        this->m_controlHandle,
        this->MessageLoopForwarder,
        0,
        0);

    SendMessage(
        this->m_controlHandle,
        WM_SETFONT,
        (WPARAM)hFont,
        TRUE);
}

METHOD void InputBox::SetDimensions(int x, int y, int w, int h)
{
    this->m_x = x;
    this->m_y = y;
    this->m_width = w;
    this->m_height = h;
}

METHOD void InputBox::SetText(wchar_t* Text)
{
    SendMessage(this->m_controlHandle, WM_SETTEXT, NULL, (LPARAM) Text);
}

METHOD LRESULT InputBox::MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    InputBox* input = (InputBox*) GetWindowLongPtr(
        hwnd,
        GWLP_USERDATA);

    return input->MessageLoop(hwnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);
}

METHOD LRESULT InputBox::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg)
    {
        case WM_CHAR:
            if (this->OnTextChanged != nullptr)
                this->OnTextChanged();
            break;
    }

    return DefSubclassProc(
        hwnd,
        uMsg,
        wParam,
        lParam);
}
