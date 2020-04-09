#include "MainWindow.h"

#include "../res/resource.h"

/**
 * MainWindow::MainWindow: Main window constructor
 */
MainWindow::MainWindow(Application* app) : Window(app)
{
    this->m_iconID = IDI_MAIN_ICON;
    this->m_menuID = IDR_MAINMENU;

    this->SetStyle(WS_OVERLAPPEDWINDOW);
}

MainWindow::~MainWindow() {
    delete this->aboutDlg;
    delete this->buttonBar;
    delete this->driveBar;
    delete this->statusbar;

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

void MainWindow::FileViewerButtonClicked(Window* window)
{
    ShellListView* activeCtrl = (ShellListView*)(((MainWindow*)window)->ActiveControl);

    std::shared_ptr<FileViewerWindow> fileViewer = std::make_shared<FileViewerWindow>(window->GetApplication(), activeCtrl->SelectedExt);

    fileViewer->SetFile(activeCtrl->SelectedPath.c_str());
    fileViewer->Show();
}

/**
 * MainWindow::OnClose: Main window close handler
 */
void MainWindow::OnClose()
{
    this->Destroy();
}

/**
 * MainWindow::OnInitializeWindow: Handler for window and control initialization
 */
void MainWindow::OnInitializeWindow()
{
    this->buttonBar = new ToolBar(this, 0, 0, this->m_width, 32);
    this->buttonBar->AddStartGripper();
    this->buttonBar->Show();

    this->driveBar = new ToolBar(this, 0, 32, this->m_width, 28);
    this->driveBar->Show();

    this->leftShellView = new ShellListView(this);
    this->leftShellView->SetDimensions(-1, 60, 401, 366);
    this->leftShellView->RefreshView();
    this->leftShellView->SetView(LVS_REPORT);
    
    this->rightShellView = new ShellListView(this);
    this->rightShellView->SetDimensions(400, 60, 401, 366);
    this->rightShellView->RefreshView();
    this->rightShellView->SetView(LVS_REPORT);
    
    this->ActiveControl = this->leftShellView;

    this->showButton = new Button(this);
    this->showButton->SetDimensions(0, this->m_height - 46, m_width / 7, 22);
    this->showButton->SetText(L"[F3] Show");
    this->showButton->OnClick = this->FileViewerButtonClicked;
    this->showButton->Show();

    this->editButton = new Button(this);
    this->editButton->SetDimensions(m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->editButton->SetText(L"[F4] Edit");
    this->editButton->Show();

    this->copyButton = new Button(this);
    this->copyButton->SetDimensions(2 * m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->copyButton->SetText(L"[F5] Copy");
    this->copyButton->Show();

    this->moveButton = new Button(this);
    this->moveButton->SetDimensions(3 * m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->moveButton->SetText(L"[F6] Move");
    this->moveButton->Show();

    this->newDirectoryButton = new Button(this);
    this->newDirectoryButton->SetDimensions(4 * m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->newDirectoryButton->SetText(L"[F7] New Directory");
    this->newDirectoryButton->Show();

    this->deleteButton = new Button(this);
    this->deleteButton->SetDimensions(5 * m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->deleteButton->SetText(L"[F8] Delete");
    this->deleteButton->Show();

    this->closeButton = new Button(this);
    this->closeButton->SetDimensions(6 * m_width / 7, this->m_height - 44, m_width / 7, 22);
    this->closeButton->SetText(L"[Alt+F4] Close");
    this->closeButton->Show();

    this->statusbar = new StatusBar(this, 0, this->m_height - 22, this->m_width, 22);
    this->statusbar->AddStartGripper();
    this->statusbar->Show();
    
    this->SetTitle(L"WFM");
    this->SetDimensions(800, 600);
}

/**
 * MainWindow::OnControlEvent: Handling method for control messages
 */
void MainWindow::OnMenuEvent(WORD menuID)
{
    ShellListView* activeControl;

    switch (menuID) {
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
        case ID_FILE_CLOSE:
            this->Close();
            break;
        case ID_VIEW_TOGGLEHIDDENFILES:
            this->leftShellView->ShowHiddenFiles = !this->leftShellView->ShowHiddenFiles;
            this->rightShellView->ShowHiddenFiles = !this->rightShellView->ShowHiddenFiles;
            this->leftShellView->RefreshView();
            this->rightShellView->RefreshView();
            break;
        case ID_HELP_ABOUTWFM:
            if (this->aboutDlg == nullptr) {
                this->aboutDlg = new AboutWindow(this->m_application, this);
                this->aboutDlg->Show();
            } else {
                if (this->aboutDlg->IsOpen()) {
                    SetFocus(this->aboutDlg->GetHandle());
                } else {
                    delete this->aboutDlg;
                    this->aboutDlg = new AboutWindow(this->m_application, this);
                    this->aboutDlg->Show();
                }
            }
            break;
    }
}

/**
 * MainWindow::OnPaint: Main window rendering handler for custom drawing
 */
void MainWindow::OnPaint(PAINTSTRUCT ps, HDC hdc)
{
    RECT rect;

    rect.left = 0;
    rect.top = 0;
    rect.right = ps.rcPaint.right;
    rect.bottom = ps.rcPaint.bottom;
    FillRect(hdc, &rect, CreateSolidBrush(RGB(217, 223, 240)));
    
    rect.left = 0;
    rect.top = ps.rcPaint.bottom - 46;
    rect.right = ps.rcPaint.right;
    rect.bottom = ps.rcPaint.bottom;
    FillRect(hdc, &rect, GetSysColorBrush(COLOR_BTNFACE));
}

/**
 * MainWindow::OnResizeWindow: Handler for main window resizing
 */
void MainWindow::OnResizeWindow()
{
    this->buttonBar->Resize(
        0,
        this->buttonBar->GetY(),
        this->m_width,
        this->buttonBar->GetHeight());

    this->driveBar->Resize(
        0,
        this->driveBar->GetY(),
        this->m_width,
        this->driveBar->GetHeight());

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
