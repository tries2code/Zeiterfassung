#pragma once
#include <regex>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/datectrl.h>


#include "../Tools/SubWindow.hpp"



enum class v_lbl{
    Vorname, Name, Strasse, Hausnummer, PLZ, Ort, Land, Benutzer, Tarif, Tarifdatum, Anzahl_lbl
};
//Beide enums müssen bis 'Land' identisch sein(siehe Prüfung)
enum class v_tc{
    Vorname, Name, Strasse, Hausnummer, PLZ, Ort, Land, Anzahl_tc
};

enum{
   ID_Update_btn,ID_ResetPW_btn, ID_Unload_btn, ID_cbo_usr, ID_cbo_tarif
};


class Verwaltung : public SubWindow{ 

    public:
        Verwaltung(wxFrame* parent, cppDatabase* DB,const wxString& this_title);
        ~Verwaltung();

    private:
        wxComboBox* cbo_benutzer;
        wxStaticText* lables[(int)v_lbl::Anzahl_lbl];
        wxTextCtrl* text_ctrls[(int)v_tc::Anzahl_tc];
       
        wxComboBox* cbo_tarife;
        wxDatePickerCtrl* tarif_date;

        wxButton *btn_update;
        wxButton *btn_reset_pwd;
        wxButton *btn_unload_empl;

        bool tarif_changed;

        void on_change_usr(wxCommandEvent& event);
        void on_change_tarif(wxCommandEvent& event);
        void on_update(wxCommandEvent& event);
        void on_reset_pw(wxCommandEvent& event);
        void on_unload(wxCommandEvent& event);
        bool check_entries();

        wxDECLARE_EVENT_TABLE();
};

        

