#include "Mitarbeiteranlage.hpp"

Mitarbeiteranlage::Mitarbeiteranlage(wxFrame* parent, cppDatabase* DB, const wxString& this_title)
:SubWindow(parent, DB, this_title){


    //Setzt das Datumsformat von wxDatePickerCtrl
    wxLocale l{wxLANGUAGE_GERMAN_GERMANY,wxLOCALE_DONT_LOAD_DEFAULT};
    
    //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
    lables[(int)m_lbl::Vorname] = new wxStaticText(this,wxID_ANY,"     Vorname");
    lables[(int)m_lbl::Name] = new wxStaticText(this,wxID_ANY,"        Name ℹ️");
    lables[(int)m_lbl::Benutzername] = new wxStaticText(this,wxID_ANY,"Benutzername ℹ️");
    lables[(int)m_lbl::Strasse] = new wxStaticText(this,wxID_ANY,"        Strasse");
    lables[(int)m_lbl::Hausnummer] = new wxStaticText(this,wxID_ANY,"Hausnummer");
    lables[(int)m_lbl::PLZ] = new wxStaticText(this,wxID_ANY,"             PLZ");
    lables[(int)m_lbl::Ort] = new wxStaticText(this,wxID_ANY,"Ort");
    lables[(int)m_lbl::Land] = new wxStaticText(this,wxID_ANY,"Land");
    lables[(int)m_lbl::SVNummer] = new wxStaticText(this,wxID_ANY,"SV-Nr");
    lables[(int)m_lbl::Anstellungsdatum] = new wxStaticText(this,wxID_ANY,"Anstellungsdatum");
    lables[(int)m_lbl::Geburtstag] = new wxStaticText(this,wxID_ANY,"Geburtstag");
    lables[(int)m_lbl::Tarif] = new wxStaticText(this,wxID_ANY,"Tarif");

    text_ctrls[(int)m_tc::Vorname] = new wxTextCtrl(this,wxID_ANY);
    text_ctrls[(int)m_tc::Name] = new wxTextCtrl(this,wxID_ANY);
    text_ctrls[(int)m_tc::Benutzername] = new wxTextCtrl(this,wxID_ANY);
    text_ctrls[(int)m_tc::Strasse] = new wxTextCtrl(this,wxID_ANY);
    text_ctrls[(int)m_tc::Hausnummer] = new wxTextCtrl(this,wxID_ANY);
    text_ctrls[(int)m_tc::PLZ] = new wxTextCtrl(this,wxID_ANY);
    text_ctrls[(int)m_tc::Ort] = new wxTextCtrl(this,wxID_ANY);
    text_ctrls[(int)m_tc::Land] = new wxTextCtrl(this,wxID_ANY,"Deutschland");
    text_ctrls[(int)m_tc::SVNummer] = new wxTextCtrl(this,ID_SvNr);
    for(wxTextCtrl* t : text_ctrls)t->SetInitialSize({150,30});


    start_date = new wxDatePickerCtrl(this,wxID_ANY,wxDefaultDateTime);
    birth_date = new wxDatePickerCtrl(this,wxID_ANY,wxDefaultDateTime);
    
    cbo_tarife = new wxComboBox(this, wxID_ANY);
    db->fill_combobox(cbo_tarife, "SELECT * FROM V_TG_Bezeichnungen");
    cbo_tarife->SetValue(default_str);
    cbo_tarife->SetEditable(false);

    btn_save = new wxButton(this,ID_Save_btn,"Mitarbeiter neu anlegen");

//Tooltips//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    lables[(int)m_lbl::Name]->SetToolTip("Beginnend wie im Sozialversicherungs­ausweis vermerkt!");
    text_ctrls[(int)m_tc::Name]->SetToolTip("Beginnend wie im Sozialversicherungs­ausweis vermerkt!");

    lables[(int)m_lbl::Benutzername]->SetToolTip("Kann nicht mehr verändert werden.");
    text_ctrls[(int)m_tc::Benutzername]->SetToolTip("Kann nicht mehr verändert werden.");


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
  try{
    //Eingaben prüfen
    if(!check_entries())return;

    //Datum formatieren
    wxString strVertragsstart = start_date->GetValue().Format(wxT("%d.%m.%Y"));  //Formatierung für wxMessagebox
    wxString strGeburtstag = birth_date->GetValue().Format(wxT("%d.%m.%Y"));        //Formatierung für wxMessagebox

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

    //Mitarbeiter anlegen
    if(choice == 2){
      strVertragsstart = start_date->GetValue().Format(wxT("%Y-%m-%d"));  //Formatierung für DB-Eintrag
      strGeburtstag = birth_date->GetValue().Format(wxT("%Y-%m-%d"));        //Formatierung für DB-Eintrag
      wxString strSQL =  "call SP_INSERT_MA('"
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
          wxString::FromUTF8("Bitte wenden Sie sich an die Administration!"),
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
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Mitarbeiteranlage::on_save: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Mitarbeiteranlage::on_save");
  }
}

bool Mitarbeiteranlage::check_entries(){
  try{
    //Prüfung ob alle Felder ausgefüllt sind
    for(int i = 0; i < (int)m_tc::Anzahl_tc; ++i){
      if(wxString::FromUTF8(text_ctrls[i]->GetValue().mb_str(wxConvUTF8)).length() == 0){
        wxMessageBox( 
        wxString::FromUTF8("Daten können nicht gespeichert werden!\nDas Feld [") 
        + wxString::FromUTF8(lables[i]->GetLabelText().mb_str(wxConvUTF8)).Trim(false)
        + wxString::FromUTF8("] enthält keinen Wert. "),
        wxString::FromUTF8("Angaben unvollständig!"),
        wxOK|wxICON_ERROR
      );
      return false;
      }
    }

    //Prüfung ob Tarif ausgewählt wurde
    if(cbo_tarife->GetValue() == default_str){
        wxMessageBox( 
        wxString::FromUTF8("Daten können nicht gespeichert werden!\nEs wurde kein [Tarif] ausgewählt."),
        wxString::FromUTF8("Angaben unvollständig!"),
        wxOK|wxICON_ERROR
      );
      return false;
    }

    //Prüfung Benutzername Mindestlänge
    int min_usrname_len = atoi(db->get_string_from_db("call SP_GET_Konfiguration('min_usrname_len')").c_str());
    int usrname_len = text_ctrls[(int)m_tc::Benutzername]->GetValue().length();
    if(usrname_len < min_usrname_len){
      wxMessageBox( 
        wxString::FromUTF8("Daten können nicht gespeichert werden!\nBitte wählen Sie einen anderen Benutzernamen mit mindestens ")
        + std::to_string(min_usrname_len) + " Zeichen.",
        wxString::FromUTF8("Benutzername ist zu kurz!"),
        wxOK|wxICON_ERROR
      );
      return false;
    }

    //Prüfung ob Benutzername bereits vergeben
    wxString strSQL = "call SP_CHECK_Benutzername('"+wxString::FromUTF8(text_ctrls[(int)m_tc::Benutzername]->GetValue().mb_str(wxConvUTF8))+"')";
    wxString check_user = db->get_string_from_db(strSQL.mb_str(wxConvUTF8));
    if(check_user != "0"){
      wxMessageBox( 
        wxString::FromUTF8("Daten können nicht gespeichert werden!\nBitte wählen Sie einen anderen Benutzernamen aus."),
        wxString::FromUTF8("Benutzername ist bereits vergeben!"),
        wxOK|wxICON_ERROR
      );
      return false;
    }

    //Prüfung auf negative Altersangabe(Wichtig, da sonst bei zu großem Negativ-Wert die nächste Prüfung versagt)
    if(wxDateTime::Now().GetTicks() < birth_date->GetValue().GetTicks()){
      wxMessageBox( 
        "Altersangabe unplausibel!",
        wxString::FromUTF8("Speichern nicht möglich!"),
        wxOK|wxICON_ERROR
      );
      return false;
    }

    //Prüfung PLZ
    bool five_digits= std::regex_match(text_ctrls[(int)m_tc::PLZ]->GetValue().ToStdString()
                                      ,std::regex("^\\d{5}$"));
    if(!five_digits){
      wxMessageBox( 
      wxString::FromUTF8("Bitte überprüfen Sie die Postleitzahl."),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }

    //Prüfung Mindestalter
    int min_age = atoi(db->get_string_from_db("call SP_GET_Konfiguration('min_entry_age')").c_str());
    int this_age = (wxDateTime::Now().GetTicks() - birth_date->GetValue().GetTicks()) / (60*60*24*365.25);
    if(this_age < min_age){
      wxMessageBox( 
        wxString::FromUTF8("Das Mindesteintrittsalter beträgt ") + std::to_string(min_age) + " Jahre.\n"
        +wxString::FromUTF8(text_ctrls[(int)m_tc::Vorname]->GetValue().mb_str(wxConvUTF8)) 
        + " " + wxString::FromUTF8(text_ctrls[(int)m_tc::Name]->GetValue().mb_str(wxConvUTF8)) 
        + " ist noch zu jung.",
        wxString::FromUTF8("Speichern nicht möglich!"),
        wxOK|wxICON_ERROR
      );
      return false;
    }

    //Prüfungen SV-Nummer
    //SV Länge
    if(text_ctrls[(int)m_tc::SVNummer]->GetValue().length()<15){
      wxMessageBox( 
      wxString::FromUTF8("Die SV-Nummer ist zur kurz."),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }
    //SV auf Umlaute prüfen
    wxString str_sv = wxString::FromUTF8(text_ctrls[(int)m_tc::SVNummer]->GetValue().mb_str(wxConvUTF8));
    std::regex umlaut_regex("[äöüß]");
    bool hat_umlaute = std::regex_search(str_sv.utf8_string(), umlaut_regex);
    if(hat_umlaute){
      wxMessageBox( 
      wxString::FromUTF8("SV-Nummern enthalten keine Umlaute."),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }
    //SV Format
    bool sv_format = std::regex_match(str_sv.ToStdString(),std::regex("^\\d{2}-\\d{6}-[a-zA-Z]-\\d{3}$"));
    if(!sv_format){
      //Wert muss komplett neu eingegeben werden, wenn die Bindestriche an den vorgegebenen Stellen nicht vorhanden sind
      bool keep_str = (str_sv[2] == '-' && str_sv[9] == '-' && str_sv[11] == '-');
      std::string msg = "Die SV-Nummer hat das falsche Format.";
      if(!keep_str){
        msg = msg + "\nBitte erneut eingeben.";
        text_ctrls[(int)m_tc::SVNummer]->Clear();
      }
      wxMessageBox( 
      wxString::FromUTF8(msg),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }
    //SV-RV Bereich
    wxString str_rvbr_nr = text_ctrls[(int)m_tc::SVNummer]->GetValue().SubString(0,1);
    std::string list_rebr_nr = db->get_string_from_db("call SP_GET_Konfiguration('list_rvbr_nr')");
    int index = list_rebr_nr.find(str_rvbr_nr);
    if(index<0){
      wxMessageBox( 
      wxString::FromUTF8("Die SV-Nummer enthält keine gültige RV-Bereichsnummer."),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }
    //SV-Geburtstag
    wxString str_sv_birthday = text_ctrls[(int)m_tc::SVNummer]->GetValue().SubString(3,8);
    wxString str_birthday = birth_date->GetValue().Format(wxT("%d%m%y"));
    if(str_sv_birthday != str_birthday){
      wxMessageBox( 
      wxString::FromUTF8("Die SV-Nummer enthält ein abweichendes Geburtsdatum."),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }
    //SV-Erster Buchstabe des Nachnamens, Umlaute werden aufgelösst
    std::string str_sv_1st_c_name = text_ctrls[(int)m_tc::SVNummer]->GetValue().SubString(10,10).ToStdString();
    wxString str_1st_c_name = wxString::FromUTF8(text_ctrls[(int)m_tc::Name]->GetValue().mb_str(wxConvUTF8))[0];
    if(str_1st_c_name.utf8_string() == "ä" || str_1st_c_name.utf8_string() == "Ä")str_1st_c_name = "a";
    if(str_1st_c_name.utf8_string() == "ö" || str_1st_c_name.utf8_string() == "Ö")str_1st_c_name = "o";
    if(str_1st_c_name.utf8_string() == "ü" || str_1st_c_name.utf8_string() == "Ü")str_1st_c_name = "u";
    if(std::tolower(str_sv_1st_c_name[0]) != std::tolower(str_1st_c_name[0])){
      wxMessageBox( 
      wxString::FromUTF8("Der Buchstabe der SV-Nummer passt nicht zum Anfangbuchstaben des Names."),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }
    //SV-Mänlich/weiblich /Prüfziffer
    wxString str_rest = text_ctrls[(int)m_tc::SVNummer]->GetValue().SubString(12,14);
    bool sv_digits_only = std::regex_match(str_rest.ToStdString(), std::regex("^\\d{3}$"));
    if(!sv_digits_only){
      wxMessageBox( 
      wxString::FromUTF8("Die letzten drei Ziffern der SV-Nummer dürfen nur Zahlen sein."),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }

    return true;
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Mitarbeiteranlage::check_entries: " +  (std::string)e.what());
    return false;
  }
  catch(...){
    LOG::log_msg("FEHLER in Mitarbeiteranlage::check_entries");
    return false;
  }
}

void Mitarbeiteranlage::style_svnr_syntax(wxCommandEvent& event){
  //Setzt Bindestriche beim eintragen der SV-Nummer
  try{
    int cur_pos = text_ctrls[(int)m_tc::SVNummer]->GetInsertionPoint();
    int len = text_ctrls[(int)m_tc::SVNummer]->GetValue().length();
    //Bindestriche werden nur bei fortlaufender Eingabe hinzugefügt, Korrekturen mitten im String sind möglich
    if(cur_pos == len){
      switch(text_ctrls[(int)m_tc::SVNummer]->GetValue().length()){
        case 2: case 9: case 11:
          text_ctrls[(int)m_tc::SVNummer]->SetValue(text_ctrls[(int)m_tc::SVNummer]->GetValue() + "-");
          text_ctrls[(int)m_tc::SVNummer]->SetInsertionPoint(text_ctrls[(int)m_tc::SVNummer]->GetValue().length());
        break;
        case 16:
          text_ctrls[(int)m_tc::SVNummer]->SetValue(text_ctrls[(int)m_tc::SVNummer]->GetValue().substr(0,15));
          text_ctrls[(int)m_tc::SVNummer]->SetInsertionPoint(text_ctrls[(int)m_tc::SVNummer]->GetValue().length());
        break;
      }
    }
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Mitarbeiteranlage::check_svnr_syntax: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Mitarbeiteranlage::check_svnr_syntax");
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


