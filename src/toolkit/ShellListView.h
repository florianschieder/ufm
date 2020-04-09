#pragma once

#include "wfmtk.h"
#include "core/DirectoryList.h"
#include "../../resource.h"

class ShellListView : public ListView {
    public:
        METHOD ShellListView(Window* parent);
        METHOD void RefreshView();

        String SelectedPath;
        String SelectedExt;
        
        bool ShowHiddenFiles;

    private:
        HIMAGELIST largeImages;
        HIMAGELIST smallImages;

        String m_Directory;
        String m_Filter;

        METHOD LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) override;
};