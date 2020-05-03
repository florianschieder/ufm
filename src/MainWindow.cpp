#include "MainWindow.h"

#include "../res/resource.h"

using namespace std::filesystem;

/**
 * MainWindow::MainWindow: Main window constructor
 */
MainWindow::MainWindow(Application* app) : Window(app)
{
    this->m_iconID = IDI_MAIN_ICON;
    this->m_menuID = IDR_MAINMENU;

    this->Style = WS_OVERLAPPEDWINDOW;
}

MainWindow::~MainWindow()
{
    delete this->toolBar;
    delete this->statusbar;

    delete this->refreshButton;
    delete this->detailsButton;
    delete this->listButton;
    delete this->largeButton;
    delete this->smallButton;
    delete this->printButton;
    delete this->adminButton;
    delete this->propertyButton;
    delete this->renameButton;
    delete this->cmdButton;

    delete this->leftBox;
    delete this->rightBox;
    delete this->leftBoxApply;
    delete this->rightBoxApply;
    delete this->leftPath;
    delete this->rightPath;

    delete this->closeButton;
    delete this->copyButton;
    delete this->deleteButton;
    delete this->editButton;
    delete this->moveButton;
    delete this->newDirectoryButton;
    delete this->showButton;

    delete this->leftShellView;
    delete this->rightShellView;
}

/**
 * MainWindow::OnClose: Main window close handler
 */
void MainWindow::OnClose()
{
    this->m_application->SetConfig(
        L"leftLocation",
        this->leftShellView->GetDirectory().c_str());

    this->m_application->SetConfig(
        L"rightLocation",
        this->rightShellView->GetDirectory().c_str());

    this->Destroy();
}

/**
 * MainWindow::OnInitializeWindow: Handler for window and control initialization
 */
