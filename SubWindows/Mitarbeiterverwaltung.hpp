#pragma once

#include <wx/frame.h>
#include <wx/button.h>
#include <wx/msgdlg.h>


#include "../Tools/SubWindow.hpp"



enum class v_lbl{
    Vorname, Name, Strasse, Hausnummer, PLZ, Ort, Land, Tarif, Anzahl_lbl
};

enum class v_tc{
    Vorname, Name, Strasse, Hausnummer, PLZ, Ort, Land, Anzahl_tc
};

enum{
   ID_Update_btn, ID_cbo
};


class Verwaltung : public SubWindow{ 

    public:
        Verwaltung(wxFrame* parent, cppDatabase* DB,const wxString& this_title);
        ~Verwaltung();
        void on_change_usr(wxCommandEvent& event);

    private:
        wxComboBox* cbo_benutzer;
        wxStaticText* lables[(int)v_lbl::Anzahl_lbl];
        wxTextCtrl* text_ctrls[(int)v_tc::Anzahl_tc];
       
        wxComboBox* cbo_tarife;

        wxButton* btn_update;

        wxDECLARE_EVENT_TABLE();
};

        

