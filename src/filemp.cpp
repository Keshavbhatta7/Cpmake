#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "cpmake.h"
#include "filemp.h"
#include "errhandle.h"

constants consts;

void FileMp::setDefVals()
{
    consts.compiler = DEF_STRING_VAL;
    consts.compilerFlags = DEF_STRING_VAL;
    consts.inputFiles = DEF_STRING_VAL;
    consts.outputFile = DEF_STRING_VAL;
}

void FileMp::printVals(constants& consts)
{
    std::cout << consts.compiler << std::endl;
    std::cout << consts.compilerFlags << std::endl;
    std::cout << consts.inputFiles << std::endl;
    std::cout << consts.outputFile << std::endl;
}

constants FileMp::fileMpInit(std::string filename)
{
    setDefVals();
    if (filename != MAKEFILE_NAME)
        printErrs(Errcodes::INVALID_FILE_NAME);

   if ( !(std::filesystem::exists(filename)) ) printErrs(Errcodes::FILE_DOESNT_EXIST);
   fReadLine(filename);
   return consts;
}

bool FileMp::checkSyntax(std::vector<std::string>& synxs, std::string str)
{
    for (const auto& syn : synxs) {
        if (str.find(syn) != std::string::npos) return true;
    }
    return false;
}

int FileMp::getEnd(size_t quote1, size_t quote2) { return quote2 - quote1 - 1; }
int FileMp::getCompiler(std::string str)
{
    std::vector<std::string> syntx = {"(CPP_C)=", "(CPP_C) ="};
    bool compiler = checkSyntax(syntx, str);

    if (compiler)
    {
        size_t quote1 = str.find_first_of('"');
        size_t quote2 = str.find_last_of('"');

        if (quote1 != std::string::npos && quote2 != std::string::npos)
        {
            consts.compiler = str.substr(quote1+1, getEnd(quote1, quote2));
        } else printErrs(Errcodes::INVALID_SYNTAX);
    }

    return EXIT_SUCCESS;
}

int FileMp::getCflags(std::string str)
{

    std::vector<std::string> synx = {"(CFS)=", "(CFS) ="};
    bool flags = checkSyntax(synx, str);

    if (flags)
    {
        size_t quote1 = str.find_first_of('"');
        size_t quote2 = str.find_last_of('"');

        if (quote1 != std::string::npos && quote2 != std::string::npos)
            consts.compilerFlags = str.substr(quote1+1, getEnd(quote1, quote2));
        else 
            printErrs(Errcodes::INVALID_SYNTAX);
    }

    return EXIT_SUCCESS;
}

int FileMp::getFileNames(std::string str)
{
    std::vector<std::string> syntxs = {"(IF)=", "(IF) =", "(IFS)=", "(IFS) ="};
    bool fname = checkSyntax(syntxs, str);

    if (fname)
    {
        size_t quote1 = str.find_first_of('"');
        size_t quote2 = str.find_last_of('"');

        if (quote1 != std::string::npos && quote2 != std::string::npos)
            consts.inputFiles = str.substr(quote1+1, getEnd(quote1, quote2));
        else printErrs(Errcodes::INVALID_SYNTAX);
    }

    return EXIT_SUCCESS;
}

int FileMp::getOutName(std::string str)
{
    std::vector<std::string> syntxs = {"(OF)=", "(OF) ="};
    bool outputFile = checkSyntax(syntxs, str);

    if (outputFile)
    {
        size_t quote1 = str.find_first_of('"');
        size_t quote2 = str.find_last_of('"');

        if (quote1 != std::string::npos && quote2 != std::string::npos)
        {
            consts.outputFile = str.substr(quote1+1, getEnd(quote1, quote2));
        } else printErrs(Errcodes::INVALID_SYNTAX);
    }

    return EXIT_SUCCESS;
}

bool FileMp::checkConsts(constants& consts)
{
    if (consts.inputFiles == DEF_STRING_VAL) printErrs(Errcodes::INPUT_FILE_NOT_PROVIDED);
    if (consts.outputFile == DEF_STRING_VAL) printErrs(Errcodes::OUTPUT_FILE_NOT_PROVIDED);
    if (consts.compiler == DEF_STRING_VAL) printErrs(Errcodes::COMPILER_NOT_PROVIDED);

    return true;
}

int FileMp::fReadLine(std::string fname)
{

    std::ifstream inf { fname.c_str() };
    std::string line_content;

    if (!inf.is_open()) return ERRCODE;
    while (std::getline(inf, line_content)) {
        if (consts.compilerFlags == DEF_STRING_VAL) getCflags(line_content);
        if (consts.compiler == DEF_STRING_VAL) getCompiler(line_content);
        if (consts.inputFiles == DEF_STRING_VAL) getFileNames(line_content);
        if (consts.outputFile == DEF_STRING_VAL) getOutName(line_content);
    }

    return true;
}
