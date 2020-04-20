#pragma once

#include "Control.h"
#include "Window.h"

class InputBox : public Control
{
    public:
        METHOD InputBox(Window* parent);
        METHOD InputBox(Window* parent, int x, int y, int w, int h);
        METHOD void Show();
        METHOD void SetDimensions(int x, int y, int w, int h);
        METHOD void SetText(wchar_t* Text);

    protected:
        wchar_t* Text;
        void* (*OnTextChanged)() = nullptr;

        METHOD static LRESULT CALLBACK MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
        METHOD LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
};

