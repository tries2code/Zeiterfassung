#pragma once
#include <regex>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/msgdlg.h>


#include "../Tools/SubWindow.hpp"



enum class v_lbl{
    Vorname, Name, Strasse, Hausnummer, PLZ, Ort, Land, Benutzer, Tarif, Anzahl_lbl
};
//Beide enums müssen bis 'Land' identisch sein(siehe Prüfung)
enum class v_tc{
    Vorname, Name, Strasse, Hausnummer, PLZ, Ort, Land, Anzahl_tc
};

enum{
   ID_Update_btn,ID_ResetPW_btn, ID_Unload_btn, ID_cbo
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

        wxButton *btn_update;
        wxButton *btn_reset_pwd;
        wxButton *btn_unload_empl;

        void on_update(wxCommandEvent& event);
        void on_reset_pw(wxCommandEvent& event);
        void on_unload(wxCommandEvent& event);
        bool check_entries();

        wxDECLARE_EVENT_TABLE();
};

        

