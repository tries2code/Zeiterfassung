#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime> 
#include <iomanip>
#include <sstream>

#include "Macros.hpp"


class LOG{

    public:
        static void log_msg(const std::string& str_msg);

    private:
        LOG();

};