#pragma once
#include <wx/window.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "../Tools/cppDatabase.hpp"

//Fenster-Klasse für MainFrame, bietet Platz für virtuelle Funktionen
class SubWindow : public wxPanel{ 

    public:
        SubWindow(wxFrame* parent, cppDatabase* DB);
        ~SubWindow();
        cppDatabase* db = NULL;
        wxStaticText* title;
    protected:
        wxString default_str = "---";
};