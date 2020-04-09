#pragma once

#include "../wfmtkdef.h"

#include "String.h"

#include <vector>
#include <stdlib.h>

struct DirectoryItem {    
    std::wstring baseName;
    std::wstring extName;
    std::wstring date;
    std::wstring size;

    int iconIndex = 2;
};

class DirectoryList {
    public:
        METHOD DirectoryList(std::wstring dir);

        METHOD void Enumerate();
        METHOD std::wstring GetAbsolutePath();
        METHOD void SetPath(std::wstring dir);

        METHOD std::vector<DirectoryItem>& GetDirectories();
        METHOD std::vector<DirectoryItem>& GetFiles();
        METHOD void SetHidden(bool show);

    private:
        bool showHiddenFiles = false;

        std::wstring DirectoryPath;

        std::vector<DirectoryItem> directories;
        std::vector<DirectoryItem> files;
};

class AccessDeniedException : public std::exception {};
