#pragma once
#include <iostream>

#include <mysql/mysql.h>

#include <wx/combobox.h>
#include <wx/grid.h>
#include <wx/string.h>

#include "XML.hpp"
#include "TEA.hpp"
#include "LOG.hpp"


class cppDatabase{
        
        MYSQL *alpha_db, *conn_db;
        MYSQL_RES *result_set;
        MYSQL_ROW row;
        XML* xml_reader;

        char* host = NULL;
        char* usr = NULL;
        char* pwd = NULL;
        char* db = NULL;
        bool login_from_xml = false;
        bool Init();

    public:
        //Hart kodierte Verbinding
        cppDatabase(char* host, char* usr, char* pwd, char* db);
        //Verbindung mit Daten aus externer XML-Datei(Resources/database.xml)
        cppDatabase();
        
        std::string get_string_from_db(const char* strSQL);
        MYSQL_ROW get_row_from_db(const char* strSQL);
        bool execute_SQL(const char* strSQL);
        bool fill_combobox(wxComboBox* cbo,const char* strSQL);
        bool fill_grid(wxGrid* grd, const char* strSQL);
        bool backUpDB();
        /*
        bool insert_in_tbl(std::vector<std::vector<std::string>> tbl_data)
        */
};