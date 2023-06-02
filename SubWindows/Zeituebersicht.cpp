#include "Zeituebersicht.hpp"

Uebersicht::Uebersicht(wxFrame* parent, cppDatabase* DB,const wxString& this_title)
:SubWindow(parent, DB,this_title){
  
  //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
  lables[(int)u_lbl::Benutzer] = new wxStaticText(this,wxID_ANY,"    Benutzer");
  lables[(int)u_lbl::Monat] = new wxStaticText(this,wxID_ANY,"    Monat");
  lables[(int)u_lbl::Jahr] = new wxStaticText(this,wxID_ANY,"    Jahr");
  lables[(int)u_lbl::Stunden] = new wxStaticText(this,wxID_ANY,"");
  lables[(int)u_lbl::ZeitTyp] = new wxStaticText(this,wxID_ANY,"Ansicht");

  combo_boxen[(int)u_cbo::Benutzer] = new wxComboBox(this, ID_USR_cbo, default_str);
  combo_boxen[(int)u_cbo::Monat] = new wxComboBox(this, wxID_ANY, "Alle");
  combo_boxen[(int)u_cbo::Jahr] = new wxComboBox(this, ID_Year_cbo, default_str,wxDefaultPosition,wxDefaultSize,0,NULL,wxTE_PROCESS_ENTER);
  combo_boxen[(int)u_cbo::ZeitTyp] = new wxComboBox(this, ID_ZTyp_cbo, "Arbeitszeiten");
  set_up_combos();

  grd_zeiten = new wxGrid(this,wxID_ANY,wxDefaultPosition,wxDefaultSize);
  set_up_grid();

  btn_show_times = new wxButton(this,ID_Show_Times,"Daten anzeigen");

//Anordnung aller sichtbaren Elemente/////////////////////////////////////////////////////////////////////////////////
 
  wxSizerFlags szrflags(1);
  szrflags.Border(wxALL,5);

  //Sizer-Pointer werden innerhalb von SetSizer() freigegeben
  wxFlexGridSizer * title_szr = new wxFlexGridSizer(1,100,10);
  title_szr->Add(title);

  wxFlexGridSizer* szr = new wxFlexGridSizer(6,10,10);
  szr->Add( lables[(int)u_lbl::Benutzer],szrflags );
  szr->Add( combo_boxen[(int)u_cbo::Benutzer],szrflags );

  szr->Add( lables[(int)u_lbl::Monat],szrflags );
  szr->Add( combo_boxen[(int)u_cbo::Monat],szrflags );

  szr->Add( lables[(int)u_lbl::Jahr],szrflags );
  szr->Add( combo_boxen[(int)u_cbo::Jahr],szrflags );
  
  szr->Add( lables[(int)u_lbl::ZeitTyp],szrflags );
  szr->Add( combo_boxen[(int)u_cbo::ZeitTyp],szrflags );

  szr->Add( btn_show_times,szrflags);

  wxFlexGridSizer * szr2 = new wxFlexGridSizer(1,10,10);
  szr2->Add(grd_zeiten,szrflags);

  wxBoxSizer * szrButtons = new wxBoxSizer( wxHORIZONTAL);
  //szrButtons->Add( btn_show_times,szrflags);
  szrButtons->AddSpacer(350);
  szrButtons->Add(  lables[(int)u_lbl::Stunden],szrflags);
  
  wxBoxSizer * szrCRUDForm = new wxBoxSizer(wxVERTICAL);
  szrCRUDForm->Add(title_szr);
  szrCRUDForm->Add(szr);
  szrCRUDForm->Add(szr2);
  szrCRUDForm->Add(szrButtons);

  SetSizer(szrCRUDForm);

  SetSize(0,0,parent->m_width,parent->m_height);
}

