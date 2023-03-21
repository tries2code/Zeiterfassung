#include "Zeiterfassung.hpp"

Zeiterfassung::Zeiterfassung(wxFrame* parent, cppDatabase* DB, const wxString& this_title):SubWindow(parent, DB,this_title){
  
  //Setzt das Datumsformat von wxDatePickerCtrl
  wxLocale l{wxLANGUAGE_GERMAN_GERMANY,wxLOCALE_DONT_LOAD_DEFAULT};

  //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
  lables[(int)z_lbl::Mitabeiter] = new wxStaticText(this,wxID_ANY,"Mitarbeiter");
  cbo_benutzer = new wxComboBox(this, wxID_ANY);
  db->fill_combobox(cbo_benutzer, "SELECT * FROM V_MA_Benutzernamen");
  cbo_benutzer->AutoComplete(cbo_benutzer->GetStrings());
  cbo_benutzer->SetValue(default_str);


  lables[(int)z_lbl::Startdatum] = new wxStaticText(this,wxID_ANY,"Startdatum");
  start_date = new wxDatePickerCtrl(this,wxID_ANY,wxDefaultDateTime);
  
  lables[(int)z_lbl::Startzeit] = new wxStaticText(this,-1,"Startzeit");
  start_time = new wxTimePickerCtrl(this,wxID_ANY,wxDefaultDateTime);

  lables[(int)z_lbl::Enddatum] = new wxStaticText(this,wxID_ANY,"Enddatum");
  end_date = new wxDatePickerCtrl(this,wxID_ANY,wxDefaultDateTime);

  lables[(int)z_lbl::Endzeit] = new wxStaticText(this,wxID_ANY,"Endzeit");
  end_time = new wxTimePickerCtrl(this,wxID_ANY,wxDefaultDateTime);
  
  btn_submit = new wxButton(this, ID_Submit,"&Senden");          
  btn_toggle_end_date = new wxButton(this, ID_Toggle_end_date,str_lbl_no_ed,wxDefaultPosition,wxSize((str_lbl_no_ed.length()*10+20),35));          


  //Anordnung aller sichtbaren Elemente/////////////////////////////////////////////////////////////////////////////////
  wxSizerFlags szrflags(1);
  szrflags.Border(wxALL,5);

  //Sizer-Pointer werden innerhalb von SetSizer() freigegeben    
  wxFlexGridSizer * title_szr = new wxFlexGridSizer(1,100,10);
  title_szr->Add(title);

  wxFlexGridSizer * szr = new wxFlexGridSizer(4,10,30);
  szr->Add(lables[(int)z_lbl::Mitabeiter],szrflags);
  szr->Add( cbo_benutzer,szrflags );
  szr->AddSpacer(0);
  szr->AddSpacer(0);

  szr->Add(lables[(int)z_lbl::Startdatum],szrflags);
  szr->Add( start_date,szrflags );
  szr->Add(lables[(int)z_lbl::Enddatum],szrflags);
  szr->Add( end_date,szrflags );

  szr->Add(lables[(int)z_lbl::Startzeit],szrflags);
  szr->Add( start_time,szrflags );
  szr->Add(lables[(int)z_lbl::Endzeit],szrflags);
  szr->Add( end_time,szrflags );

  wxBoxSizer * szrButtons = new wxBoxSizer( wxHORIZONTAL);
  szrButtons->Add( btn_submit,szrflags);
  szrButtons->Add( btn_toggle_end_date,szrflags);
  
  wxBoxSizer * szrCRUDForm = new wxBoxSizer(wxVERTICAL);
  szrCRUDForm->Add(title_szr);
  szrCRUDForm->Add(szr);
  szrCRUDForm->Add(szrButtons);
  SetSizer(szrCRUDForm);

  SetSize(0,0,parent->m_width,parent->m_height);
  //Hide() erst nach dem SetSize nutzen, da sich die Elemente sonst nicht im grid befinden
  lables[(int)z_lbl::Enddatum]->Hide();
  end_date->Hide();
}

