#pragma once

#include <fstream>
#include <sstream>

#include <libufm.h>

using namespace libufm::GUI;

class FileViewerWindow : public Window
{
    public:
        FileViewerWindow(Application* app, String ext);
        ~FileViewerWindow();

        void SetFile(const wchar_t* file);

    private:
        void LoadFile();

        void OnClose();
        void OnInitializeWindow();
        void OnKeyDown(DWORD);
        void OnMenuEvent(WORD menuID);
        void OnPaint(PAINTSTRUCT ps, HDC hdc);
        void OnPostParam(void* param, int reason);
        void OnResizeWindow();

        String fileExtension;
        String fileName; 

        Image* image = 0;
        TextBox* fileView = 0;
        StatusBar* statusBar = 0;
};
