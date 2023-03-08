#include "MainFrame.hpp"

class App: public wxApp{
    public:
        virtual bool OnInit();
};


wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(ID_Zeit, MainFrame::on_zeit)
    EVT_MENU(ID_Uebersicht, MainFrame::on_show_times)
    EVT_MENU(ID_Mitarbeiteranlage, MainFrame::on_new_employee)
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
    EVT_BUTTON(ID_Save_MA, Mitarbeiteranlage::on_save)
    EVT_TEXT(ID_SvNr, Mitarbeiteranlage::style_svnr_syntax)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(App);

bool App::OnInit(){

    //Eine Verbindung für alles
    cppDatabase* DB = new cppDatabase();
    MainFrame *frame = new MainFrame("[Programm-Name]", wxPoint(50, 50), wxSize(1100, 600), DB );

    frame->Show( true );
    return true;
}

