#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstdlib>
#include <string>
#include <vector>

#include "cpmake.h"
#include "filemp.h"
#include "errhandle.h"
#include "flags.h"

namespace make {
    Main main;
}

int Cpmake::print(size_t times, char ch, bool nline = false, size_t nlineTimes = 1)
{
   for (int i = 1; i <= times; i++) {
        std::cout << ch;
    }

    if (nline) {
        for (int i = 1; i <= nlineTimes; i++) {
           std::cout << '\n';
        }
    }

    return EXIT_SUCCESS;
}
    
int Cpmake::isValidFile(std::string file)
{
   std::string extension;
   int dotPos = file.find_last_of(".");

   if (dotPos != std::string::npos) {
        extension = file.substr(dotPos);
    }

    for (int i = 0; i < vectorSize; i++) {
            if (extension == extensions[i]) {
                return i;
            }
        }

    return ERRCODE;
}


std::string Cpmake::eraseDot(std::string str)
{
    int dotPos = str.find_last_of('.');
    str.erase(dotPos);

    return str;
}

void Cpmake::setOutputFile(std::string outFile = DEF_STRING_VAL)
{
    if (outputFile != DEF_STRING_VAL) return;
    if (outFile == DEF_STRING_VAL || outFile == "" || outFile == " ") {
        std::string temp = eraseDot(inputFile);
        temp += ".exe";
        outputFile = temp;
    } else {
        int _ = outFile.find(".exe");
        if (_ == std::string::npos) {
            outFile += ".exe";
            outputFile = outFile;
        } else 
            outputFile = outFile;
    }
}

void Cpmake::setCompileCmd()
{
    if (compiler == DEF_STRING_VAL) compiler = compilers[vectorPos];
        compileCmd = compiler;
        if (compilerFlags != DEF_STRING_VAL) {
            compileCmd += " ";
            compileCmd += compilerFlags;
        }

        compileCmd += " -o ";
        compileCmd += outputFile;
        compileCmd += " ";
        compileCmd += inputFile;

}

int Cpmake::compile()
{
    if (compileCmd == DEF_STRING_VAL) setCompileCmd();
    int len = compileCmd.size();


    print(len+DEF_SPACES, '_', true, 2);
    print(DEF_SPACES/2, ' ');

    std::cout << compileCmd << std::endl;

    if (deleteOutputFile)
    {
        print(DEF_SPACES/2, ' ');
        std::cout << delcmd << std::endl;
    }

    std::system(compileCmd.c_str());
    print(len+DEF_SPACES, '_', true);

    return EXIT_SUCCESS;
}

void Cpmake::printUsage()
{
   std::cout << "Usage: " << PROGRAM_NAME << " <Args>\n";
   std::cout << "Options:\n";
   std::cout << "[NONE]: Will check for '" << buildFileName << "'" " file with certain syntax to compile a project/file, if not found will print usage" << std::endl;
   std::cout << "[file name]: Will check it's file type if it's cpp/c/cc then it will compile" << std::endl;
   std::cout << "[-o Ouput File]: lets you choose output file name" << std::endl;
   std::cout << "[-m Compilers flags]: lets you add some extra flags for the compiler" << std::endl;
   std::cout << "[-h Help]: prints Usage [this]" << std::endl;
   std::cout << "[-r Run]: runs the output file (if compiled)" << std::endl;
   std::cout << "[-k kill/delete]: delete's output file after compiled" << std::endl;
   std::cout << "[-f File]: If instead of using default file name '" << buildFileName << "', you want to use something else, you can use -f flag " << std::endl;
   std::cout << "[-c Compiler]: lets you choose compilers other than clang and clang++" << std::endl;
   EXIT;
}

void Cpmake::tryFileTypes(std::string fname)
{
   for (int i = 0; i < vectorSize; i++) {
        std::string temp = fname;
        temp += extensions[i];

        if  (std::filesystem::exists(temp)) {
            if (inputFile == DEF_STRING_VAL) inputFile = temp;
            else {
                inputFile += " ";
                inputFile += temp;
            }
        }
    }
}

