#include "Mitarbeiteranlage.hpp"


Mitarbeiteranlage::Mitarbeiteranlage(wxFrame* parent, cppDatabase* DB, const wxString& this_title):SubWindow(parent, DB, this_title){
    
    //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
    lables[(int)m_lbl::Vorname] = new wxStaticText(this,wxID_ANY,"     Vorname");
    lables[(int)m_lbl::Name] = new wxStaticText(this,wxID_ANY,"               Name");
    lables[(int)m_lbl::Benutzername] = new wxStaticText(this,wxID_ANY,"Benutzername");
    lables[(int)m_lbl::Strasse] = new wxStaticText(this,wxID_ANY,"        Strasse");
    lables[(int)m_lbl::Hausnummer] = new wxStaticText(this,wxID_ANY,"Hausnummer");
    lables[(int)m_lbl::PLZ] = new wxStaticText(this,wxID_ANY,"PLZ");
    lables[(int)m_lbl::Ort] = new wxStaticText(this,wxID_ANY,"Ort");
    lables[(int)m_lbl::Land] = new wxStaticText(this,wxID_ANY,"Land");
    lables[(int)m_lbl::SVNummer] = new wxStaticText(this,wxID_ANY,"SV-Nr");
    lables[(int)m_lbl::Anstellungsdatum] = new wxStaticText(this,wxID_ANY,"Anstellungsdatum");
    lables[(int)m_lbl::Geburtstag] = new wxStaticText(this,wxID_ANY,"Geburtstag");
    lables[(int)m_lbl::Tarif] = new wxStaticText(this,wxID_ANY,"Tarif");

    text_ctrls[(int)m_tc::Vorname] = new wxTextCtrl(this,wxID_ANY,default_str);
    text_ctrls[(int)m_tc::Name] = new wxTextCtrl(this,wxID_ANY,default_str);
    text_ctrls[(int)m_tc::Benutzername] = new wxTextCtrl(this,wxID_ANY,default_str);
    text_ctrls[(int)m_tc::Strasse] = new wxTextCtrl(this,wxID_ANY,default_str);
    text_ctrls[(int)m_tc::Hausnummer] = new wxTextCtrl(this,wxID_ANY,default_str);
    text_ctrls[(int)m_tc::PLZ] = new wxTextCtrl(this,wxID_ANY,default_str);
    text_ctrls[(int)m_tc::Ort] = new wxTextCtrl(this,wxID_ANY,default_str);
    text_ctrls[(int)m_tc::Land] = new wxTextCtrl(this,wxID_ANY,"Deutschland");
    text_ctrls[(int)m_tc::SVNummer] = new wxTextCtrl(this,wxID_ANY,default_str);
    for(wxTextCtrl* t : text_ctrls)t->SetInitialSize({150,30});


    start_date = new wxGtkCalendarCtrl(this,wxID_ANY,wxDefaultDateTime);
    birth_date = new wxGtkCalendarCtrl(this,wxID_ANY,wxDefaultDateTime);
    
    cbo_tarife = new wxComboBox(this, wxID_ANY);
    db->fill_combobox(cbo_tarife, "SELECT * FROM V_TG_Bezeichnungen");
    cbo_tarife->SetValue(default_str);
    cbo_tarife->SetEditable(false);

    btn_save = new wxButton(this,ID_Save_MA,"Mitarbeiter neu anlegen");


//Anordnung aller sichtbaren Elemente/////////////////////////////////////////////////////////////////////////////////
  wxSizerFlags szrflags(1);
    szrflags.Border(wxALL,5);
    szrflags.Right(); //Allignment
      
  //Sizer-Pointer werden innerhalb von SetSizer() freigegeben
  wxFlexGridSizer * title_szr = new wxFlexGridSizer(1,100,10);
    title_szr->Add(title);

  wxFlexGridSizer * szr = new wxFlexGridSizer(6,10,10);

    szr->Add(lables[(int)m_lbl::Vorname],szrflags);
    szr->Add(text_ctrls[(int)m_tc::Vorname],szrflags);

    szr->Add(lables[(int)m_lbl::Name],szrflags);
    szr->Add(text_ctrls[(int)m_tc::Name],szrflags);

    szr->Add(lables[(int)m_lbl::Benutzername],szrflags);
    szr->Add(text_ctrls[(int)m_tc::Benutzername],szrflags);


  wxFlexGridSizer * szr2 = new wxFlexGridSizer(6,10,10);

    szr2->Add(lables[(int)m_lbl::Strasse],szrflags);
    szr2->Add(text_ctrls[(int)m_tc::Strasse],szrflags);
    
    szr2->Add(lables[(int)m_lbl::Hausnummer],szrflags);
    szr2->Add(text_ctrls[(int)m_tc::Hausnummer],szrflags);

    szr2->Add(lables[(int)m_lbl::PLZ],szrflags);
    szr2->Add(text_ctrls[(int)m_tc::PLZ],szrflags);

    szr2->Add(lables[(int)m_lbl::Ort],szrflags);
    szr2->Add(text_ctrls[(int)m_tc::Ort],szrflags);

    szr2->Add(lables[(int)m_lbl::Land],szrflags);
    szr2->Add(text_ctrls[(int)m_tc::Land],szrflags);

    szr2->AddSpacer(1);
    szr2->AddSpacer(1);

    szr2->Add(lables[(int)m_lbl::SVNummer],szrflags);
    szr2->Add(text_ctrls[(int)m_tc::SVNummer],szrflags);

    szr2->Add(lables[(int)m_lbl::Tarif],szrflags);
    szr2->Add(cbo_tarife,szrflags);
  

  wxFlexGridSizer * szr3 = new wxFlexGridSizer(4,10,10);
    szr3->Add(lables[(int)m_lbl::Anstellungsdatum],szrflags);
    szr3->Add(start_date,szrflags);

    szr3->Add(lables[(int)m_lbl::Geburtstag],szrflags);
    szr3->Add(birth_date,szrflags);


  wxBoxSizer * szrButtons = new wxBoxSizer(wxVERTICAL);
    szrButtons->Add( btn_save,szrflags);
    

  wxBoxSizer * szrCRUDForm = new wxBoxSizer(wxVERTICAL);
    szrCRUDForm->Add(title_szr);
    szrCRUDForm->Add( szr);
    szrCRUDForm->Add( szr2);
    
    szrCRUDForm->Add( szr3);
    szrCRUDForm->Add( szrButtons );

  SetSizer(szrCRUDForm);

  SetSize(0,0,parent->m_width,parent->m_height);
}

