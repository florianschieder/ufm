#include "TextBox.h"

METHOD TextBox::TextBox(Window* parent) : InputBox(parent)
{
    this->AddSpecificStyle(ES_MULTILINE | WS_VSCROLL | WS_HSCROLL);
}

METHOD void TextBox::Show()
{
    InputBox::Show();

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

    SendMessage(
        this->m_controlHandle,
        WM_SETFONT,
        (WPARAM) hFont,
        TRUE);
}