void MainWindow::OnInitializeWindow()
{
    // Construct toolbar

    this->toolBar = new ToolBar(this);
    this->toolBar->AddStartGripper();
    this->toolBar->AddSeparator(42);
    this->toolBar->AddSeparator(166);//
    this->toolBar->AddSeparator(290);
    this->toolBar->Place(0, 0, this->Width, 32);

    // Add toolbar buttons

    this->refreshButton = new ToolButton(this->toolBar);
    this->refreshButton->SetLocation(0);
    this->refreshButton->SetIcon(IDI_REFRESH, 16, 16);
    this->refreshButton->OnClick = this->RefreshButtonClicked;
    this->refreshButton->AddToolTip(L"Refresh directory");

    this->detailsButton = new ToolButton(this->toolBar);
    this->detailsButton->SetLocation(1);
    this->detailsButton->SetIcon(IDI_DETAILS, 16, 16);
    this->detailsButton->OnClick = this->DetailsButtonClicked;
    this->detailsButton->AddToolTip(L"Details view");

    this->listButton = new ToolButton(this->toolBar);
    this->listButton->SetLocation(2);
    this->listButton->SetIcon(IDI_LIST, 16, 16);
    this->listButton->OnClick = this->ListButtonClicked;
    this->listButton->AddToolTip(L"List view");

    this->smallButton = new ToolButton(this->toolBar);
    this->smallButton->SetLocation(3);
    this->smallButton->SetIcon(IDI_SMALL, 16, 16);
    this->smallButton->OnClick = this->SmallIconsButtonClicked;
    this->smallButton->AddToolTip(L"Small icon view");

    this->largeButton = new ToolButton(this->toolBar);
    this->largeButton->SetLocation(4);
    this->largeButton->SetIcon(IDI_LARGE, 16, 16);
    this->largeButton->OnClick = this->LargeIconsButtonClicked;
    this->largeButton->AddToolTip(L"Large icon view");

    this->printButton = new ToolButton(this->toolBar);
    this->printButton->SetLocation(5);
    this->printButton->SetIcon(IDI_PRINT, 16, 16);
    this->printButton->OnClick = this->PrintButtonClicked;
    this->printButton->AddToolTip(L"Print selected document");

    this->adminButton = new ToolButton(this->toolBar);
    this->adminButton->SetLocation(6);
    this->adminButton->SetIcon(IDI_ADMIN, 16, 16);
    this->adminButton->OnClick = this->AdminButtonClicked;
    this->adminButton->AddToolTip(L"Run program as administrator");

    this->propertyButton = new ToolButton(this->toolBar);
    this->propertyButton->SetLocation(7);
    this->propertyButton->SetIcon(IDI_PROPERTY, 16, 16);
    this->propertyButton->OnClick = this->PropertiesButtonClicked;
    this->propertyButton->AddToolTip(L"Show file/directory properties");

    this->renameButton = new ToolButton(this->toolBar);
    this->renameButton->SetLocation(8);
    this->renameButton->SetIcon(IDI_RENAME, 16, 16);
    this->renameButton->OnClick = this->RenameButtonClicked;
    this->renameButton->AddToolTip(L"Rename file/directory");

    this->cmdButton = new ToolButton(this->toolBar);
    this->cmdButton->SetLocation(9);
    this->cmdButton->SetIcon(IDI_CMD, 16, 16);
    this->cmdButton->OnClick = this->CmdButtonClicked;
    this->cmdButton->AddToolTip(L"Open command prompt");

    // Construct "address bar"

    this->leftBox = new DriveComboBox(this);
    this->leftBox->OnSelectionChanged = this->ComboBoxSelectionChanged;
    this->leftBox->Place(5, 35, 60, 22);

    this->leftPath = new ShellInputBox(this);
    this->leftPath->Place(70, 35, 60, 22);

    this->leftBoxApply = new Button(this);
    this->leftBoxApply->Place(360, 34, 30, 24);
    this->leftBoxApply->SetIcon(IDI_FORWARD, 16, 16);
    this->leftBoxApply->OnClick = this->LeftApplyButtonClicked;

    this->rightBox = new DriveComboBox(this);
    this->rightBox->OnSelectionChanged = this->ComboBoxSelectionChanged;
    this->rightBox->Place(395, 35, 60, 22);

    this->rightPath = new ShellInputBox(this);
    this->rightPath->Place(460, 35, 60, 22);

    this->rightBoxApply = new Button(this);
    this->rightBoxApply->Place(750, 34, 30, 24);
    this->rightBoxApply->SetIcon(IDI_FORWARD, 16, 16);
    this->rightBoxApply->OnClick = this->RightApplyButtonClicked;

    // Construct shell views

    this->leftShellView = new ShellListView(this);
    this->leftShellView->Place(-1, 60, 401, 366);
    this->leftShellView->OnSelectionChanged = this->ShellListViewSelectionChanged;
    this->leftShellView->RefreshView();
    this->leftShellView->SetView(LVS_REPORT);
    
    this->rightShellView = new ShellListView(this);
    this->rightShellView->Place(400, 60, 401, 366);
    this->rightShellView->OnSelectionChanged = this->ShellListViewSelectionChanged;
    this->rightShellView->RefreshView();
    this->rightShellView->SetView(LVS_REPORT);
    
    this->ActiveControl = this->leftShellView;

    // Construct button bar

    this->showButton = new Button(this);
    this->showButton->Place(0, this->Height - 44, this->Width / 7, 22);
    this->showButton->Text = L"[F3] Show";
    this->showButton->OnClick = this->FileViewerButtonClicked;

    this->editButton = new Button(this);
    this->editButton->Place(this->Width / 7, this->Height - 44, this->Width / 7, 22);
    this->editButton->Text = L"[F4] Edit";
    this->editButton->OnClick = this->EditButtonClicked;

    this->copyButton = new Button(this);
    this->copyButton->Place(2 * this->Width / 7, this->Height - 44, this->Width / 7, 22);
    this->copyButton->Text = L"[F5] Copy";
    this->copyButton->OnClick = this->CopyButtonClicked;

    this->moveButton = new Button(this);
    this->moveButton->Place(3 * this->Width / 7, this->Height - 44, this->Width / 7, 22);
    this->moveButton->Text = L"[F6] Move";
    this->moveButton->OnClick = this->MoveButtonClicked;

    this->newDirectoryButton = new Button(this);
    this->newDirectoryButton->Place(4 * this->Width / 7, this->Height - 44, this->Width / 7, 22);
    this->newDirectoryButton->Text = L"[F7] New Directory";
    this->newDirectoryButton->OnClick = this->NewDirButtonClicked;

    this->deleteButton = new Button(this);
    this->deleteButton->Place(5 * this->Width / 7, this->Height - 44, this->Width / 7, 22);
    this->deleteButton->Text = L"[F8] Delete";
    this->deleteButton->OnClick = this->DeleteButtonClicked;

    this->closeButton = new Button(this);
    this->closeButton->Place(6 * this->Width / 7, this->Height - 44, this->Width / 7, 22);
    this->closeButton->Text = L"[Alt+F4] Close";
    this->closeButton->OnClick = this->CloseWindow;

    // Construct status bar

    this->statusbar = new StatusBar(this);
    this->statusbar->Place(0, this->Height - 22, this->Width, 22);
    this->statusbar->AddStartGripper();

    // Setup window

    this->Title = L"UFM";
    this->Width = 800;
    this->Height = 600;

    // If already started once, recover last location

    String myDocuments;

    myDocuments = this->m_application->GetEnvironmentVar(L"UserProfile");
    myDocuments += L"\\Documents\\";
    
    String leftLocation = this->m_application->GetConfig(L"leftLocation");
    String rightLocation = this->m_application->GetConfig(L"rightLocation");

    this->leftShellView->SetDirectory(
        (leftLocation == L"" ? myDocuments : leftLocation));
    
    this->rightShellView->SetDirectory(
        (rightLocation == L"" ? myDocuments : rightLocation));
}

