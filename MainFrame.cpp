#include "MainFrame.hpp"
#include "SubWindows/Uebersicht.hpp"


using time_point = std::chrono::system_clock::time_point;
wxString serializeTimePoint( const time_point& time, const std::string& format);

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, cppDatabase* DB)
          :db(DB),wxFrame(NULL, wxID_ANY, title, pos, size){


    SetBackgroundColour(wxColour(255,136,0,50));
    wxString app_Icon = _T("Resources/app_Icon.png");
    SetIcon(app_Icon);


    wxMenu *menu_Zeit = new wxMenu;
    menu_Zeit->Append(ID_Zeit, "&Erfassen \tCtrl-E", "Hier ist die Zeiterfassung.");
    menu_Zeit->AppendSeparator();
    menu_Zeit->Append(ID_Uebersicht, wxString::FromUTF8("&Übersicht \tCtrl-U"), "Hier ist die Auflistung.");
    menu_Zeit->AppendSeparator();
    menu_Zeit->Append(wxID_EXIT, "&Beenden \tCtrl-Q", "Programm beenden.");

    wxMenu *menu_Administration = new wxMenu;
    menu_Administration->Append(ID_Mitarbeiteranlage,"&Anlegen \tCtrl-M", wxString::FromUTF8("Neue Mitarbeiter anlegen."));
    wxMenu *menu_Hilfe = new wxMenu;
    menu_Hilfe->Append(wxID_ABOUT,"&Hilfe \tCtrl-H", wxString::FromUTF8("Nützliche Informationen."));
    
    wxMenuBar *menu_Bar = new wxMenuBar;
    menu_Bar->Append( menu_Zeit, "&Zeit");
    menu_Bar->Append( menu_Administration, "&Mitarbeiter");
    menu_Bar->Append( menu_Hilfe, "&Hilfe");
    menu_Bar->SetBackgroundColour(wxColour(255,136,0,75));
    SetMenuBar(menu_Bar);

    CreateStatusBar();
    time_point input = std::chrono::system_clock::now();
    SetStatusText( serializeTimePoint(input, "Angemeldet seit: %H:%M:%S Uhr am %d.%m.%Y"));
    
    //Verhindert das man die Fenstergröße ändern kann(entfernt das Viereck-Icon aus der control-bar)
    this->SetSizeHints(this->m_width,this->m_height,this->m_width,this->m_height);
    //Versteckt das Reszing-Symbol unten rechts im wxFrame
    this->SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER);
    //wxDEFAULT_FRAME_STYLE besteht aus folgenden Styles wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN. 
    // & ~ ist(sind?) Bitwise-Operator(s) aus C. In diesem Fall deaktivieren sie wxRESIZE_BORDER in wxDEFAULT_FRAME_STYLE
}

void MainFrame::OnExit(wxCommandEvent& event){
    Close( true );
}

void MainFrame::OnAbout(wxCommandEvent& event){
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
                  wxLogMessage("Hello world from wxWidgets!");
}

void MainFrame::on_zeit(wxCommandEvent& event){
    DestroyChildren();
    Subwindows[(int)sub::Zeiterfassung] = new Zeiterfassung(this,db);
}

void MainFrame::on_show_times(wxCommandEvent& event){
    DestroyChildren();
    Subwindows[(int)sub::Uebersicht] = new Uebersicht(this,db);
}

void MainFrame::on_new_employee(wxCommandEvent& event){
    DestroyChildren();
    Subwindows[(int)sub::Mitarbeiteranlage] = new Mitarbeiteranlage(this,db);
}


///////////////Hilfsfunktionen///////////////////////

wxString serializeTimePoint( const time_point& time, const std::string& format)
{
    std::time_t tt = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::localtime(&tt); //Locale time-zone, usually UTC(std::::gmtime()) by default.
    std::stringstream ss;
    ss << std::put_time( &tm, format.c_str() );
    return (wxString)ss.str();
}
