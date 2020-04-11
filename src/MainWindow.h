#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#include "AboutWindow.h"
#include "FileViewerWindow.h"

#include "toolkit/Application.h"
#include "toolkit/Button.h"
#include "toolkit/Icon.h"
#include "toolkit/InputBox.h"
#include "toolkit/ListView.h"
#include "toolkit/Window.h"

#include "toolkit/ToolBar.h"
#include "toolkit/StatusBar.h"

// Define control IDs for this window

#define IDC_MAINWINDOW_BACK_BUTTON 101
#define IDC_MAINWINDOW_FORWARD_BUTTON 102
#define IDC_MAINWINDOW_UP_BUTTON 103
#define IDC_MAINWINDOW_REFRESH_BUTTON 104
#define IDC_MAINWINDOW_REBAR 105
#define IDC_MAINWINDOW_ADDRESSBAR 106
#define IDC_MAINWINDOW_LISTVIEW 107
#define IDC_MAINWINDOW_STATUSBAR 120

// Class declaration

class MainWindow : public Window {
    public:
        MainWindow(Application* app);
        ~MainWindow();

    private:
        EventHandler FileViewerButtonClicked(Window* window);
        EventHandler EditButtonClicked(Window* window);

        void OnClose();
        void OnInitializeWindow();
        void OnMenuEvent(WORD menuID);
        void OnPaint(PAINTSTRUCT ps, HDC hdc);
        void OnResizeWindow();

        ToolBar* buttonBar = 0;
        ToolBar* driveBar = 0;

        StatusBar* statusbar = 0;

        ShellListView* leftShellView = 0;
        ShellListView* rightShellView = 0;

        Button* showButton = 0;
        Button* editButton = 0;
        Button* copyButton = 0;
        Button* moveButton = 0;
        Button* newDirectoryButton = 0;
        Button* deleteButton = 0;
        Button* closeButton = 0;

        AboutWindow* aboutDlg = 0;
};
