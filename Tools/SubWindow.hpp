#pragma once
#include <wx/window.h>
#include "../Tools/cppDatabase.hpp"

//Fenster-Klasse für MainFrame, bietet Platz für virtuelle Funktionen
class SubWindow : public wxPanel{ 

    public:
        SubWindow(wxFrame* parent, cppDatabase* DB);
        cppDatabase* db = NULL;
    protected:
        wxString default_str = "---";
};