#include "ListView.h"

METHOD ListView::ListView(Window* parent) : Control(parent) {
    this->m_controlHandle = { 0 };
}

METHOD void ListView::SetDimensions(int x, int y, int w, int h) {
    this->m_x = x;
    this->m_y = y;
    this->m_width = w;
    this->m_height = h;
}

void ListView::AddItem(int id, int imageID) {
    LV_ITEM item = { 0 };
    item.mask = LVIF_IMAGE;
    item.iItem = id;
    item.iImage = imageID;

    ListView_InsertItem(
        this->m_controlHandle,
        &item);
}

void ListView::AddSubItem(int id, int subID, String Text) {
    ListView_SetItemText(
        this->m_controlHandle,
        id,
        subID,
        (LPWSTR) Text.c_str());
}

METHOD void ListView::Show() {
    HFONT hFont = CreateFont(
        PT(9),
        0,
        0,
        0,
        FW_DONTCARE,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Segoe UI"));

    this->m_controlHandle = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WC_LISTVIEW,
        L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | this->specificStyles,
        this->m_x,
        this->m_y,
        this->m_width,
        this->m_height,
        this->m_parentWindow->GetHandle(),
        (HMENU) this->m_ctrlID,
        this->m_parentWindow->GetApplication()->GetInstance(),
        NULL);

    SetWindowLongPtr(
        m_controlHandle,
        GWLP_USERDATA,
        (LONG_PTR)this);

    SetWindowSubclass(
        m_controlHandle,
        this->MessageLoopForwarder,
        0,
        0);

    SendMessage(
        this->m_controlHandle,
        LVM_SETEXTENDEDLISTVIEWSTYLE,
        0,
        LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

    SendMessage(
        this->m_controlHandle,
        WM_SETFONT,
        (WPARAM)hFont,
        TRUE);
}

METHOD void ListView::SetView(int view) {
    SendMessage(
        this->m_controlHandle,
        LVM_SETVIEW,
        view,
        0);
}

METHOD void ListView::AddColumn(String Header, int width, int pos) {
    LVCOLUMN lvc;

    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.iSubItem = 0;
    lvc.pszText = (LPWSTR) Header.c_str();
    lvc.cx = width;
    lvc.fmt = LVCFMT_LEFT;

    ListView_InsertColumn(
        this->m_controlHandle,
        pos,
        &lvc);
}

METHOD LRESULT ListView::MessageLoopForwarder(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    ListView* listView = (ListView*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    return listView->MessageLoop(hwnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);
}

METHOD LRESULT ListView::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}
