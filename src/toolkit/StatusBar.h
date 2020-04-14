#pragma once

#include "Control.h"

class StatusBar : public Control {
    public:
        METHOD StatusBar(Window* parent, int x, int y, int w, int h);
        METHOD ~StatusBar();

        METHOD void AddControl(std::shared_ptr<Control> control);
        METHOD void Show();

        METHOD void AddStartGripper(int dx = 0);

        METHOD void OnDraw(HDC hdc);

        bool m_DrawResize = true;

    private:
        METHOD static LRESULT CALLBACK MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
        METHOD LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

        METHOD void DrawStartGripper(HDC hdc, int dx);

        int drawStartGripperX;

        HBRUSH whiteBrush;
        HBRUSH blackBrush;
        HBRUSH sepBrush;
};
