#include "cppDatabase.hpp"

cppDatabase::cppDatabase(char* host, char* usr, char* pwd, char* db):host(host),usr(usr),pwd(pwd),db(db){}
cppDatabase::cppDatabase(){
    login_from_xml = true;
    //Der nachfolgende Block prüft letzendlich nur ob die Datei vorhanden ist, und verhindert im Zweifelsfall den Programmstart
    xml_reader = new XML("Resources/database.xml");
    delete xml_reader;
}

bool cppDatabase::Init(){
    try{
        alpha_db = mysql_init(NULL);
        if (!alpha_db) return false;

        if(login_from_xml){
            xml_reader = new XML("Resources/database.xml");
            host = xml_reader->get_XML_field("host");
            usr  = xml_reader->get_XML_field("usr");
            pwd  = xml_reader->get_XML_field("password");
            db   = xml_reader->get_XML_field("db_name");

            //Passwort entschlüsseln
            TEA crypt{};
            std::stringstream sstr_to_decrypt;
	        sstr_to_decrypt<<std::hex;
	        sstr_to_decrypt = (std::stringstream)pwd;
            std::strcpy(pwd,crypt.decrypt(sstr_to_decrypt).c_str());
        }

        conn_db = mysql_real_connect(alpha_db, host, usr, pwd, db, 0, NULL, 0);
        if (!conn_db) {
            LOG::log_msg("FEHLER bei der Datenbank-Anmeldung: " + (std::string)mysql_error(alpha_db) + " Bitte kontrollieren Sie die Verbindung zur Datenbank!");
            mysql_close(conn_db);
            return false;
        }
        if(login_from_xml){
            delete xml_reader;
            delete host;
            delete usr;
            delete pwd;
            delete db;
        }
        return true;
    }
    catch(std::exception& e){
        LOG::log_msg("FEHLER in cppDatabase::Init: " +  (std::string)e.what());
        return false;
    }
}

std::string cppDatabase::get_string_from_db(const char* strSQL){
    try{

        Init();

        mysql_query(conn_db, strSQL);
        result_set = mysql_store_result(conn_db); 

        std::string output = "";
        //Output Concatinieren
	    while((row = mysql_fetch_row(result_set)) !=0){
            for(unsigned int i = 0; i<result_set->field_count;++i){
                output += row[i];
            }
	    }
        mysql_free_result(result_set);
        mysql_close(conn_db);

        return output;
    }
    catch(std::exception& e){
        LOG::log_msg("FEHLER in cppDatabase::get_string_from_db: " +  (std::string)e.what());
        return "error";
    }
};


MYSQL_ROW cppDatabase::get_row_from_db(const char* strSQL){
    try{

        Init();

        mysql_query(conn_db, strSQL);
        result_set = mysql_store_result(conn_db); 

        MYSQL_ROW output;

	    while((row = mysql_fetch_row(result_set)) !=0){
            output = row;
	    }
        mysql_free_result(result_set);
        mysql_close(conn_db);

        return output;
    }
    catch(std::exception& e){
        LOG::log_msg("FEHLER in cppDatabase::get_row_from_db: " +  (std::string)e.what());
        return MYSQL_ROW();
    }
};



bool cppDatabase::execute_SQL(const char* strSQL){
    //Auf Substrings prüfen("delete", "drop","insert", usw)   
    try{

        Init();

        bool fehler_bei_ausfuehrung = mysql_query(conn_db, strSQL);

        mysql_close(conn_db);
        return fehler_bei_ausfuehrung;
    }
    catch(std::exception& e){
        LOG::log_msg("FEHLER in cppDatabase::executeSQL: " +  (std::string)e.what());
        return false;
    }
};

bool cppDatabase::fill_combobox(wxComboBox* cbo,const char* strSQL){
    //Auf Substrings prüfen("delete", "drop","insert", usw)   
    try{
        cbo->Clear();

        Init();
        mysql_query(conn_db, strSQL);
        result_set = mysql_store_result(conn_db); 

        //cbo füllen
	    while((row = mysql_fetch_row(result_set)) !=0){
            for(unsigned int i = 0; i<result_set->field_count;++i){
                cbo->Insert(wxString::FromUTF8(row[i]),i);
            }
	    }
    
        mysql_free_result(result_set);
        mysql_close(conn_db);
        return true;
    }
    catch(std::exception& e){
        LOG::log_msg("FEHLER in cppDatabase::fill_combobox: " +  (std::string)e.what());
        return false;
    }
};


bool cppDatabase::fill_grid(wxGrid* grd,const char* strSQL){
    try{
        Init();
      
        //Ändert Spracheinstellung für Monats- und Wochentagsnamen auf Deutsch
        mysql_query(conn_db, "SET @@lc_time_names = 'de_DE';");

        mysql_query(conn_db, strSQL);
        result_set = mysql_store_result(conn_db); 

        //grd füllen
        int intRow= 0;
	    while((row = mysql_fetch_row(result_set)) !=0){

            //Neue Zeile Falls nötig
            if(intRow>grd->GetNumberRows()-1)grd->AppendRows(1);

            //Zeilen-Name(Monat) setzen
            grd->SetRowLabelValue(intRow, wxString::FromUTF8(row[0]));

            //Zeile füllen
            for(unsigned int intCol = 0; intCol<result_set->field_count-1;++intCol)
              grd->SetCellValue(intRow,intCol,wxString::FromUTF8(row[intCol+1]));
            
            ++intRow;
	    }
        grd->InvalidateBestSize();
        
        mysql_free_result(result_set);
        mysql_close(conn_db);
        return true;
    }
    catch(std::exception& e){
        LOG::log_msg("FEHLER in cppDatabase::fill_grid: " +  (std::string)e.what());
        return false;
    }
};