void MainWindow::OnKeyDown(DWORD key)
{
    switch (key)
    {
        case VK_F3:
            this->FileViewerButtonClicked(this);
            break;
        case VK_F4:
            this->EditButtonClicked(this);
            break;
        case VK_F5:
            this->CopyButtonClicked(this);
            break;
        case VK_F6:
            this->MoveButtonClicked(this);
            break;
        case VK_F7:
            this->NewDirButtonClicked(this);
            break;
        case VK_F8:
        case VK_DELETE:
            this->DeleteButtonClicked(this);
            break;
        default:
            break;
    }
}

/**
 * MainWindow::OnControlEvent: Handling method for control messages
 */
void MainWindow::OnMenuEvent(WORD menuID)
{
    ShellListView* activeControl;

    switch (menuID)
    {
        case ID_FILE_OPENCOMMANDPROMPT:
            this->CmdButtonClicked(this);
            break;
        case ID_FILE_RENAMEFILE:
            this->RenameButtonClicked(this);
            break;
        case ID_PRINTDOC:
            this->PrintButtonClicked(this);
            break;
        case ID_RUNASADMIN:
            this->AdminButtonClicked(this);
            break;
        case ID_SHOWPROPS:
            this->PropertiesButtonClicked(this);
            break;

        case ID_FILE_SHOWSELECTEDFILE:
            this->FileViewerButtonClicked(this);
            break;
        case ID_FILE_EDITFILEWITHITSSTANDARDEDITOR:
            this->EditButtonClicked(this);
            break;
        case ID_FILE_COPYOBJECT:
            this->CopyButtonClicked(this);
            break;
        case ID_FILE_DELETEOBJECT:
            this->DeleteButtonClicked(this);
            break;
        case ID_FILE_MOVEOBJECT:
            this->MoveButtonClicked(this);
            break;
        case ID_FILE_NEWFOLDER:
            this->NewDirButtonClicked(this);
            break;

        case ID_FILE_CLOSE:
            this->Close();
            break;

        case ID_SHELLVIEW_DETAILS:
            activeControl = (ShellListView*) this->ActiveControl;
            activeControl->SetView(LVS_REPORT);
            break;

        case ID_SHELLVIEW_LI:
            activeControl = (ShellListView*) this->ActiveControl;
            activeControl->SetView(LVS_ICON);
            break;

        case ID_SHELLVIEW_SI:
            activeControl = (ShellListView*) this->ActiveControl;
            activeControl->SetView(LVS_SMALLICON);
            break;

        case ID_SHELLVIEW_LIST:
            activeControl = (ShellListView*) this->ActiveControl;
            activeControl->SetView(LVS_LIST);
            break;

        case ID_VIEW_TOGGLEHIDDENFILES:
            this->leftShellView->ShowHiddenFiles = !this->leftShellView->ShowHiddenFiles;
            this->rightShellView->ShowHiddenFiles = !this->rightShellView->ShowHiddenFiles;
            this->leftShellView->RefreshView();
            this->rightShellView->RefreshView();
            break;

        case ID_HELP_ABOUTUFM:
            DialogBox(
                ((Application*) this->AppContext)->AppInstance,
                MAKEINTRESOURCE(IDD_ABOUTDLG),
                this->Handle,
                this->AboutDlgProc);
            break;
    }
}