void Uebersicht::on_show_times(wxCommandEvent& event){
  try{
    //Benutzer muss vorhanden sein
    wxString strSQL = "call SP_CHECK_Benutzername('"+wxString::FromUTF8(combo_boxen[(int)u_cbo::Benutzer]->GetValue().mb_str(wxConvUTF8))+"')";
    wxString entry_count = db->get_string_from_db(strSQL.mb_str(wxConvUTF8));
    if(entry_count =="0"){
      wxMessageBox( 
      wxString::FromUTF8(combo_boxen[(int)u_cbo::Benutzer]->GetValue().mb_str(wxConvUTF8))+" ist kein registrierter Benutzer.\n"+wxString::FromUTF8("Bitte wählen Sie einen gültigen Benutzer aus."),
      wxString::FromUTF8("Benutzer unbekannt!"),
      wxOK|wxICON_ERROR);
    return;
    }

    if(combo_boxen[(int)u_cbo::Benutzer]->GetValue() == default_str){
      wxMessageBox( 
        wxString::FromUTF8("Bitte wählen Sie zunächst ein Jahr aus."),
        wxString::FromUTF8("Angaben unvollständig!"),
        wxOK|wxICON_ERROR
      );
      return;
    }

    //Grid Inhalt und Row-Labels löschen
    grd_zeiten->ClearGrid();
    for(unsigned int intRow = 0;intRow < grd_zeiten->GetNumberRows();++intRow)
      grd_zeiten->SetRowLabelValue(intRow, "");

    //Zeitraum von/bis 
    wxString strFromMonth = std::to_string(combo_boxen[(int)u_cbo::Monat]->GetCurrentSelection()), 
             strTillMonth = std::to_string(combo_boxen[(int)u_cbo::Monat]->GetCurrentSelection());
    if(combo_boxen[(int)u_cbo::Monat]->GetValue()=="Alle"){strFromMonth = "1";strTillMonth = "12"; }

    std::string str_Auswahl = combo_boxen[(int)u_cbo::ZeitTyp]->GetValue().ToStdString();
    if(str_Auswahl=="Arbeit")get_Arbeitszeiten(strFromMonth,strTillMonth);
    else if(str_Auswahl=="Urlaub")get_Urlaubszeiten(strFromMonth,strTillMonth);
    else if(str_Auswahl=="Krankheit")get_Krankheitszeiten(strFromMonth,strTillMonth);
    else get_Sonstigezeiten(strFromMonth,strTillMonth);
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Uebersicht::on_show_times: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Uebersicht::on_show_times");
  }
}


void Uebersicht::get_Arbeitszeiten(wxString& strFromMonth,wxString& strTillMonth){
  try{
    wxString strSQL = "call SP_GET_Arbeitszeiten('"+wxString::FromUTF8(combo_boxen[(int)u_cbo::Benutzer]->GetValue().mb_str(wxConvUTF8))+"',"+strFromMonth+","+strTillMonth+","+combo_boxen[(int)u_cbo::Jahr]->GetValue()+");";
    db->fill_grid(grd_zeiten, strSQL.mb_str(wxConvUTF8));

    //Summe der Stunden aus der Übersicht ermitteln
    wxDouble db_lSum = 0.0F;
    for(int row_Index = 0; row_Index < grd_zeiten->GetNumberRows(); ++row_Index ){
      if(grd_zeiten->GetCellValue(row_Index,grd_zeiten->GetNumberCols()-1) != "")
        db_lSum = db_lSum + std::stod((const std::string)grd_zeiten->GetCellValue(row_Index,grd_zeiten->GetNumberCols()-1));
    
      //Bei der Gelegenheit kann man auch bei der Arbeitstunden-Spalte <.> mit <,> ersetzen
      wxString str_hours = grd_zeiten->GetCellValue(row_Index,grd_zeiten->GetNumberCols()-1);
      std::replace(str_hours.begin(),str_hours.end(),'.', ',');
      grd_zeiten->SetCellValue(row_Index,grd_zeiten->GetNumberCols()-1, str_hours);
    }

    //Summe der Arbeitstunden formatieren und dem Label zuweisen
    wxString str_Sum = std::to_string(db_lSum);
    std::replace(str_Sum.begin(),str_Sum.end(),'.', ',');
    str_Sum = str_Sum.substr(0,str_Sum.length()-4);
    lables[(int)u_lbl::Stunden]->SetLabel("Insgesamt: " + str_Sum + " Stunden");
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Uebersicht::get_Arbeitszeiten: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Uebersicht::get_Arbeitszeiten");
  }
}


void Uebersicht::get_Urlaubszeiten(wxString& strFromMonth,wxString& strTillMonth){
  try{
    wxString strSQL = "call SP_GET_Urlaubszeiten('"+wxString::FromUTF8(combo_boxen[(int)u_cbo::Benutzer]->GetValue().mb_str(wxConvUTF8))+"',"+strFromMonth+","+strTillMonth+","+combo_boxen[(int)u_cbo::Jahr]->GetValue()+");";
    db->fill_grid(grd_zeiten, strSQL.mb_str(wxConvUTF8));
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Uebersicht::get_Arbeitszeiten: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Uebersicht::get_Arbeitszeiten");
  }
}

