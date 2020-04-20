#include "Icon.h"

METHOD Icon::Icon(Window* parent, int id, int x, int y, int w, int h) : Control(parent)
{
    this->m_x = x;
    this->m_y = y;
    this->m_width = w;
    this->m_height = h;

    this->iconID = id;
    this->icon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(id));
}

METHOD void Icon::Show()
{
    this->m_controlHandle = CreateWindowEx(
        0,
        L"STATIC",
        L"",
        WS_CHILD | WS_VISIBLE,
        this->m_x,
        this->m_y,
        this->m_width,
        this->m_height,
        this->m_parentWindow->GetHandle(),
        (HMENU)-1,
        this->m_parentWindow->GetApplication()->GetInstance(),
        this);

    SetWindowPos(
        this->m_controlHandle,
        HWND_NOTOPMOST,
        this->m_x,
        this->m_y,
        this->m_width,
        this->m_height,
        0);

    SetWindowLongPtr(
        this->m_controlHandle,
        GWLP_USERDATA,
        (LONG_PTR)this);

    SetWindowSubclass(
        this->m_controlHandle,
        this->MessageLoopForwarder,
        0,
        0);
}

METHOD LRESULT Icon::MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    Icon* toolbar = (Icon*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    return toolbar->MessageLoop(hwnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);
}

METHOD LRESULT Icon::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg)
    {
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc;

            hdc = BeginPaint(hwnd, &ps);

            this->OnDraw(hdc);

            EndPaint(hwnd, &ps);

            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

METHOD void Icon::OnDraw(HDC hdc)
{
    DrawIcon(hdc, 0, 0, this->icon);
}
