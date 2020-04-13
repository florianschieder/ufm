#pragma once

#include <fstream>
#include <sstream>

#include "toolkit/Image.h"
#include "toolkit/Window.h"
#include "toolkit/StatusBar.h"
#include "toolkit/TextBox.h"

#include "toolkit/core/FileType.h"

class FileViewerWindow : public Window
{
    public:
        FileViewerWindow(Application* app, String ext);
        ~FileViewerWindow();

        void SetFile(const wchar_t* file);

    private:
        void ReadBinary();
        void ReadText(FILE* fp);

        void OnClose();
        void OnInitializeWindow();
        void OnKeyDown(DWORD);
        void OnMenuEvent(WORD menuID);
        void OnPaint(PAINTSTRUCT ps, HDC hdc);
        void OnPostParam(void* param, int reason);
        void OnResizeWindow();
        
        char* fileContent = 0;
        long fileSize;

        String fileExtension;
        String fileName; 

        Image* image = 0;
        TextBox* fileView = 0;
        StatusBar* statusBar = 0;
};
