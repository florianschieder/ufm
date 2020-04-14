#include "TextBox.h"

METHOD TextBox::TextBox(Window* parent) : InputBox(parent)
{
    this->AddSpecificStyle(ES_MULTILINE | WS_VSCROLL | WS_HSCROLL);

    this->hFont = CreateFont(
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
}

METHOD TextBox::~TextBox()
{
    DeleteObject(this->hFont);
}

METHOD void TextBox::Show()
{
    InputBox::Show();

    SendMessage(
        this->m_controlHandle,
        WM_SETFONT,
        (WPARAM) this->hFont,
        TRUE);
}
