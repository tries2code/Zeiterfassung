#pragma once
#include <iostream>
#include <fstream>
#include <cstring>

class XML{

    private:
        std::string file_text = "";
        char c = ' ';
        int pos = 0, start = 0;
    
        bool get_file_text(const std::string& filename);

    public:
        XML(const std::string& this_filename);
        char* get_XML_field(const std::string& field);
};