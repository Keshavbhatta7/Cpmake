#ifndef CPMAKE_H
#define CPMAKE_H

#include <iostream>
#include "filemp.h"

const std::string PROGRAM_NAME = "cpmake";
const std::string DEF_STRING_VAL = "NULL";
const std::string DEF_BUILDFILE_NAME = "build.cpk";

const int ERRCODE = -1;
const int DEF_SPACES = 10;
const int DEF_INT_VAL = -235;

#define EXIT exit(ERRCODE)

enum class Errcodes {
    INVALID_FILE_EXTENSION,
    INVALID_FLAG,
    INPUT_FILE_NOT_PROVIDED,
    FLAG_DOESNT_EXPECT_ARGS,
    FLAG_EXPECTS_ARGS,
    INVALID_SYNTAX,
    FILE_DOESNT_EXIST,
    OUTPUT_FILE_NOT_PROVIDED,
    COMPILER_NOT_PROVIDED,
};

class Cpmake {
public:
    std::string compiler = DEF_STRING_VAL;
    std::string inputFile = DEF_STRING_VAL;
    std::string outputFile = DEF_STRING_VAL;
    std::string compileCmd = DEF_STRING_VAL;
    std::string delcmd = DEF_STRING_VAL;
    std::string compilerFlags = DEF_STRING_VAL;
    std::string buildFileName = DEF_BUILDFILE_NAME;

    std::vector<std::string> extensions = {".cpp", ".c", ".cc"};
    std::vector<std::string> compilers =  {"clang++", "clang", "clang++"};
    std::vector<std::string> flags = {"-o", "-h", "-m", "-r", "-k", "-c", "-f"};

    bool definedOutputFile = false;
    bool definedVectorPos = false;
    bool runOutputFile = false;
    bool deleteOutputFile = false;

    size_t vectorSize = extensions.size();
    size_t flagsSize = flags.size();
    size_t vectorPos = DEF_INT_VAL;

public:

    int print(size_t times, char ch, bool nline, size_t nlineTimes);
    int isValidFile(std::string file);
    void fileStuff();

    int setVectorPos();
    int compile();

    int runFile();
    int setThings(constants& consts);

    bool fileExists(std::string fname);

    void setOutputFile(std::string outFile);
    void setInputFile(const size_t argc, std::vector<std::string>& args);
    void tryFileTypes(std::string fname);
    void manageChecks(std::vector<std::string>& args, size_t pos);
    void checkNxtArgs(std::vector<std::string>& args, const size_t pos, const size_t size, Errcodes errcode);

    void checkIF();
    void areDefined();

    void setDelCmd();
    void delFile();

    void printUsage();
    void printEvryThing();
    void setCompileCmd();
    void callFuncs();

    std::string eraseDot(std::string str);
};

#endif
