#include "FileViewerWindow.h"

FileViewerWindow::FileViewerWindow(Application* app, String ext) : Window(app)
{
    this->fileExtension = ext;
}

FileViewerWindow::~FileViewerWindow() {
    delete this->fileView;
    delete this->image;
    delete this->statusBar;
}

void FileViewerWindow::OnClose()
{
    this->Destroy();
}

void FileViewerWindow::OnInitializeWindow()
{
    HICON icon = LoadIcon(
        this->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_STDFILE));

    if (fileExtension == L"bmp" || fileExtension == L"dib" || fileExtension == L"ico" || fileExtension == L"gif" || fileExtension == L"jpg" || fileExtension == L"jpeg" || fileExtension == L"png" || fileExtension == L"tiff" || fileExtension == L"wmf" || fileExtension == L"emf") {
        this->image = new Image(this, this->fileName, 0, 0, this->m_width, this->m_height - 22);
        this->image->Show();
    } else {
        this->fileView = new TextBox(this);
        this->fileView->SetDimensions(0, 0, this->m_width, this->m_height - 22);
        this->fileView->AddSpecificStyle(ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL);
        this->fileView->Show();
        this->fileView->SetText(this->fileContent);
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

    this->SetIcon(icon);
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

    if (fp) {
        
        if (MimeTypeFromString(this->fileName) == L"text" || MimeTypeFromString(this->fileName) == L"unknown") {
            ReadText(fp);
        } else {
            if (!(fileExtension == L"bmp" ||
                fileExtension == L"dib" ||
                fileExtension == L"ico" ||
                fileExtension == L"gif" ||
                fileExtension == L"jpg" ||
                fileExtension == L"jpeg" ||
                fileExtension == L"png" ||
                fileExtension == L"tiff" ||
                fileExtension == L"wmf" ||
                fileExtension == L"emf")) {
                ReadBinary(fp);
            }
        }

        fclose(fp);
    }
}

void FileViewerWindow::ReadBinary(FILE* fp)
{
    std::ifstream is(this->fileName, std::ifstream::binary);
    if (is) {
        is.seekg(0, is.end);
        UINT length = (UINT) is.tellg();
        is.seekg(0, is.beg);

        char* buffer = new char[length];

        is.read(buffer, length);

        is.close();
        
        for (unsigned int i = 0; i < length; i++) {
            if (buffer[i] <= 0 || buffer[i] == '\n' || buffer[i] == '\r') {
                this->fileContent += '.';
            }
            else {
                this->fileContent += buffer[i];
            }
            if (i != 0 && i % 85 == 0) {
                this->fileContent += '\r';
                this->fileContent += '\n';
            }
        }
        
        delete[] buffer;
    }
}

void FileViewerWindow::ReadText(FILE* fp)
{
    char fc;
    std::string content;

    fc = fgetc(fp);

    while (!feof(fp)) {
        content += fc;
        fc = fgetc(fp);
    }

    std::wstringstream cls;
    cls << content.c_str();

    this->fileContent = StringReplace(cls.str(), L"\n", L"\r\n");
}

void FileViewerWindow::OnResizeWindow() {
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