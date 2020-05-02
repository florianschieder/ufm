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
        this->image = new Image(this, this->fileName, 0, 0, this->m_width, this->m_height - 22);
        this->image->Show();
    }
    else
    {
        this->fileView = new TextBox(this);
        this->fileView->SetDimensions(0, 0, this->m_width, this->m_height - 22);
        this->fileView->AddSpecificStyle(ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL);
        this->fileView->Show();
        this->LoadFile();
    }

    this->statusBar = new StatusBar(this, 0, this->m_height - 22, this->m_width, 22);
    this->statusBar->AddStartGripper();
    this->statusBar->Show();

    if (image != nullptr) {
        int rw, rh;

        rw = this->image->GetRealWidth();
        rh = this->image->GetRealHeight();
        if (this->image != nullptr && (rw >= 400 && rh >= 300)) {
            this->SetDimensions(rw, rh + 22);
        }
        else {
            this->SetDimensions(400, 322);
        }
    }
    else this->SetDimensions(800, 600);

    this->SetIcon(fileInfo.hIcon);
    this->SetTitle(this->fileName);
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
    if(fileView != nullptr) this->fileView->Resize(
        0,
        0,
        this->m_width,
        this->m_height - 22);

    if (image != nullptr)
        if(image->CanResize())
            this->image->Resize(
                0,
                0,
                this->m_width,
                this->m_height - 22);

    this->statusBar->Resize(
        0,
        this->m_height - 22,
        this->m_width,
        22);
}