bool Cpmake::fileExists(std::string fname)
{
   if (std::filesystem::exists(fname)) {
        if (inputFile == DEF_STRING_VAL) inputFile = fname;
        else {
            // Necessary when we are compiling more than one file
            inputFile += " ";
            inputFile += fname;
        }
        return true;
    }

    printErrs(Errcodes::INVALID_FILE_EXTENSION);
    return false;
}

void Cpmake::manageChecks(std::vector<std::string>& args, size_t pos)
{
    if (args[pos] == " " || args[pos] == "") return;

    bool _ = make::main.isItFlag(args, pos);
    if (_) return;

    int temp = isValidFile(args[pos]);
    if (temp != ERRCODE)  {
        vectorPos = temp;
        definedVectorPos = true;
        fileExists(args[pos]);
    }
    else tryFileTypes(args[pos]);
    if ( (std::filesystem::exists(args[pos])) ) inputFile = args[pos];
}

int Cpmake::setVectorPos()
{
    if (vectorPos != DEF_INT_VAL) return EXIT_SUCCESS;
    for (int i = 0; i < vectorSize; i++) {
        if ((inputFile.find(extensions[i])) != std::string::npos) {
            vectorPos = i;
            return EXIT_SUCCESS;
        }
    }
    return ERRCODE;
}

void Cpmake::setInputFile(const size_t argc, std::vector<std::string>& args)
{
    for (int i = 1; i < argc; i++) {
        manageChecks(args, i);
    }
}

void Cpmake::checkNxtArgs(std::vector<std::string>& args, size_t pos, const size_t size, Errcodes errcode)
{
   for (int i = pos+1; i < size; i++) {
       if (args[i] != " " && args[i] != "") printErrs(errcode);
    }
 }

void Cpmake::printEvryThing()
{
        std::cout << inputFile << std::endl;
        std::cout << outputFile << std::endl;
        std::cout << compiler << std::endl;
        std::cout << compilerFlags << std::endl;
        std::cout << compileCmd << std::endl;
        std::cout << vectorSize << std::endl;
        std::cout << vectorPos << std::endl;
}

void Cpmake::checkIF()
{
   if (inputFile == DEF_STRING_VAL) 
       printErrs(Errcodes::INPUT_FILE_NOT_PROVIDED);
}

void Cpmake::areDefined()
{
    checkIF();
    if (!definedVectorPos) setVectorPos();
    if (!definedOutputFile) setOutputFile();
}

void Cpmake::setDelCmd()
{
   delcmd = "del ";
   delcmd += outputFile;
}

void Cpmake::delFile()
{

    if (deleteOutputFile && inputFile != DEF_STRING_VAL && outputFile != DEF_STRING_VAL)
        system(delcmd.c_str());
}

void Cpmake::callFuncs()
{
   runFile();
   delFile();
}

int Cpmake::runFile()
{
    if (runOutputFile) {
        checkIF();
        std::cout <<  "running " << outputFile << std::endl;

        system(outputFile.c_str());
        std::cout << std::endl;

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int Cpmake::setThings(constants& consts)
{
    inputFile = consts.inputFiles;
    compiler = consts.compiler;
    compilerFlags = consts.compilerFlags;

    setVectorPos();
    setOutputFile(consts.outputFile);
    setCompileCmd();

    return EXIT_SUCCESS;
}
    
void Cpmake::fileStuff()
{
   FileMp filemp;
   constants consts = filemp.fileMpInit(buildFileName);
   filemp.checkConsts(consts);
   setThings(consts);
   compile();

   EXIT;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        make::main.fileStuff();
        make::main.printUsage();
    }

    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }

    args.push_back(" ");

    make::main.setInputFile(argc, args);
    make::main.checkFlags(argc, args);

    make::main.areDefined();
    make::main.setDelCmd();

    make::main.compile();
    make::main.callFuncs();

    return 0;
}
