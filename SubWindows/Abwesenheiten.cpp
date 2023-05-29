#include "Abwesenheiten.hpp"


Abwesenheiten::Abwesenheiten(wxFrame* parent, cppDatabase* DB, const wxString& this_title):SubWindow(parent, DB,this_title){
  
  //Setzt das Datumsformat von wxDatePickerCtrl
  wxLocale l{wxLANGUAGE_GERMAN_GERMANY,wxLOCALE_DONT_LOAD_DEFAULT};

  //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
  lables[(int)a_lbl::Mitabeiter] = new wxStaticText(this,wxID_ANY,"Mitarbeiter");
  cbo_benutzer = new wxComboBox(this, wxID_ANY);
  db->fill_combobox(cbo_benutzer, "SELECT * FROM V_MA_Benutzernamen");
  cbo_benutzer->AutoComplete(cbo_benutzer->GetStrings());
  cbo_benutzer->SetValue(default_str);

  lables[(int)a_lbl::Typ_Abwesenheit] = new wxStaticText(this,wxID_ANY,"Grund der Abwesenheit");
  cbo_typ_abwesenheit = new wxComboBox(this, wxID_ANY);
  db->fill_combobox(cbo_typ_abwesenheit, "SELECT * FROM V_ABW_Bezeichnung");
  cbo_typ_abwesenheit->AutoComplete(cbo_typ_abwesenheit->GetStrings());
  cbo_typ_abwesenheit->SetValue(default_str);

  lables[(int)a_lbl::Startdatum] = new wxStaticText(this,wxID_ANY,"Startdatum");
  start_date = new wxDatePickerCtrl(this,wxID_ANY,wxDefaultDateTime);
  lables[(int)a_lbl::Enddatum] = new wxStaticText(this,wxID_ANY,"Enddatum");
  end_date = new wxDatePickerCtrl(this,wxID_ANY,wxDefaultDateTime);

  lables[(int)a_lbl::Bemerkungen] = new wxStaticText(this,wxID_ANY,"Bemerkungen");
  tc_Bemerkungen = new wxTextCtrl(this,wxID_ANY);

  btn_submit = new wxButton(this, ID_Abwesenheit_speichern,"&Senden");          


  //Anordnung aller sichtbaren Elemente/////////////////////////////////////////////////////////////////////////////////
  wxSizerFlags szrflags(1);
  szrflags.Border(wxALL,5);

  //Sizer-Pointer werden innerhalb von SetSizer() freigegeben    
  wxFlexGridSizer * title_szr = new wxFlexGridSizer(1,100,10);
  title_szr->Add(title);

  wxFlexGridSizer * szr = new wxFlexGridSizer(4,10,30);
  szr->Add(lables[(int)a_lbl::Mitabeiter],szrflags);
  szr->Add( cbo_benutzer,szrflags );
  szr->Add(lables[(int)a_lbl::Typ_Abwesenheit],szrflags);
  szr->Add( cbo_typ_abwesenheit,szrflags );

  szr->Add(lables[(int)a_lbl::Startdatum],szrflags);
  szr->Add( start_date,szrflags );
  szr->Add(lables[(int)a_lbl::Enddatum],szrflags);
  szr->Add( end_date,szrflags );

  szr->Add(lables[(int)a_lbl::Bemerkungen],szrflags);
  szr->Add( tc_Bemerkungen,szrflags );


  wxBoxSizer * szrButtons = new wxBoxSizer( wxHORIZONTAL);
  szrButtons->Add( btn_submit,szrflags);
  
  wxBoxSizer * szrCRUDForm = new wxBoxSizer(wxVERTICAL);
  szrCRUDForm->Add(title_szr);
  szrCRUDForm->Add(szr);
  szrCRUDForm->Add(szrButtons);
  SetSizer(szrCRUDForm);

  SetSize(0,0,parent->m_width,parent->m_height);
}



