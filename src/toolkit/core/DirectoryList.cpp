#include "DirectoryList.h"

METHOD DirectoryList::DirectoryList(std::wstring dir)
{
    this->SetPath(dir);
}

METHOD void DirectoryList::SetPath(std::wstring dir) {
    wchar_t buffer[MAX_PATH];

    if (_wfullpath(buffer, dir.c_str(), MAX_PATH)) {
        this->DirectoryPath = buffer;

        if (this->DirectoryPath[this->DirectoryPath.size() - 1] != L'\\')
            this->DirectoryPath.append(L"\\");

    }
    else {
        throw new std::exception("Sorry, you passed an invalid path");
    }
}

METHOD std::wstring DirectoryList::GetAbsolutePath()
{
    return this->DirectoryPath;
}

METHOD std::vector<DirectoryItem>& DirectoryList::GetDirectories()
{
    return this->directories;
}

METHOD std::vector<DirectoryItem>& DirectoryList::GetFiles()
{
    return this->files;
}

METHOD void DirectoryList::SetHidden(bool show)
{
    this->showHiddenFiles = show;
}

METHOD void DirectoryList::Enumerate() noexcept(false)
{
    WIN32_FIND_DATA findData;
    HANDLE hSearch;
    LPCTSTR hSearchData;

    String directoryWithFilter = this->DirectoryPath;

    ZeroMemory(
        &findData,
        sizeof(findData));

    directoryWithFilter.append(L"*.*");

    hSearchData = (LPCTSTR)directoryWithFilter.c_str();

    hSearch = FindFirstFile(
        hSearchData,
        &findData);

    if (GetLastError() == ERROR_ACCESS_DENIED)
    {
        throw new AccessDeniedException();
    }

    while (FindNextFile(hSearch, &findData))
    {
        if (!this->showHiddenFiles && (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) continue;

        DirectoryItem item;

        FILETIME ft = findData.ftLastWriteTime;

        SYSTEMTIME st;
        wchar_t szLocalDate[255], szLocalTime[255];

        FileTimeToLocalFileTime(&ft, &ft);
        FileTimeToSystemTime(&ft, &st);
        GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL,
            szLocalDate, 255);
        GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, szLocalTime, 255);

        item.date = szLocalDate;
        item.date.append(L" ");
        item.date.append(szLocalTime);

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL)
                && !(findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
                && !(findData.dwFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)) {
                
                item.baseName = findData.cFileName;
                item.extName = L"";

                if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
                    item.iconIndex = 12;
                }
                else {
                    item.iconIndex = (item.baseName == L"..") ? 0 : 1;
                }

                directories.push_back(item);
            }
        }
        else if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL)
            && !(findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
            && !(findData.dwFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)) {

            std::vector<String> cont;
            const std::wstring str(findData.cFileName);

            std::size_t current, previous = 0;
            current = str.find(L'.');
            while (current != std::string::npos) {
                cont.push_back(str.substr(previous, current - previous));
                previous = current + 1;
                current = str.find(L'.', previous);
            }
            cont.push_back(str.substr(previous, current - previous));

            if (findData.cFileName[0] == L'.') {
                item.baseName = findData.cFileName;
                item.extName = L"";
            }
            else {
                if (cont.size() == 1) {
                    item.baseName = cont[0];
                    item.extName = L"";
                }
                else {
                    item.extName = cont[cont.size() - 1];

                    item.baseName = StringReplace(findData.cFileName, String(L".").append(item.extName), L"");
                }

                for (unsigned int i = 0; i < item.extName.size(); i++) {
                    item.extName[i] = std::tolower(item.extName[i]);
                }
            }

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
                item.iconIndex = 13;
            }
            else {
                if (item.extName == L"doc" || item.extName == L"docx" || item.extName == L"dot" || item.extName == L"odt") {
                    item.iconIndex = 4;
                }
                else if (item.extName == L"xls" || item.extName == L"xlsx" || item.extName == L"ods") {
                    item.iconIndex = 3;
                }
                else if (item.extName == L"ppt" || item.extName == L"pptx" || item.extName == L"odp") {
                    item.iconIndex = 11;
                }
                else if (item.extName == L"pdf") {
                    item.iconIndex = 5;
                }
                else if (item.extName == L"mp3" || item.extName == L"wav") {
                    item.iconIndex = 9;
                }
                else if (item.extName == L"zip" || item.extName == L"7z" || item.extName == L"tar" || item.extName == L"gz" || item.extName == L"rar" || item.extName == L"bz" || item.extName == L"bz2") {
                    item.iconIndex = 6;
                }
                else if (item.extName == L"exe" || item.extName == L"bat" || item.extName == L"com" || item.extName == L"pif" || item.extName == L"cmd" || item.extName == L"vbs" || item.extName == L"vbe" || item.extName == L"js" || item.extName == L"jse" || item.extName == L"wsf" || item.extName == L"wsh" || item.extName == L"msc") {
                    item.iconIndex = 7;
                }
                else if (item.extName == L"dll" || item.extName == L"lib" || item.extName == L"sys") {
                    item.iconIndex = 8;
                }
                else if (item.extName == L"txt" || item.extName == L"log" || item.extName == L"nfo" || item.extName == L"diz" || item.extName == L"ini" || item.extName == L"cfg") {
                    item.iconIndex = 10;
                }
                else {
                    item.iconIndex = 2;
                }
            }

            if (findData.nFileSizeLow <= 1024)
            {
                item.size = std::to_wstring(findData.nFileSizeLow).append(L" B").c_str();
            }
            else if (findData.nFileSizeLow <= 1024 * 1024)
            {
                item.size = std::to_wstring(findData.nFileSizeLow / 1024).append(L" KB").c_str();
            }
            else
            {
                item.size = std::to_wstring(findData.nFileSizeLow / (1024 * 1024)).append(L" MB").c_str();
            }

            files.push_back(item);
        }
    }
}
