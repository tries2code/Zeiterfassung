#include "Mitarbeiterverwaltung.hpp"

Verwaltung::Verwaltung(wxFrame* parent, cppDatabase* DB, const wxString& this_title):SubWindow(parent, DB,this_title){

  tarif_changed = false;
  //Setzt das Datumsformat von wxDatePickerCtrl
  wxLocale l{wxLANGUAGE_GERMAN_GERMANY,wxLOCALE_DONT_LOAD_DEFAULT};

  //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
  cbo_benutzer = new wxComboBox(this, ID_cbo_usr);
  db->fill_combobox(cbo_benutzer, "SELECT * FROM V_MA_Benutzernamen");
  cbo_benutzer->AutoComplete(cbo_benutzer->GetStrings());
  cbo_benutzer->SetValue(default_str);

  lables[(int)v_lbl::Vorname] = new wxStaticText(this,wxID_ANY,"Vorname");
  lables[(int)v_lbl::Name] = new wxStaticText(this,wxID_ANY,"Name");
  lables[(int)v_lbl::Strasse] = new wxStaticText(this,wxID_ANY,"Strasse");
  lables[(int)v_lbl::Hausnummer] = new wxStaticText(this,wxID_ANY,"Hausnummer");
  lables[(int)v_lbl::PLZ] = new wxStaticText(this,wxID_ANY,"PLZ");
  lables[(int)v_lbl::Ort] = new wxStaticText(this,wxID_ANY,"Ort");
  lables[(int)v_lbl::Land] = new wxStaticText(this,wxID_ANY,"Land");
  lables[(int)v_lbl::Benutzer] = new wxStaticText(this,wxID_ANY,"Benutzer");
  lables[(int)v_lbl::Tarif] = new wxStaticText(this,wxID_ANY,"Tarif");
  lables[(int)v_lbl::Tarifdatum] = new wxStaticText(this,wxID_ANY,"Tarif-Wechsel ab");

  text_ctrls[(int)v_tc::Vorname] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::Name] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::Strasse] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::Hausnummer] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::PLZ] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::Ort] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::Land] = new wxTextCtrl(this,wxID_ANY);
  for(wxTextCtrl* t : text_ctrls)t->SetInitialSize({150,30});
  
  
  cbo_tarife = new wxComboBox(this, ID_cbo_tarif);
  db->fill_combobox(cbo_tarife, "SELECT * FROM V_TG_Bezeichnungen");
  cbo_tarife->SetValue(default_str);  ////////
  cbo_tarife->SetEditable(false);

  tarif_date = new wxDatePickerCtrl(this,wxID_ANY,wxDefaultDateTime);

  btn_update = new wxButton(this,ID_Update_btn,"Aktualisieren");
  btn_reset_pwd = new wxButton(this,ID_ResetPW_btn,"Passwort Reset");
  btn_unload_empl = new wxButton(this,ID_Unload_btn,"Auslagern");
  

  
  
  //Anordnung aller sichtbaren Elemente/////////////////////////////////////////////////////////////////////////////////
  wxSizerFlags szrflags(1);
    szrflags.Border(wxALL,5);
    szrflags.Right(); //Allignment
      
  //Sizer-Pointer werden innerhalb von SetSizer() freigegeben
  wxFlexGridSizer * title_szr = new wxFlexGridSizer(1,100,10);
    title_szr->Add(title);

  wxFlexGridSizer * szr = new wxFlexGridSizer(4,10,10);
    szr->Add(lables[(int)v_lbl::Benutzer],szrflags);
    szr->Add(cbo_benutzer);
    szr->AddSpacer(1);
    szr->AddSpacer(1);

    szr->Add(lables[(int)v_lbl::Vorname],szrflags);
    szr->Add(text_ctrls[(int)v_tc::Vorname],szrflags);

    szr->Add(lables[(int)v_lbl::Name],szrflags);
    szr->Add(text_ctrls[(int)v_tc::Name],szrflags);


  wxFlexGridSizer * szr2 = new wxFlexGridSizer(6,10,10);
    szr2->Add(lables[(int)v_lbl::Strasse],szrflags);
    szr2->Add(text_ctrls[(int)v_tc::Strasse],szrflags);
    
    szr2->Add(lables[(int)v_lbl::Hausnummer],szrflags);
    szr2->Add(text_ctrls[(int)v_tc::Hausnummer],szrflags);

    szr2->Add(lables[(int)v_lbl::PLZ],szrflags);
    szr2->Add(text_ctrls[(int)v_tc::PLZ],szrflags);

  


    szr2->Add(lables[(int)v_lbl::Ort],szrflags);
    szr2->Add(text_ctrls[(int)v_tc::Ort],szrflags);

    szr2->Add(lables[(int)v_lbl::Land],szrflags);
    szr2->Add(text_ctrls[(int)v_tc::Land],szrflags);

    szr2->Add(lables[(int)v_lbl::Tarif],szrflags);
    szr2->Add(cbo_tarife,szrflags);

    szr2->AddSpacer(0);
    szr2->AddSpacer(0);

    szr2->AddSpacer(0);
    szr2->AddSpacer(0);

    szr2->Add(lables[(int)v_lbl::Tarifdatum],szrflags);
    szr2->Add(tarif_date,szrflags);

  wxBoxSizer * szrButtons = new wxBoxSizer(wxVERTICAL);
    szrButtons->Add( btn_update,szrflags);
    szrButtons->Add( btn_reset_pwd,szrflags);
    szrButtons->Add( btn_unload_empl,szrflags);
    

  wxBoxSizer * szrCRUDForm = new wxBoxSizer(wxVERTICAL);
    szrCRUDForm->Add(title_szr);
    szrCRUDForm->Add( szr);
    szrCRUDForm->Add( szr2);
    szrCRUDForm->Add( szrButtons );

  SetSizer(szrCRUDForm);
  SetSize(0,0,parent->m_width,parent->m_height);

  //Hide() erst nach dem SetSize nutzen, da sich die Elemente sonst nicht im grid befinden
  lables[(int)v_lbl::Tarifdatum]->Hide();
  tarif_date->Hide();
}


