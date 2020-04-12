#include "ComboBox.h"

METHOD ComboBox::ComboBox(Window* parent) : Control(parent)
{
}

METHOD void ComboBox::AddItem(String item)
{
    SendMessage(
        this->m_controlHandle,
        CB_ADDSTRING,
        0,
        (LPARAM) item.c_str());
}

METHOD void ComboBox::Show()
{
    HFONT hFont = CreateFont(
        PT(8),
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

    this->m_controlHandle = CreateWindowEx(
        0,
        WC_COMBOBOX,
        L"",
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
        this->m_x,
        this->m_y,
        this->m_width,
        this->m_height,
        this->m_parentWindow->GetHandle(),
        (HMENU)this->m_ctrlID,
        this->m_parentWindow->GetApplication()->GetInstance(),
        this);

    SetWindowLongPtr(
        m_controlHandle,
        GWLP_USERDATA,
        (LONG_PTR)this);

    SetWindowSubclass(
        m_controlHandle,
        this->MessageLoopForwarder,
        0,
        0);

    SendMessage(
        this->m_controlHandle,
        WM_SETFONT,
        (WPARAM)hFont,
        TRUE);
}

METHOD LRESULT ComboBox::MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    ComboBox* box = (ComboBox*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    return box->MessageLoop(hwnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);
}

METHOD LRESULT ComboBox::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg)
    {
    default:
        break;
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}
