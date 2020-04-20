#include "Application.h"

METHOD Application::Application(HINSTANCE h, int n, String name)
{
    this->m_hInstance = h;
    this->m_nCmdShow = n;
    this->m_appName = name;

    this->InitializeApplicationComponents();

    #ifdef _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif
}

METHOD void Application::InitializeApplicationComponents()
{
    // Initialize GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;

    HRESULT coInit = CoInitializeEx(
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
    this->InitializeShellImageBucket();
}

void Application::InitializeShellImageBucket()
{
    SHFILEINFO sfi;
    ZeroMemory(&sfi, sizeof(sfi));

    // Get the system image list
    this->m_largeShellImageBucket = reinterpret_cast<HIMAGELIST>(
        SHGetFileInfo(
            L"C:\\",
            0,
            &sfi,
            sizeof(SHFILEINFO),
            SHGFI_SYSICONINDEX));

    this->m_smallShellImageBucket = reinterpret_cast<HIMAGELIST>(
        SHGetFileInfo(
            L"C:\\",
            0,
            &sfi,
            sizeof(SHFILEINFO),
            SHGFI_SYSICONINDEX | SHGFI_SMALLICON));

    // Add our generic icons we need internally
    this->m_iconUp = LoadIcon(
        this->m_hInstance,
        MAKEINTRESOURCE(IDI_UP));

    this->m_iUpIconIndex = ImageList_AddIcon(
        this->m_largeShellImageBucket,
        this->m_iconUp);

    ImageList_AddIcon(
        this->m_smallShellImageBucket,
        this->m_iconUp);
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

METHOD int Application::Exit()
{
    CoUninitialize();
    Gdiplus::GdiplusShutdown(this->gdiplusToken);
    
    DestroyIcon(this->m_iconUp);
    DestroyCursor(this->m_cursorHand);
    DestroyCursor(this->m_cursorWait);

    ImageList_Destroy(this->m_largeShellImageBucket);
    ImageList_Destroy(this->m_smallShellImageBucket);

    return EXIT_SUCCESS;
}

void Application::GetConfig(String key, int* value)
{
    DWORD bufferSize = 4;

    auto val = RegGetValue(
        HKEY_CURRENT_USER,
        String(L"SOFTWARE\\").append(this->m_appName).c_str(),
        key.c_str(),
        RRF_RT_DWORD,
        NULL,
        (PVOID)value,
        &bufferSize);

    if (val != ERROR_SUCCESS)
    {
        *value = 0;
    }
}

String Application::GetConfig(String key)
{
    TCHAR buffer[1024];
    DWORD BufferSize = 1024 * sizeof(TCHAR);

    auto val = RegGetValue(HKEY_CURRENT_USER,
        String(L"SOFTWARE\\").append(this->m_appName).c_str(),
        key.c_str(),
        RRF_RT_ANY,
        NULL,
        (PVOID) &buffer,
        &BufferSize);

    if (val != ERROR_SUCCESS)
    {
        return L"";
    }
    else
    {
        buffer[1023] = L'\0';
        return buffer;
    }
}

void Application::SetConfig(String key, int value)
{
    DWORD bufferSize = 4;

    RegSetKeyValue(
        HKEY_CURRENT_USER,
        String(L"SOFTWARE\\").append(this->m_appName).c_str(),
        key.c_str(),
        REG_DWORD,
        (LPVOID)&value,
        bufferSize);
}

void Application::SetConfig(String key, LPCWSTR value)
{
    DWORD bufferSize = lstrlen(value) * sizeof(TCHAR);

    RegSetKeyValue(
        HKEY_CURRENT_USER,
        String(L"SOFTWARE\\").append(this->m_appName).c_str(),
        key.c_str(),
        RRF_RT_REG_SZ,
        (LPVOID)value,
        bufferSize);
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

HIMAGELIST* Application::GetShellImageBucketLarge()
{
    return &this->m_largeShellImageBucket;
}

HIMAGELIST* Application::GetShellImageBucketSmall()
{
    return &this->m_smallShellImageBucket;
}

int Application::GetInternalIconIndex(DWORD icon)
{
    switch (icon)
    {
        case IDI_UP:
            return this->m_iUpIconIndex;
        default:
            return 0;
    }
}
