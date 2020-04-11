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

    SHFILEINFO  sfi;

    // Get the system image list
    HIMAGELIST hLargeImages = reinterpret_cast<HIMAGELIST>(
        SHGetFileInfo(
            L"C:\\",
            0,
            &sfi,
            sizeof(SHFILEINFO),
            SHGFI_SYSICONINDEX));

    HIMAGELIST hSmallImages = reinterpret_cast<HIMAGELIST>(
        SHGetFileInfo(
            L"C:\\",
            0,
            &sfi,
            sizeof(SHFILEINFO),
            SHGFI_SYSICONINDEX | SHGFI_SMALLICON));

    // Add our generic icons we need internally
    HICON upIcon = LoadIcon(
        this->m_parentWindow->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDI_UP));

    iUpIconIndex = ImageList_AddIcon(hLargeImages, upIcon);
    ImageList_AddIcon(hSmallImages, upIcon);

    // Set image list
    ListView_SetImageList(this->m_controlHandle, hLargeImages, LVSIL_NORMAL);
    ListView_SetImageList(this->m_controlHandle, hSmallImages, LVSIL_SMALL);
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
    String              absolutePath;
    wchar_t             buffer[MAX_PATH];
    String              directoryWithFilter;
    FILETIME            ft;
    WIN32_FIND_DATA     findData;
    HANDLE              hSearch;
    LPCTSTR             hSearchData;
    DirectoryItem       item;
    int                 itemIdx = 0;
    SHFILEINFO          shellFileInformation;
    SYSTEMTIME          st;
    wchar_t             szLocalDate[255];
    wchar_t             szLocalTime[255];

    // Prepare structures

    ZeroMemory(
        &findData,
        sizeof(findData));

    ZeroMemory(
        &shellFileInformation,
        sizeof(shellFileInformation));

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

        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL)
            && !(findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
            && !(findData.dwFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM))
        {
            absolutePath = this->m_Directory + String(findData.cFileName);

            SHGetFileInfo(
                absolutePath.c_str(),
                0,
                &shellFileInformation,
                sizeof(shellFileInformation),
                SHGFI_SYSICONINDEX);

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

            item.baseName = findData.cFileName;
            item.extName = L"";

            item.iconIndex = (item.baseName == L"..") ? this->iUpIconIndex : shellFileInformation.iIcon;

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
            && !(findData.dwFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM))
        {
            absolutePath = this->m_Directory + String(findData.cFileName);

            SHGetFileInfo(
                absolutePath.c_str(),
                0,
                &shellFileInformation,
                sizeof(shellFileInformation),
                SHGFI_SYSICONINDEX);

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

            item.iconIndex = shellFileInformation.iIcon;

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
