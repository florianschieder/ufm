#include "ShellListView.h"

METHOD ShellListView::ShellListView(Window* parent) : ListView(parent)
{
    this->m_Directory = L"C:\\";
    this->m_Filter = L"*.*";

    this->specificStyles = LVS_EDITLABELS;

    this->Show();

    this->AddColumn(
        L"File",
        300,
        0);

    this->AddColumn(
        L"Type",
        75,
        1);

    this->AddColumn(
        L"Size",
        75,
        2);

    this->AddColumn(
        L"Date",
        200,
        3);

    // Create image list

    this->smallImages = ImageList_Create(16, 16, ILC_MASK | ILC_COLOR32, 10, 0);
    this->largeImages = ImageList_Create(32, 32, ILC_MASK | ILC_COLOR32, 10, 0);

    HICON upIcon, folderIcon, folderIconAH, stdIcon, stdIconAH,
        odsIcon, odtIcon, odpIcon, pdfIcon, archIcon, appIcon, libIcon, audioIcon, textIcon;

    upIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_UP));

    folderIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FOLDER));

    stdIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_STDFILE));

    folderIconAH = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_FOLDER_HIDDEN));

    stdIconAH = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_STDFILE_HIDDEN));

    odsIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_ODS));

    odtIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_ODT));

    pdfIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_PDF));

    archIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_ARCH));

    appIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_APP));

    audioIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_MUSIC));

    libIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_LIB));

    textIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_TEXT));

    odpIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_FT_ODP));

    // 0
    ImageList_AddIcon(this->largeImages, upIcon);
    ImageList_AddIcon(this->smallImages, upIcon);

    // 1
    ImageList_AddIcon(this->largeImages, folderIcon);
    ImageList_AddIcon(this->smallImages, folderIcon);

    // 2
    ImageList_AddIcon(this->largeImages, stdIcon);
    ImageList_AddIcon(this->smallImages, stdIcon);

    // 3
    ImageList_AddIcon(this->largeImages, odsIcon);
    ImageList_AddIcon(this->smallImages, odsIcon);

    // 4
    ImageList_AddIcon(this->largeImages, odtIcon);
    ImageList_AddIcon(this->smallImages, odtIcon);

    // 5
    ImageList_AddIcon(this->largeImages, pdfIcon);
    ImageList_AddIcon(this->smallImages, pdfIcon);

    // 6
    ImageList_AddIcon(this->largeImages, archIcon);
    ImageList_AddIcon(this->smallImages, archIcon);

    // 7
    ImageList_AddIcon(this->largeImages, appIcon);
    ImageList_AddIcon(this->smallImages, appIcon);

    // 8
    ImageList_AddIcon(this->largeImages, libIcon);
    ImageList_AddIcon(this->smallImages, libIcon);

    // 9
    ImageList_AddIcon(this->largeImages, audioIcon);
    ImageList_AddIcon(this->smallImages, audioIcon);

    // 10
    ImageList_AddIcon(this->largeImages, textIcon);
    ImageList_AddIcon(this->smallImages, textIcon);

    // 11
    ImageList_AddIcon(this->largeImages, odpIcon);
    ImageList_AddIcon(this->smallImages, odpIcon);

    // 12
    ImageList_AddIcon(this->largeImages, folderIconAH);
    ImageList_AddIcon(this->smallImages, folderIconAH);

    // 13
    ImageList_AddIcon(this->largeImages, stdIconAH);
    ImageList_AddIcon(this->smallImages, stdIconAH);

    ListView_SetImageList(this->m_controlHandle, this->largeImages, LVSIL_NORMAL);
    ListView_SetImageList(this->m_controlHandle, this->smallImages, LVSIL_SMALL);
}

METHOD void ShellListView::RefreshView()
{
    // Locals
    int itemIdx = 0;

    // Start time intensive process
    this->InitializeTimeIntensiveProcess();

    // Clear list
    ListView_DeleteAllItems(this->m_controlHandle);

    // Set up path
    if (this->m_Directory[this->m_Directory.size() - 1] != L'\\')
        this->m_Directory.append(L"\\");

    // Start enumeration
    if (!this->Enumerate())
    {
        // Show access denied message
        ShellMessageBox(
            this->m_parentWindow->GetApplication()->GetInstance(),
            this->m_parentWindow->GetHandle(),
            L"Access to this folder is denied. Re-start the program as admin and try again.",
            L"Access denied!",
            MB_ICONWARNING,
            MB_OK);

        // Stop time intensive process
        this->UninitializeTimeIntensiveProcess();

        // directory back
        this->m_Directory.append(L"\\..\\");
        this->Enumerate();
    }

    // Stop time intensive process
    this->UninitializeTimeIntensiveProcess();
}

