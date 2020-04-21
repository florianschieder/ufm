#pragma once

#include "Control.h"

class Icon : public Control {
    public:
        METHOD Icon(Window* parent, int id, int x, int y, int w, int h);
        METHOD void Show();

    private:
        METHOD static LRESULT CALLBACK MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
        METHOD LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

        METHOD void OnDraw(HDC hdc);

        int iconID;
        HICON icon;
};