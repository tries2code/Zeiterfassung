#pragma once
#include <cstddef>
#include <chrono>
#include <ctime> 
#include <iomanip>
#include <sstream>

#include <wx/colour.h>
#include <wx/gdicmn.h>
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/wxprec.h>
#include <wx/string.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>

#include "Tools/SubWindow.hpp"
#include "SubWindows/Zeiterfassung.hpp"
#include "SubWindows/Uebersicht.hpp"
#include "SubWindows/Mitarbeiteranlage.hpp"




enum class sub{
    Zeiterfassung,Uebersicht,Mitarbeiteranlage, Summe_aller_fenster
};

//Für wxWidgets muss(?) es ein einfaches Enum sein
enum{
    ID_Zeit =1, ID_Uebersicht, ID_Mitarbeiteranlage
};

class MainFrame: public wxFrame{

    public:

        MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, cppDatabase* DB);
    
    private:

        SubWindow* Subwindows[(int)sub::Summe_aller_fenster];
        cppDatabase* db;
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void on_zeit(wxCommandEvent& event);
        void on_show_times(wxCommandEvent& event);
        void on_new_employee(wxCommandEvent& event);
        wxDECLARE_EVENT_TABLE();
};
