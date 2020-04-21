#pragma once

#include <fstream>

#include "InputBox.h"

class TextBox : public InputBox
{
    public:
        METHOD TextBox(Window* parent);
        METHOD ~TextBox();
        METHOD void Show();
        METHOD void ReadBinaryFile(String file);
        METHOD void ReadTextFile(String file);
    private:
        HFONT hFont;
};
