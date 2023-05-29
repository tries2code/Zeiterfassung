#pragma once
#include <wx/button.h>
#include <wx/timectrl.h>
#include <wx/datectrl.h>
#include <wx/msgdlg.h>
#include <wx/combobox.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "../Tools/SubWindow.hpp"


enum class a_lbl{
    Mitabeiter,Typ_Abwesenheit,Startdatum,Enddatum,Bemerkungen,Anzahl_lbl
};
enum{
    ID_Abwesenheit_speichern 
};

class Abwesenheiten : public SubWindow{ 

    public:
        Abwesenheiten(wxFrame* parent, cppDatabase* DB, const wxString& this_title);
      // ~Abwesenheiten();
    private:

        wxComboBox* cbo_benutzer;
        wxComboBox* cbo_typ_abwesenheit;

        wxDatePickerCtrl* start_date;
        wxDatePickerCtrl* end_date;

        wxTextCtrl* tc_Bemerkungen;
        wxButton* btn_submit;

        wxStaticText* lables[(int)a_lbl::Anzahl_lbl];


        void on_submit(wxCommandEvent& event);
        wxDECLARE_EVENT_TABLE();
};
