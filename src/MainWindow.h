#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#include "FileViewerWindow.h"

#include "toolkit/Application.h"
#include "toolkit/Button.h"
#include "toolkit/Icon.h"
#include "toolkit/InputBox.h"
#include "toolkit/ListView.h"
#include "toolkit/ShellInputBox.h"
#include "toolkit/ShellListView.h"
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

#define NEEDED_TO_PASS_NEW_FOLDER_TEXT 1

// Class declaration

class MainWindow : public Window {
    public:
        MainWindow(Application* app);
        ~MainWindow();

    private:
        static INT_PTR AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static INT_PTR NewDirDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

        EventHandler FileViewerButtonClicked(Window* window);
        EventHandler EditButtonClicked(Window* window);
        EventHandler NewDirButtonClicked(Window* window);

        void OnClose();
        void OnInitializeWindow();
        void OnMenuEvent(WORD menuID);
        void OnPaint(PAINTSTRUCT ps, HDC hdc);
        void OnPostParam(void* param, int reason);
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
};