void Uebersicht::get_Krankheitszeiten(wxString& strFromMonth,wxString& strTillMonth){
  try{
    wxString strSQL = "call SP_GET_Krankheitszeiten('"+wxString::FromUTF8(combo_boxen[(int)u_cbo::Benutzer]->GetValue().mb_str(wxConvUTF8))+"',"+strFromMonth+","+strTillMonth+","+combo_boxen[(int)u_cbo::Jahr]->GetValue()+");";
    db->fill_grid(grd_zeiten, strSQL.mb_str(wxConvUTF8));
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Uebersicht::get_Krankheitszeiten: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Uebersicht::get_Krankheitszeiten");
  }
}

void Uebersicht::get_Sonstigezeiten(wxString& strFromMonth,wxString& strTillMonth){
  try{
    wxString strSQL = "call SP_GET_Sonstigezeiten('"+wxString::FromUTF8(combo_boxen[(int)u_cbo::Benutzer]->GetValue().mb_str(wxConvUTF8))+"',"+strFromMonth+","+strTillMonth+","+combo_boxen[(int)u_cbo::Jahr]->GetValue()+");";
    db->fill_grid(grd_zeiten, strSQL.mb_str(wxConvUTF8));
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Uebersicht::get_Sonstigezeiten: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Uebersicht::get_Sonstigezeiten");
  }
}


void Uebersicht::on_change_usr(wxCommandEvent& event){
  try{
    //Prüfen ob benutzer vorhanden ist
    int index = combo_boxen[(int)u_cbo::Benutzer]->GetStrings().Index(combo_boxen[(int)u_cbo::Benutzer]->GetValue());
    if(index<0){
      combo_boxen[(int)u_cbo::Jahr]->Clear();
      combo_boxen[(int)u_cbo::Jahr]->SetValue(default_str);
      return;
    }
    
    //Prüfen ob bereits Arbeitszeiten erfasst wurden
    wxString strSQL = "call SP_CHECK_Jahre('"+wxString::FromUTF8(combo_boxen[(int)u_cbo::Benutzer]->GetValue().mb_str(wxConvUTF8))+"')";
    wxString entry_count = db->get_string_from_db(strSQL.mb_str(wxConvUTF8));
    if(entry_count =="0"){
      combo_boxen[(int)u_cbo::Jahr]->Clear();
      combo_boxen[(int)u_cbo::Jahr]->SetValue(default_str);
      wxMessageBox( 
      wxString::FromUTF8(combo_boxen[(int)u_cbo::Benutzer]->GetValue().mb_str(wxConvUTF8))+" hat bisher keine Arbeitszeiten erfasst.",
      wxString::FromUTF8("Keine Daten vorhanden!"),
      wxOK|wxICON_ERROR
      );
      return;
    }

    //Jahre in denen Arbeitszeiten erfasst wurden in die Combo-Box füllen
    combo_boxen[(int)u_cbo::Jahr]->Clear();
    strSQL = "call SP_GET_Jahre('"+wxString::FromUTF8(combo_boxen[(int)u_cbo::Benutzer]->GetValue().mb_str(wxConvUTF8))+"')";
    db->fill_combobox(combo_boxen[(int)u_cbo::Jahr], strSQL.mb_str(wxConvUTF8));
    combo_boxen[(int)u_cbo::Jahr]->SetValue(combo_boxen[(int)u_cbo::Jahr]->GetString(0));
    combo_boxen[(int)u_cbo::Jahr]->SetToolTip("");
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Uebersicht::on_change_ma: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Uebersicht::on_change_ma");
  }
}


void Uebersicht::on_enter_year(wxCommandEvent& event){
  try{
    if(combo_boxen[(int)u_cbo::Benutzer]->GetValue() == default_str){
      wxMessageBox( 
        wxString::FromUTF8("Bitte wählen Sie zunächst einen Mitarbeiter aus."),
        wxString::FromUTF8("Jahr(e) noch nicht verfügbar!"),
        wxOK|wxICON_ERROR
      );
      return;
    }
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Uebersicht::on_enter_year: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Uebersicht::on_enter_year");
  }
}


