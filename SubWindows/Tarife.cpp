#include "Tarife.hpp"


Tarife::Tarife(wxFrame* parent, cppDatabase* DB, const wxString& this_title):SubWindow(parent, DB,this_title){

  //Setzt das Datumsformat von wxDatePickerCtrl
  wxLocale l{wxLANGUAGE_GERMAN_GERMANY,wxLOCALE_DONT_LOAD_DEFAULT};

  //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
  lables[(int)t_lbl::Bezeichnung] = new wxStaticText(this,wxID_ANY,"Bezeichnung");
  lables[(int)t_lbl::Wochenstunden] = new wxStaticText(this,wxID_ANY,"Wochenstunden");
  lables[(int)t_lbl::Wochentage] = new wxStaticText(this,wxID_ANY,"Wochentage");
  lables[(int)t_lbl::Stundenlohn] = new wxStaticText(this,wxID_ANY,"Stundenlohn");
  lables[(int)t_lbl::Auswahl_Tarif] = new wxStaticText(this,wxID_ANY,"Tarifauswahl");
  lables[(int)t_lbl::str_Tarif] = new wxStaticText(this,wxID_ANY,"");

  text_ctrls[(int)t_tc::Bezeichnung] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)t_tc::Wochenstunden] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)t_tc::Wochentage] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)t_tc::Stundenlohn] = new wxTextCtrl(this,wxID_ANY);
  
  for(wxTextCtrl* t : text_ctrls)t->SetInitialSize({150,30});
  
  cbo_tarife = new wxComboBox(this, ID_cbo_tarif_edit);
  db->fill_combobox(cbo_tarife, "SELECT * FROM V_TG_Bezeichnungen");
  cbo_tarife->SetValue(default_str);  ////////
  cbo_tarife->SetEditable(false);

  btn_new_tarif = new wxButton(this,ID_New_Tarif_btn,"Neuer Tarif");
  btn_save_tarif = new wxButton(this,ID_Save_Tarif_btn,"Tarif Speichern");
  btn_delete_tarif = new wxButton(this,ID_Delete_Tarif_btn,wxString::FromUTF8("Tarif Löschen"));
  
  //Anordnung aller sichtbaren Elemente/////////////////////////////////////////////////////////////////////////////////
  wxSizerFlags szrflags(1);
    szrflags.Border(wxRIGHT,1);
    szrflags.Left(); //Allignment
      
  //Sizer-Pointer werden innerhalb von SetSizer() freigegeben
  wxFlexGridSizer * title_szr = new wxFlexGridSizer(1,1,1);
    title_szr->Add(title,szrflags);

  wxFlexGridSizer * szr = new wxFlexGridSizer(2,1,1);
    szr->Add(lables[(int)t_lbl::Auswahl_Tarif],szrflags);
    szr->Add(cbo_tarife,szrflags);
    szr->Add(lables[(int)t_lbl::str_Tarif],szrflags);
    szr->AddSpacer(10);
    szr->Add( btn_delete_tarif);


  wxFlexGridSizer * szr2 = new wxFlexGridSizer(8,1,1);
    szr2->Add(lables[(int)t_lbl::Bezeichnung],szrflags);
    szr2->Add(text_ctrls[(int)t_tc::Bezeichnung],szrflags);
    
    szr2->Add(lables[(int)t_tc::Wochenstunden],szrflags);
    szr2->Add(text_ctrls[(int)t_tc::Wochenstunden],szrflags);

    szr2->Add(lables[(int)t_tc::Wochentage],szrflags);
    szr2->Add(text_ctrls[(int)t_tc::Wochentage],szrflags);

    szr2->Add(lables[(int)t_lbl::Stundenlohn],szrflags);
    szr2->Add(text_ctrls[(int)t_tc::Stundenlohn],szrflags);

    szr2->Add( btn_save_tarif,szrflags);


  wxBoxSizer * szrButtons = new wxBoxSizer(wxVERTICAL);
    szrButtons->Add( btn_new_tarif);    

  wxBoxSizer * szrCRUDForm = new wxBoxSizer(wxVERTICAL);
    szrCRUDForm->Add(title_szr);
    szrCRUDForm->Add( szr,szrflags);
    szrCRUDForm->Add( szrButtons,szrflags);
    szrCRUDForm->Add( szr2,szrflags);

  SetSizer(szrCRUDForm);
  SetSize(0,0,parent->m_width,parent->m_height);

  //Hide() erst nach dem SetSize nutzen, da sich die Elemente sonst nicht im grid befinden
  lables[(int)t_lbl::Bezeichnung]->Hide();
  lables[(int)t_lbl::Wochenstunden]->Hide();
  lables[(int)t_lbl::Wochentage]->Hide();
  lables[(int)t_lbl::Stundenlohn]->Hide();

  text_ctrls[(int)t_lbl::Bezeichnung]->Hide();
  text_ctrls[(int)t_lbl::Wochenstunden]->Hide();
  text_ctrls[(int)t_lbl::Wochentage]->Hide();
  text_ctrls[(int)t_lbl::Stundenlohn]->Hide();

  btn_save_tarif->Hide();
  btn_delete_tarif->Hide();
}