void Zeiterfassung::on_submit(wxCommandEvent& event){
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
    wxDateTime start = {
            start_date->GetValue().GetDay(),
          start_date->GetValue().GetMonth(),
           start_date->GetValue().GetYear(),
           start_time->GetValue().GetHour(),
         start_time->GetValue().GetMinute(),
         start_time->GetValue().GetSecond(),
       start_time->GetValue().GetMillisecond()
    };

    wxDateTime end = {
            show_end_date? end_date->GetValue().GetDay() : start_date->GetValue().GetDay() ,
          show_end_date? end_date->GetValue().GetMonth() : start_date->GetValue().GetMonth(),
           show_end_date? end_date->GetValue().GetYear() : start_date->GetValue().GetYear(),
           end_time->GetValue().GetHour(),
         end_time->GetValue().GetMinute(),
         end_time->GetValue().GetSecond(),          
       end_time->GetValue().GetMillisecond()
    };

    wxString Anfangszeit = start.Format(wxT("%y-%m-%d %H:%M:%S"));
    wxString Endzeit = end.Format(wxT("%y-%m-%d %H:%M:%S"));

    //Prüfung des Zeitraums////////////
    if(start > end){
       wxMessageBox( 
        "Dienstende("+end.Format(" %H:%M Uhr am %d.%m.%Y ")+")\ndarf nicht vor \nDienstbeginn("+start.Format(" %H:%M Uhr am %d.%m.%Y ")+") liegen!",
        wxString::FromUTF8("Angaben nicht plausibel!"),
        wxOK|wxICON_ERROR
      );
      return;
    }

    //Prüfung max_Tagesarbeitszeit////////////
    std::string str_max_hours_per_day = db->get_string_from_db("call SP_GET_Konfiguration('max_h_pro_tag')");
    std::string hours = std::to_string((end.GetTicks() - start.GetTicks()) / float(60*60));
    if(std::stof(hours) > std::stof(str_max_hours_per_day)){
       wxMessageBox( 
        wxString::FromUTF8("Maximale Arbeitszeit pro Tag("+str_max_hours_per_day+" Stunden) überschritten.\nEingegebene Stundenanzahl: "+hours.substr(0,hours.length()-4)+" Stunden."),
        wxString::FromUTF8("Speichern nicht möglich!"),
        wxOK|wxICON_ERROR
      );
      return;
    }

//  Prüfung auf Zeitüberlappung und ob Zeitraum vor Vertragsbeginn oder in der Zukunft liegt////////////
    strSQL = "call SP_CHECK_Arbeitszeiten('"+wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"','"+Anfangszeit.ToStdString()+"', '"+Endzeit.ToStdString()+"')";
    wxString check = db->get_string_from_db(strSQL.mb_str(wxConvUTF8));
    if(check == "-9"){
      wxMessageBox( 
        wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8)) + wxString::FromUTF8(" war am "+start.Format("%d.%m.%Y")+" noch nicht angestellt."),
        wxString::FromUTF8("Zeitraum nicht plausibel!"),
        wxOK|wxICON_ERROR
      );
      return;
    }
    if(check == "-8"){
      wxMessageBox( 
        wxString::FromUTF8("Der "+start.Format("%d.%m.%Y")+" liegt in der Zukunft."),
        wxString::FromUTF8("Zeitraum nicht plausibel!"),
        wxOK|wxICON_ERROR
      );
      return;
    }
    if(check != "0"){
      wxMessageBox( 
        wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8)) + wxString::FromUTF8(" hat innerhalb des angegebenen Zeitraums \nvon "+start.Format(" %H:%M Uhr am %d.%m.%Y ")+" \nbis  "+end.Format(" %H:%M Uhr am %d.%m.%Y ")+" \nbereits ein Eintrag."),
        wxString::FromUTF8("Zeitraum nicht plausibel!"),
        wxOK|wxICON_ERROR
      );
      return;
    }
    //Prüfung Ende//////////////////////////////////////////////////////////////////

    strSQL = "call SP_INSERT_Arbeitszeit('"+wxString::FromUTF8(cbo_benutzer->GetValue().mb_str(wxConvUTF8))+"','"+Anfangszeit.ToStdString()+"', '"+Endzeit.ToStdString()+"')";
    bool fail = db->execute_SQL(strSQL.mb_str(wxConvUTF8));

    // Enddatum nach dem senden verbergen, falls sichtbar
    if(show_end_date)toggle();

    wxMessageBox( 
    fail ? "SQL Insert Fehlgeschlagen" : "SQL Insert Erfolgreich",
    "Ausgabe der DB", 
      wxOK | wxICON_INFORMATION 
    ); 
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Zeiterfassung::on_submit: " +  (std::string)e.what());
  }
}

void Zeiterfassung::on_toggle(wxCommandEvent& event){
 toggle();
}

void Zeiterfassung::toggle(){
  try{
    end_date->SetValue(start_date->GetValue());
    show_end_date = !show_end_date;
    end_date->Show(show_end_date);
    lables[(int)z_lbl::Enddatum]->Show(show_end_date);
    btn_toggle_end_date->SetLabel(show_end_date ? str_lbl_with_ed : str_lbl_no_ed);
    btn_toggle_end_date->SetSize(show_end_date ? wxSize((str_lbl_with_ed.length()*10+20),35) : wxSize((str_lbl_no_ed.length()*10+20),35));
  }
  catch(std::exception& e){
    LOG::log_msg("FEHLER in Zeiterfassung::toggle: " +  (std::string)e.what());
  }
}

Zeiterfassung::~Zeiterfassung(){
  delete cbo_benutzer;
  delete start_date;
  delete start_time;
  delete end_date;
  delete end_time;
  delete btn_submit;
  delete btn_toggle_end_date;
  for(wxStaticText* st : lables)delete st;
}