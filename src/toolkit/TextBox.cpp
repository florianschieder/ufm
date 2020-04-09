#include "TextBox.h"

METHOD TextBox::TextBox(Window* parent) : InputBox(parent) {
    this->AddSpecificStyle(ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN);
}