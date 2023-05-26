#pragma once
#include <regex>

#include <wx/frame.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/datectrl.h>
#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include "../Tools/SubWindow.hpp"




enum class t_lbl{
    Bezeichnung, Wochenstunden, Wochentage, Stundenlohn, Auswahl_Tarif, str_Tarif, Anzahl_lbl
};

enum class t_tc{
    Bezeichnung, Wochenstunden, Wochentage, Stundenlohn, Anzahl_tc
};

enum{
   ID_New_Tarif_btn, ID_Save_Tarif_btn,ID_Delete_Tarif_btn, ID_cbo_tarif_edit
};


class Tarife : public SubWindow{ 

  public:
    Tarife(wxFrame* parent, cppDatabase* DB, const wxString& this_title);
   ~Tarife(); 
  private:
    wxStaticText* lables[(int)t_lbl::Anzahl_lbl];
    wxTextCtrl* text_ctrls[(int)t_tc::Anzahl_tc];
    
    wxComboBox* cbo_tarife;
    wxButton* btn_new_tarif;
    wxButton* btn_save_tarif;
    wxButton* btn_delete_tarif;

    void on_change_tarif(wxCommandEvent& event);
    void on_new(wxCommandEvent& event);
    void on_save(wxCommandEvent& event);
    void on_delete(wxCommandEvent& event);
    bool check_entries();
    
    
    wxDECLARE_EVENT_TABLE();
};
