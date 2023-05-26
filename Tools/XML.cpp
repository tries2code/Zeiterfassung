#include "XML.hpp"

XML::XML(const std::string& this_filename){
    //filename = this_filename;
    if(!get_file_text(this_filename)){
    std::cerr << "Dateiname \""+this_filename+"\" unbekannt!\n Start des Programs wurde abgebrochen!";
    exit(-1);
    }
}

bool XML::get_file_text(const std::string& filename){
   try{
        std::ifstream input_stream(filename);  
        if(!input_stream)return false;

        while(input_stream.get(c)) file_text += c;
        input_stream.close();
        return true;
    }
    catch(std::exception& e){
        LOG::log_msg("FEHLER in XML::get_file_text: " +  (std::string)e.what());
        return false;
    }
    catch(...){
        LOG::log_msg("FEHLER in XML::get_file_text");
        return false;
    }
}

char* XML::get_XML_field(const std::string& field){                                                          
   try{
        start = file_text.find( "<" + field, pos );   
        start = file_text.find( ">" , start );
        start++;
        pos = file_text.find( "</" + field, start );  
        std::string tmp = file_text.substr(start, pos - start);

        char* result = new char[file_text.substr(start, pos - start).length() + 1];
        strcpy(result, file_text.substr(start, pos - start).c_str());
        return result;
    }
    catch(std::exception& e){
        LOG::log_msg("FEHLER in XML::get_XML_field: " +  (std::string)e.what());
        return (char*)"-1";
    }
    catch(...){
        LOG::log_msg("FEHLER in XML::get_XML_field");
        return (char*)"-1";
    }
}