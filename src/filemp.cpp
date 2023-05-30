#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "cpmake.h"
#include "filemp.h"

typedef enum class Errcodes Errcodes;


constants consts;

void FileMp::setDefVals()
{
    consts.compiler = DEF_STRING_VAL;
    consts.compiler_flags = DEF_STRING_VAL;
    consts.input_files = DEF_STRING_VAL;
    consts.output_file = DEF_STRING_VAL;
}

void FileMp::printVals(constants& consts)
{
    std::cout << consts.compiler << std::endl;
    std::cout << consts.compiler_flags << std::endl;
    std::cout << consts.input_files << std::endl;
    std::cout << consts.output_file << std::endl;
}

void FileMp::printErs(Errcodes errcode)
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

constants FileMp::fileMpInit(std::string filename)
{
    setDefVals();
    if (filename != MAKEFILE_NAME)
    {
        printErs(Errcodes::INVALID_FILE_NAME);
    }

   if ( !(std::filesystem::exists(filename)) ) printErs(Errcodes::FILE_DOESNT_EXIST);
   fReadLine(filename);
   return consts;
}

bool FileMp::checkSyntax(std::vector<std::string>& synxs, std::string txt)
{
    for (const auto& syn : synxs) {
        if (txt.find(syn) != std::string::npos) return true;
    }
    return false;
}

int FileMp::getEnd(size_t quote1, size_t quote2) { return quote2 - quote1 - 1; }


int FileMp::getCompiler(std::string line_txt)
{
    std::vector<std::string> syntx = {"(C)=", "(C) =", "cpl=", "cpl ="};
    bool compiler = checkSyntax(syntx, line_txt);

    if (compiler)
    {
        size_t quote1 = line_txt.find_first_of('"');
        size_t quote2 = line_txt.find_last_of('"');

        if (quote1 != std::string::npos && quote2 != std::string::npos)
        {
            consts.compiler = line_txt.substr(quote1+1, getEnd(quote1, quote2));
        } else printErs(Errcodes::INVALID_SYNTAX);
    }

    return EXIT_SUCCESS;
}

int FileMp::getCflags(std::string line_txt)
{

    std::vector<std::string> synx = {"cflags=", "cflags =", "(CF)=", "(CF) ="};
    bool flags = checkSyntax(synx, line_txt);

    if (flags)
    {
        size_t quote1 = line_txt.find_first_of('"');
        size_t quote2 = line_txt.find_last_of('"');

        if (quote1 != std::string::npos && quote2 != std::string::npos)
        {
            consts.compiler_flags = line_txt.substr(quote1+1, getEnd(quote1, quote2));
        } else 
        {
            printErs(Errcodes::INVALID_SYNTAX);
        }
    }

    return EXIT_SUCCESS;
}

int FileMp::getFileNames(std::string line_txt)
{
    std::vector<std::string> syntxs = {"(IF)=", "(IF) =", "(IFNS)=", "(IFNS) ="};
    bool fname = checkSyntax(syntxs, line_txt);

    if (fname)
    {
        size_t quote1 = line_txt.find_first_of('"');
        size_t quote2 = line_txt.find_last_of('"');

        if (quote1 != std::string::npos && quote2 != std::string::npos)
        {
            consts.input_files = line_txt.substr(quote1+1, getEnd(quote1, quote2));
        } else printErs(Errcodes::INVALID_SYNTAX);
    }

    return EXIT_SUCCESS;
}

int FileMp::getOutName(std::string line_txt)
{
    std::vector<std::string> syntxs = {"(OF)=", "(OF) ="};
    bool outputFile = checkSyntax(syntxs, line_txt);

    if (outputFile)
    {
        size_t quote1 = line_txt.find_first_of('"');
        size_t quote2 = line_txt.find_last_of('"');

        if (quote1 != std::string::npos && quote2 != std::string::npos)
        {
            consts.output_file = line_txt.substr(quote1+1, getEnd(quote1, quote2));
        } else printErs(Errcodes::INVALID_SYNTAX);
    }

    return EXIT_SUCCESS;
}

int FileMp::fReadLine(std::string fname)
{

    std::ifstream inf { fname.c_str() };
    std::string line_content;

    if (!inf.is_open()) return ERRCODE;
    while (std::getline(inf, line_content)) {
        if (consts.compiler_flags == DEF_STRING_VAL) getCflags(line_content);
        if (consts.compiler == DEF_STRING_VAL) getCompiler(line_content);
        if (consts.input_files == DEF_STRING_VAL) getFileNames(line_content);
        if (consts.output_file == DEF_STRING_VAL) getOutName(line_content);
    }

    return true;
}