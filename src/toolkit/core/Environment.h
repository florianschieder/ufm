#pragma once

#include "../wfmtkdef.h"

#include <vector>

typedef struct s_Environment {
    int argc;
    wchar_t** argv;

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

    bool Is64BitOperatingSystem;
    bool Is64BitProcess;

    int OSVersionMajor;
    int OSVersionMinor;
    int OSVersionCode;

    int ProcessorCount;
    int SystemPageSize;
    int TickCount;

    long WorkingSet;
} Environment;
