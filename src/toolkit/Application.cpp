#include "Application.h"

METHOD Application::Application(HINSTANCE h, int n)
{
    this->m_hInstance = h;
    this->m_nCmdShow = n;

    this->InitializeApplicationComponents();
}

METHOD void Application::InitializeApplicationComponents()
{
    // Initialize GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;

    CoInitializeEx(
        NULL,
        COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE | COINIT_SPEED_OVER_MEMORY);

    GdiplusStartup(
        &gdiplusToken,
        &gdiplusStartupInput,
        NULL);

    // Load cursors
    this->m_cursorHand = LoadCursor(NULL, IDC_HAND);
    this->m_cursorWait = LoadCursor(NULL, IDC_WAIT);

    // Call more complex initialization routines
    this->InitializeCommonControls();
    this->InitializeEnvironment();
}

METHOD void Application::InitializeCommonControls()
{
    INITCOMMONCONTROLSEX icex;
    
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);

    icex.dwICC = ICC_ANIMATE_CLASS | ICC_BAR_CLASSES | ICC_COOL_CLASSES
        | ICC_DATE_CLASSES | ICC_HOTKEY_CLASS | ICC_INTERNET_CLASSES
        | ICC_LINK_CLASS | ICC_LISTVIEW_CLASSES | ICC_NATIVEFNTCTL_CLASS
        | ICC_PAGESCROLLER_CLASS | ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES
        | ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES | ICC_UPDOWN_CLASS
        | ICC_USEREX_CLASSES | ICC_WIN95_CLASSES;

    InitCommonControlsEx(&icex);
}

METHOD void Application::InitializeEnvironment()
{
    this->m_Environment = {0};

    // "Easy" member variables

    this->m_Environment.CommandLine = GetCommandLine();

    this->m_Environment.argv = CommandLineToArgvW(
        this->m_Environment.CommandLine.c_str(),
        &this->m_Environment.argc);

    WCHAR currentDirectoryBuffer[MAX_PATH];
    GetCurrentDirectory(
        MAX_PATH,
        currentDirectoryBuffer);

    this->m_Environment.CurrentDirectory = currentDirectoryBuffer;
    this->m_Environment.ExecutablePath = this->m_Environment.argv[0];

    // Locale Data

    wchar_t language[3];
    wchar_t country[3];

    GetLocaleInfo(
        LOCALE_USER_DEFAULT,
        LOCALE_SISO639LANGNAME,
        language,
        sizeof(language) / sizeof(wchar_t));

    GetLocaleInfo(
        LOCALE_USER_DEFAULT,
        LOCALE_SISO3166CTRYNAME,
        country,
        sizeof(country) / sizeof(wchar_t));

    this->m_Environment.Language = language;
    this->m_Environment.Country = country;

    this->m_Environment.LanguageISOCode = language;
    this->m_Environment.LanguageISOCode += L"_";
    this->m_Environment.LanguageISOCode += country;

    // OS Version

    int majorVersion;
    int minorVersion;

    RTL_OSVERSIONINFOW rovi = { 0 };

    HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
    if (hMod)
    {
        RtlGetVersion fxPtr = (RtlGetVersion)::GetProcAddress(
            hMod,
            "RtlGetVersion");

        if (fxPtr != nullptr)
        {
            rovi.dwOSVersionInfoSize = sizeof(rovi);
            if (fxPtr(&rovi) != STATUS_SUCCESS)
            {
                return;
            }
        }
    }

    majorVersion = rovi.dwMajorVersion;
    minorVersion = rovi.dwMinorVersion;

    if (majorVersion == 10)
    {
        this->m_Environment.OSVersionCode = OS_10;
    }
    else if (majorVersion == 6)
    {
        switch (minorVersion)
        {
        case 3:
            this->m_Environment.OSVersionCode = OS_8_1;
            break;
        case 2:
            this->m_Environment.OSVersionCode = OS_8;
            break;
        case 1:
            this->m_Environment.OSVersionCode = OS_7;
            break;
        case 0:
            this->m_Environment.OSVersionCode = OS_VISTA;
            break;
        }
    }
    else if (minorVersion == 5)
    {
        this->m_Environment.OSVersionCode = OS_XP;
    }
}

METHOD int Application::GetShowState()
{
    return this->m_nCmdShow;
}

METHOD void Application::IndicateTimeIntensiveProcess()
{
    SetCursor(this->m_cursorWait);
}

METHOD HINSTANCE Application::GetInstance()
{
    return this->m_hInstance;
}

METHOD wchar_t* Application::GetLanguageString(int id) {
    wchar_t buffer[4096];

    LoadString(
        this->m_hInstance,
        id,
        buffer,
        sizeof(buffer) / sizeof(wchar_t));

    String loadedString = buffer;

    return (wchar_t*) loadedString.c_str();
}

METHOD void Application::Restart()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcess(
        NULL,
        (LPWSTR) this->m_Environment.CommandLine.c_str(),
        NULL,
        NULL,
        FALSE,
        NORMAL_PRIORITY_CLASS,
        NULL,
        this->m_Environment.CurrentDirectory.c_str(),
        &si,
        &pi)) {
        this->Exit();
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

METHOD void Application::SetEnvironmentVar(String key, String value)
{
    SetEnvironmentVariable(
        key.c_str(),
        value.c_str());
}

METHOD void Application::UnindicateTimeIntensiveProcess()
{
    SetCursor(this->m_cursorHand);
}

METHOD void Application::Exit()
{
    CoUninitialize();
    Gdiplus::GdiplusShutdown(this->gdiplusToken);
    exit(0);
}

METHOD Environment Application::GetEnvironment()
{
    return this->m_Environment;
}

METHOD String Application::GetEnvironmentVar(String key)
{
    wchar_t buffer[4096];

    GetEnvironmentVariable(
        key.c_str(),
        buffer,
        sizeof(buffer) / sizeof(wchar_t));

    return buffer;
}
