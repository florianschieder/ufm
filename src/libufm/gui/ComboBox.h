#pragma once

#include "Control.h"
#include "../libufmdef.h"

class ComboBox : public Control
{
    public:
        METHOD ComboBox(Window* parent);

        METHOD void AddItem(String item);
        METHOD void Show();

        String SelectedItem;

        void (*OnSelectionChanged)(ComboBox* object, Window* parent) = nullptr;

        METHOD static LRESULT CALLBACK MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
        METHOD LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
};
