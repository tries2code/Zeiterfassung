#include "SubWindow.hpp"



SubWindow::SubWindow(wxFrame* parent, cppDatabase* DB):db(DB),wxPanel((wxWindow*)parent, wxID_ANY){}


