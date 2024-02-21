#include <iostream>
#include <filesystem>

#include "flags.h"
#include "cpmake.h"
#include "errhandle.h"

void Main::flagO(std::vector<std::string>& args, const size_t pos)
{
    int _ = setVectorPos();
    if (_ == ERRCODE) checkIF();

    definedVectorPos = true;
    setOutputFile(args[pos+1]);

    definedOutputFile = true;
}

void Main::flagH(std::vector<std::string>& args, const size_t pos, const size_t argsLen)
{
    checkNxtArgs(args, pos, argsLen, Errcodes::FLAG_DOESNT_EXPECT_ARGS);
    printUsage();
    EXIT;
}

void Main::flagM(std::vector<std::string>& args, const size_t pos)
{

    checkIF();
    setVectorPos();
    setOutputFile(DEF_STRING_VAL);
    compilerFlags = args[pos+1];
    setCompileCmd();
}

void Main::flagR(std::vector<std::string>& args, const size_t pos)
{

    runOutputFile = true;
    size_t size = args.size();
    if (pos == size && inputFile == DEF_STRING_VAL)
    {
       printErrs(Errcodes::FLAG_EXPECTS_ARGS);
   }
}

void Main::flagC(std::vector<std::string>& args, const size_t pos, const size_t argsLen)
{

    if (compiler != DEF_STRING_VAL) return;
    if (argsLen == pos) printErrs(Errcodes::FLAG_EXPECTS_ARGS);
    else compiler = args[pos+1];
}

void Main::flagF(std::vector<std::string>& args, const size_t pos)
{
    buildFileName = args[pos+1];
    fileStuff();
}

void Main::processFlags(std::vector<std::string>& args, const size_t pos)
{
    size_t argsLen = args.size();

    if (args[pos] == "-o") flagO(args, pos);
    else if (args[pos] == "-h") flagH(args, pos, argsLen);
    else if (args[pos] == "-m") flagM(args, pos);
    else if (args[pos] == "-r") flagR(args, pos);
    else if (args[pos] == "-c") flagC(args, pos, argsLen);
    else if (args[pos] == "-k") deleteOutputFile = true;
    else if (args[pos] == "-f") flagF(args, pos);
    else if (args[pos] == "-hf" || args[pos] == "-h file") flagHF(args, pos);
    
}

void Main::checkFlags(const int argc, std::vector<std::string>& args)
{
    for (int i = 1; i < argc; i++) {
        processFlags(args, i);
    }
}

std::string Main::translateFlags(std::string flag)
{
    if (flag == "--help") return "-h";
    else if (flag == "--run") return "-r";
    else if (flag == "--kill" || flag == "--delete") return "-k";
    else if (flag == "--more" || flag == "--extra") return "-m";
    else if (flag == "--output") return "-o";
    else if (flag == "--compiler") return "-c";
    else if (flag == "--file") return "-f";

    return DEF_STRING_VAL;
}

bool Main::isItFlag(std::vector<std::string>& args, const size_t pos)
{
    std::string temp = translateFlags(args[pos]);

    if (pos > 0)
    {
        std::string temp2 = translateFlags(args[pos-1]);
        if (temp2 != DEF_STRING_VAL)
            if (temp2 == "-o" || temp2 == "-c") return true;

        if (args[pos-1] == "-o" || args[pos-1] == "-c") return true;
    }

    for (int i = 0; i < flagsSize; i++)
    {
        if (args[pos] == flags[i] || temp == flags[i])
            return true;
    }

    return false;
}

void Main::flagHF(std::vector<std::string>& args, const size_t pos)
{
    std::cout << "Flag f:" << std::endl;
    std::cout << "You can create a file 'build.cpk' and write some code in following syntax." << std::endl;
    std::cout << "EXAMPLE: " << std::endl;
}
