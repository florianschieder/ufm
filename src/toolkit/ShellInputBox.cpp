#include "ShellInputBox.h"

METHOD ShellInputBox::ShellInputBox(Window* parent, int x, int y, int width, int height) : InputBox(parent, x, y, width, height)
{
}

METHOD void ShellInputBox::Show()
{
    InputBox::Show();
    HRESULT r = SHAutoComplete(
        this->m_controlHandle,
        SHACF_FILESYS_DIRS | SHACF_AUTOSUGGEST_FORCE_ON);
}
