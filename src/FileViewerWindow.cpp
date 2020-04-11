#include "FileViewerWindow.h"

FileViewerWindow::FileViewerWindow(Application* app, String ext) : Window(app)
{
    this->fileExtension = ext;
}

FileViewerWindow::~FileViewerWindow()
{
    delete this->fileView;
    delete[] this->fileContent;
    delete this->image;
    delete this->statusBar;
}

void FileViewerWindow::OnClose()
{
    this->Destroy();
}

void FileViewerWindow::OnInitializeWindow()
{
    if (fileExtension == L"bmp" || fileExtension == L"dib" || fileExtension == L"ico" || fileExtension == L"gif" || fileExtension == L"jpg" || fileExtension == L"jpeg" || fileExtension == L"png" || fileExtension == L"tiff" || fileExtension == L"wmf" || fileExtension == L"emf")
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
        //this->fileView->SetText(this->fileContent);
        SendMessageA(this->fileView->GetHandle(), WM_SETTEXT, 0, (LPARAM)this->fileContent);
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

    this->SetTitle(this->fileName);
}

void FileViewerWindow::OnMenuEvent(WORD menuID)
{
}

void FileViewerWindow::OnPaint(PAINTSTRUCT ps, HDC hdc)
{
}

void FileViewerWindow::SetFile(const wchar_t* file)
{
    FILE* fp;

    this->fileName = file;

    _wfopen_s(&fp, file, L"rb");

    if (fp)
    {
        // Get file size

        fseek(fp, 0, SEEK_END);
        this->fileSize = ftell(fp);
        rewind(fp);

        // Initialize file buffer
        this->fileContent = new char[this->fileSize + 1];

        if (MimeTypeFromString(this->fileName) == L"text" || MimeTypeFromString(this->fileName) == L"unknown")
        {
            ReadText(fp);
        }
        else
        {
            if (!(fileExtension == L"bmp" ||
                fileExtension == L"dib" ||
                fileExtension == L"ico" ||
                fileExtension == L"gif" ||
                fileExtension == L"jpg" ||
                fileExtension == L"jpeg" ||
                fileExtension == L"png" ||
                fileExtension == L"tiff" ||
                fileExtension == L"wmf" ||
                fileExtension == L"emf"))
            {
                ReadBinary();
            }
        }

        this->fileContent[this->fileSize] = '\0';

        fclose(fp);
    }
}

void FileViewerWindow::ReadBinary()
{
    std::ifstream is(this->fileName, std::ifstream::binary);

    if (is)
    {
        is.read(this->fileContent, this->fileSize);
        
        for (long i = 0; i < this->fileSize; i++)
        {
            if (this->fileContent[i] <= 0 || this->fileContent[i] == '\n' || this->fileContent[i] == '\r')
            {
                this->fileContent[i] = '.';
            }
        }

        is.close();
    }
}

void FileViewerWindow::ReadText(FILE* fp)
{
    fread(this->fileContent, this->fileSize, sizeof(char), fp);
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