void MainWindow::OnPostParam(void* param, int reason)
{
    switch (reason)
    {
        case NEEDED_TO_PASS_RENAME_OBJECT_NAME:
        {
            ShellListView* activeCtrl = (ShellListView*)this->ActiveControl;
            LPWSTR name = (LPWSTR)param;
            
            if (!MoveFile(
                activeCtrl->SelectedPath.c_str(),
                (String(activeCtrl->GetDirectory()).append(name)).c_str()))
            {
                LPTSTR errorText = NULL;

                FormatMessage(
                    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR)&errorText,
                    0,
                    NULL);

                if (NULL != errorText)
                {
                    ShellMessageBox(
                        this->m_application->AppInstance,
                        this->m_windowHandle,
                        errorText,
                        String(this->Title).c_str(),
                        MB_OK | MB_ICONEXCLAMATION);

                    LocalFree(errorText);
                    errorText = NULL;
                }
            }
            break;
        }

        case NEEDED_TO_PASS_NEW_FOLDER_TEXT:
        {
            ShellListView* activeCtrl = (ShellListView*)this->ActiveControl;
            String dirPath = activeCtrl->GetDirectory() + String((LPCWSTR)param);

            if (!CreateDirectory(dirPath.c_str(), NULL))
            {
                switch (GetLastError())
                {
                case ERROR_ALREADY_EXISTS:
                    ShellMessageBox(
                        ((Application*)this->AppContext)->AppInstance,
                        this->Handle,
                        L"The directory you tried to create already exists.",
                        String(this->Title).c_str(),
                        MB_ICONWARNING,
                        MB_OK);
                    break;
                case ERROR_ACCESS_DENIED:
                    ShellMessageBox(
                        ((Application*)this->AppContext)->AppInstance,
                        this->Handle,
                        L"Failed to create the directory - access denied.",
                        String(this->Title).c_str(),
                        MB_ICONWARNING,
                        MB_OK);
                    break;
                default:
                    ShellMessageBox(
                        ((Application*)this->AppContext)->AppInstance,
                        this->Handle,
                        L"An unexpected error occurred trying to create the directory. A common "\
                        "reason for this is an invalid parameter you specified. Please try again with "\
                        "directory name. We\'re sorry for the inconvenience.",
                        String(this->Title).c_str(),
                        MB_ICONWARNING,
                        MB_OK);
                    break;
                }
            }
            else
            {
                this->leftShellView->RefreshView();
                this->rightShellView->RefreshView();
            }
            break;
        }
    }
}

/**
 * MainWindow::OnPaint: Main window rendering handler for custom drawing
 */
