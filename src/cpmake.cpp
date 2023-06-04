#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <vector>

#include "filemp.h"
#include "cpmake.h"
#include "errhandle.h"
#include "flags.h"

/* What is vectorPos variabe?
* It is the most important variable in this whole code
* After checking the file extension of the inputFile
* We set vectorPos to the postition extension of the inputFile in vector
* [extensions] and as the position of compilers in [compilers] vector
* is set relative to the extensions in [extensions] vector it
* also tells the compiler needed to compile current file and a lot other.
* So, It is very very important.
*/

Errcodes errcodes;

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
            outFile+= ".exe";
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
   /* std::cout << "Usage: " << PROGRAM_NAME << 
   " [inputFile] [-o (outputFile)]" <<
   " [-m (compilerFlags)]" << std::endl; */

   std::cout << "Usage: " << PROGRAM_NAME << ":\n";
   std::cout << "[No Args]: will check for 'Cpfile' with certain syntax, if not found print usage" << std::endl;
   std::cout << "[Input File]: check file type if it's supported compiles" << std::endl;
   std::cout << "[-o Ouput File]: lets you choose output file name" << std::endl;
   std::cout << "[-m Compilers flags]: lets you add some extra flags for the compiler" << std::endl;
   std::cout << "[-h Help]: prints Usage [this]" << std::endl;
   std::cout << "[-r Run]: runs the output file (if compiled)" << std::endl;
   std::cout << "[-k kill/delete]: delete's output file after compiled" << std::endl;
   EXIT;
}

void Cpmake::tryFileTypes(std::string fname)
{
   for (int i = 0; i < vectorSize; i++) {
        std::string temp = fname;
        temp += extensions[i];

        if  (std::filesystem::exists(temp)) {
            inputFile = temp;
            std::cout << "Using '" << inputFile << "' as input file!!" << std::endl;
        }
    }
}

bool Cpmake::fileExists(std::string fname)
{
   if (std::filesystem::exists(fname)) {
        inputFile = fname;
        return true;
    }

    printErrs(Errcodes::INVALID_FILE_EXTENSION);

}

void Cpmake::manageChecks(std::vector<std::string>& args, size_t pos)
{
    if (args[pos] == " " || args[pos] == "") return;

    for (int i = 0; i < flagsSize; i++) {
       if (args[pos] == flags[i])
        return;
       else if (pos > 0) 
           if  (args[pos-1] == "-o") return;
    }

        if ( (isValidFile(args[pos]) ) != ERRCODE )
            fileExists(args[pos]);
        else tryFileTypes(args[pos]);
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
    if (inputFile != DEF_STRING_VAL) return;
    for (int i = 1; i < argc; i++) {
        manageChecks(args, i);
        if (inputFile != DEF_STRING_VAL) break;
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
    
int Cpmake::fileStuff()
{
   FileMp filemp;
   constants consts = filemp.fileMpInit(MAKEFILE_NAME);
   filemp.checkConsts(consts);
   setThings(consts);
   compile();

   return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }

    // Is important for functins that run loop til args.size()+1;
    args.push_back(" ");

    Cpmake cpmake;
    Flags flags;

    if (argc < 2) {
        cpmake.fileStuff();
        cpmake.printUsage();
    }

    cpmake.setInputFile(argc, args);

    // flags.cpp stuff
    flags.checkFlags(argc, args, cpmake);

    cpmake.areDefined();
    cpmake.setDelCmd();

    cpmake.compile();
    cpmake.callFuncs();

    return 0;
}
