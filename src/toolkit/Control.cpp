#include "Control.h"

METHOD Control::Control(Window* parent)
{
    this->defaultFont = CreateFont(
        PT(9),
        0,
        0,
        0,
        FW_REGULAR,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Tahoma"));

    this->m_className = (wchar_t*) L"";
    this->m_parentWindow = parent;
    this->m_controlHandle = { 0 };
    this->m_ctrlID = 0;

    this->m_x = 0;
    this->m_y = 0;
    this->m_width = 0;
    this->m_height = 0;

    this->SetID(parent->controlIDSequence);
    parent->controlIDSequence++;
}

METHOD HWND Control::GetHandle()
{
    return this->m_controlHandle;
}

METHOD void Control::Resize(int x, int y, int width, int height)
{
    this->m_x = x;
    this->m_y = y;
    this->m_width = width;
    this->m_height = height;

    SetWindowPos(
        this->m_controlHandle,
        0,
        x,
        y,
        width,
        height,
        SWP_NOZORDER | SWP_SHOWWINDOW);
}

METHOD String Control::GetText()
{
    TCHAR buffer[512];
    GetWindowText(this->m_controlHandle, buffer, 512);
    return buffer;
}

METHOD Window* Control::GetParent()
{
    return this->m_parentWindow;
}

METHOD int Control::GetX()
{
    return this->m_x;
}

METHOD int Control::GetY()
{
    return this->m_y;
}

METHOD int Control::GetWidth()
{
    return this->m_width;
}

METHOD int Control::GetHeight()
{
    return this->m_height;
}

METHOD void Control::SetDimensions(int x, int y, int w, int h)
{
    this->m_x = x;
    this->m_y = y;
    this->m_width = w;
    this->m_height = h;
}

void Control::SetID(int id)
{
    this->m_ctrlID = id;
}

METHOD void Control::AddSpecificStyle(int style)
{
    this->specificStyles = this->specificStyles | style;
}
