#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/calctrl.h>
#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include "../Tools/SubWindow.hpp"


enum class m_lbl{
    Vorname, Name, Benutzername,Strasse, Hausnummer, PLZ, Ort, Land, SVNummer, Anstellungsdatum, Geburtstag,Tarif, Anzahl_lbl
};

enum class m_tc{
    Vorname, Name, Benutzername, Strasse, Hausnummer, PLZ, Ort, Land, SVNummer, Anzahl_tc
};

enum{
   ID_Save_MA
};


class Mitarbeiteranlage : public SubWindow{ 

  public:
    Mitarbeiteranlage(wxFrame* parent, cppDatabase* DB, const wxString& this_title);
   ~Mitarbeiteranlage(); 
  private:
    wxStaticText* lables[(int)m_lbl::Anzahl_lbl];
    wxTextCtrl* text_ctrls[(int)m_tc::Anzahl_tc];
    wxGtkCalendarCtrl* start_date;
    wxGtkCalendarCtrl* birth_date;
    wxComboBox* cbo_tarife;
    wxButton* btn_save;
   
     

    void on_save(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};
