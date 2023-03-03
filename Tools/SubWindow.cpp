#include "SubWindow.hpp"


SubWindow::SubWindow(wxFrame* parent, cppDatabase* DB):db(DB),wxPanel((wxWindow*)parent, wxID_ANY){

    title = new wxStaticText(this,wxID_ANY, "default_title",wxDefaultPosition,{400,50});
    wxFont title_font = wxFont(20, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, true);
    title->SetFont(title_font);

}


 SubWindow::~SubWindow(){
    delete title;
 }

