#include "ToolButton.h"

METHOD ToolButton::ToolButton(ToolBar* toolBar)
    : Button(toolBar->GetParent())
{
    this->toolBar = toolBar;
    this->SetDimensions(0, 0, 24, 24);
}

METHOD ToolButton::ToolButton(ToolBar* toolBar, int width, int height) : ToolButton(toolBar)
{
    this->SetDimensions(0, 0, width, height);
}

METHOD void ToolButton::SetPosition(int num)
{
    this->m_x = 15 + (num * 31);
    this->m_y = 3;
}

METHOD void ToolButton::Show()
{
    Button::Show();
    toolBar->AddControl(this);
}
