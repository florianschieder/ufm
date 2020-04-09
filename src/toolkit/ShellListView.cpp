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

METHOD void ShellListView::RefreshView() {
    int itemIdx = 0;

    if (this->m_Directory[this->m_Directory.size() - 1] != L'\\')
        this->m_Directory.append(L"\\");

    ListView_DeleteAllItems(this->m_controlHandle);

    DirectoryList list(this->m_Directory);
    list.SetHidden(this->ShowHiddenFiles);

    try {
        list.Enumerate();
    }
    catch (AccessDeniedException e) {
        ShellMessageBox(
            this->m_parentWindow->GetApplication()->GetInstance(),
            this->m_parentWindow->GetHandle(),
            L"Access to this folder is denied. Re-start the program as admin and try again.",
            L"Access denied!",
            MB_ICONWARNING,
            MB_OK);
        this->m_Directory.append(L"\\..\\");
        list.SetPath(this->m_Directory);
        list.Enumerate();
    }

    this->m_Directory = list.GetAbsolutePath();

    std::vector<DirectoryItem> directories = list.GetDirectories();
    std::vector<DirectoryItem> files = list.GetFiles();

    for (unsigned int i = 0; i < directories.size(); i++) {
        this->AddItem(
            itemIdx,
            directories[i].iconIndex);

        this->AddSubItem(
            itemIdx,
            0,
            directories[i].baseName.c_str());

        this->AddSubItem(
            itemIdx,
            1,
            L"Folder");

        this->AddSubItem(
            itemIdx,
            3,
            directories[i].date.c_str());

        itemIdx++;
    }

    for (unsigned int i = 0; i < files.size(); i++) {

        this->AddItem(
            itemIdx,
            files[i].iconIndex);

        this->AddSubItem(
            itemIdx,
            0,
            files[i].baseName.c_str());

        this->AddSubItem(
            itemIdx,
            1,
            files[i].extName.c_str());

        this->AddSubItem(
            itemIdx,
            2,
            files[i].size.c_str());

        this->AddSubItem(
            itemIdx,
            3,
            files[i].date.c_str());

        itemIdx++;
    }
}


METHOD LRESULT CALLBACK ShellListView::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg) {
        case WM_SETFOCUS:
            this->m_parentWindow->ActiveControl = this;
            break;

        case WM_NOTIFY:
            LPNMITEMACTIVATE item;

            wchar_t* fileName;
            wchar_t* ext;

            switch (wParam) {
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

                    if (ListView_GetSelectedCount(this->m_controlHandle) > 0) {
                        String fullPath(L"");
                        fullPath.append(this->m_Directory);
                        fullPath.append(fileName);

                        if (String(ext) != String(L"Folder")) {
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

                    if (ListView_GetSelectedCount(this->m_controlHandle) > 0) {
                        String fullPath(L"");
                        fullPath.append(this->m_Directory);
                        fullPath.append(fileName);

                        if (String(ext) != String(L"Folder")) {
                            fullPath.append(L".");
                            fullPath.append(ext);
                        }

                        DWORD attrib = GetFileAttributes(fullPath.c_str());

                        if ((attrib & FILE_ATTRIBUTE_DIRECTORY) != 0) {
                            this->m_Directory = fullPath;
                            this->RefreshView();
                        }
                        else {
                            ShellExecute(this->m_parentWindow->GetHandle(), L"open", fullPath.c_str(), NULL, NULL, SW_SHOW);
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