void Verwaltung::on_change_usr(wxCommandEvent& event){
  try{

    //Folgender Block versteckt Elemente, falls die aus einem vorangegangenem Vorgang noc sichtbar sind
    if(tarif_changed){
      tarif_changed = false;
      lables[(int)v_lbl::Tarifdatum]->Hide();
      tarif_date->Hide();
    }
    
    if(cbo_benutzer->GetValue() == default_str)return;
    //Prüfen ob benutzer vorhanden ist
    int index = cbo_benutzer->GetStrings().Index(cbo_benutzer->GetValue());
    if(index<0){
      //Aufräumen falls benutzer nicht vorhanden ist
      for( wxTextCtrl *k :text_ctrls)k->Clear();
      cbo_tarife->SetValue(default_str);
      return;
    }

    //Benutzerdaten abfragen
    wxString tmp = "call SP_GET_MA_Data('"+wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"')";
    std::string strSQL = tmp.utf8_string();
    MYSQL_ROW data_row = db->get_row_from_db(strSQL.c_str());

    //Benutzerdaten zuordnen
    for(int i =0; i < (int)v_tc::Anzahl_tc;++i){
      text_ctrls[i]->SetValue(wxString::FromUTF8(data_row[i]));
    }
    cbo_tarife->SetValue(data_row[7]);
    
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Verwaltung::on_change_ma: " +  (std::string)e.what());
  }
}


void Verwaltung::on_change_tarif(wxCommandEvent& event){
  try{
    
    if(cbo_benutzer->GetValue() == default_str)return;
    //Prüfen ob benutzer vorhanden ist
    int index = cbo_benutzer->GetStrings().Index(cbo_benutzer->GetValue());
    if(index<0){
      tarif_changed = false;
      lables[(int)v_lbl::Tarifdatum]->Hide();
      tarif_date->Hide();
      return;
    }
    tarif_changed= true;
    lables[(int)v_lbl::Tarifdatum]->Show();
    tarif_date->Show();
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Verwaltung::on_change_tarif: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Verwaltung::on_change_tarif");
  }
}


