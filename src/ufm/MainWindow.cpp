#include "MainWindow.h"

#include "../../res/ufm/resource.h"

using namespace std::filesystem;

/**
 * MainWindow::MainWindow: Main window constructor
 */
MainWindow::MainWindow(Application* app) : Window(app)
{
    this->m_iconID = IDI_MAIN_ICON;
    this->m_menuID = IDR_MAINMENU;

    this->SetStyle(WS_OVERLAPPEDWINDOW);
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

    this->toolBar = new ToolBar(this, 0, 0, this->m_width, 32);
    this->toolBar->AddStartGripper();
    this->toolBar->AddSeparator(42);
    this->toolBar->AddSeparator(166);
    this->toolBar->AddSeparator(290);
    this->toolBar->Show();

    // Add toolbar buttons

    this->refreshButton = new ToolButton(this->toolBar);
    this->refreshButton->SetPosition(0);
    this->refreshButton->SetIcon(IDI_REFRESH, 16, 16);
    this->refreshButton->OnClick = this->RefreshButtonClicked;
    this->refreshButton->Show();
    this->refreshButton->AddToolTip(L"Refresh directory");

    this->detailsButton = new ToolButton(this->toolBar);
    this->detailsButton->SetPosition(1);
    this->detailsButton->SetIcon(IDI_DETAILS, 16, 16);
    this->detailsButton->OnClick = this->DetailsButtonClicked;
    this->detailsButton->Show();
    this->detailsButton->AddToolTip(L"Details view");

    this->listButton = new ToolButton(this->toolBar);
    this->listButton->SetPosition(2);
    this->listButton->SetIcon(IDI_LIST, 16, 16);
    this->listButton->OnClick = this->ListButtonClicked;
    this->listButton->Show();
    this->listButton->AddToolTip(L"List view");

    this->smallButton = new ToolButton(this->toolBar);
    this->smallButton->SetPosition(3);
    this->smallButton->SetIcon(IDI_SMALL, 16, 16);
    this->smallButton->OnClick = this->SmallIconsButtonClicked;
    this->smallButton->Show();
    this->smallButton->AddToolTip(L"Small icon view");

    this->largeButton = new ToolButton(this->toolBar);
    this->largeButton->SetPosition(4);
    this->largeButton->SetIcon(IDI_LARGE, 16, 16);
    this->largeButton->OnClick = this->LargeIconsButtonClicked;
    this->largeButton->Show();
    this->largeButton->AddToolTip(L"Large icon view");

    this->printButton = new ToolButton(this->toolBar);
    this->printButton->SetPosition(5);
    this->printButton->SetIcon(IDI_PRINT, 16, 16);
    this->printButton->OnClick = this->PrintButtonClicked;
    this->printButton->Show();
    this->printButton->AddToolTip(L"Print selected document");

    this->adminButton = new ToolButton(this->toolBar);
    this->adminButton->SetPosition(6);
    this->adminButton->SetIcon(IDI_ADMIN, 16, 16);
    this->adminButton->OnClick = this->AdminButtonClicked;
    this->adminButton->Show();
    this->adminButton->AddToolTip(L"Run program as administrator");

    this->propertyButton = new ToolButton(this->toolBar);
    this->propertyButton->SetPosition(7);
    this->propertyButton->SetIcon(IDI_PROPERTY, 16, 16);
    this->propertyButton->OnClick = this->PropertiesButtonClicked;
    this->propertyButton->Show();
    this->propertyButton->AddToolTip(L"Show file/directory properties");

    this->renameButton = new ToolButton(this->toolBar);
    this->renameButton->SetPosition(8);
    this->renameButton->SetIcon(IDI_RENAME, 16, 16);
    this->renameButton->OnClick = this->RenameButtonClicked;
    this->renameButton->Show();
    this->renameButton->AddToolTip(L"Rename file/directory");

    this->cmdButton = new ToolButton(this->toolBar);
    this->cmdButton->SetPosition(9);
    this->cmdButton->SetIcon(IDI_CMD, 16, 16);
    this->cmdButton->OnClick = this->CmdButtonClicked;
    this->cmdButton->Show();
    this->cmdButton->AddToolTip(L"Open command prompt");

    // Construct "address bar"

    this->leftBox = new DriveComboBox(this);
    this->leftBox->OnSelectionChanged = this->ComboBoxSelectionChanged;
    this->leftBox->SetDimensions(5, 35, 60, 22);
    this->leftBox->Show();

    this->leftPath = new ShellInputBox(this, 70, 35, 60, 22);
    this->leftPath->Show();

    this->leftBoxApply = new Button(this);
    this->leftBoxApply->SetDimensions(360, 34, 30, 24);
    this->leftBoxApply->SetIcon(IDI_FORWARD, 16, 16);
    this->leftBoxApply->OnClick = this->LeftApplyButtonClicked;
    this->leftBoxApply->Show();

    this->rightBox = new DriveComboBox(this);
    this->rightBox->OnSelectionChanged = this->ComboBoxSelectionChanged;
    this->rightBox->SetDimensions(395, 35, 60, 22);
    this->rightBox->Show();

    this->rightPath = new ShellInputBox(this, 460, 35, 60, 22);
    this->rightPath->Show();

    this->rightBoxApply = new Button(this);
    this->rightBoxApply->SetDimensions(750, 34, 30, 24);
    this->rightBoxApply->SetIcon(IDI_FORWARD, 16, 16);
    this->rightBoxApply->OnClick = this->RightApplyButtonClicked;
    this->rightBoxApply->Show();

    // Construct shell views

    this->leftShellView = new ShellListView(this);
    this->leftShellView->SetDimensions(-1, 60, 401, 366);
    this->leftShellView->OnSelectionChanged = this->ShellListViewSelectionChanged;
    this->leftShellView->RefreshView();
    this->leftShellView->SetView(LVS_REPORT);
    
    this->rightShellView = new ShellListView(this);
    this->rightShellView->SetDimensions(400, 60, 401, 366);
    this->rightShellView->OnSelectionChanged = this->ShellListViewSelectionChanged;
    this->rightShellView->RefreshView();
    this->rightShellView->SetView(LVS_REPORT);
    
    this->ActiveControl = this->leftShellView;

    // Construct button bar

    this->showButton = new Button(this);
    this->showButton->SetDimensions(0, this->m_height - 46, m_width / 7, 22);
    this->showButton->SetText(L"[F3] Show");
    this->showButton->OnClick = this->FileViewerButtonClicked;
    this->showButton->Show();

    this->editButton = new Button(this);
    this->editButton->SetDimensions(m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->editButton->SetText(L"[F4] Edit");
    this->editButton->OnClick = this->EditButtonClicked;
    this->editButton->Show();

    this->copyButton = new Button(this);
    this->copyButton->SetDimensions(2 * m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->copyButton->SetText(L"[F5] Copy");
    this->copyButton->OnClick = this->CopyButtonClicked;
    this->copyButton->Show();

    this->moveButton = new Button(this);
    this->moveButton->SetDimensions(3 * m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->moveButton->SetText(L"[F6] Move");
    this->moveButton->OnClick = this->MoveButtonClicked;
    this->moveButton->Show();

    this->newDirectoryButton = new Button(this);
    this->newDirectoryButton->SetDimensions(4 * m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->newDirectoryButton->SetText(L"[F7] New Directory");
    this->newDirectoryButton->OnClick = this->NewDirButtonClicked;
    this->newDirectoryButton->Show();

    this->deleteButton = new Button(this);
    this->deleteButton->SetDimensions(5 * m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->deleteButton->SetText(L"[F8] Delete");
    this->deleteButton->OnClick = this->DeleteButtonClicked;
    this->deleteButton->Show();

    this->closeButton = new Button(this);
    this->closeButton->SetDimensions(6 * m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->closeButton->SetText(L"[Alt+F4] Close");
    this->closeButton->OnClick = this->CloseWindow;
    this->closeButton->Show();

    // Construct status bar

    this->statusbar = new StatusBar(this, 0, this->m_height - 22, this->m_width, 22);
    this->statusbar->AddStartGripper();
    this->statusbar->Show();

    // Prepare window + show

    this->SetTitle(L"UFM");
    this->SetDimensions(800, 600);

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
                this->m_application->GetInstance(),
                MAKEINTRESOURCE(IDD_ABOUTDLG),
                this->GetHandle(),
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
                        this->m_application->GetInstance(),
                        this->m_windowHandle,
                        errorText,
                        this->GetTitle().c_str(),
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
                        this->GetApplication()->GetInstance(),
                        this->GetHandle(),
                        L"The directory you tried to create already exists.",
                        this->GetTitle().c_str(),
                        MB_ICONWARNING,
                        MB_OK);
                    break;
                case ERROR_ACCESS_DENIED:
                    ShellMessageBox(
                        this->GetApplication()->GetInstance(),
                        this->GetHandle(),
                        L"Failed to create the directory - access denied.",
                        this->GetTitle().c_str(),
                        MB_ICONWARNING,
                        MB_OK);
                    break;
                default:
                    ShellMessageBox(
                        this->GetApplication()->GetInstance(),
                        this->GetHandle(),
                        L"An unexpected error occurred trying to create the directory. A common "\
                        "reason for this is an invalid parameter you specified. Please try again with "\
                        "directory name. We\'re sorry for the inconvenience.",
                        this->GetTitle().c_str(),
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
        (Gdiplus::REAL) this->m_width,
        (Gdiplus::REAL) 28);

    Gdiplus::RectF rectBg(
        (Gdiplus::REAL) 0,
        (Gdiplus::REAL) 60,
        (Gdiplus::REAL) this->m_width,
        (Gdiplus::REAL) this->leftShellView->GetHeight());

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
    this->toolBar->Resize(
        0,
        this->toolBar->GetY(),
        this->m_width,
        this->toolBar->GetHeight());

    this->leftPath->Resize(
        this->leftPath->GetX(),
        this->leftPath->GetY(),
        (this->m_width / 2) - (this->leftPath->GetX() + 10 + this->leftBoxApply->GetWidth()),
        this->leftPath->GetHeight());

    this->leftBoxApply->Resize(
        (this->m_width / 2) - (this->leftBoxApply->GetWidth() + 5),
        this->leftBoxApply->GetY(),
        this->leftBoxApply->GetWidth(),
        this->leftBoxApply->GetHeight());

    this->rightBox->Resize(
        (this->m_width / 2) + 5,
        this->rightBox->GetY(),
        this->rightBox->GetWidth(),
        this->rightBox->GetHeight());

    this->rightPath->Resize(
        this->rightBox->GetX() + this->rightBox->GetWidth() + 5,
        this->rightPath->GetY(),
        (this->m_width / 2) - (this->leftPath->GetX() + 10 + this->leftBoxApply->GetWidth()),
        this->rightPath->GetHeight());

    this->rightBoxApply->Resize(
        this->m_width - 35,
        this->rightBoxApply->GetY(),
        this->rightBoxApply->GetWidth(),
        this->rightBoxApply->GetHeight());

    this->leftShellView->Resize(
        -1,
        this->leftShellView->GetY(),
        this->m_width / 2,
        this->m_height - (this->leftShellView->GetY() + 46));

    this->rightShellView->Resize(
        (this->m_width / 2) + 1,
        this->rightShellView->GetY(),
        this->m_width / 2,
        this->m_height - (this->rightShellView->GetY() + 46));

    this->statusbar->Resize(
        this->statusbar->GetX(),
        this->m_height - 22,
        this->m_width,
        22);

    this->showButton->Resize(
        0,
        this->m_height - 44,
        this->m_width / 7,
        22);

    this->editButton->Resize(
        this->m_width / 7,
        this->m_height - 44,
        this->m_width / 7,
        22);

    this->copyButton->Resize(
        2 * this->m_width / 7,
        this->m_height - 44,
        this->m_width / 7,
        22);

    this->moveButton->Resize(
        3 * this->m_width / 7,
        this->m_height - 44,
        this->m_width / 7,
        22);

    this->newDirectoryButton->Resize(
        4 * this->m_width / 7,
        this->m_height - 44,
        this->m_width / 7,
        22);

    this->deleteButton->Resize(
        5 * this->m_width / 7,
        this->m_height - 44,
        this->m_width / 7,
        22);

    this->closeButton->Resize(
        6 * this->m_width / 7,
        this->m_height - 44,
        this->m_width / 7,
        22);
}

// Event Handler

void MainWindow::FileViewerButtonClicked(Window* window)
{
    ShellListView* activeCtrl = (ShellListView*)(((MainWindow*)window)->ActiveControl);

    if (!(GetFileAttributes(activeCtrl->SelectedPath.c_str()) & FILE_ATTRIBUTE_DIRECTORY))
    {
        std::shared_ptr<FileViewerWindow> fileViewer = std::make_shared<FileViewerWindow>(window->GetApplication(), activeCtrl->SelectedExt);

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
    info.hwnd = window->GetHandle();
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

    wnd->GetApplication()->IndicateTimeIntensiveProcess();

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
        wnd->GetApplication()->UnindicateTimeIntensiveProcess();
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
    wnd->GetApplication()->UnindicateTimeIntensiveProcess();
}

void MainWindow::MoveButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*)window;

    wnd->GetApplication()->IndicateTimeIntensiveProcess();

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
        wnd->GetApplication()->UnindicateTimeIntensiveProcess();
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

    wnd->GetApplication()->UnindicateTimeIntensiveProcess();
}

void MainWindow::NewDirButtonClicked(Window* window)
{
    MainWindow* wnd = (MainWindow*) window;

    DialogBox(
        wnd->GetApplication()->GetInstance(),
        MAKEINTRESOURCE(IDD_NEWDIR),
        wnd->GetHandle(),
        wnd->NewDirDlgProc);
}

void MainWindow::ShellListViewSelectionChanged(ShellListView* object, Window* parent)
{
    MainWindow* wnd = (MainWindow*)parent;

    if (object == wnd->leftShellView)
    {
        wnd->leftPath->SetText(
            (TCHAR*) wnd->leftShellView->GetDirectory().c_str());

        SendMessage(
            wnd->leftBox->GetHandle(),
            CB_SELECTSTRING,
            -1,
            (LPARAM) wnd->leftShellView->CurrentDrive.c_str());
    }
    else if (object == wnd->rightShellView)
    {
        wnd->rightPath->SetText(
            (TCHAR*)wnd->rightShellView->GetDirectory().c_str());

        SendMessage(
            wnd->rightBox->GetHandle(),
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

        fileOp.hwnd = wnd->GetHandle();
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
        wnd->leftPath->GetText());
}

void MainWindow::RightApplyButtonClicked(Window* parent)
{
    MainWindow* wnd = (MainWindow*) parent;
    wnd->rightShellView->SetDirectory(
        wnd->rightPath->GetText());
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
    info.hwnd = wnd->GetHandle();
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
    info.hwnd = wnd->GetHandle();
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
    info.hwnd = wnd->GetHandle();
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
            wnd->GetApplication()->GetInstance(),
            MAKEINTRESOURCE(IDD_RENAMEOBJECT),
            wnd->GetHandle(),
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
    info.hwnd = wnd->GetHandle();
    info.lpVerb = L"open";
    info.lpFile = L"C:\\Windows\\System32\\cmd.exe";
    info.lpParameters = param.c_str();
    info.lpDirectory = NULL;
    info.nShow = SW_SHOW;

    ShellExecuteEx(&info);
}