void Tarife::on_change_tarif(wxCommandEvent& event){
  try{
    std::string str_tarif_data =  db->get_string_from_db("call SP_GET_Tarif_DATA('"+wxString::FromUTF8(cbo_tarife->GetValue().mb_str(wxConvUTF8))+"')");
    std::replace(str_tarif_data.begin(),str_tarif_data.end(),'.', ',');
    lables[(int)t_lbl::str_Tarif]->SetLabel(wxString::FromUTF8(str_tarif_data));
    btn_delete_tarif->Show();
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Tarife::on_change_tarif: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Tarife::on_change_tarif");
  }
}


void Tarife::on_new(wxCommandEvent& event){
  try{

    lables[(int)t_lbl::Bezeichnung]->Show();
    lables[(int)t_lbl::Wochenstunden]->Show();
    lables[(int)t_lbl::Wochentage]->Show();
    lables[(int)t_lbl::Stundenlohn]->Show();
  
    text_ctrls[(int)t_lbl::Bezeichnung]->Show();
    text_ctrls[(int)t_lbl::Wochenstunden]->Show();
    text_ctrls[(int)t_lbl::Wochentage]->Show();
    text_ctrls[(int)t_lbl::Stundenlohn]->Show();

    btn_save_tarif->Show();
    
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Tarife::on_new: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Tarife::on_new");
  }
}


void Tarife::on_save(wxCommandEvent& event){
  try{

    if(!check_entries())return;

    std::string str_stunden = text_ctrls[(int)t_lbl::Wochenstunden]->GetValue().ToStdString();
    std::string str_tage = text_ctrls[(int)t_lbl::Wochentage]->GetValue().ToStdString();
    std::string str_lohn = text_ctrls[(int)t_lbl::Stundenlohn]->GetValue().ToStdString();

    //Angaben vom Nutzer prüfen lassen
    int choice = wxMessageBox( 
    "Tarif-Bezeichnung:\n"+wxString::FromUTF8(text_ctrls[(int)t_lbl::Bezeichnung]->GetValue().mb_str(wxConvUTF8))
    +"\n\nStunden pro Woche:\n"+str_stunden
    +"\n\nTage pro Woche:\n"+str_tage
    +"\n\nBrutto Stundenlohn:\n"+str_lohn,
    wxString::FromUTF8("Bitte prüfen Sie die Angaben!"),
    wxYES_NO|wxICON_ERROR
    );

    //Tarif anlegen
    if(choice == 2){
      std::replace(str_stunden.begin(),str_stunden.end(),',','.');
      std::replace(str_tage.begin(),str_tage.end(),',','.');
      std::replace(str_lohn.begin(),str_lohn.end(),',','.');

      wxString strSQL =  "call SP_INSERT_Tarif('"
      +wxString::FromUTF8(text_ctrls[(int)t_lbl::Bezeichnung]->GetValue().mb_str(wxConvUTF8))+"',"
      +str_stunden+","
      +str_tage+","
      +str_lohn
      +")";

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
        "Tarif  "+wxString::FromUTF8(text_ctrls[(int)t_lbl::Bezeichnung]->GetValue().mb_str(wxConvUTF8)) +
        " wurde angelegt.",
        wxString::FromUTF8("Speichervorgang erfolgreich."),
        wxOK|wxICON_INFORMATION
      );
      //Tarifauswahl neu laden
      cbo_tarife->Clear();
      db->fill_combobox(cbo_tarife, "SELECT * FROM V_TG_Bezeichnungen");
      cbo_tarife->SetValue(default_str);
    } 
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Tarife::on_save: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Tarife::on_save");
  }
}