void Verwaltung::on_update(wxCommandEvent& event){
  try{
    //Eingaben prüfen
    if(!check_entries())return;

    //Angaben vom Nutzer prüfen lassen
    int choice = wxMessageBox( 
    wxString::FromUTF8("Möchten Sie die Änderungen Übernehmen?"),
    wxString::FromUTF8("Speichern?"),
    wxYES_NO|wxICON_ERROR
    );

    //Mitarbeiter anlegen
    if(choice == 2){

      if(tarif_changed){
        //Tarifwechsel dokumentieren, falls nötig. Soll später helfen das Zeitkontingent zu berechnen.
         
        wxString str_date_of_change = tarif_date->GetValue().Format(wxT("%Y-%m-%d"));
         //Datum prüfen

        wxString strSQL =  "call SP_INSERT_Tarif_LOG('"
        +wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"','"
        +str_date_of_change.ToStdString()
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
        lables[(int)v_lbl::Tarifdatum]->Hide();
        tarif_date->Hide();
        tarif_changed = false;
      }
      
      
      //Daten aktualisieren
      wxString strSQL =  "call SP_UPDATE_MA('"
      +wxString::FromUTF8(text_ctrls[(int)v_tc::Vorname]->GetValue().mb_str(wxConvUTF8))+"','"
      +wxString::FromUTF8(text_ctrls[(int)v_tc::Name]->GetValue().mb_str(wxConvUTF8))+"','"
      +wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"','"
      +wxString::FromUTF8(text_ctrls[(int)v_tc::Strasse]->GetValue().mb_str(wxConvUTF8))+"','"
      +wxString::FromUTF8(text_ctrls[(int)v_tc::Hausnummer]->GetValue().mb_str(wxConvUTF8))+"','"
      +wxString::FromUTF8(text_ctrls[(int)v_tc::PLZ]->GetValue().mb_str(wxConvUTF8))+"','"
      +wxString::FromUTF8(text_ctrls[(int)v_tc::Ort]->GetValue().mb_str(wxConvUTF8))+"','"
      +wxString::FromUTF8(text_ctrls[(int)v_tc::Land]->GetValue().mb_str(wxConvUTF8))+"','"
      +wxString::FromUTF8(cbo_tarife->GetValue().mb_str(wxConvUTF8))
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
        "Die Daten von " + wxString::FromUTF8(text_ctrls[(int)v_tc::Vorname]->GetValue().mb_str(wxConvUTF8)) + " "+
        wxString::FromUTF8(text_ctrls[(int)v_tc::Name]->GetValue().mb_str(wxConvUTF8)) + " wurden berarbeitet.",
        wxString::FromUTF8("Änderung erfolgreich."),
        wxOK|wxICON_INFORMATION
      );
    }
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Verwaltung::on_save: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Verwaltung::on_save");
  }
}


void Verwaltung::on_reset_pw(wxCommandEvent& event){
  try{
    //Passwort vom Nutzer zurücksetzen
    wxString strSQL = "call SP_GET_MA_Namen('"+wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"')";
    std::string str_name_empl = db->get_string_from_db(strSQL.mb_str(wxConvUTF8));
    int choice = wxMessageBox( 
    wxString::FromUTF8("Möchten Sie das Passwort von ") + wxString::FromUTF8(str_name_empl) +wxString::FromUTF8(" wirklich zurücksetzen?"),
    wxString::FromUTF8("Sind sie sicher?"),
    wxYES_NO|wxICON_ERROR
    );

    //Mitarbeiter auslagern
    if(choice == 2){
      wxString strSQL =  "call SP_RESET_PW('"+wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"')";
      bool fail = db->execute_SQL(strSQL.mb_str(wxConvUTF8));    //Hier sollte kein Fehler passieren
      if(fail){
        wxMessageBox( 
          wxString::FromUTF8("Bitte wenden Sie sich an die Administration!"),
          wxString::FromUTF8("Fehler beim Auslagern!"),
          wxOK|wxICON_ERROR
        );
        return;
      }

      wxMessageBox( 
        "Das Passwort von " + wxString::FromUTF8(str_name_empl) + wxString::FromUTF8(" wurde erfolgreich zurückgesetzt."),
        wxString::FromUTF8("Passwort zurücksetzen erfolgreich."),
        wxOK|wxICON_INFORMATION
      );
    }
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Verwaltung::on_reset_pw: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Verwaltung::on_reset_pw");
  }
}


