#pragma once

#include <vector>

#include <Windows.h>
#include <CommCtrl.h>
#include <gdiplus.h>

#include "wfmtkdef.h"

#include "core/Environment.h"

class Application final
{
    public:
        METHOD Application(HINSTANCE h, int n);

        METHOD void Exit();

        METHOD Environment GetEnvironment();
        METHOD String GetEnvironmentVar(String key);
        METHOD HINSTANCE GetInstance();
        METHOD wchar_t* GetLanguageString(int id);
        METHOD int GetShowState();
        METHOD void IndicateTimeIntensiveProcess();
        METHOD void Restart();
        METHOD void SetEnvironmentVar(String key, String value);
        METHOD void UnindicateTimeIntensiveProcess();

    private:
        METHOD void InitializeApplicationComponents();

        METHOD void InitializeCommonControls();
        METHOD void InitializeEnvironment();
        
        ULONG_PTR gdiplusToken;

        HCURSOR m_cursorHand;
        HCURSOR m_cursorWait;
        Environment m_Environment;
        HINSTANCE m_hInstance;
        int m_nCmdShow;
};
