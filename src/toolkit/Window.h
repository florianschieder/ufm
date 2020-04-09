#pragma once

#include "wfmtkdef.h"

#include <memory>

#include "Application.h"

class Window
{
    Event OnClose() = 0;
    Event OnInitializeWindow() = 0;
    Event OnMenuEvent(WORD id) = 0;
    Event OnPaint(PAINTSTRUCT ps, HDC hdc) = 0;
    Event OnResizeWindow() = 0;

    public:
        METHOD Window(Application* app);
        METHOD Window(Application* app, Window* parent);

        METHOD void Close();
        METHOD void Destroy();
        METHOD bool IsOpen();
        METHOD void Show();
        METHOD void SetDimensions(int width, int height);
        
        METHOD Application* GetApplication();
        METHOD HWND GetHandle();
        METHOD int GetStyle();
        METHOD String GetTitle();

        METHOD void SetIcon(HICON hIcon);
        METHOD void SetStyle(int style);
        METHOD void SetTitle(String Title);

        METHOD operator HWND();

        int controlIDSequence = 50000;
        void* ActiveControl = nullptr;
        
    private:
        METHOD int Create();

    protected:
        bool m_isOpen = false;

        int m_style;
        int m_iconID;
        int m_menuID;
        int m_width;
        int m_height;

        Application* m_application;
        HWND m_windowHandle;
        Window* m_parentWindow;
        WNDCLASS m_windowClass;

        METHOD static LRESULT CALLBACK MessageLoopForwarder(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        METHOD LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
