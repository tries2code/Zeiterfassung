#pragma once
#include <wx/button.h>
#include <wx/timectrl.h>
#include <wx/calctrl.h>
#include <wx/msgdlg.h>
#include <wx/combobox.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "../Tools/SubWindow.hpp"


enum class z_lbl{
    Mitabeiter,Startzeit,Endzeit,Startdatum,Enddatum,Anzahl_lbl
};
enum{
    ID_Submit, ID_Toggle_end_date 
};

class Zeiterfassung : public SubWindow{ 

    public:
        Zeiterfassung(wxFrame* parent, cppDatabase* DB);
       ~Zeiterfassung();
    private:

        wxComboBox* cbo_benutzer;

        wxGtkCalendarCtrl* start_date;
        wxTimePickerCtrl* start_time;

        wxGtkCalendarCtrl* end_date;
        bool show_end_date = false;
        wxTimePickerCtrl* end_time;

        wxButton* btn_submit;
        wxButton* btn_toggle_end_date;

        wxStaticText* lables[(int)z_lbl::Anzahl_lbl];

        const wxString str_lbl_no_ed = wxString::FromUTF8("End-Datum ändern");
        const wxString str_lbl_with_ed = "End-Datum ausblenden";

        void on_submit(wxCommandEvent& event);
        void on_toggle(wxCommandEvent& event);
        void toggle();
        wxDECLARE_EVENT_TABLE();
};
