#include "Benutzereinstellungen.hpp"




Benutzereinstellungen::Benutzereinstellungen(wxFrame* parent, cppDatabase* DB, const wxString& this_title):SubWindow(parent, DB,this_title){
    //Initialisierung aller sichtbaren Elemente///////////////////////////////////////////////////////////////////////
    lables[(int)b_lbl::Mindestalter] = new wxStaticText(this,wxID_ANY,"Mindestalter");
    lables[(int)b_lbl::MaxStdProTag] = new wxStaticText(this,wxID_ANY,"Max. Stunden");
    lables[(int)b_lbl::MinUsernameLen] = new wxStaticText(this,wxID_ANY,"Min. Username");


    text_ctrls[(int)b_tc::Mindestalter] = new wxTextCtrl(this,wxID_ANY);
    text_ctrls[(int)b_tc::MaxStdProTag] = new wxTextCtrl(this,wxID_ANY);
    text_ctrls[(int)b_tc::MinUsernameLen] = new wxTextCtrl(this,wxID_ANY);
    load_all_values();

    btn_save = new wxButton(this,ID_SaveSetting_btn,"Speichern");

  //Anordnung aller sichtbaren Elemente/////////////////////////////////////////////////////////////////////////////////
  wxSizerFlags szrflags(1);
    szrflags.Border(wxALL,5);
    szrflags.Right(); //Allignment
      
  //Sizer-Pointer werden innerhalb von SetSizer() freigegeben
  wxFlexGridSizer * title_szr = new wxFlexGridSizer(1,100,10);
    title_szr->Add(title);

  wxFlexGridSizer * szr = new wxFlexGridSizer(3,10,10);
    szr->Add(lables[(int)b_lbl::Mindestalter],szrflags);
    szr->AddSpacer(1);
    szr->Add(text_ctrls[(int)b_tc::Mindestalter],szrflags);

    szr->Add(lables[(int)b_lbl::MaxStdProTag],szrflags);
    szr->AddSpacer(1);
    szr->Add(text_ctrls[(int)b_tc::MaxStdProTag],szrflags);

    szr->Add(lables[(int)b_lbl::MinUsernameLen],szrflags);
    szr->AddSpacer(1);
    szr->Add(text_ctrls[(int)b_tc::MinUsernameLen],szrflags);

  wxBoxSizer * szrButtons = new wxBoxSizer(wxVERTICAL);
    szrButtons->Add( btn_save,szrflags);

    

  wxBoxSizer * szrCRUDForm = new wxBoxSizer(wxVERTICAL);
    szrCRUDForm->Add(title_szr);
    szrCRUDForm->Add( szr);
    szrCRUDForm->Add( szrButtons );

  SetSizer(szrCRUDForm);
  SetSize(0,0,parent->m_width,parent->m_height);
}


void Benutzereinstellungen::on_save(wxCommandEvent& event){
  try{
    std::string str_min_age = text_ctrls[(int)b_tc::Mindestalter]->GetValue().ToStdString();
    std::string str_max_hrs = text_ctrls[(int)b_tc::MaxStdProTag]->GetValue().ToStdString();
    std::string str_min_len = text_ctrls[(int)b_tc::MinUsernameLen]->GetValue().ToStdString();

    if(!check_entries(str_min_age,str_max_hrs,str_min_len))return;

    std::string strSQL = "call SP_UPDATE_Konfiguration('min_entry_age','" + str_min_age + "')";
    db->execute_SQL(strSQL.c_str());
    strSQL = "call SP_UPDATE_Konfiguration('max_h_pro_tag','" + str_max_hrs + "')";
    db->execute_SQL(strSQL.c_str());
    strSQL = "call SP_UPDATE_Konfiguration('min_usrname_len','" + str_min_len + "')";
    db->execute_SQL(strSQL.c_str());
  }
  catch(std::exception& e){
    std::cerr<<"FEHLER in Benutzereinstellungen::on_save: " << e.what();
  }
}

bool Benutzereinstellungen::load_all_values(){
  try{
    std::string str_min_age = db->get_string_from_db("call SP_GET_Konfiguration('min_entry_age')");
    std::string str_max_hrs = db->get_string_from_db("call SP_GET_Konfiguration('max_h_pro_tag')");
    std::string str_min_len = db->get_string_from_db("call SP_GET_Konfiguration('min_usrname_len')");

    text_ctrls[(int)b_tc::Mindestalter]->SetValue(str_min_age);
    text_ctrls[(int)b_tc::MaxStdProTag]->SetValue(str_max_hrs);
    text_ctrls[(int)b_tc::MinUsernameLen]->SetValue(str_min_len);
    

    return true;
    
  }
  catch(std::exception& e){
    std::cerr<<"FEHLER in Benutzereinstellungen::load_all_values: " << e.what();
    return false;
  }
}

bool Benutzereinstellungen::check_entries(std::string& str_min_age,std::string& str_max_hrs,std::string& str_usr_len){
  try{
    std::string str_error_msg = "Bitte überprüfen Sie: ";

    //Jeder Block prüft ein Feld
    bool digits_age = std::regex_match(str_min_age,std::regex("^\\d{2}$"));
    if(!digits_age)
      str_error_msg = str_error_msg + lables[(int)b_lbl::Mindestalter]->GetLabelText().utf8_string() + ",";

    bool digits_hrs = std::regex_match(str_max_hrs,std::regex("^\\d{1,2}$"));
    if(!digits_hrs || std::atoi(str_max_hrs.c_str()) > 25){
      str_error_msg = str_error_msg + lables[(int)b_lbl::MaxStdProTag]->GetLabelText().utf8_string() + ",";
      digits_hrs = false;
    }

    bool digits_usr_len = std::regex_match(str_usr_len,std::regex("^\\d{1,2}$"));
    if(!digits_usr_len || std::atoi(str_usr_len.c_str()) > 64){
      str_error_msg = str_error_msg + lables[(int)b_lbl::MinUsernameLen]->GetLabelText().utf8_string() + ".";
      digits_usr_len = false;
    }
    
    //Falls etwas nicht passt folgt Error-Nachricht und Abbruch
    if(!digits_age || !digits_hrs || !digits_usr_len){
      str_error_msg[str_error_msg.length()-1]='.';
      wxMessageBox( 
      wxString::FromUTF8(str_error_msg),
      wxString::FromUTF8("Speichern nicht möglich!"),
      wxOK|wxICON_ERROR
      );
      return false;
    }
    return true;
  }
  catch(std::exception& e){
    std::cerr<<"FEHLER in Benutzereinstellungen::check_entries: " << e.what();
    return false;
  }                                     
}


Benutzereinstellungen::~Benutzereinstellungen(){
  for(wxStaticText* st : lables)delete st;
  for(wxTextCtrl* tc : text_ctrls)delete tc;
  delete btn_save;
}