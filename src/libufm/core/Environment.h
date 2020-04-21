#pragma once

#include "../libufmdef.h"

#include <vector>

typedef struct s_Environment {
    int argc = 0;
    wchar_t** argv = nullptr;

    std::vector<String> Drives;

    String CommandLine;
    String CompanyName;
    String Country;
    String CurrentDirectory;
    String ExecutablePath;
    String Language;
    String LanguageISOCode;
    String MachineName;
    String ProductName;
    String ProductVersion;
    String SystemDirectory;

    bool Is64BitOperatingSystem = false;
    bool Is64BitProcess = false;

    int OSVersionMajor = 0;
    int OSVersionMinor = 0;
    int OSVersionCode = 0;

    int ProcessorCount = 0;
    int SystemPageSize = 0;
    int TickCount = 0;

    long WorkingSet = 0L;
} Environment;
