#include "FileType.h"

String MimeTypeFromString(const String& szExtension)
{
    char buff[256];
    LPWSTR out = (wchar_t*) L"unkown/unkown";

    FILE* in;
    _wfopen_s(&in, szExtension.c_str(), L"rb");

    if (in != 0)
    {
        fread(buff, 1, 256, in);

        FindMimeFromData(
            NULL,
            NULL,
            buff,
            256,
            NULL,
            FMFD_DEFAULT,
            &out,
            0);
    }

    return StringSplit(out, L"/")[0];
}
