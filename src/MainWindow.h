#pragma once

#include <libufm.h>

#include "FileViewerWindow.h"

using namespace libufm::GUI;

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
#define NEEDED_TO_PASS_RENAME_OBJECT_NAME 2

// Class declaration

class MainWindow : public Window {
    public:
        MainWindow(Application* app);
        ~MainWindow();

    private:
        static INT_PTR AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static INT_PTR NewDirDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static INT_PTR RenameObjectDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

        EventHandler RefreshButtonClicked(Window* window);
        EventHandler DetailsButtonClicked(Window* window);
        EventHandler ListButtonClicked(Window* window);
        EventHandler SmallIconsButtonClicked(Window* window);
        EventHandler LargeIconsButtonClicked(Window* window);
        EventHandler PrintButtonClicked(Window* window);
        EventHandler AdminButtonClicked(Window* window);
        EventHandler PropertiesButtonClicked(Window* window);
        EventHandler RenameButtonClicked(Window* window);
        EventHandler CmdButtonClicked(Window* window);

        EventHandler FileViewerButtonClicked(Window* window);
        EventHandler EditButtonClicked(Window* window);
        EventHandler CopyButtonClicked(Window* window);
        EventHandler MoveButtonClicked(Window* window);
        EventHandler DeleteButtonClicked(Window* window);
        EventHandler NewDirButtonClicked(Window* window);

        EventHandler LeftApplyButtonClicked(Window* window);
        EventHandler RightApplyButtonClicked(Window* window);
        EventHandler ComboBoxSelectionChanged(ComboBox* object, Window* parent);
        EventHandler ShellListViewSelectionChanged(ShellListView* object, Window* parent);

        void OnClose();
        void OnInitializeWindow();
        void OnKeyDown(DWORD key);
        void OnMenuEvent(WORD menuID);
        void OnPaint(PAINTSTRUCT ps, HDC hdc);
        void OnPostParam(void* param, int reason);
        void OnResizeWindow();

        ToolBar* toolBar = 0;

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

        ToolButton* refreshButton = 0;
        ToolButton* detailsButton = 0;
        ToolButton* listButton = 0;
        ToolButton* smallButton = 0;
        ToolButton* largeButton = 0;
        ToolButton* printButton = 0;
        ToolButton* adminButton = 0;
        ToolButton* propertyButton = 0;
        ToolButton* renameButton = 0;
        ToolButton* cmdButton = 0;
        
        DriveComboBox* leftBox = 0;
        DriveComboBox* rightBox = 0;
        ShellInputBox* leftPath = 0;
        ShellInputBox* rightPath = 0;
        Button* leftBoxApply = 0;
        Button* rightBoxApply = 0;
};
