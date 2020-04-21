#pragma once

#include "Window.h"

class Control {
    public:
        METHOD Control(Window* parent);
        METHOD ~Control();

        METHOD HWND GetHandle();
        METHOD void Resize(int x, int y, int width, int height);
        
        METHOD void AddToolTip(const TCHAR text[80]);
        METHOD String GetText();
        METHOD Window* GetParent();
        METHOD int GetX();
        METHOD int GetY();
        METHOD int GetWidth();
        METHOD int GetHeight();

        METHOD void SetDimensions(int x, int y, int w, int h);
        METHOD void SetID(int id);

        METHOD virtual void Show() = 0;

        METHOD void AddSpecificStyle(int style);

    protected:
        HFONT defaultFont;

        int specificStyles = 0;

        int m_x;
        int m_y;
        int m_width;
        int m_height;

        long long m_ctrlID;
        
        String m_className;

        HWND m_controlHandle;
        Window* m_parentWindow;
};
