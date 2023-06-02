#pragma once
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/grid.h>
#include <wx/event.h>
#include <wx/stattext.h>

#include "../Tools/SubWindow.hpp"
#include "wx/gtk/combobox.h"

enum class u_cbo{
    Benutzer,Monat,Jahr,ZeitTyp,Anzahl_cbo
};
enum class u_lbl{
    Benutzer,Monat,Jahr,Stunden,ZeitTyp, Anzahl_lbl
};
enum{
   ID_Show_Times, ID_USR_cbo, ID_Year_cbo, ID_ZTyp_cbo
};

class Uebersicht : public SubWindow{ 

  public:
    Uebersicht(wxFrame* parent, cppDatabase* DB,const wxString& this_title);
    ~Uebersicht();
    void on_change_usr(wxCommandEvent& event);

  private:
    
    wxComboBox* combo_boxen[(int)u_cbo::Anzahl_cbo];
    wxStaticText* lables[(int)u_lbl::Anzahl_lbl];
    wxGrid* grd_zeiten;
    wxButton* btn_show_times;
    wxString tt_Jahr = wxString::FromUTF8("Es muss zunächst ein Benutzer ausgewählt sein.");
     
    void on_show_times(wxCommandEvent& event);
    void on_enter_year(wxCommandEvent& event);

    bool set_up_combos();
    bool set_up_grid();
    void get_Arbeitszeiten(wxString& strFromMonth,wxString& strTillMonth);
    void get_Urlaubszeiten(wxString& strFromMonth,wxString& strTillMonth);
    void get_Krankheitszeiten(wxString& strFromMonth,wxString& strTillMonth);
    void get_Sonstigezeiten(wxString& strFromMonth,wxString& strTillMonth);

    wxDECLARE_EVENT_TABLE();
};
