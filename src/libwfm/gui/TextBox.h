#pragma once

#include "InputBox.h"

class TextBox : public InputBox
{
    public:
        METHOD TextBox(Window* parent);
        METHOD ~TextBox();
        METHOD void Show();
    private:
        HFONT hFont;
};