void MainWindow::OnPaint(PAINTSTRUCT ps, HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);

    Gdiplus::RectF rectGradient(
        (Gdiplus::REAL) 0,
        (Gdiplus::REAL) 32,
        (Gdiplus::REAL) this->Width,
        (Gdiplus::REAL) 28);

    Gdiplus::RectF rectBg(
        (Gdiplus::REAL) 0,
        (Gdiplus::REAL) 60,
        (Gdiplus::REAL) this->Width,
        (Gdiplus::REAL) this->leftShellView->Height);

    Gdiplus::LinearGradientBrush brushGradient(
        rectGradient,
        Gdiplus::Color(208, 208, 208),
        Gdiplus::Color(198, 198, 198),
        Gdiplus::LinearGradientMode::LinearGradientModeVertical);

    Gdiplus::LinearGradientBrush brushBg(
        rectBg,
        Gdiplus::Color(198, 198, 198),
        Gdiplus::Color(240, 240, 240),
        Gdiplus::LinearGradientMode::LinearGradientModeVertical);

    graphics.FillRectangle(&brushGradient, rectGradient);
    graphics.FillRectangle(&brushBg, rectBg);
}

/**
 * MainWindow::OnResizeWindow: Handler for main window resizing
 */
void MainWindow::OnResizeWindow()
{
    int h = Height;
    int w = Width;

    this->toolBar->Width = w;
    this->leftPath->Width = (w / 2) - (this->leftPath->X + 10 + this->leftBoxApply->Width);
    this->leftBoxApply->X = (w / 2) - (this->leftBoxApply->Width + 5);
    
    this->rightBox->X = (w / 2) + 5;
    this->rightPath->X = this->rightBox->X + this->rightBox->Width + 5;
    this->rightPath->Width = (w / 2) - (this->leftPath->X + 10 + this->leftBoxApply->Width);
    this->rightBoxApply->X = w - 35;

    this->leftShellView->X = -1;
    this->leftShellView->Width = w / 2;
    this->leftShellView->Height = h - (this->leftShellView->Y + 46);

    this->rightShellView->X = (w / 2) + 1;
    this->rightShellView->Width = w / 2;
    this->rightShellView->Height = h - (this->leftShellView->Y + 46);

    this->statusbar->Y = h - 22;
    this->statusbar->Width = w;
    
    this->showButton->X = 0;
    this->showButton->Y = h - 44;
    this->showButton->Width = w / 7;

    this->editButton->X = w / 7;
    this->editButton->Y = h - 44;
    this->editButton->Width = w / 7;

    this->copyButton->X = 2 * w / 7;
    this->copyButton->Y = h - 44;
    this->copyButton->Width = w / 7;

    this->moveButton->X = 3 * w / 7;
    this->moveButton->Y = h - 44;
    this->moveButton->Width = w / 7;

    this->newDirectoryButton->X = 4 * w / 7;
    this->newDirectoryButton->Y = h - 44;
    this->newDirectoryButton->Width = w / 7;

    this->deleteButton->X = 5 * w / 7;
    this->deleteButton->Y = h - 44;
    this->deleteButton->Width = w / 7;

    this->closeButton->X = 6 * w / 7;
    this->closeButton->Y = h - 44;
    this->closeButton->Width = w / 7;
}

// Event Handler

void MainWindow::FileViewerButtonClicked(Window* window)
{
    ShellListView* activeCtrl = (ShellListView*)(((MainWindow*)window)->ActiveControl);

    if (!(GetFileAttributes(activeCtrl->SelectedPath.c_str()) & FILE_ATTRIBUTE_DIRECTORY))
    {
        std::shared_ptr<FileViewerWindow> fileViewer = std::make_shared<FileViewerWindow>(
            window->AppContext,
            activeCtrl->SelectedExt);

        fileViewer->SetFile(activeCtrl->SelectedPath.c_str());
        fileViewer->Show();
    }
    else
    {
        window->ShowMessage(
            L"No file selected",
            MB_ICONINFORMATION);
    }
}

