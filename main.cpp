#include "MainFrame.hpp"
#include "SubWindows/Mitarbeiterverwaltung.hpp"
#include <iostream>
#include <string>


#ifdef _DEBUG
    #define dbg_mode true
#else
    #define dbg_mode false
#endif

class App: public wxApp{
    public:
        virtual bool OnInit();
};


wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(ID_Zeiterfassung, MainFrame::on_zeit)
    EVT_MENU(ID_Uebersicht, MainFrame::on_show_times)
    EVT_MENU(ID_Mitarbeiteranlage, MainFrame::on_new_employee)
    EVT_MENU(ID_Verwaltung, MainFrame::on_edit_employee)
wxEND_EVENT_TABLE()

//Event Table muss für jedes Subfenster erstellt werden
wxBEGIN_EVENT_TABLE(Zeiterfassung, SubWindow)
    EVT_BUTTON(ID_Submit, Zeiterfassung::on_submit)
    EVT_BUTTON(ID_Toggle_end_date, Zeiterfassung::on_toggle)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Uebersicht, SubWindow)
    EVT_BUTTON(ID_Show_Times, Uebersicht::on_show_times)
    EVT_TEXT(ID_USR_cbo, Uebersicht::on_change_usr)
    EVT_TEXT_ENTER(ID_Year_cbo,Uebersicht::on_enter_year)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Mitarbeiteranlage, SubWindow)
    EVT_BUTTON(ID_Save_btn, Mitarbeiteranlage::on_save)
    EVT_TEXT(ID_SvNr, Mitarbeiteranlage::style_svnr_syntax)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Verwaltung, SubWindow)
    EVT_TEXT(ID_cbo, Verwaltung::on_change_usr)
    EVT_BUTTON(ID_Update_btn, Verwaltung::on_update)
    EVT_BUTTON(ID_ResetPW_btn, Verwaltung::on_reset_pw)
    EVT_BUTTON(ID_Unload_btn, Verwaltung::on_unload)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(App);

bool App::OnInit(){

    //Eine Verbindung für alles
    cppDatabase* DB = new cppDatabase();

    std::string str_mode = (dbg_mode)?"Debug_Mode":"Release_Mode";
    MainFrame *frame = new MainFrame("[Programm-Name]-"+str_mode, wxPoint(50, 50), wxSize(1100, 600), DB );

    frame->Show( true );
    return true;
}