void Mitarbeiteranlage::on_save(wxCommandEvent& event){
 
 //Prüfung Benutzername
  wxString strSQL = "call SP_CHECK_Benutzername('"+wxString::FromUTF8(text_ctrls[(int)m_tc::Benutzername]->GetValue().mb_str(wxConvUTF8))+"')";
  wxString check_user = db->get_string_from_db(strSQL.mb_str(wxConvUTF8));
  if(check_user != "0"){
    wxMessageBox( 
      wxString::FromUTF8("Daten können nicht gespeichert werden!\nBitte wählen Sie einen anderen Benutzernamen aus."),
      wxString::FromUTF8("Benutzername ist bereits vergeben!"),
      wxOK|wxICON_ERROR
    );
    return;
  }

  //Prüfung auf negative Altersangabe
  if(wxDateTime::Now().GetTicks() < birth_date->GetDate().GetTicks()){
    wxMessageBox( 
      "Altersangabe unplausibel!",
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
    );
    return;
  }

  //Prüfung Mindestalter
  int min_age = atoi(db->get_string_from_db("call SP_GET_Konfiguration('min_entry_age')").c_str());
  int this_age = (wxDateTime::Now().GetTicks() - birth_date->GetDate().GetTicks()) / (60*60*24*365.25);
  if(this_age < min_age){
    wxMessageBox( 
      wxString::FromUTF8("Das Mindesteintrittsalter beträgt ") + std::to_string(min_age) + " Jahre.\n"
      +wxString::FromUTF8(text_ctrls[(int)m_tc::Vorname]->GetValue().mb_str(wxConvUTF8)) 
      + " " + wxString::FromUTF8(text_ctrls[(int)m_tc::Name]->GetValue().mb_str(wxConvUTF8)) 
      + " ist noch zu jung.",
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
    );
    return;
  }

  //Datum formatieren
  wxDateTime vertragsstart = {
          start_date->GetDate().GetDay(), 
        start_date->GetDate().GetMonth(),
         start_date->GetDate().GetYear(),
         0,0,0,0
  };
  wxDateTime geburtstag = {
         birth_date->GetDate().GetDay(), 
        birth_date->GetDate().GetMonth(),
         birth_date->GetDate().GetYear(),
         0,0,0,0
  };
  wxString strVertragsstart = vertragsstart.Format(wxT("%d.%m.%Y"));  //Formatierung für wxMessagebox
  wxString strGeburtstag = geburtstag.Format(wxT("%d.%m.%Y"));        //Formatierung für wxMessagebox

  //Angaben vom Nutzer prüfen lassen
  int choice = wxMessageBox( 
  "Voller Name:\n"+wxString::FromUTF8(text_ctrls[(int)m_tc::Vorname]->GetValue().mb_str(wxConvUTF8))+ " "+wxString::FromUTF8(text_ctrls[(int)m_tc::Name]->GetValue().mb_str(wxConvUTF8)) 
  +"\n\nBenutzername:\n"+wxString::FromUTF8(text_ctrls[(int)m_tc::Benutzername]->GetValue().mb_str(wxConvUTF8))
  +"\n\nAdresse:\n"+wxString::FromUTF8(text_ctrls[(int)m_tc::Strasse]->GetValue().mb_str(wxConvUTF8))+ " "+wxString::FromUTF8(text_ctrls[(int)m_tc::Hausnummer]->GetValue().mb_str(wxConvUTF8))
  +"\n"+wxString::FromUTF8(text_ctrls[(int)m_tc::PLZ]->GetValue().mb_str(wxConvUTF8))+ " "+wxString::FromUTF8(text_ctrls[(int)m_tc::Ort]->GetValue().mb_str(wxConvUTF8))
  +"\n\nSozielversicherungsnummer:\n"+wxString::FromUTF8(text_ctrls[(int)m_tc::SVNummer]->GetValue().mb_str(wxConvUTF8))
  +"\n\nAnstellungsdatum:\n"+strVertragsstart
  +"\n\nGeburtstag:\n"+strGeburtstag,
  wxString::FromUTF8("Bitte prüfen Sie die Angaben!"),
  wxYES_NO|wxICON_ERROR
  );

  if(choice == 2){
    strVertragsstart = vertragsstart.Format(wxT("%y-%m-%d %H:%M:%S"));  //Formatierung für DB-Eintrag
    strGeburtstag = geburtstag.Format(wxT("%y-%m-%d %H:%M:%S"));        //Formatierung für DB-Eintrag
    //Mitarbeiter anlegen
    strSQL =  "call SP_INSERT_MA('"
    +wxString::FromUTF8(text_ctrls[(int)m_tc::Vorname]->GetValue().mb_str(wxConvUTF8))+"','"
    +wxString::FromUTF8(text_ctrls[(int)m_tc::Name]->GetValue().mb_str(wxConvUTF8))+"','"
    +wxString::FromUTF8(text_ctrls[(int)m_tc::Benutzername]->GetValue().mb_str(wxConvUTF8))+"','"
    +wxString::FromUTF8(text_ctrls[(int)m_tc::Strasse]->GetValue().mb_str(wxConvUTF8))+"','"
    +wxString::FromUTF8(text_ctrls[(int)m_tc::Hausnummer]->GetValue().mb_str(wxConvUTF8))+"','"
    +wxString::FromUTF8(text_ctrls[(int)m_tc::PLZ]->GetValue().mb_str(wxConvUTF8))+"','"
    +wxString::FromUTF8(text_ctrls[(int)m_tc::Ort]->GetValue().mb_str(wxConvUTF8))+"','"
    +wxString::FromUTF8(text_ctrls[(int)m_tc::Land]->GetValue().mb_str(wxConvUTF8))+"','"
    +wxString::FromUTF8(text_ctrls[(int)m_tc::SVNummer]->GetValue().mb_str(wxConvUTF8))+"','"
    +wxString::FromUTF8(cbo_tarife->GetValue().mb_str(wxConvUTF8))+"','"
    +strVertragsstart.ToStdString()+"','"
    +strGeburtstag.ToStdString()
    +"')";

    bool fail = db->execute_SQL(strSQL.mb_str(wxConvUTF8));    //Hier sollte kein Fehler passieren
    if(fail){
      wxMessageBox( 
        wxString::FromUTF8("Bitten wenden Sie sich an die Administration!"),
        wxString::FromUTF8("Fehler beim Speichervorgang!"),
        wxOK|wxICON_ERROR
      );
      return;
    }
    
    wxMessageBox( 
      wxString::FromUTF8(text_ctrls[(int)m_tc::Vorname]->GetValue().mb_str(wxConvUTF8)) + " "+
      wxString::FromUTF8(text_ctrls[(int)m_tc::Name]->GetValue().mb_str(wxConvUTF8)) + " wurde angelegt.",
      wxString::FromUTF8("Speichervorgang erfolgreich."),
      wxOK|wxICON_INFORMATION
    );
  }
}

Mitarbeiteranlage::~Mitarbeiteranlage(){
  for(wxStaticText* st : lables)delete st;
  for(wxTextCtrl* tc : text_ctrls)delete tc;
  delete start_date;
  delete birth_date;
  delete cbo_tarife;
  delete btn_save;
}