void Tarife::on_delete(wxCommandEvent& event){
  try{
    //Löschvorgang bestätigen lassen
    int choice = wxMessageBox( 
    "Tarif "+wxString::FromUTF8(text_ctrls[(int)t_lbl::Bezeichnung]->GetValue().mb_str(wxConvUTF8))
    +" löschen?",
    wxString::FromUTF8("Bitte Löschvorgang bestätigen!"),
    wxYES_NO|wxICON_ERROR
    );

    //Tarif anlegen
    if(choice == 2){
      
      wxString strSQL =  "call SP_DELETE_Tarif('"
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
        "Tarif  " + wxString::FromUTF8(cbo_tarife->GetValue().mb_str(wxConvUTF8)) +
        " wurde gelöscht.",
        wxString::FromUTF8("Löschen erfolgreich."),
        wxOK|wxICON_INFORMATION
      );
      //Tarifauswahl neu laden
      cbo_tarife->Clear();
      db->fill_combobox(cbo_tarife, "SELECT * FROM V_TG_Bezeichnungen");
      cbo_tarife->SetValue(default_str);

      lables[(int)t_lbl::str_Tarif]->SetLabel("");
      btn_delete_tarif->Hide();
    } 
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Tarife::on_delete: " +  (std::string)e.what());
  }
  catch(...){
    LOG::log_msg("FEHLER in Tarife::on_delete");
  }
}


bool Tarife::check_entries(){
  try{
    //Prüfung ob alle Felder ausgefüllt sind
    for(int i = 0; i < (int)t_tc::Anzahl_tc; ++i){
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
    
    //Prüfen ob Stunden,Tage, und Lohn eine plausible sind
    std::string str_dec_regex = "^[0-9]+(\\.[0-9]{1,2})?$"; // Püft ob es sich um eine dezimalzal mit maximal zwei Nachkommstellen handelt
    
    wxString str_hours = text_ctrls[(int)t_lbl::Wochenstunden]->GetValue();
    std::replace(str_hours.begin(),str_hours.end(),',', '.');

    wxString str_days = text_ctrls[(int)t_lbl::Wochentage]->GetValue();
    std::replace(str_days.begin(),str_days.end(),',', '.');

    wxString str_wage = text_ctrls[(int)t_lbl::Stundenlohn]->GetValue();
    std::replace(str_wage.begin(),str_wage.end(),',','.');

    float hours  = std::atof(str_hours);
    float days    = std::atof(str_days);

    bool valid_float = std::regex_match(str_hours.ToStdString(),std::regex(str_dec_regex));
    if(!valid_float || hours > 24 * 7){
      wxMessageBox(wxString::FromUTF8(
                       "Bitte überprüfen Sie die eingegebenen Wochenstunden."),
                   wxString::FromUTF8("Speichern nicht möglich!"),
                   wxOK | 0x00000200);
      return false;
    }
    valid_float = std::regex_match(str_days.ToStdString(),std::regex(str_dec_regex));
    if(!valid_float || days > 7){
      wxMessageBox( 
      wxString::FromUTF8("Bitte überprüfen Sie die eingegebenen Wochentage."),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }
    valid_float = std::regex_match(str_wage.ToStdString(),std::regex(str_dec_regex));
    if(!valid_float){
      wxMessageBox( 
      wxString::FromUTF8("Bitte überprüfen Sie den eingegebenen Stundenlohn."),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }

    return true;
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Tarife::check_entries: " +  (std::string)e.what());
    return false;
  }
  catch(...){
    LOG::log_msg("FEHLER in Tarife::check_entries");
    return false;
  }
}


Tarife::~Tarife(){
  delete cbo_tarife;
  for(wxStaticText* st : lables)delete st;
  for(wxTextCtrl* tc : text_ctrls)delete tc;
  delete btn_new_tarif;
  delete btn_save_tarif;
  delete btn_delete_tarif;
}