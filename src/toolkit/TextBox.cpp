#include "TextBox.h"

METHOD TextBox::TextBox(Window* parent) : InputBox(parent)
{
    this->AddSpecificStyle(ES_MULTILINE | WS_VSCROLL | WS_HSCROLL);
}
