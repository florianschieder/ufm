#pragma once

#include <Windows.h>
#include <gdiplus.h>

#include "Control.h"

class Image : public Control {
    public:
        METHOD Image(Window* parent, String path, int x, int y, int w, int h);
        METHOD ~Image();
        METHOD void Show();
        METHOD int GetRealWidth();
        METHOD int GetRealHeight();
        METHOD bool CanResize();

    private:
        int realDX;
        int realDY;
        bool canResize = true;

        Gdiplus::Image* img;
        String path;

        METHOD static LRESULT CALLBACK MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
        METHOD LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

        METHOD void OnDraw(HDC hdc);
};
