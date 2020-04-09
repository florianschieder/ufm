#include "Image.h"

METHOD Image::Image(Window* parent, String path, int x, int y, int w, int h) : Control(parent)
{
    this->m_x = x;
    this->m_y = y;
    this->m_width = w;
    this->m_height = h;

    this->path = path;

    this->img = new Gdiplus::Image(path.c_str());
    
    this->realDX = img->GetWidth();
    this->realDY = img->GetHeight();
}

METHOD Image::~Image() {
    delete this->img;
}

METHOD void Image::Show() {
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
        (HMENU)-2,
        this->m_parentWindow->GetApplication()->GetInstance(),
        this);

    SetWindowLongPtr(
        this->m_controlHandle,
        GWLP_USERDATA,
        (LONG_PTR)this);

    SetWindowSubclass(
        this->m_controlHandle,
        &this->MessageLoopForwarder,
        0,
        0);
}

METHOD int Image::GetRealWidth()
{
    return this->realDX;
}

METHOD int Image::GetRealHeight()
{
    return this->realDY;
}

METHOD bool Image::CanResize()
{
    return canResize;
}

METHOD LRESULT Image::MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    Image* img = (Image*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    return img->MessageLoop(hwnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);
}

METHOD LRESULT Image::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
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

METHOD void Image::OnDraw(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    if (this->m_width < 400 && this->m_height < 300) {
        graphics.DrawImage(this->img, 0, 0, this->img->GetWidth(), this->img->GetHeight());
        canResize = false;
    }
    else {
        graphics.DrawImage(this->img, 0, 0, this->m_width, this->m_height);
    }
}