void MainWindow::EditButtonClicked(Window* window)
{
    ShellListView* activeCtrl = (ShellListView*)(((MainWindow*)window)->ActiveControl);

    SHELLEXECUTEINFO info;
    info.cbSize = sizeof(info);
    info.fMask = SEE_MASK_DEFAULT | SEE_MASK_NOCLOSEPROCESS;
    info.hwnd = window->Handle;
    info.lpVerb = L"edit";
    info.lpFile = activeCtrl->SelectedPath.c_str();
    info.lpParameters = NULL;
    info.lpDirectory = NULL;
    info.nShow = SW_SHOW;

    ShellExecuteEx(&info);
}

void MainWindow::CopyButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*)window;

    ((Application*) wnd->AppContext)->IndicateTimeIntensiveProcess();

    // true = left to right, false = right to left
    bool ltr;
    ltr = (wnd->ActiveControl == wnd->leftShellView);

    // From / to
    String from = (ltr) ? wnd->leftShellView->SelectedPath : wnd->rightShellView->SelectedPath;
    String to = (ltr) ?
        (wnd->rightShellView->GetDirectory() + wnd->leftShellView->SelectedFile) :
        (wnd->leftShellView->GetDirectory() +wnd->rightShellView->SelectedFile);
    
    if (from == L"")
    {
        ((Application*)wnd->AppContext)->UnindicateTimeIntensiveProcess();
        return;
    }

    if (PathFileExists(to.c_str()))
    {
        if (wnd->ShowMessage(
            String(L"Are you sure to overwrite \"").append(to).append(L"\"?").c_str(),
            MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
        {
            CopyFile(from.c_str(), to.c_str(), FALSE);

            wnd->leftShellView->RefreshView();
            wnd->rightShellView->RefreshView();
        }
    }
    else
    {
        CopyFile(from.c_str(), to.c_str(), FALSE);

        wnd->leftShellView->RefreshView();
        wnd->rightShellView->RefreshView();
    }
    ((Application*)wnd->AppContext)->UnindicateTimeIntensiveProcess();
}

void MainWindow::MoveButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*)window;

    ((Application*)wnd->AppContext)->IndicateTimeIntensiveProcess();

    // true = left to right, false = right to left
    bool ltr;
    ltr = (wnd->ActiveControl == wnd->leftShellView);

    // From / to
    String from = (ltr) ? wnd->leftShellView->SelectedPath : wnd->rightShellView->SelectedPath;
    String to = (ltr) ?
        (wnd->rightShellView->GetDirectory() + wnd->leftShellView->SelectedFile) :
        (wnd->leftShellView->GetDirectory() + wnd->rightShellView->SelectedFile);
    
    if (from == L"")
    {
        ((Application*)wnd->AppContext)->UnindicateTimeIntensiveProcess();
        return;
    }

    if (PathFileExists(to.c_str()))
    {
        if (wnd->ShowMessage(
            String(L"Are you sure to overwrite \"").append(to).append(L"\"?").c_str(),
            MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
        {
            MoveFileEx(from.c_str(), to.c_str(), MOVEFILE_REPLACE_EXISTING);

            wnd->leftShellView->RefreshView();
            wnd->rightShellView->RefreshView();
        }
    }
    else
    {
        MoveFile(from.c_str(), to.c_str());

        wnd->leftShellView->RefreshView();
        wnd->rightShellView->RefreshView();
    }

    ((Application*)wnd->AppContext)->UnindicateTimeIntensiveProcess();
}

void MainWindow::NewDirButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*) window;

    DialogBox(
        ((Application*)wnd->AppContext)->AppInstance,
        MAKEINTRESOURCE(IDD_NEWDIR),
        wnd->Handle,
        wnd->NewDirDlgProc);
}

