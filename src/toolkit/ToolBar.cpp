#include "Toolbar.h"

METHOD ToolBar::ToolBar(Window* parent, int x, int y, int w, int h) : Control(parent)
{
    this->m_x = x;
    this->m_y = y;
    this->m_width = w;
    this->m_height = h;

    this->drawStartGripperX = -1;

    switch (this->m_parentWindow->GetApplication()->GetEnvironment().OSVersionCode)
    {
        case OS_10:
            this->toolbarBrush = CreateSolidBrush(
                RGB(217, 223, 240));
            break;

        case OS_8:
        case OS_8_1:
            this->toolbarBrush = CreateSolidBrush(
                RGB(240, 240, 240));
            break;

        case OS_7:
        case OS_VISTA:
            this->toolbarBrush = CreateSolidBrush(
                RGB(217, 223, 240));
            break;

        case OS_XP:
            this->toolbarBrush = CreateSolidBrush(
                RGB(236, 233, 216));
            break;

        default:
            this->toolbarBrush = CreateSolidBrush(
                RGB(240, 240, 240));
            break;
    }
}

void ToolBar::AddControl(std::shared_ptr<Control> control)
{
    SetParent(control->GetHandle(), this->m_controlHandle);
}

void ToolBar::Show()
{
    this->m_controlHandle = CreateWindowEx(
        0,
        TOOLBARCLASSNAME,
        L"",
        WS_CHILD | WS_VISIBLE,
        this->m_x,
        this->m_y,
        this->m_width,
        this->m_height,
        this->m_parentWindow->GetHandle(),
        (HMENU) -1,
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
        (LONG_PTR) this);

    SetWindowSubclass(
        this->m_controlHandle,
        this->MessageLoopForwarder,
        0,
        0);
}

METHOD void ToolBar::AddStartGripper(int dx)
{
    this->drawStartGripperX = dx;
}

METHOD void ToolBar::AddSeparator(int x)
{
    this->separators.push_back(x);
}

METHOD void ToolBar::DrawStartGripper(HDC hdc, int dx)
{
    for (int i = 5; i <= this->m_height - 7; i += 4) {
        RECT rect;
        
        rect.left = 5 + dx;
        rect.right = rect.left + 3;
        rect.top = i;
        rect.bottom = rect.top + 3;
        FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));

        rect.left = 5 + dx;
        rect.right = rect.left + 2;
        rect.top = i;
        rect.bottom = rect.top + 2;
        FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 0, 0)));
    }
}

METHOD void ToolBar::DrawSeparator(HDC hdc, int x)
{
    RECT rect;

    rect = { x, 5, 1, this->m_height - 10 };
    FillRect(hdc, &rect, CreateSolidBrush(RGB(182, 188, 204)));

    rect = { 1 + x, 5, 1, this->m_height - 10 };
    FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
}

void ToolBar::OnDraw(HDC hdc)
{    
    RECT rect;

    rect.left = 0;
    rect.right = this->m_width;
    rect.top = 0;
    rect.bottom = this->m_height;
    FillRect(hdc, &rect, this->toolbarBrush);

    rect.left = 0;
    rect.right = this->m_width;
    rect.top = this->m_height - 2;
    rect.bottom = this->m_height - 1;
    FillRect(hdc, &rect, CreateSolidBrush(RGB(182, 188, 204)));
    
    rect.left = 0;
    rect.right = this->m_width;
    rect.top = this->m_height - 1;
    rect.bottom = this->m_height;
    FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));

    if(this->drawStartGripperX != -1) 
        this->DrawStartGripper(hdc, this->drawStartGripperX);
    
    for (unsigned int i = 0; i < this->separators.size(); i++) {
        this->DrawSeparator(hdc, this->separators[i]);
    }
}

METHOD LRESULT ToolBar::MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    ToolBar* toolbar = (ToolBar*) GetWindowLongPtr(hwnd, GWLP_USERDATA);

    return toolbar->MessageLoop(
        hwnd,
        uMsg,
        wParam,
        lParam,
        uIdSubclass,
        dwRefData);
}

METHOD LRESULT ToolBar::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg)
    {
        case WM_COMMAND:
            switch (HIWORD(wParam))
            {
                case BN_CLICKED:
                    SendMessage(
                        GetDlgItem(hwnd, LOWORD(wParam)),
                        WM_COMMAND,
                        wParam,
                        lParam);
                    break;
            }
            break;

        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc;

            InvalidateRgn(this->m_controlHandle, NULL, TRUE);

            hdc = BeginPaint(hwnd, &ps);

            this->OnDraw(hdc);

            EndPaint(hwnd, &ps);

            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
