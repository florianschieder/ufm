#pragma once

#include "Control.h"

class ToolBar : public Control
{
    public:
        METHOD ToolBar(Window* parent, int x, int y, int w, int h);
        METHOD ~ToolBar();

        METHOD void AddControl(Control* control);
        METHOD void Show();

        METHOD void AddStartGripper(int dx = 3);
        METHOD void AddSeparator(int x);

        METHOD void OnDraw(HDC hdc);

    private:
        METHOD static LRESULT CALLBACK MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
        METHOD LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

        METHOD void DrawStartGripper(HDC hdc, int dx);
        METHOD void DrawSeparator(HDC hdc, int x);

        int drawStartGripperX;
        std::vector<int> separators;
        
        HBRUSH toolbarSepBrush;
        HBRUSH whiteBrush;
        HBRUSH blackBrush;
        HBRUSH sepBrush;
};
