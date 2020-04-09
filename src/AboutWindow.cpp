#include "AboutWindow.h"

/**
 * AboutWindow::AboutWindow: About dialog constructor
 */
AboutWindow::AboutWindow(
    Application* app, Window* parent) : Window(app, parent)
{
    this->m_iconID = IDI_MAIN_ICON;

    this->SetStyle(WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER);
}

void AboutWindow::OnInitializeWindow() {
    this->SetTitle(L"About WFM");
    this->SetDimensions(410, 339);
}

/**
 * About::OnClose: About dialog closing handler (empty here)
 */
void AboutWindow::OnClose()
{  
    this->Destroy();
}

/**
 * About::OnControlEvent: Handle control messages here
 */
void AboutWindow::OnMenuEvent(WORD menuID)
{
    if (menuID == IDC_ABOUTWINDOW_CLOSE_BUTTON)
    {
        this->Close();
    }
}

/**
 * About::OnPaint: About dialog rendering here
 */
void AboutWindow::OnPaint(PAINTSTRUCT ps, HDC hdc)
{
    BITMAP bm;
    HBITMAP aboutBitmap = (HBITMAP) LoadImage(
        this->m_application->GetInstance(),
        MAKEINTRESOURCE(IDB_ABOUT),
        IMAGE_BITMAP,
        400,
        300,
        0);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, aboutBitmap);

    GetObject(aboutBitmap, sizeof(bm), &bm);

    BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
}

/**
 * About::OnResizeWindow: Dialog resizing handler. Unnecessary here
 * (no resizing / minimizing / maximizing in this dialog)
 */
void AboutWindow::OnResizeWindow()
{
}
