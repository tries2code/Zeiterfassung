#include "LOG.hpp"

using time_point = std::chrono::system_clock::time_point;
std::string get_log_time(const time_point& this_ime, const std::string& format){
    std::time_t time = std::chrono::system_clock::to_time_t(this_ime);
    std::tm tm = *std::localtime(&time); //Locale time-zone, usually UTC(std::::gmtime()) by default.
    std::stringstream ss;
    ss << std::put_time( &tm, format.c_str() );
    return ss.str();
}


void LOG::log_msg(const std::string& str_msg){
    std::ofstream ofs_err_file;
    ofs_err_file.open("Resources/LOG.txt",std::ios_base::app);
    ofs_err_file << get_log_time(std::chrono::system_clock::now(), "%d.%m.%Y %H:%M:%S -> ")<< str_msg << "\n";

    if(debug_mode)std::cerr << str_msg << "\n";
}