METHOD bool ShellListView::Enumerate()
{
    // Locals
    wchar_t             buffer[MAX_PATH];
    String              directoryWithFilter;
    FILETIME            ft;
    WIN32_FIND_DATA     findData;
    HANDLE              hSearch;
    LPCTSTR             hSearchData;
    DirectoryItem       item;
    int                 itemIdx = 0;
    SYSTEMTIME          st;
    wchar_t             szLocalDate[255];
    wchar_t             szLocalTime[255];

    // Update path
    if (_wfullpath(buffer, this->m_Directory.c_str(), MAX_PATH)) 
    {
        this->m_Directory = buffer;

        if (this->m_Directory[this->m_Directory.size() - 1] != L'\\')
            this->m_Directory.append(L"\\");

    }
    else
    {
        throw new std::exception("Sorry, you passed an invalid path");
    }

    directoryWithFilter = this->m_Directory;
    directoryWithFilter.append(L"*.*");

    ZeroMemory(
        &findData,
        sizeof(findData));

    hSearchData = (LPCTSTR) directoryWithFilter.c_str();
    
    // Start enumeration
    hSearch = FindFirstFile(
        hSearchData,
        &findData);

    // Fail if permission is not high enough
    if (GetLastError() == ERROR_ACCESS_DENIED)
    {
        return false;
    }

    // Process directories
    while (FindNextFile(hSearch, &findData))
    {
        // Skip if it's a file
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            continue;
        }

        // Skip if it's a hidden directory and the flag is not set
        // to show hidden elements
        if (!this->ShowHiddenFiles
            && (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
        {
            continue;
        }

        ft = findData.ftLastWriteTime;

        FileTimeToLocalFileTime(
            &ft,
            &ft);

        FileTimeToSystemTime(
            &ft,
            &st);

        GetDateFormat(
            LOCALE_USER_DEFAULT,
            DATE_SHORTDATE,
            &st,
            NULL,
            szLocalDate, 255);

        GetTimeFormat(
            LOCALE_USER_DEFAULT,
            0,
            &st,
            NULL,
            szLocalTime,
            255);

        item.date = szLocalDate;
        item.date.append(L" ");
        item.date.append(szLocalTime);

        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL)
            && !(findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
            && !(findData.dwFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)) {

            item.baseName = findData.cFileName;
            item.extName = L"";

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
            {
                item.iconIndex = 12;
            }
            else
            {
                item.iconIndex = (item.baseName == L"..") ? 0 : 1;
            }

            this->AddItem(
                itemIdx,
                item.iconIndex);

            this->AddSubItem(
                itemIdx,
                0,
                item.baseName.c_str());

            this->AddSubItem(
                itemIdx,
                1,
                L"Folder");

            this->AddSubItem(
                itemIdx,
                3,
                item.date.c_str());

            itemIdx++;
        }
    }

    // Process files
    hSearch = FindFirstFile(
        directoryWithFilter.c_str(),
        &findData);

    while (FindNextFile(hSearch, &findData))
    {
        // Skip if it's a directory
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            continue;
        }

        // Skip if it's a hidden directory and the flag is not set
        // to show hidden elements
        if (!this->ShowHiddenFiles
            && (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
        {
            continue;
        }

        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL)
            && !(findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
            && !(findData.dwFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)) {

            std::vector<String> cont;
            const std::wstring str(findData.cFileName);

            std::size_t current, previous = 0;
            current = str.find(L'.');

            while (current != std::string::npos) {
                cont.push_back(
                    str.substr(
                        previous,
                        current - previous));
                previous = current + 1;
                current = str.find(L'.', previous);
            }

            cont.push_back(
                str.substr(
                    previous,
                    current - previous));

            if (findData.cFileName[0] == L'.')
            {
                item.baseName = findData.cFileName;
                item.extName = L"";
            }
            else
            {
                if (cont.size() == 1)
                {
                    item.baseName = cont[0];
                    item.extName = L"";
                }
                else
                {
                    item.extName = cont[cont.size() - 1];

                    item.baseName = StringReplace(
                        findData.cFileName,
                        String(L".").append(item.extName),
                        L"");
                }

                for (unsigned int i = 0; i < item.extName.size(); i++)
                {
                    item.extName[i] = std::tolower(item.extName[i]);
                }
            }

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
            {
                item.iconIndex = 13;
            }
            else
            {
                if (item.extName == L"doc" || item.extName == L"docx" || item.extName == L"dot" || item.extName == L"odt")
                {
                    item.iconIndex = 4;
                }
                else if (item.extName == L"xls" || item.extName == L"xlsx" || item.extName == L"ods")
                {
                    item.iconIndex = 3;
                }
                else if (item.extName == L"ppt" || item.extName == L"pptx" || item.extName == L"odp")
                {
                    item.iconIndex = 11;
                }
                else if (item.extName == L"pdf")
                {
                    item.iconIndex = 5;
                }
                else if (item.extName == L"mp3" || item.extName == L"wav")
                {
                    item.iconIndex = 9;
                }
                else if (item.extName == L"zip" || item.extName == L"7z" || item.extName == L"tar" || item.extName == L"gz" || item.extName == L"rar" || item.extName == L"bz" || item.extName == L"bz2")
                {
                    item.iconIndex = 6;
                }
                else if (item.extName == L"exe" || item.extName == L"bat" || item.extName == L"com" || item.extName == L"pif" || item.extName == L"cmd" || item.extName == L"vbs" || item.extName == L"vbe" || item.extName == L"js" || item.extName == L"jse" || item.extName == L"wsf" || item.extName == L"wsh" || item.extName == L"msc")
                {
                    item.iconIndex = 7;
                }
                else if (item.extName == L"dll" || item.extName == L"lib" || item.extName == L"sys")
                {
                    item.iconIndex = 8;
                }
                else if (item.extName == L"txt" || item.extName == L"log" || item.extName == L"nfo" || item.extName == L"diz" || item.extName == L"ini" || item.extName == L"cfg")
                {
                    item.iconIndex = 10;
                }
                else
                {
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

            this->AddItem(
                itemIdx,
                item.iconIndex);

            this->AddSubItem(
                itemIdx,
                0,
                item.baseName.c_str());

            this->AddSubItem(
                itemIdx,
                1,
                item.extName.c_str());

            this->AddSubItem(
                itemIdx,
                2,
                item.size.c_str());

            this->AddSubItem(
                itemIdx,
                3,
                item.date.c_str());

            itemIdx++;
        }
    }

    return true;
}


METHOD LRESULT CALLBACK ShellListView::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg)
    {
        case WM_SETFOCUS:
            this->m_parentWindow->ActiveControl = this;
            break;

        case WM_NOTIFY:
            LPNMITEMACTIVATE item;

            wchar_t* fileName;
            wchar_t* ext;

            switch (wParam)
            {
                case NM_CLICK:
                    item = (LPNMITEMACTIVATE)lParam;
                    fileName = new wchar_t[260];
                    ext = new wchar_t[260];

                    ListView_GetItemText(
                        this->m_controlHandle,
                        item->iItem,
                        0,
                        (LPTSTR)fileName,
                        260 * sizeof(wchar_t));

                    ListView_GetItemText(
                        this->m_controlHandle,
                        item->iItem,
                        1,
                        (LPTSTR)ext,
                        260 * sizeof(wchar_t));

                    if (ListView_GetSelectedCount(this->m_controlHandle) > 0)
                    {
                        String fullPath(L"");
                        fullPath.append(this->m_Directory);
                        fullPath.append(fileName);

                        if (String(ext) != String(L"Folder"))
                        {
                            fullPath.append(L".");
                            fullPath.append(ext);
                        }

                        this->SelectedPath = fullPath;
                        this->SelectedExt = ext;
                    }

                    delete[] fileName;
                    delete[] ext;

                    break;

                case NM_DBLCLK:
                    item = (LPNMITEMACTIVATE) lParam;
                    fileName = new wchar_t[260];
                    ext = new wchar_t[260];

                    ListView_GetItemText(
                        this->m_controlHandle,
                        item->iItem,
                        0,
                        (LPTSTR)fileName,
                        260 * sizeof(wchar_t));

                    ListView_GetItemText(
                        this->m_controlHandle,
                        item->iItem,
                        1,
                        (LPTSTR)ext,
                        260 * sizeof(wchar_t));

                    if (ListView_GetSelectedCount(this->m_controlHandle) > 0)
                    {
                        String fullPath(L"");
                        fullPath.append(this->m_Directory);
                        fullPath.append(fileName);

                        if (String(ext) != String(L"Folder"))
                        {
                            fullPath.append(L".");
                            fullPath.append(ext);
                        }

                        DWORD attrib = GetFileAttributes(fullPath.c_str());

                        if ((attrib & FILE_ATTRIBUTE_DIRECTORY) != 0)
                        {
                            this->m_Directory = fullPath;
                            this->RefreshView();
                        }
                        else
                        {
                            SHELLEXECUTEINFO info;
                            info.cbSize = sizeof(info);
                            info.fMask = SEE_MASK_DEFAULT | SEE_MASK_NOCLOSEPROCESS;
                            info.hwnd = this->m_parentWindow->GetHandle();
                            info.lpVerb = L"open";
                            info.lpFile = fullPath.c_str();
                            info.lpParameters = NULL;
                            info.lpDirectory = NULL;
                            info.nShow = SW_SHOW;

                            ShellExecuteEx(&info);
                        }
                    }
                    
                    delete[] fileName;
                    delete[] ext;

                    break;
            }
            break;
    }
    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}
