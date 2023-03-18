#pragma once
#include <regex>

#include <wx/frame.h>
#include <wx/button.h>
#include <wx/msgdlg.h>

#include "../Tools/SubWindow.hpp"


enum class b_lbl{
    Mindestalter, MaxStdProTag, MinUsernameLen, Anzahl_lbl
};
enum class b_tc{
    Mindestalter, MaxStdProTag, MinUsernameLen, Anzahl_tc
};
enum{
   ID_SaveSetting_btn 
};

class Benutzereinstellungen : public SubWindow{

    public:
        Benutzereinstellungen(wxFrame* parent, cppDatabase* DB,const wxString& this_title);
        ~Benutzereinstellungen();

    private:

        wxStaticText* lables[(int)b_lbl::Anzahl_lbl];
        wxTextCtrl* text_ctrls[(int)b_tc::Anzahl_tc];
        wxButton *btn_save;

        void on_save(wxCommandEvent& event);
        bool load_all_values();
        bool check_entries(std::string& str_min_age,std::string& str_max_hrs,std::string& str_usr_len);

        wxDECLARE_EVENT_TABLE();
};