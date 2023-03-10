#include "Mitarbeiterverwaltung.hpp"




Verwaltung::Verwaltung(wxFrame* parent, cppDatabase* DB, const wxString& this_title):SubWindow(parent, DB,this_title){

  //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
  cbo_benutzer = new wxComboBox(this, wxID_ANY);
  db->fill_combobox(cbo_benutzer, "SELECT * FROM V_MA_Benutzernamen");
  cbo_benutzer->AutoComplete(cbo_benutzer->GetStrings());
  cbo_benutzer->SetValue(default_str);


  
  //Anordnung aller sichtbaren Elemente/////////////////////////////////////////////////////////////////////////////////
  wxSizerFlags szrflags(1);
  szrflags.Border(wxALL,5);

  //Sizer-Pointer werden innerhalb von SetSizer() freigegeben    
  wxFlexGridSizer * title_szr = new wxFlexGridSizer(1,100,10);
  title_szr->Add(title);

  wxFlexGridSizer * szr = new wxFlexGridSizer(6,10,10);
  //szr->Add(lables[(int)z_lbl::Mitabeiter],szrflags);
  szr->Add( cbo_benutzer,szrflags );
  
  

  

  //wxBoxSizer * szrButtons = new wxBoxSizer( wxHORIZONTAL);
  //szrButtons->Add( btn_submit,szrflags);
  
  wxBoxSizer * szrCRUDForm = new wxBoxSizer(wxVERTICAL);
  szrCRUDForm->Add(title_szr);
  szrCRUDForm->Add(szr);
  //szrCRUDForm->Add(szrButtons);
  SetSizer(szrCRUDForm);

  SetSize(0,0,parent->m_width,parent->m_height);
  
  
}




Verwaltung::~Verwaltung(){
  delete cbo_benutzer;
  
  //for(wxStaticText* st : lables)delete st;
}