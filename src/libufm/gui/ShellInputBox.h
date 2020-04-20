#pragma once

#include "InputBox.h"

#include <Shlwapi.h>

class ShellInputBox : public InputBox
{
    public:
        METHOD ShellInputBox(Window* parent, int x, int y, int width, int height);
        METHOD void Show();
};