void MainWindow::ShellListViewSelectionChanged(ShellListView* object, Window* parent)
{
    MainWindow* wnd = (MainWindow*)parent;

    if (object == wnd->leftShellView)
    {
        wnd->leftPath->Text = (TCHAR*)wnd->leftShellView->GetDirectory().c_str();

        SendMessage(
            wnd->leftBox->Handle,
            CB_SELECTSTRING,
            -1,
            (LPARAM) wnd->leftShellView->CurrentDrive.c_str());
    }
    else if (object == wnd->rightShellView)
    {
        wnd->rightPath->Text = (TCHAR*)wnd->rightShellView->GetDirectory().c_str();

        SendMessage(
            wnd->rightBox->Handle,
            CB_SELECTSTRING,
            -1,
            (LPARAM)wnd->rightShellView->CurrentDrive.c_str());
    }
}

void MainWindow::ComboBoxSelectionChanged(ComboBox* object, Window* parent)
{
    MainWindow* wnd = (MainWindow*) parent;

    if (object == wnd->leftBox)
    {
        wnd->leftShellView->SetDirectory(object->SelectedItem);
    }
    else if (object == wnd->rightBox)
    {
        wnd->rightShellView->SetDirectory(object->SelectedItem);
    }
}

void MainWindow::DeleteButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*) window;
    ShellListView* active = (ShellListView*) wnd->ActiveControl;

    String myPath(active->SelectedPath);
    myPath += L'\0';

    if (wnd->ShowMessage(
        String(L"Are you sure to recycle \"").append(active->SelectedPath).append(L"\" (and, in case of a directory, each of its contents)?").c_str(),
        MB_YESNO | MB_ICONWARNING) == IDYES)
    {
        SHFILEOPSTRUCT fileOp;

        ZeroMemory(
            &fileOp,
            sizeof(fileOp));

        fileOp.hwnd = wnd->Handle;
        fileOp.wFunc = FO_DELETE;
        fileOp.pFrom = (myPath).c_str();
        fileOp.fFlags = FOF_ALLOWUNDO;
        fileOp.pTo = NULL;

        SHFileOperation(&fileOp);

        wnd->leftShellView->RefreshView();
        wnd->rightShellView->RefreshView();
    }
}

INT_PTR MainWindow::AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hDlg, IDOK);
                    break;
                case IDCANCEL:
                    EndDialog(hDlg, IDCANCEL);
                    break;
            }
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

INT_PTR MainWindow::NewDirDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            wchar_t str[MAX_PATH];
            GetDlgItemText(hDlg, IDC_NEWDIR_NAME, str, MAX_PATH);

            HWND parent = ::GetParent(hDlg);
            SendMessage(
                parent,
                WM_POSTPARAM,
                (WPARAM) str,
                NEEDED_TO_PASS_NEW_FOLDER_TEXT);

            EndDialog(hDlg, IDOK);

            break;
        }
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            break;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

INT_PTR MainWindow::RenameObjectDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_RENAMEOBJECT_NEWNAME, (LPCWSTR) lParam);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            wchar_t str[MAX_PATH];
            GetDlgItemText(hDlg, IDC_RENAMEOBJECT_NEWNAME, str, MAX_PATH);

            HWND parent = ::GetParent(hDlg);
            SendMessage(
                parent,
                WM_POSTPARAM,
                (WPARAM)str,
                NEEDED_TO_PASS_RENAME_OBJECT_NAME);

            EndDialog(hDlg, IDOK);

            break;
        }
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            break;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

void MainWindow::LeftApplyButtonClicked(Window* parent)
{
    MainWindow* wnd = (MainWindow*) parent;
    wnd->leftShellView->SetDirectory(
        wnd->leftPath->Text);
}

void MainWindow::RightApplyButtonClicked(Window* parent)
{
    MainWindow* wnd = (MainWindow*) parent;
    wnd->rightShellView->SetDirectory(
        wnd->rightPath->Text);
}

void MainWindow::RefreshButtonClicked(Window* parent)
{
    MainWindow* wnd = (MainWindow*)parent;
    ((ShellListView*)wnd->ActiveControl)->RefreshView();
}

void MainWindow::DetailsButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*) window;
    ((ShellListView*) wnd->ActiveControl)->SetView(LVS_REPORT);
}