void Abwesenheiten::on_submit(wxCommandEvent& event){
  try{ 
    if(cbo_benutzer->GetValue() == default_str){
      wxMessageBox( 
        wxString::FromUTF8("Bitte wählen Sie zunächst einen Mitarbeiter aus."),
        wxString::FromUTF8("Angaben unvollständig!"),
        wxOK|wxICON_ERROR
      );
      return;
    }

    wxString strSQL = "call SP_CHECK_Benutzername('"+wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"')";
    wxString entry_count = db->get_string_from_db(strSQL.mb_str(wxConvUTF8));
    if(entry_count =="0"){
      wxMessageBox( 
      wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+" ist kein registrierter Benutzer.\n"+wxString::FromUTF8("Bitte wählen Sie einen gültigen Benutzer aus."),
      wxString::FromUTF8("Benutzer unbekannt!"),
      wxOK|wxICON_ERROR);
      return;
    }
   

    wxString Anfangszeit = start_date->GetValue().Format(wxT("%y-%m-%d %H:%M:%S"));
    wxString Endzeit = end_date->GetValue().Format(wxT("%y-%m-%d %H:%M:%S"));

    //Prüfung des Zeitraums////////////
    if(start_date->GetValue() > end_date->GetValue()){
       wxMessageBox( 
        "End-Zeit("+end_date->GetValue().Format(" %d.%m.%Y ")+")\ndarf nicht vor Start-Zeit("+start_date->GetValue().Format(" %d.%m.%Y ")+") liegen!",
        wxString::FromUTF8("Angaben nicht plausibel!"),
        wxOK|wxICON_ERROR
      );
      return;
    }

   
    //  Prüfung auf Zeitüberlappung und ob Zeitraum vor Vertragsbeginn oder in der Zukunft liegt////////////
    strSQL = "call SP_CHECK_Abwesenheiten('"
    +wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))
    +"','"+wxString::FromUTF8(cbo_typ_abwesenheit->GetValue().mb_str(wxConvUTF8))
    +"','"+Anfangszeit.ToStdString()
    +"', '"+Endzeit.ToStdString()
    +"')";
    wxString check = db->get_string_from_db(strSQL.mb_str(wxConvUTF8));
    if(check == "-9"){
      wxMessageBox( 
        wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8)) + wxString::FromUTF8(" war am "+start_date->GetValue().Format("%d.%m.%Y")+" noch nicht angestellt."),
        wxString::FromUTF8("Zeitraum nicht plausibel!"),
        wxOK|wxICON_ERROR
      );
      return;
    } 
    if(check == "-8"){
      wxMessageBox( 
        wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8)) + wxString::FromUTF8(" hat innerhalb des angegebenen Zeitraums \nvom "+start_date->GetValue().Format("  %d.%m.%Y ")+" \nbis zum "+end_date->GetValue().Format("  %d.%m.%Y ")+" \nArbeitszeiten gespeichert."),
        wxString::FromUTF8("Zeitraum nicht plausibel!"),
        wxOK|wxICON_ERROR
      );
      return;
    }
    if(check != "0"){
      wxMessageBox( 
        wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8)) + wxString::FromUTF8(" hat innerhalb des angegebenen Zeitraums \nvom "+start_date->GetValue().Format(" %d.%m.%Y ")+" \nbis zum "+end_date->GetValue().Format(" %d.%m.%Y ")+" \nbereits eine Abwesenheit mit gleichem Grund."),
        wxString::FromUTF8("Zeitraum nicht plausibel!"),
        wxOK|wxICON_ERROR
      );
      return;
    }
    //Prüfung Ende//////////////////////////////////////////////////////////////////

    strSQL = "call SP_INSERT_Abwesenheit('"
      +wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))
      +"','"+wxString::FromUTF8(cbo_typ_abwesenheit->GetValue().mb_str(wxConvUTF8))
      +"','"+Anfangszeit.ToStdString()
      +"', '"+Endzeit.ToStdString()
      +"','"+wxString::FromUTF8(tc_Bemerkungen->GetValue().mb_str(wxConvUTF8))
      +"')";
    
    bool fail = db->execute_SQL(strSQL.mb_str(wxConvUTF8));

    

    wxMessageBox( 
    fail ? "SQL Insert Fehlgeschlagen" : "SQL Insert Erfolgreich",
    "Ausgabe der DB", 
      wxOK | wxICON_INFORMATION 
    ); 
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Abwesenheiten::on_submit: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Abwesenheiten::on_submit");
  }
}
