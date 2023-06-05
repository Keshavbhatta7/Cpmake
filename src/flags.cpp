#include <iostream>
#include <filesystem>

#include "flags.h"
#include "cpmake.h"
#include "errhandle.h"

void Main::processFlags(std::vector<std::string>& args, size_t pos)
{
    size_t argsLen = args.size();

    if (args[pos] == "-o") {
        int _ = setVectorPos();
        if (_ == ERRCODE) checkIF();

        definedVectorPos = true;
        setOutputFile(args[pos+1]);

        definedOutputFile = true;
    } else if (args[pos] == "-h") {
       checkNxtArgs(args, pos, argsLen, Errcodes::FLAG_DOESNT_EXPECT_ARGS);
       printUsage();
       EXIT;
    } else if (args[pos] == "-m") {
        checkIF();
        setVectorPos();
        setOutputFile(DEF_STRING_VAL);
        compilerFlags = args[pos+1];
        setCompileCmd();
    } else if (args[pos] == "-r") {
        runOutputFile = true;
        size_t size = args.size();
        if (pos == size && inputFile == DEF_STRING_VAL) {
                printErrs(Errcodes::FLAG_EXPECTS_ARGS);
            }
    } else if (args[pos] == "-k") {
        deleteOutputFile = true;
    }
}

void Main::checkFlags(const int argc, std::vector<std::string>& args)
{
    for (int i = 1; i < argc; i++) {
        processFlags(args, i);
    }
}