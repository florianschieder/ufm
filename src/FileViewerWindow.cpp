#include "FileViewerWindow.h"

using namespace libufm::Core::FileDetection;

FileViewerWindow::FileViewerWindow(Application* app, String ext) : Window(app)
{
    this->fileExtension = ext;
}

FileViewerWindow::~FileViewerWindow()
{
    delete this->fileView;
    delete this->image;
    delete this->statusBar;
}

void FileViewerWindow::LoadFile()
{
    String mimeType = MimeTypeFromString(this->fileExtension);

    if (mimeType == L"application/unknown")
    {
        if (IsTextFile(this->fileName))
        {
            this->fileView->ReadTextFile(this->fileName);
        }
        else
        {
            this->fileView->ReadBinaryFile(this->fileName);
        }
    }
    else if (mimeType.substr(0, 4) == L"text")
    {
        this->fileView->ReadTextFile(this->fileName);
    }
    else
    {
        this->fileView->ReadBinaryFile(this->fileName);
    }
}

void FileViewerWindow::OnClose()
{
    this->Destroy();
}

void FileViewerWindow::OnInitializeWindow()
{
    SHFILEINFO fileInfo;

    ZeroMemory(
        &fileInfo,
        sizeof(fileInfo));

    SHGetFileInfo(
        this->fileName.c_str(),
        0,
        &fileInfo,
        sizeof(fileInfo),
        SHGFI_SMALLICON | SHGFI_ICON | SHGFI_ADDOVERLAYS);

    if (IsGdipSupportedImage(fileExtension))
    {
        this->image = new Image(this, this->fileName);
        this->image->Place(0, 0, this->Width, this->Height - 22);
    }
    else
    {
        this->fileView = new TextBox(this, true);
        this->fileView->Place(0, 0, this->Width, this->Height - 22);
        this->LoadFile();
    }

    this->statusBar = new StatusBar(this);
    this->statusBar->Place(0, this->Height - 22, this->Width, 22);
    this->statusBar->AddStartGripper();

    if (image != nullptr) {
        int rw, rh;

        rw = this->image->GetRealWidth();
        rh = this->image->GetRealHeight();
        if (this->image != nullptr && (rw >= 400 && rh >= 300))
        {
            this->Width = rw;
            this->Height = rh + 22;
        }
        else
        {
            this->Width = 400;
            this->Height = 322;
        }
    }
    else
    {
        this->Width = 800;
        this->Height = 600;
    }

    this->Icon = fileInfo.hIcon;
    this->Title = this->fileName;
}

void FileViewerWindow::OnKeyDown(DWORD)
{
}

void FileViewerWindow::OnMenuEvent(WORD menuID)
{
}

void FileViewerWindow::OnPaint(PAINTSTRUCT ps, HDC hdc)
{
}

void FileViewerWindow::OnPostParam(void* param, int reason)
{
}

void FileViewerWindow::SetFile(const wchar_t* file)
{
    this->fileName = file;
}

void FileViewerWindow::OnResizeWindow()
{
    if (fileView != nullptr)
    {
        this->fileView->Width = this->Width;
        this->fileView->Height = this->Height - 22;
    }

    if (image != nullptr)
    {
        if (image->CanResize())
        {
            this->image->Width = this->Width;
            this->image->Height = this->Height - 22;
        }
    }

    this->statusBar->Y = this->Height - 22;
    this->statusBar->Width = this->Width;
}
