#include "Zeituebersicht.hpp"
#include "wx/string.h"


Uebersicht::Uebersicht(wxFrame* parent, cppDatabase* DB,const wxString& this_title)
:SubWindow(parent, DB,this_title){
  
  //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
  lables[(int)u_lbl::Benutzer] = new wxStaticText(this,wxID_ANY,"    Benutzer");
  lables[(int)u_lbl::Monat] = new wxStaticText(this,wxID_ANY,"    Monat");
  lables[(int)u_lbl::Jahr] = new wxStaticText(this,wxID_ANY,"    Jahr");
  lables[(int)u_lbl::Stunden] = new wxStaticText(this,wxID_ANY,"");

  combo_boxen[(int)u_cbo::Benutzer] = new wxComboBox(this, ID_USR_cbo, default_str);
  combo_boxen[(int)u_cbo::Monat] = new wxComboBox(this, wxID_ANY, "Alle");
  combo_boxen[(int)u_cbo::Jahr] = new wxComboBox(this, ID_Year_cbo, default_str,wxDefaultPosition,wxDefaultSize,0,NULL,wxTE_PROCESS_ENTER);
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
  

  wxFlexGridSizer * szr2 = new wxFlexGridSizer(1,10,10);
  szr2->Add(grd_zeiten,szrflags);

  wxBoxSizer * szrButtons = new wxBoxSizer( wxHORIZONTAL);
  szrButtons->Add( btn_show_times,szrflags);
  szrButtons->AddSpacer(290);
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

    strSQL = "call SP_GET_Arbeitszeiten('"+wxString::FromUTF8(combo_boxen[(int)u_cbo::Benutzer]->GetValue().mb_str(wxConvUTF8))+"',"+strFromMonth+","+strTillMonth+","+combo_boxen[(int)u_cbo::Jahr]->GetValue()+");";
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
  catch(...){
    std::cerr<<"FEHLER in Uebersicht::on_show_times!\n";
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
  catch(...){
    std::cerr<<"FEHLER in Uebersicht::on_change_ma!\n";
  }
}

void Uebersicht::on_enter_year(wxCommandEvent& event){
if(combo_boxen[(int)u_cbo::Benutzer]->GetValue() == default_str){
      wxMessageBox( 
        wxString::FromUTF8("Bitte wählen Sie zunächst einen Mitarbeiter aus."),
        wxString::FromUTF8("Jahr(e) noch nicht verfügbar!"),
        wxOK|wxICON_ERROR
      );
      return;
    }
  
}

bool Uebersicht::set_up_grid(){
  try{
    grd_zeiten->CreateGrid(10, 5);
    grd_zeiten->SetColLabelValue(0, _(" Datum Anfang "));  
    grd_zeiten->SetColLabelValue(1, _("Zeit Anfang")); 
    grd_zeiten->SetColLabelValue(2, _("   Datum Ende   "));  
    grd_zeiten->SetColLabelValue(3, _("Zeit Ende")); 
    grd_zeiten->SetColLabelValue(4, _("Arbeitsstunden")); 
    grd_zeiten->AutoSizeColumns(1300);
    for(unsigned int intRow = 0;intRow < grd_zeiten->GetNumberRows();++intRow){
      grd_zeiten->SetRowLabelValue(intRow, "");
    }
    grd_zeiten->SetColLabelAlignment(50, 300);
    grd_zeiten->DisableDragColSize();
    grd_zeiten->DisableDragRowSize();
    return true;
  }
  catch(...){
    std::cerr<<"FEHLER in Uebersicht::set_up_grid!\n";
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
    
    return true;
  }
  catch(...){
    std::cerr<<"FEHLER in Uebersicht::set_up_combo!\n";
    return false;
  }
}

Uebersicht::~Uebersicht(){
  for(wxComboBox* cb : combo_boxen) delete cb;
  for(wxStaticText* st : lables) delete st;
  delete grd_zeiten;
  delete btn_show_times;
}