void MainWindow::ListButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*)window;
    ((ShellListView*)wnd->ActiveControl)->SetView(LVS_LIST);
}

void MainWindow::SmallIconsButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*)window;
    ((ShellListView*)wnd->ActiveControl)->SetView(LVS_SMALLICON);
}

void MainWindow::LargeIconsButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*)window;
    ((ShellListView*)wnd->ActiveControl)->SetView(LVS_ICON);
}

void MainWindow::PrintButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*)window;

    ShellListView* activeCtrl = (ShellListView*)wnd->ActiveControl;
    String obj = activeCtrl->SelectedPath;

    if (obj == L"")
        return;

    SHELLEXECUTEINFO info;

    ZeroMemory(
        &info,
        sizeof(info));

    info.cbSize = sizeof(info);
    info.fMask = SEE_MASK_DEFAULT | SEE_MASK_NOCLOSEPROCESS;
    info.hwnd = wnd->Handle;
    info.lpVerb = L"print";
    info.lpFile = ((ShellListView*)wnd->ActiveControl)->SelectedPath.c_str();
    info.lpParameters = NULL;
    info.lpDirectory = NULL;
    info.nShow = SW_SHOW;

    ShellExecuteEx(&info);
}

void MainWindow::AdminButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*)window;

    ShellListView* activeCtrl = (ShellListView*)wnd->ActiveControl;
    String obj = activeCtrl->SelectedPath;

    if (obj == L"")
        return;

    SHELLEXECUTEINFO info;
    info.cbSize = sizeof(info);
    info.fMask = SEE_MASK_DEFAULT | SEE_MASK_NOCLOSEPROCESS;
    info.hwnd = wnd->Handle;
    info.lpVerb = L"runas";
    info.lpFile = ((ShellListView*)wnd->ActiveControl)->SelectedPath.c_str();
    info.lpParameters = NULL;
    info.lpDirectory = NULL;
    info.nShow = SW_SHOW;

    ShellExecuteEx(&info);
}

void MainWindow::PropertiesButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*)window;

    ShellListView* activeCtrl = (ShellListView*)wnd->ActiveControl;
    String obj = activeCtrl->SelectedPath;

    if (obj == L"")
        return;

    SHELLEXECUTEINFO info;

    ZeroMemory(
        &info,
        sizeof(info));

    info.cbSize = sizeof(info);
    info.fMask = SEE_MASK_INVOKEIDLIST;
    info.hwnd = wnd->Handle;
    info.lpVerb = L"properties";
    info.lpFile = ((ShellListView*)wnd->ActiveControl)->SelectedPath.c_str();
    info.nShow = SW_SHOW;

    ShellExecuteEx(&info);
}

void MainWindow::RenameButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*)window;
    ShellListView* activeCtrl = (ShellListView*) wnd->ActiveControl;
    String obj = activeCtrl->SelectedFile;

    if (obj != L"")
    {
        DialogBoxParam(
            ((Application*)wnd->AppContext)->AppInstance,
            MAKEINTRESOURCE(IDD_RENAMEOBJECT),
            wnd->Handle,
            wnd->RenameObjectDlgProc,
            (LPARAM)obj.c_str());
    }
}

void MainWindow::CmdButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*) window;
    String dir = ((ShellListView*)wnd->ActiveControl)->GetDirectory();
    String param = (String(L"/k \"cd \"").append(dir).append(L"\"\""));

    SHELLEXECUTEINFO info;

    ZeroMemory(
        &info,
        sizeof(info));

    info.cbSize = sizeof(info);
    info.fMask = SEE_MASK_DEFAULT | SEE_MASK_NOCLOSEPROCESS;
    info.hwnd = wnd->Handle;
    info.lpVerb = L"open";
    info.lpFile = L"C:\\Windows\\System32\\cmd.exe";
    info.lpParameters = param.c_str();
    info.lpDirectory = NULL;
    info.nShow = SW_SHOW;

    ShellExecuteEx(&info);
}
