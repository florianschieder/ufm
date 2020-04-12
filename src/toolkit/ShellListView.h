#pragma once

#include "wfmtk.h"
#include "../../res/resource.h"

struct DirectoryItem
{
    std::wstring    baseName;
    std::wstring    extName;
    std::wstring    date;
    std::wstring    size;
    int             iconIndex;
};

class ShellListView : public ListView
{
    public:
        METHOD ShellListView(Window* parent);
        METHOD String GetDirectory();
        METHOD void RefreshView();

        String SelectedPath;
        String SelectedFile;
        String SelectedExt;
        
        bool ShowHiddenFiles;

    private:
        METHOD bool Enumerate();

        String m_Directory;
        String m_Filter;

        METHOD LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) override;
};