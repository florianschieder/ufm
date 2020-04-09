#include <Windows.h>

#include "toolkit/Application.h"
#include "toolkit/Icon.h"
#include "toolkit/Window.h"

#include "../res/resource.h"

// Define about dialog specific IDs here

#define IDC_ABOUTWINDOW_CLOSE_BUTTON 201

// Class declaration

class AboutWindow : public Window {
    public:
        AboutWindow(Application* app, Window* parent);

    private:
        void OnClose();
        void OnInitializeWindow();
        void OnMenuEvent(WORD menuID);
        void OnPaint(PAINTSTRUCT ps, HDC hdc);
        void OnResizeWindow();
};
