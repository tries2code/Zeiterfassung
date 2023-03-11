#pragma once

#include <wx/frame.h>
#include <wx/datectrl.h>

#include "../Tools/SubWindow.hpp"

class Verwaltung : public SubWindow{ 

    public:
        Verwaltung(wxFrame* parent, cppDatabase* DB,const wxString& this_title);
        ~Verwaltung();

    private:

        wxComboBox* cbo_benutzer;

        wxDatePickerCtrl* date;

        wxDECLARE_EVENT_TABLE();
};