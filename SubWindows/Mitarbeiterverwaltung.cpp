#include "Mitarbeiterverwaltung.hpp"




Verwaltung::Verwaltung(wxFrame* parent, cppDatabase* DB, const wxString& this_title):SubWindow(parent, DB,this_title){

  //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
  cbo_benutzer = new wxComboBox(this, ID_cbo);
  db->fill_combobox(cbo_benutzer, "SELECT * FROM V_MA_Benutzernamen");
  cbo_benutzer->AutoComplete(cbo_benutzer->GetStrings());
  cbo_benutzer->SetValue(default_str);

  lables[(int)v_lbl::Vorname] = new wxStaticText(this,wxID_ANY,"     Vorname");
  lables[(int)v_lbl::Name] = new wxStaticText(this,wxID_ANY,"               Name");
  lables[(int)v_lbl::Strasse] = new wxStaticText(this,wxID_ANY,"        Strasse");
  lables[(int)v_lbl::Hausnummer] = new wxStaticText(this,wxID_ANY,"Hausnummer");
  lables[(int)v_lbl::PLZ] = new wxStaticText(this,wxID_ANY,"PLZ");
  lables[(int)v_lbl::Ort] = new wxStaticText(this,wxID_ANY,"Ort");
  lables[(int)v_lbl::Land] = new wxStaticText(this,wxID_ANY,"Land");
  lables[(int)v_lbl::Tarif] = new wxStaticText(this,wxID_ANY,"Tarif");

  text_ctrls[(int)v_tc::Vorname] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::Name] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::Strasse] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::Hausnummer] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::PLZ] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::Ort] = new wxTextCtrl(this,wxID_ANY);
  text_ctrls[(int)v_tc::Land] = new wxTextCtrl(this,wxID_ANY);
  for(wxTextCtrl* t : text_ctrls)t->SetInitialSize({150,30});
  
  
  cbo_tarife = new wxComboBox(this, wxID_ANY);
  db->fill_combobox(cbo_tarife, "SELECT * FROM V_TG_Bezeichnungen");
  cbo_tarife->SetValue(default_str);  ////////
  cbo_tarife->SetEditable(false);

  btn_update = new wxButton(this,ID_Update_btn,"Speichern");
  
  
  //Anordnung aller sichtbaren Elemente/////////////////////////////////////////////////////////////////////////////////
  wxSizerFlags szrflags(1);
    szrflags.Border(wxALL,5);
    szrflags.Right(); //Allignment
      
  //Sizer-Pointer werden innerhalb von SetSizer() freigegeben
  wxFlexGridSizer * title_szr = new wxFlexGridSizer(1,100,10);
    title_szr->Add(title);

  wxFlexGridSizer * szr = new wxFlexGridSizer(4,10,10);
    szr->AddSpacer(1);    //label einf??gen
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
  

  wxBoxSizer * szrButtons = new wxBoxSizer(wxVERTICAL);
    szrButtons->Add( btn_update,szrflags);
    

  wxBoxSizer * szrCRUDForm = new wxBoxSizer(wxVERTICAL);
    szrCRUDForm->Add(title_szr);
    szrCRUDForm->Add( szr);
    szrCRUDForm->Add( szr2);
    szrCRUDForm->Add( szrButtons );

  SetSizer(szrCRUDForm);
  SetSize(0,0,parent->m_width,parent->m_height);
}


void Verwaltung::on_change_usr(wxCommandEvent& event){
  try{
    
    if(cbo_benutzer->GetValue() == default_str)return;
    //Pr??fen ob benutzer vorhanden ist
    int index = cbo_benutzer->GetStrings().Index(cbo_benutzer->GetValue());
    if(index<0)return;

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
    std::cerr<<"FEHLER in Verwaltung::on_change_ma: " << e.what();
  }
}








void Verwaltung::on_save(wxCommandEvent& event){
  try{
    //Eingaben pr??fen
    if(!check_entries())return;

    //Angaben vom Nutzer pr??fen lassen
    int choice = wxMessageBox( 
    wxString::FromUTF8("M??chten Sie die ??nderungen ??bernehmen?"),
    wxString::FromUTF8("Speichern?"),
    wxYES_NO|wxICON_ERROR
    );

    //Mitarbeiter anlegen
    if(choice == 2){
      
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
        wxString::FromUTF8("??nderung erfolgreich."),
        wxOK|wxICON_INFORMATION
      );
    }
  }
  catch(std::exception& e){
    std::cerr<<"FEHLER in Verwaltung::on_save: "<< e.what();
  }
}

bool Verwaltung::check_entries(){
  try{
    //Pr??fung ob alle Felder ausgef??llt sind
    for(int i = 0; i < (int)v_tc::Anzahl_tc; ++i){
      if(wxString::FromUTF8(text_ctrls[i]->GetValue().mb_str(wxConvUTF8)).length() == 0){
        wxMessageBox( 
        wxString::FromUTF8("Daten k??nnen nicht gespeichert werden!\nDas Feld [") 
        + wxString::FromUTF8(lables[i]->GetLabelText().mb_str(wxConvUTF8)).Trim(false)
        + wxString::FromUTF8("] enth??lt keinen Wert. "),
        wxString::FromUTF8("Angaben unvollst??ndig!"),
        wxOK|wxICON_ERROR
      );
      return false;
      }
    }

    //Pr??fung ob Tarif ausgew??hlt wurde
    if(cbo_tarife->GetValue() == default_str){
        wxMessageBox( 
        wxString::FromUTF8("Daten k??nnen nicht gespeichert werden!\nEs wurde kein [Tarif] ausgew??hlt."),
        wxString::FromUTF8("Angaben unvollst??ndig!"),
        wxOK|wxICON_ERROR
      );
      return false;
    }

    //Pr??fung PLZ
    bool five_digits= std::regex_match(text_ctrls[(int)v_tc::PLZ]->GetValue().ToStdString()
                                      ,std::regex("^\\d{5}$"));
    if(!five_digits){
      wxMessageBox( 
      wxString::FromUTF8("Bitte ??berpr??fen Sie die Postleitzahl."),
      wxString::FromUTF8("Speichern nicht m??glich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }

    return true;
  }
  catch(std::exception& e){
    std::cerr<<"FEHLER in Verwaltung::check_entries: " << e.what();
    return false;
  }
}





Verwaltung::~Verwaltung(){
  
  delete cbo_benutzer;
  for(wxStaticText* st : lables)delete st;
  for(wxTextCtrl* tc : text_ctrls)delete tc;
  delete cbo_tarife;
  delete btn_update;
}