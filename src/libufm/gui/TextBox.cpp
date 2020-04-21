#include "TextBox.h"

METHOD TextBox::TextBox(Window* parent) : InputBox(parent)
{
    this->AddSpecificStyle(ES_MULTILINE | WS_VSCROLL | WS_HSCROLL);

    this->hFont = CreateFont(
        PT(10),
        0,
        0,
        0,
        FW_DONTCARE,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Courier New"));
}

METHOD TextBox::~TextBox()
{
    DeleteObject(this->hFont);
}

METHOD void TextBox::Show()
{
    InputBox::Show();

    SendMessage(
        this->m_controlHandle,
        WM_SETFONT,
        (WPARAM) this->hFont,
        TRUE);
}

METHOD void TextBox::ReadBinaryFile(String file)
{
    char* fileContent;
    std::streampos fileSize = 0L;

    std::ifstream is(file, std::ifstream::binary);

    if (is)
    {
        is.seekg(0, is.end);
        fileSize = is.tellg();
        is.seekg(0, is.beg);

        std::streampos offs = fileSize;
        offs += 1;

        fileContent = new char[offs];

        is.read(fileContent, fileSize);

        for (long i = 0; i < fileSize; i++)
        {
            if (fileContent[i] <= 0 || fileContent[i] == '\n' || fileContent[i] == '\r')
            {
                fileContent[i] = '.';
            }
        }

        fileContent[fileSize] = '\0';

        SendMessageA(
            this->GetHandle(),
            WM_SETTEXT,
            0,
            (LPARAM)fileContent);

        delete[] fileContent;

        is.close();
    }
}

METHOD void TextBox::ReadTextFile(String file)
{
    char* fileContent;
    long fileSize;
    FILE* fp;

    _wfopen_s(
        &fp,
        file.c_str(),
        L"rb");

    if (fp)
    {
        // Get file size

        fseek(fp, 0, SEEK_END);
        fileSize = ftell(fp);
        rewind(fp);

        long offs = fileSize + 1L;

        // Initialize file buffer
        fileContent = new char[offs];
        fread(fileContent, fileSize, sizeof(char), fp);
        fileContent[fileSize] = '\0';

        SendMessageA(
            this->GetHandle(),
            WM_SETTEXT,
            0,
            (LPARAM)fileContent);
        
        delete[] fileContent;

        fclose(fp);
    }
}
