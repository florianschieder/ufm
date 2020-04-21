#include "StatusBar.h"

METHOD StatusBar::StatusBar(Window* parent, int x, int y, int w, int h) : Control(parent)
{
    this->m_x = x;
    this->m_y = y;
    this->m_width = w;
    this->m_height = h;

    this->m_DrawResize = true;
    this->drawStartGripperX = 0;

    this->blackBrush = 0;
    this->whiteBrush = 0;
    this->sepBrush = 0;
}

METHOD StatusBar::~StatusBar()
{
}

void StatusBar::AddControl(std::shared_ptr<Control> control)
{
    SetParent(control->GetHandle(), this->m_controlHandle);
}

void StatusBar::Show()
{
    this->m_controlHandle = CreateWindowEx(
        0,
        STATUSCLASSNAME,
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

METHOD void StatusBar::AddStartGripper(int dx)
{
    this->drawStartGripperX = dx;
}

METHOD void StatusBar::DrawStartGripper(HDC hdc, int dx)
{
    RECT rect1, rect2;

    for (int i = 5; i <= this->m_height - 3; i += 4)
    {   
        rect1.left = 5 + dx;
        rect1.right = 5 + dx + 3;
        rect1.top = i;
        rect1.bottom = i + 3;

        rect2.left = 5 + dx;
        rect2.right = 5 + dx + 2;
        rect2.top = i;
        rect2.bottom = i + 2;

        FillRect(hdc, &rect1, this->whiteBrush);
        FillRect(hdc, &rect2, this->blackBrush);
    }
}

void StatusBar::OnDraw(HDC hdc)
{
    RECT rect;

    rect.left = 0;
    rect.right = this->m_width;
    rect.top = 0;
    rect.bottom = 1;
    FillRect(
        hdc,
        &rect,
        this->sepBrush);

    Gdiplus::Graphics graphics(hdc);
    Gdiplus::RectF rectF(
        (Gdiplus::REAL) 0,
        (Gdiplus::REAL) 1,
        (Gdiplus::REAL) this->m_width,
        (Gdiplus::REAL) this->m_height - 1);

    Gdiplus::LinearGradientBrush brush(
        rectF,
        Gdiplus::Color(240, 240, 240),
        Gdiplus::Color(208, 208, 208),
        Gdiplus::LinearGradientMode::LinearGradientModeVertical);

    graphics.FillRectangle(&brush, rectF);

    this->DrawStartGripper(hdc, this->drawStartGripperX);

    if (this->m_DrawResize)
    {
        // Upper "line" / point

        rect.left = this->m_width - 5;
        rect.right = this->m_width - 2;
        rect.top = this->m_height - 13;
        rect.bottom = this->m_height - 10;
        FillRect(
            hdc,
            &rect,
            this->whiteBrush);
        
        rect.left = this->m_width - 5;
        rect.right = this->m_width - 3;
        rect.top = this->m_height - 13;
        rect.bottom = this->m_height - 11;
        FillRect(
            hdc,
            &rect,
            this->blackBrush);

        // Mid line

        rect.left = this->m_width - 5;
        rect.right = this->m_width - 2;
        rect.top = this->m_height - 9;
        rect.bottom = this->m_height - 6;

        FillRect(
            hdc,
            &rect,
            this->whiteBrush);

        rect.left = this->m_width - 5;
        rect.right = this->m_width - 3;
        rect.top = this->m_height - 9;
        rect.bottom = this->m_height - 7;
        FillRect(
            hdc,
            &rect,
            this->blackBrush);

        rect.left = this->m_width - 9;
        rect.right = this->m_width - 6;
        rect.top = this->m_height - 9;
        rect.bottom = this->m_height - 6;
        FillRect(
            hdc,
            &rect,
            this->whiteBrush);

        rect.left = this->m_width - 9;
        rect.right = this->m_width - 7;
        rect.top = this->m_height - 9;
        rect.bottom = this->m_height - 7;
        FillRect(
            hdc,
            &rect,
            this->blackBrush);

        // Lower line

        rect.left = this->m_width - 5;
        rect.right = this->m_width - 2;
        rect.top = this->m_height - 5;
        rect.bottom = this->m_height - 2;
        FillRect(
            hdc,
            &rect,
            this->whiteBrush);

        rect.left = this->m_width - 5;
        rect.right = this->m_width - 3;
        rect.top = this->m_height - 5;
        rect.bottom = this->m_height - 3;
        FillRect(
            hdc,
            &rect,
            this->blackBrush);

        rect.left = this->m_width - 9;
        rect.right = this->m_width - 6;
        rect.top = this->m_height - 5;
        rect.bottom = this->m_height - 2;
        FillRect(
            hdc,
            &rect,
            this->whiteBrush);

        rect.left = this->m_width - 9;
        rect.right = this->m_width - 7;
        rect.top = this->m_height - 5;
        rect.bottom = this->m_height - 3;
        FillRect(
            hdc,
            &rect,
            this->blackBrush);

        rect.left = this->m_width - 13;
        rect.right = this->m_width - 10;
        rect.top = this->m_height - 5;
        rect.bottom = this->m_height - 2;
        FillRect(
            hdc,
            &rect,
            this->whiteBrush);

        rect.left = this->m_width - 13;
        rect.right = this->m_width - 11;
        rect.top = this->m_height - 5;
        rect.bottom = this->m_height - 3;
        FillRect(
            hdc,
            &rect,
            this->blackBrush);
    }
}

METHOD LRESULT StatusBar::MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    StatusBar* statusbar = (StatusBar*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    return statusbar->MessageLoop(hwnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);
}

METHOD LRESULT StatusBar::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
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

            hdc = BeginPaint(hwnd, &ps);

            this->blackBrush = CreateSolidBrush(
                RGB(0, 0, 0));
            this->whiteBrush = CreateSolidBrush(
                RGB(255, 255, 255));
            this->sepBrush = CreateSolidBrush(
                RGB(182, 188, 204));

            this->OnDraw(hdc);

            DeleteObject(this->blackBrush);
            DeleteObject(this->whiteBrush);
            DeleteObject(this->sepBrush);

            EndPaint(hwnd, &ps);

            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
