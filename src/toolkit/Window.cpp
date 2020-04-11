#include "Window.h"

METHOD Window::Window(Application* app)
{
    this->m_application = app;
    this->m_windowHandle = { 0 };
    this->m_parentWindow = { 0 };
    this->m_windowClass = { };

    this->m_iconID = -1;
    this->m_menuID = -1;

    this->m_style = WS_OVERLAPPEDWINDOW;

    this->m_width = 300;
    this->m_height = 300;
}

METHOD Window::Window(Application* app, Window* parent) : Window(app)
{
    this->m_parentWindow = parent;
}

METHOD String Window::GetTitle()
{
    wchar_t title[300];
    GetWindowText(this->m_windowHandle, title, 300);
    return title;
}

METHOD Window* Window::GetParent()
{
    return this->m_parentWindow;
}

METHOD bool Window::IsOpen()
{
    return this->m_isOpen;
}

METHOD void Window::SetDimensions(int width, int height)
{
    SetWindowPos(
        this->m_windowHandle,
        0,
        -1, -1,
        width, height,
        SWP_NOZORDER | SWP_SHOWWINDOW);
}

METHOD void Window::SetTitle(String Title)
{
    SendMessage(this->m_windowHandle, WM_SETTEXT, 0, (LPARAM) Title.c_str());
}

METHOD int Window::Create()
{
    MakeUniqueClassNameW(className, L"WINDOW");

    this->m_windowClass = { };

    this->m_windowClass.lpfnWndProc = this->MessageLoopForwarder;
    this->m_windowClass.hInstance = this->m_application->GetInstance();
    this->m_windowClass.lpszClassName = className.c_str();
    this->m_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

    if(this->m_iconID != -1)
        this->m_windowClass.hIcon = LoadIcon(
            this->m_application->GetInstance(),
            MAKEINTRESOURCE(this->m_iconID));

    if(this->m_menuID != -1)
        this->m_windowClass.lpszMenuName = MAKEINTRESOURCE(this->m_menuID);

    if (!RegisterClass(&this->m_windowClass)) return -1;

    this->m_windowHandle = CreateWindowEx(
        0,
        this->m_windowClass.lpszClassName,
        L"Window",
        this->m_style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (this->m_parentWindow == NULL) ? NULL : this->m_parentWindow->GetHandle(),
        NULL,
        this->m_application->GetInstance(),
        this);

    return (this->m_windowHandle == NULL) ? -2 : 0;
}

METHOD void Window::Show()
{
    if (this->Create() == 0)
    {
        this->m_isOpen = true;
        ShowWindow(this->m_windowHandle, SW_SHOW);

        if (this->m_parentWindow == NULL)
        {
            MSG msg = { };
            while (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}

METHOD void Window::Destroy()
{
    DestroyWindow(this->m_windowHandle);
    this->m_isOpen = false;
}

METHOD void Window::Close()
{
    SendMessage(this->m_windowHandle, WM_CLOSE, 0, 0);
}

METHOD void Window::SetIcon(HICON hIcon)
{
    SendMessage(this->m_windowHandle, WM_SETICON, 0, (LPARAM)hIcon);
    SendMessage(this->m_windowHandle, WM_SETICON, 1, (LPARAM)hIcon);
}

METHOD void Window::SetStyle(int style)
{
    this->m_style = style;
}

METHOD LRESULT CALLBACK Window::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_NOTIFY:
            SendMessage(
                GetDlgItem(hwnd, wParam),
                WM_NOTIFY,
                ((LPNMHDR)lParam)->code,
                lParam);
            break;

        case WM_COMMAND:
            switch (HIWORD(wParam))
            {
                case BN_CLICKED:
                    SendMessage(
                        GetDlgItem(hwnd, LOWORD(wParam)),
                        WM_COMMAND,
                        wParam,
                        lParam);

                    if (GetLastError() == ERROR_INVALID_WINDOW_HANDLE)
                    {
                        // It was a menu
                        this->OnMenuEvent(LOWORD(wParam));
                    }
                    break;

                case BN_DOUBLECLICKED:
                    SendMessage(
                        GetDlgItem(hwnd, LOWORD(wParam)),
                        WM_COMMAND,
                        wParam,
                        lParam);
            }
            return 0;

        case WM_CLOSE:
            this->OnClose();
            return 0;

        case WM_DESTROY:
            UnregisterClass(
                this->m_windowClass.lpszClassName,
                this->m_application->GetInstance());
            
            if(this->m_parentWindow == nullptr) PostQuitMessage(0);

            return 0;

        case WM_POSTPARAM:
            this->OnPostParam(
                (void*) wParam,
                lParam);
            return 0;

        case WM_CREATE:
            this->m_windowHandle = hwnd;

            this->OnInitializeWindow();

            return 0;

        case WM_SIZE:
            this->m_width = LOWORD(lParam);
            this->m_height = HIWORD(lParam);
            
            this->OnResizeWindow();

            return 0;

        case WM_PAINT:
            this->m_windowHandle = hwnd;

            InvalidateRgn(this->m_windowHandle, NULL, TRUE);

            PAINTSTRUCT ps;
            RECT rect = { 0 };
            HDC paint_hdc;

            paint_hdc = BeginPaint(hwnd, &ps);
            rect.left = 0;
            rect.top = 0;
            rect.right = this->m_width;
            rect.bottom = this->m_height;
            ps.rcPaint = rect;

            FillRect(paint_hdc, &rect, (HBRUSH)(RGB(255, 255, 255)));
            
            this->OnPaint(ps, paint_hdc);

            EndPaint(hwnd, &ps);

            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

METHOD LRESULT CALLBACK Window::MessageLoopForwarder(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* wnd;
    if (msg == WM_CREATE)
    {
        wnd = (Window*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)wnd);
    }
    else
    {
        wnd = (Window*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    return wnd->MessageLoop(hWnd, msg, wParam, lParam);
}

METHOD HWND Window::GetHandle()
{
    return this->m_windowHandle;
}

Window::operator HWND()
{
    return this->m_windowHandle;
}

METHOD Application* Window::GetApplication()
{
    return this->m_application;
}

METHOD void Window::CloseWindow(Window* window)
{
    window->Destroy();
}
