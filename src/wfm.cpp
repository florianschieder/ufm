#include "wfm.h"

/**
 * wWinMain: Application entry point
 */

int __stdcall wWinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
    #ifdef _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    Application* app = new Application(hInstance, nCmdShow);
    MainWindow* wnd = new MainWindow(app);
    
    wnd->Show();

    delete app;
    delete wnd;

    return 0;
}
