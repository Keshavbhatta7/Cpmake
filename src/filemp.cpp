#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "cpmake.h"
#include "filemp.h"

typedef enum class Errcodes Errcodes;
constants consts;

void setvals()
{
    consts.compiler_flags = DEF_STRING_VAL;
    consts.input_files = DEF_STRING_VAL;
    consts.output_file = DEF_STRING_VAL;
}

void printers(Errcodes errcode)
{
    switch (errcode)
    {
        case Errcodes::INVALID_FILE_NAME: std::cerr << "error: file name should be 'Cpfile'" << std::endl;
        EXIT;

        case Errcodes::INVALID_SYNTAX: std::cerr << "error: invalid syntax" << std::endl;
        EXIT;

        case Errcodes::FILE_DOESNT_EXIST: std::cerr << "error: file doesn't exist" << std::endl;
        EXIT;
    }
}

constants filemp_init(std::string filename)
{
    setvals();
    if (filename != MAKEFILE_NAME)
    {
        printers(Errcodes::INVALID_FILE_NAME);
    }

   if ( !(std::filesystem::exists(filename)) ) printers(Errcodes::FILE_DOESNT_EXIST);
   freadline(filename);
   return consts;
}


std::string getcflags(std::string line_txt)
{

    size_t syn = line_txt.find("cflags=");
    size_t syn2 = line_txt.find("cflags =");

    if (syn != std::string::npos || syn2 != std::string::npos)
    {
        size_t quote1 = line_txt.find_first_of("'");
        size_t quote2 = line_txt.find_last_of("'");

        if (quote1 != std::string::npos && quote2 != std::string::npos)
        {
            consts.compiler_flags = line_txt.substr(quote1+1, quote2 - quote1 - 1);
        } else 
        {
            printers(Errcodes::INVALID_SYNTAX);
            return DEF_STRING_VAL;
        }
    }

    return DEF_STRING_VAL;
}

int freadline(std::string fname)
{

    std::ifstream inf { fname.c_str() };
    std::string line_content;

    if (!inf.is_open()) return ERRCODE;
    while (std::getline(inf, line_content)) {
        if (consts.compiler_flags == DEF_STRING_VAL) getcflags(line_content);
    }

    return true;
}