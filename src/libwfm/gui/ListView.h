#pragma once

#include "Control.h"

class ListView : public Control
{
    public:
        METHOD ListView(Window* parent);
        METHOD void Show();
        METHOD void SetView(int view);
        METHOD void SetDimensions(int x, int y, int w, int h);
        METHOD void AddColumn(String Header, int width, int pos);
        METHOD void AddItem(int id, int imageID = -1);
        METHOD void AddSubItem(int id, int subID, String Text);
        METHOD void InitializeTimeIntensiveProcess();
        METHOD void UninitializeTimeIntensiveProcess();

    protected:
        METHOD static LRESULT CALLBACK MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
        METHOD virtual LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

        int specificStyles = 0;
};