void Verwaltung::on_unload(wxCommandEvent& event){
  try{
    //Löschen vom Nutzer prüfen lassen
    wxString strSQL = "call SP_GET_MA_Namen('"+wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"')";
    std::string str_name_empl = db->get_string_from_db(strSQL.mb_str(wxConvUTF8));
    int choice = wxMessageBox( 
    wxString::FromUTF8("Möchten Sie ") + wxString::FromUTF8(str_name_empl) +" wirklich auslagern?",
    wxString::FromUTF8("Sind sie sicher?"),
    wxYES_NO|wxICON_ERROR
    );

    //Mitarbeiter auslagern
    if(choice == 2){
      wxString strSQL =  "call SP_REMOVE_MA('"+wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"')";
      bool fail = db->execute_SQL(strSQL.mb_str(wxConvUTF8));    //Hier sollte kein Fehler passieren
      if(fail){
        wxMessageBox( 
          wxString::FromUTF8("Bitte wenden Sie sich an die Administration!"),
          wxString::FromUTF8("Fehler beim Auslagern!"),
          wxOK|wxICON_ERROR
        );
        return;
      }

      wxMessageBox( 
        wxString::FromUTF8(str_name_empl) + " wurde ausgelagert.",
        wxString::FromUTF8("Auslagern erfolgreich."),
        wxOK|wxICON_INFORMATION
      );

      //Aufräumen
      db->fill_combobox(cbo_benutzer, "SELECT * FROM V_MA_Benutzernamen");
      cbo_benutzer->AutoComplete(cbo_benutzer->GetStrings());
      cbo_benutzer->SetValue(default_str);
      for( wxTextCtrl *k :text_ctrls)k->Clear();
      cbo_tarife->SetValue(default_str);
    }
    
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Verwaltung::on_unload: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Verwaltung::on_unload");
  }
}


bool Verwaltung::check_entries(){
  try{
    //Prüfung ob alle Felder ausgefüllt sind
    for(int i = 0; i < (int)v_tc::Anzahl_tc; ++i){
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
    //Tarif-Wechsel-Datum prüfen
    if(tarif_changed){
      wxString str_date_of_change = tarif_date->GetValue().Format(wxT("%Y-%m-%d"));
      wxString strSQL = "call SP_CHECK_Tarif_LOG('"+wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"','"+str_date_of_change+"')";
      wxString check = db->get_string_from_db(strSQL.mb_str(wxConvUTF8));
      if(check == "-8"){
        wxMessageBox( 
          wxString::FromUTF8("Der "+str_date_of_change+" liegt in der Vergangenheit."),
          wxString::FromUTF8("Tarif-Wechsel nicht möglich!"),
          wxOK|wxICON_ERROR
        );
        return false;
      }
      if(check == "-7"){
        wxMessageBox( 
        wxString::FromUTF8("Tarif-Wechsel sind nur zum Monatsanfang möglich."),
        wxString::FromUTF8("Tarif-Wechsel nicht möglich!"),
        wxOK|wxICON_ERROR
        );
        return false;
      }
      if(check != "0"){
        wxMessageBox( 
         wxString::FromUTF8("Der bisher geplante Tarif-Wechsel am " + check + " steht noch aus."),
          wxString::FromUTF8("Tarif-Wechsel nicht möglich!"),
          wxOK|wxICON_ERROR
        );
        return false;
      }
  
    }

    //Prüfung PLZ
    bool five_digits= std::regex_match(text_ctrls[(int)v_tc::PLZ]->GetValue().ToStdString()
                                      ,std::regex("^\\d{5}$"));
    if(!five_digits){
      wxMessageBox( 
      wxString::FromUTF8("Bitte überprüfen Sie die Postleitzahl."),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }

    return true;
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Verwaltung::check_entries: " +  (std::string)e.what());
    return false;
  }
   catch(...){
    LOG::log_msg("FEHLER in Verwaltung::check_entries");
    return false;
  }
}


Verwaltung::~Verwaltung(){
  delete cbo_benutzer;
  for(wxStaticText* st : lables)delete st;
  for(wxTextCtrl* tc : text_ctrls)delete tc;
  delete cbo_tarife;
  delete btn_update;
  delete btn_reset_pwd;
  delete btn_unload_empl;
}