#include "ufm.h"

/**
 * wWinMain: Application entry point
 */

int __stdcall wWinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
    Application app(
        hInstance,
        nCmdShow,
        L"WFM");

    MainWindow* wnd = new MainWindow(&app);
    
    wnd->Show();

    delete wnd;

    return app.Exit();
}
