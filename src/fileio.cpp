#include <iostream>
#include <fstream>
#include <string>
#include "fileio.h"
#include "cpmake.h"

std::string getcflgas(std::string curr_line_content)
{
    /*
     * Take curr_line_content: cflags=/cflags = "-Wall" 
     * Extract the -Wall and return; 
     */

    return "_";
}

int freadline(std::string fname)
{
    std::ifstream inf { fname.c_str() };
    std::string line_content;

    if (!inf.is_open()) return ERRCODE;
    while (std::getline(inf, line_content)) {
        std::cout << line_content << std::endl;
    }

    return true;
}