bool Uebersicht::set_up_grid(){
  try{
    grd_zeiten->CreateGrid(10, 5);

    //Nächster Block soll eigentlich nur eine Mindestgröße des grids erstellen, Text wird weiter unten wieder gelöscht
    for(int i = 0;i < grd_zeiten->GetNumberCols();++i){
      grd_zeiten->SetColLabelValue(i, _("____________")); 
    }    
    grd_zeiten->AutoSize();

    //Zeilen-Bezeichnung entfernen
    for(unsigned int intRow = 0;intRow < grd_zeiten->GetNumberRows();++intRow){
      grd_zeiten->SetRowLabelValue(intRow, "");
    }
    //Grid nicht editierbar machen
    grd_zeiten->DisableDragColSize();
    grd_zeiten->DisableDragRowSize(); 
    grd_zeiten->EnableEditing(false);

    //...hier wird der Text wieder gelöscht. Ja, muss auch anders gehen...
    for(int i = 0;i < grd_zeiten->GetNumberCols();++i){
      grd_zeiten->SetColLabelValue(i, _(" ")); 
    }    
    return true;
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Uebersicht::set_up_grid: " +  (std::string)e.what());
    return false;
  }
  catch(...){
    LOG::log_msg("FEHLER in Uebersicht::set_up_grid");
    return false;
  }
}


bool Uebersicht::set_up_combos(){
  try{
    db->fill_combobox(combo_boxen[(int)u_cbo::Benutzer], "SELECT * FROM V_MA_Benutzernamen");
    combo_boxen[(int)u_cbo::Benutzer]->AutoComplete(combo_boxen[(int)u_cbo::Benutzer]->GetStrings());
    combo_boxen[(int)u_cbo::Benutzer]->SetValue(default_str);

    combo_boxen[(int)u_cbo::Monat]->Insert("Alle",0);
    combo_boxen[(int)u_cbo::Monat]->Insert("Januar",1);
    combo_boxen[(int)u_cbo::Monat]->Insert("Februar",2);
    combo_boxen[(int)u_cbo::Monat]->Insert(wxString::FromUTF8("März"),3);
    combo_boxen[(int)u_cbo::Monat]->Insert("April",4);
    combo_boxen[(int)u_cbo::Monat]->Insert("Mai",5);
    combo_boxen[(int)u_cbo::Monat]->Insert("Juni",6);
    combo_boxen[(int)u_cbo::Monat]->Insert("Juli",7);
    combo_boxen[(int)u_cbo::Monat]->Insert("August",8);
    combo_boxen[(int)u_cbo::Monat]->Insert("September",9);
    combo_boxen[(int)u_cbo::Monat]->Insert("Oktober",10);
    combo_boxen[(int)u_cbo::Monat]->Insert("November",11);
    combo_boxen[(int)u_cbo::Monat]->Insert("Dezember",12);

    combo_boxen[(int)u_cbo::Jahr]->SetEditable(false);
    combo_boxen[(int)u_cbo::Jahr]->SetToolTip(tt_Jahr);
    
    //db->fill_combobox(combo_boxen[(int)u_cbo::ZeitTyp], "SELECT 'Arbeit' AS Bezeichnung UNION SELECT * FROM `V_ABW_Bezeichnung` LIMIT 100;");
    combo_boxen[(int)u_cbo::ZeitTyp]->Insert("Arbeit",0);
    combo_boxen[(int)u_cbo::ZeitTyp]->Insert("Urlaub",1);
    combo_boxen[(int)u_cbo::ZeitTyp]->Insert("Krankheit",2);
    combo_boxen[(int)u_cbo::ZeitTyp]->Insert("Sonstige",3);

    combo_boxen[(int)u_cbo::ZeitTyp]->SetValue("Arbeit");
    combo_boxen[(int)u_cbo::ZeitTyp]->SetEditable(false);
    
    return true;
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Uebersicht::set_up_combo: " +  (std::string)e.what());
    return false;
  }
  catch(...){
    LOG::log_msg("FEHLER in Uebersicht::set_up_combo");
    return false;
  }
}


Uebersicht::~Uebersicht(){
  for(wxComboBox* cb : combo_boxen) delete cb;
  for(wxStaticText* st : lables) delete st;
  delete grd_zeiten;
  delete btn_show_times;
}